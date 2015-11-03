// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/components/mce/memory_pressure_monitor_mce.h"

#include "base/bind.h"
#include "base/location.h"
#include "base/threading/thread.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_proxy.h"

namespace base {

namespace {

const char kWorkerThreadName[] = "MceMemoryPressureMonitorWorker";

const char kMceServiceName[] = "com.nokia.mce";
const char kMceSignalPath[] = "/com/nokia/mce/signal";
const char kMceSignalServiceName[] = "com.nokia.mce.signal";
const char kMceMemoryLevelSignalName[] = "sig_memory_level_ind";
const char kMceRequestServiceName[] = "com.nokia.mce.request";
const char kMceRequestPath[] = "/com/nokia/mce/request";
const char kMceGetMemoryLevelMethodName[] = "get_memory_level";
const char kMceMemoryPressureNormal[] = "normal";
const char kMceMemoryPressureWarning[] = "warning";
const char kMceMemoryPressureCritical[] = "critical";

} // namespace

class MemoryPressureMonitorMce::WorkerThread : public base::Thread {
 public:
  typedef base::Callback<void(const MemoryPressureLevel&)>
      MemoryPressureUpdateCallback;

  WorkerThread(const MemoryPressureUpdateCallback& cb)
      : base::Thread(kWorkerThreadName)
      , callback_(cb) {
  }

  ~WorkerThread() override {
  }

  void StartListening() {
    DCHECK(OnWorkerThread());

    dbus::Bus::Options options;
    options.bus_type = dbus::Bus::SYSTEM;
    options.connection_type = dbus::Bus::PRIVATE;
    system_bus_ = new dbus::Bus(options);

    mce_signal_proxy_ = system_bus_->
        GetObjectProxy(kMceServiceName, dbus::ObjectPath(kMceSignalPath));
    mce_signal_proxy_->ConnectToSignal(
        kMceSignalServiceName,
        kMceMemoryLevelSignalName,
        base::Bind(&WorkerThread::OnMemoryPressureSignal,
                   base::Unretained(this)),
        base::Bind(&WorkerThread::OnConnected,
                   base::Unretained(this)));

    dbus::ObjectProxy* proxy = system_bus_->
        GetObjectProxy(kMceServiceName, dbus::ObjectPath(kMceRequestPath));
    dbus::MethodCall method_call(kMceRequestServiceName,
                                 kMceGetMemoryLevelMethodName);
    const int timeout_ms = dbus::ObjectProxy::TIMEOUT_USE_DEFAULT;
    proxy->CallMethod(&method_call, timeout_ms,
        base::Bind(&WorkerThread::OnInitialLevelResponse,
                   base::Unretained(this)));

    DVLOG(1) << "MCE memory pressure monitor started";
  }

  void StopListening() {
    DCHECK(OnWorkerThread());
    DCHECK(system_bus_);

    message_loop()->PostTask(FROM_HERE,
                             base::Bind(&dbus::Bus::ShutdownAndBlock,
                                        system_bus_));
    system_bus_ = NULL;
    mce_signal_proxy_ = NULL;

    DVLOG(1) << "MCE memory pressure monitor stopped";
  }

 private:
  bool OnWorkerThread() {
    return task_runner()->BelongsToCurrentThread();
  }

  void NotifyLevelUpdated(const std::string& level) {
    VLOG(1) << "Memory pressure level changed to: " << level;

    MemoryPressureLevel new_level;
    if (level ==  kMceMemoryPressureNormal) {
      new_level = MemoryPressureListener::MEMORY_PRESSURE_LEVEL_NONE;
    } else if (level == kMceMemoryPressureWarning) {
      new_level = MemoryPressureListener::MEMORY_PRESSURE_LEVEL_MODERATE;
    } else if (level == kMceMemoryPressureCritical) {
      new_level = MemoryPressureListener::MEMORY_PRESSURE_LEVEL_CRITICAL;
    }
    callback_.Run(new_level);
  }

  void OnMemoryPressureSignal(dbus::Signal* signal) {
    DCHECK(OnWorkerThread());

    dbus::MessageReader reader(signal);
    std::string level;
    reader.PopString(&level);

    DCHECK(reader.HasMoreData());
    reader.PopString(&level);
    DCHECK(!reader.HasMoreData());

    NotifyLevelUpdated(level);
  }

  void OnInitialLevelResponse(dbus::Response* response) {
    DCHECK(OnWorkerThread());

    if (!response) {
      LOG(ERROR) << "Failed to get initial memory pressure level from MCE!";
      return;
    }

    dbus::MessageReader reader(response);
    std::string level;

    DCHECK(reader.HasMoreData());
    reader.PopString(&level);
    DCHECK(!reader.HasMoreData());

    NotifyLevelUpdated(level);
  }

  void OnConnected(const std::string& interface_name,
                   const std::string& signal_name,
                   bool success) {
    DVLOG(1) << "Connected to " << interface_name << ", signal: "
             << signal_name << ", success: " << success;
  }

  scoped_refptr<dbus::Bus> system_bus_;
  dbus::ObjectProxy* mce_signal_proxy_;
  base::MessageLoop* message_loop_;
  MemoryPressureUpdateCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(WorkerThread);
};

MemoryPressureMonitorMce::MemoryPressureMonitorMce()
    : current_pressure_level_(
        MemoryPressureListener::MEMORY_PRESSURE_LEVEL_NONE)
    , message_loop_(base::MessageLoop::current()) {

  WorkerThread::MemoryPressureUpdateCallback update_callback =
          base::Bind(&MemoryPressureMonitorMce::OnPressureLevelChanged,
                     base::Unretained(this));
  base::Thread::Options thread_options(base::MessageLoop::TYPE_IO, 0);
  worker_thread_.reset(new WorkerThread(update_callback));
  if (!worker_thread_->StartWithOptions(thread_options)) {
    worker_thread_.reset();
    LOG(ERROR) << "Could not start the " << kWorkerThreadName << " thread!";
    return;
  }

  worker_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&WorkerThread::StartListening,
                 base::Unretained(worker_thread_.get())));
  DVLOG(1) << "MCE memory pressure monitor created";
}

MemoryPressureMonitorMce::~MemoryPressureMonitorMce() {
  if (worker_thread_) {
    worker_thread_->message_loop()->PostTask(
        FROM_HERE,
        base::Bind(&WorkerThread::StopListening,
                   base::Unretained(worker_thread_.get())));
    worker_thread_->Stop();
  }

  DVLOG(1) << "MCE memory pressure monitor destroyed";
}

MemoryPressureListener::MemoryPressureLevel
MemoryPressureMonitorMce::GetCurrentPressureLevel() const {
  return current_pressure_level_;
}

void MemoryPressureMonitorMce::OnPressureLevelChanged(
    const MemoryPressureLevel& new_level) {
  current_pressure_level_ = new_level;

  message_loop_->PostTask(FROM_HERE,
      base::Bind(&MemoryPressureListener::NotifyMemoryPressure,
                 new_level));
}

} // namespace base

