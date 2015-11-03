// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_COMPONENTS_MCE_MEMORY_PRESSURE_MONITOR_MCE_H_
#define OZONE_QT_COMPONENTS_MCE_MEMORY_PRESSURE_MONITOR_MCE_H_

#include "base/memory/memory_pressure_monitor.h"

namespace base {

class MessageLoop;

class MemoryPressureMonitorMce : public MemoryPressureMonitor {
 public:
  MemoryPressureMonitorMce();
  ~MemoryPressureMonitorMce() override;

  // MemoryPressureMonitor:
  MemoryPressureLevel GetCurrentPressureLevel() const override;

 private:
  void OnPressureLevelChanged(const MemoryPressureLevel&);

  class WorkerThread;

  MemoryPressureLevel current_pressure_level_;
  base::MessageLoop* message_loop_;
  scoped_ptr<WorkerThread> worker_thread_;

  DISALLOW_COPY_AND_ASSIGN(MemoryPressureMonitorMce);
};

} // namespace base

#endif // OZONE_QT_COMPONENTS_MCE_MEMORY_PRESSURE_MONITOR_MCE_H_
