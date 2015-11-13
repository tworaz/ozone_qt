// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/surface_ozone_egl_qt.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QWindow>
#include <qpa/qplatformnativeinterface.h>

#include "ui/gfx/geometry/size.h"
#include "ui/gfx/vsync_provider.h"

namespace ui {

static const QString kX11PlatformName = "xcb";
#if defined(OS_SAILFISH)
// Technically ozone_qt only works on EGL wayland platforms. On Sailfish
// however the EGL platform is simply reported as "wayland".
static QString kWaylandPlatformName = "wayland";
#else
static QString kWaylandPlatformName = "wayland-egl";
#endif

SurfaceOzoneEGLQt::SurfaceOzoneEGLQt(gfx::AcceleratedWidget widget)
    : window_(reinterpret_cast<QWindow*>(widget)) {
}

SurfaceOzoneEGLQt::~SurfaceOzoneEGLQt() {
  window_ = nullptr;
}

intptr_t SurfaceOzoneEGLQt::GetNativeWindow() {
  QString platform = qApp->platformName().toLower();
  if (platform == kX11PlatformName) {
    return window_->winId();
  } else if (platform == kWaylandPlatformName) {
    QPlatformNativeInterface* pni = QGuiApplication::platformNativeInterface();
    void* wl_egl_window = pni->nativeResourceForWindow(
        QByteArrayLiteral("wl_egl_window"), window_);
    DCHECK(wl_egl_window);
    return reinterpret_cast<intptr_t>(wl_egl_window);
  }
  CHECK(false) << "Unsupported platform: " << qPrintable(platform);
  return 0;
}

bool SurfaceOzoneEGLQt::ResizeNativeWindow(const gfx::Size& size) {
  window_->resize(size.width(), size.height());
  return true;
}

bool SurfaceOzoneEGLQt::OnSwapBuffers() {
  return true;
}

void SurfaceOzoneEGLQt::OnSwapBuffersAsync(
    const SwapCompletionCallback& callback) {
  callback.Run(gfx::SwapResult::SWAP_ACK);
}

scoped_ptr<gfx::VSyncProvider> SurfaceOzoneEGLQt::CreateVSyncProvider() {
  return nullptr;
}

} // namespace ui
