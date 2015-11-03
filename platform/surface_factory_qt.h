// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_PLATFORM_SURFACE_FACTORY_QT
#define OZONE_QT_PLATFORM_SURFACE_FACTORY_QT

#include "ui/ozone/public/surface_factory_ozone.h"

namespace ui {

class SurfaceOzoneEGL;

class SurfaceFactoryQt : public SurfaceFactoryOzone {
 public:
  SurfaceFactoryQt();
  ~SurfaceFactoryQt() override;

  // Overrides for SurfaceFactoryOzone
  scoped_ptr<SurfaceOzoneEGL> CreateEGLSurfaceForWidget(
      gfx::AcceleratedWidget widget) override;
  bool LoadEGLGLES2Bindings(
      AddGLLibraryCallback add_gl_library,
      SetGLGetProcAddressProcCallback set_gl_get_proc_address) override;
  intptr_t GetNativeDisplay() override;
  const int32* GetEGLSurfaceProperties(const int32* desired_list) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(SurfaceFactoryQt);
};

} // namespace ui

#endif // OZONE_QT_PLATFORM_SURFACE_FACTORY_QT
