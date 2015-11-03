// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/ozone_platform_qt.h"

#include <QtGui/QGuiApplication>

#include "base/command_line.h"
#include "ozone_qt/platform/cursor_factory_ozone_qt.h"
#include "ozone_qt/platform/overlay_manager_ozone_qt.h"
#include "ozone_qt/platform/ozone_qt_switches.h"
#include "ozone_qt/platform/platform_event_source_qt.h"
#include "ozone_qt/platform/platform_window_qt.h"
#include "ozone_qt/platform/surface_factory_qt.h"
#include "ui/display/types/native_display_delegate.h"
#include "ui/ozone/public/gpu_platform_support.h"
#include "ui/ozone/public/gpu_platform_support_host.h"
#include "ui/ozone/public/system_input_injector.h"

namespace ui {

OzonePlatformQt::OzonePlatformQt()
    : OzonePlatform()
    , qt_app_(nullptr) {
}

OzonePlatformQt::~OzonePlatformQt() {
}

SurfaceFactoryOzone* OzonePlatformQt::GetSurfaceFactoryOzone() {
  return surface_factory_.get();
}

OverlayManagerOzone* OzonePlatformQt::GetOverlayManager() {
  return overlay_manager_.get();
}

CursorFactoryOzone* OzonePlatformQt::GetCursorFactoryOzone() {
  return cursor_factory_.get();
}

InputController* OzonePlatformQt::GetInputController() {
  NOTREACHED();
  return NULL;
}

GpuPlatformSupport* OzonePlatformQt::GetGpuPlatformSupport() {
  return gpu_platform_.get();
}

GpuPlatformSupportHost* OzonePlatformQt::GetGpuPlatformSupportHost() {
  return gpu_platform_host_.get();
}

scoped_ptr<SystemInputInjector> OzonePlatformQt::CreateSystemInputInjector() {
  NOTREACHED();
  return NULL;
}

scoped_ptr<PlatformWindow> OzonePlatformQt::CreatePlatformWindow(
    PlatformWindowDelegate* delegate,
    const gfx::Rect& bounds) {
  scoped_ptr<PlatformWindow> platform_window(
      new PlatformWindowQt(*event_source_.get(), delegate, bounds));
  return platform_window.Pass();
}

scoped_ptr<ui::NativeDisplayDelegate>
OzonePlatformQt::CreateNativeDisplayDelegate() {
  NOTREACHED();
  return NULL;
}

base::ScopedFD OzonePlatformQt::OpenClientNativePixmapDevice() const {
  return base::ScopedFD();
}

void OzonePlatformQt::InitializeUI() {
  static int argc = 0;
  qt_app_.reset(new QGuiApplication(argc, 0));
  qt_app_->setQuitOnLastWindowClosed(true);
  gpu_platform_host_.reset(CreateStubGpuPlatformSupportHost());
  cursor_factory_.reset(new CursorFactoryOzoneQt);
  overlay_manager_.reset(new OverlayManagerOzoneQt);
  event_source_.reset(new PlatformEventSourceQt);

  if (base::CommandLine::ForCurrentProcess()->
          HasSwitch(switches::kSimulateTouchScreenWithMouse)) {
    qt_app_->setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);
    qt_app_->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);
  }
}

void OzonePlatformQt::InitializeGPU() {
  surface_factory_.reset(new SurfaceFactoryQt);
  gpu_platform_.reset(CreateStubGpuPlatformSupport());
}

OzonePlatform* CreateOzonePlatformQt() {
  return new OzonePlatformQt;
}

} // namespace ui
