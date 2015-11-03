// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_PLATFORM_SURFACE_OZONE_EGL_QT_H_
#define OZONE_QT_PLATFORM_SURFACE_OZONE_EGL_QT_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/ozone/public/surface_ozone_egl.h"

class QWindow;

namespace ui {

class SurfaceOzoneEGLQt : public SurfaceOzoneEGL {
 public:
  SurfaceOzoneEGLQt(gfx::AcceleratedWidget);
  ~SurfaceOzoneEGLQt() override;

  // SurfaceOzoneEGL:
  intptr_t GetNativeWindow() override;
  bool ResizeNativeWindow(const gfx::Size&) override;
  bool OnSwapBuffers() override;
  bool OnSwapBuffersAsync(const SwapCompletionCallback& callback) override;
  scoped_ptr<gfx::VSyncProvider> CreateVSyncProvider() override;

 private:
  QWindow* window_; // Not owned

  DISALLOW_COPY_AND_ASSIGN(SurfaceOzoneEGLQt);
};

} // namespace ui

#endif // OZONE_QT_PLATFORM_SURFACE_OZONE_EGL_QT_H_
