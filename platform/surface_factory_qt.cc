// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "surface_factory_qt.h"

#include <QtGui/QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "base/command_line.h"
#include "ozone_qt/platform/surface_ozone_egl_qt.h"
#include "ui/gl/gl_implementation.h"
#include "ui/ozone/common/egl_util.h"
#include "ui/ozone/public/surface_ozone_egl.h"

namespace ui {

namespace {

void* NativeResourceForIntegration(const QByteArray& resource) {
  return qApp->platformNativeInterface()->
      nativeResourceForIntegration(resource);
}

void* NativeResourceForScreen(const QByteArray& resource) {
  return qApp->platformNativeInterface()->
      nativeResourceForScreen(resource, qApp->primaryScreen());
}

} // namespace

SurfaceFactoryQt::SurfaceFactoryQt()
    : SurfaceFactoryOzone() {
}

SurfaceFactoryQt::~SurfaceFactoryQt() {
}

scoped_ptr<SurfaceOzoneEGL> SurfaceFactoryQt::CreateEGLSurfaceForWidget(
    gfx::AcceleratedWidget widget) {
  scoped_ptr<SurfaceOzoneEGLQt> surface(new SurfaceOzoneEGLQt(widget));
  return surface.Pass();
}

bool SurfaceFactoryQt::LoadEGLGLES2Bindings(
    AddGLLibraryCallback add_gl_library,
    SetGLGetProcAddressProcCallback set_gl_get_proc_address) {
  return LoadDefaultEGLGLES2Bindings(add_gl_library, set_gl_get_proc_address);
}

intptr_t SurfaceFactoryQt::GetNativeDisplay() {
  static intptr_t display = 0;
  if (display)
    return display;

  void* nativedisp = NativeResourceForIntegration(
      QByteArrayLiteral("nativedisplay"));
  if (nativedisp) {
    display =  reinterpret_cast<intptr_t>(nativedisp);
    return display;
  }

  nativedisp = NativeResourceForScreen(QByteArrayLiteral("display"));
  if (nativedisp) {
    display =  reinterpret_cast<intptr_t>(nativedisp);
    return display;
  }

  NOTREACHED() << "No default display found!";
  return 0;
}

const int32* SurfaceFactoryQt::GetEGLSurfaceProperties(
    const int32* desired_list) {
#if defined(OS_SAILFISH)
  return desired_list;
#else
  EGLint renderable_type = EGL_OPENGL_ES2_BIT;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableUnsafeES3APIs)) {
    renderable_type = EGL_OPENGL_ES3_BIT_KHR;
  }
  static const EGLint kConfigAttribs[] = {
      EGL_BUFFER_SIZE, 32,
      EGL_ALPHA_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_RED_SIZE, 8,
      // According to egl spec depth size defaulted to zero and smallest size
      // preffered. Force depth to 24 bits to have same depth buffer on different
      // platforms.
      EGL_DEPTH_SIZE, 24,
      EGL_RENDERABLE_TYPE, renderable_type,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
  };
  return kConfigAttribs;
#endif
}

} // namespace ui

