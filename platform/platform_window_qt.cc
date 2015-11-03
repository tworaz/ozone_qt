// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/platform_window_qt.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QResizeEvent>
#include <qpa/qplatformnativeinterface.h>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "ozone_qt/platform/ozone_qt_switches.h"
#include "ozone_qt/platform/platform_event_source_qt.h"
#include "ozone_qt/util/type_conversion.h"
#include "ui/events/event.h"
#include "ui/events/ozone/events_ozone.h"
#include "ui/events/platform/platform_event_source.h"
#include "ui/platform_window/platform_window_delegate.h"

namespace ui {

namespace {

#if defined(OS_SAILFISH)
// Technically ozone_qt only works on EGL wayland platforms. On Sailfish
// however the EGL platform is simply reported as "wayland".
static QString kWaylandPlatformName = "wayland";
#else
static QString kWaylandPlatformName = "wayland-egl";
#endif

PlatformWindowQt* g_current_capture = nullptr;

} // namespace

PlatformWindowQt::PlatformWindowQt(PlatformEventSourceQt& event_source,
                                   PlatformWindowDelegate* delegate,
                                   const gfx::Rect& bounds)
    : event_source_(event_source)
    , delegate_(delegate)
    , need_swap_on_first_expose_(false)
    , support_pointer_capture_(true) {

  simulate_touch_events_ = base::CommandLine::ForCurrentProcess()->
      HasSwitch(switches::kSimulateTouchScreenWithMouse);

  QWindow::resize(ToQt(bounds.size()));
  QWindow::setSurfaceType(QSurface::OpenGLSurface);
#if defined(OS_SAILFISH)
  QSurfaceFormat format(QWindow::requestedFormat());
  format.setAlphaBufferSize(0);
  QWindow::setFormat(format);
#endif
  QWindow::create();

  PlatformInit();

  connect(this, &QWindow::windowStateChanged,
          this, &PlatformWindowQt::OnWindowStateChanged);

  ui::PlatformEventSource::GetInstance()->AddPlatformEventDispatcher(this);

  // XXX: Scale factor is currently unused, but this may change in the future.
  delegate_->OnAcceleratedWidgetAvailable(
      reinterpret_cast<gfx::AcceleratedWidget>(this), 1.f);
}

PlatformWindowQt::~PlatformWindowQt() {
  ui::PlatformEventSource::GetInstance()->RemovePlatformEventDispatcher(this);
  ReleaseCapture();
  delegate_ = nullptr;
}

void PlatformWindowQt::Show() {
  QWindow::show();
}
void PlatformWindowQt::Hide() {
  QWindow::hide();
}

void PlatformWindowQt::Close() {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::SetBounds(const gfx::Rect& bounds) {
  DVLOG(1) << "New PlatformWindow bounds:" << bounds.ToString();
  QWindow::resize(ToQt(bounds.size()));
  delegate_->OnBoundsChanged(bounds);
}

gfx::Rect PlatformWindowQt::GetBounds() {
  return ToGfx(QWindow::geometry());
}

void PlatformWindowQt::SetTitle(const base::string16& title) {
  QWindow::setTitle(ToQt(title));
}

void PlatformWindowQt::SetCapture() {
  if (!support_pointer_capture_) {
    return;
  }
  if (g_current_capture) {
    g_current_capture->ReleaseCapture();
  }
  g_current_capture = this;
  QWindow::setMouseGrabEnabled(true);
}

void PlatformWindowQt::ReleaseCapture() {
  if (!support_pointer_capture_) {
    return;
  }
  if (g_current_capture == this) {
    QWindow::setMouseGrabEnabled(false);
    g_current_capture = nullptr;
    delegate_->OnLostCapture();
  }
}

void PlatformWindowQt::ToggleFullscreen() {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::Maximize() {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::Minimize() {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::Restore() {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::SetCursor(PlatformCursor cursor) {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::MoveCursorTo(const gfx::Point& location) {
  NOTIMPLEMENTED();
}

void PlatformWindowQt::ConfineCursorToBounds(const gfx::Rect& bounds) {
  NOTIMPLEMENTED();
}

PlatformImeController* PlatformWindowQt::GetPlatformImeController() {
  NOTIMPLEMENTED();
  return nullptr;
}

bool PlatformWindowQt::CanDispatchEvent(const PlatformEvent& event) {
  // XXX: This won't work in multi window setups.
  return true;
}

uint32_t PlatformWindowQt::DispatchEvent(const PlatformEvent& native_event) {
  DispatchEventFromNativeUiEvent(
      native_event, base::Bind(&PlatformWindowDelegate::DispatchEvent,
                               base::Unretained(delegate_)));
  return POST_DISPATCH_STOP_PROPAGATION;
}

void PlatformWindowQt::OnWindowStateChanged(Qt::WindowState state) {
  DVLOG(1) << "Window state changed:" << state;
  PlatformWindowState platform_state;
  switch(state) {
    case Qt::WindowNoState:
    case Qt::WindowActive:
      platform_state = PLATFORM_WINDOW_STATE_NORMAL;
      break;
    case Qt::WindowMinimized:
      platform_state = PLATFORM_WINDOW_STATE_MINIMIZED;
      break;
    case Qt::WindowMaximized:
      platform_state = PLATFORM_WINDOW_STATE_MAXIMIZED;
      break;
    case Qt::WindowFullScreen:
      platform_state = PLATFORM_WINDOW_STATE_FULLSCREEN;
      break;
    default:
      LOG(ERROR) << "Unhandled Qt window state: " << state;
      platform_state = PLATFORM_WINDOW_STATE_UNKNOWN;
      break;
  }
  delegate_->OnWindowStateChanged(platform_state);
}

bool PlatformWindowQt::event(QEvent* event) {
  if (event->type() == QEvent::Close) {
    delegate_->OnCloseRequest();
    // XXX: How to properly handle window close?
    return true;
  }
  return QWindow::event(event);
}

void PlatformWindowQt::exposeEvent(QExposeEvent* event) {
  if (QWindow::isExposed()) {
    static bool first_expose = true;
    if (need_swap_on_first_expose_ && first_expose) {
      QOpenGLContext context;
      context.create();
      context.makeCurrent(this);
      context.swapBuffers(this);
      context.doneCurrent();
      first_expose = false;
    }
    delegate_->OnDamageRect(ToGfx(event->region().boundingRect()));
  }
}

void PlatformWindowQt::keyPressEvent(QKeyEvent* event) {
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::keyReleaseEvent(QKeyEvent* event) {
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::resizeEvent(QResizeEvent* event) {
  delegate_->OnBoundsChanged(gfx::Rect(ToGfx(event->size())));
}

void PlatformWindowQt::mouseMoveEvent(QMouseEvent* event) {
  if (simulate_touch_events_) {
    event->ignore();
    return;
  }
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::mousePressEvent(QMouseEvent* event) {
  if (simulate_touch_events_) {
    event->ignore();
    return;
  }
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::mouseReleaseEvent(QMouseEvent* event) {
  if (simulate_touch_events_) {
    event->ignore();
    return;
  }
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::touchEvent(QTouchEvent* event) {
  event_source_.ProcessEvent(*event);
  event->setAccepted(true);
}

void PlatformWindowQt::wheelEvent(QWheelEvent* event) {
  if (simulate_touch_events_) {
    event->ignore();
    return;
  }
  event_source_.ProcessEvent(*event);
}

void PlatformWindowQt::PlatformInit() {
  QString platform = qApp->platformName().toLower();

  if (platform == kWaylandPlatformName) {
    // Only chromium should draw to this QWindow. Client side decorations
    // wouldn't work in current setup.
    QWindow::setFlags(QWindow::flags() | Qt::FramelessWindowHint);

    // On wayland we need to trigger swap buffers in order to trigger
    // wl_egl_window creation. Without this SurfaceOzoneEGLQt::GetNativeWindow
    // will fail.
    need_swap_on_first_expose_ = true;

    // Qt wayland platform plugin does not support input capture for top level
    // windows.
    support_pointer_capture_ = false;
  }
}

} // namespace ui
