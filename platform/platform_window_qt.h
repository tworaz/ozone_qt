// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_PLATFORM_PLATFORM_WINDOW_QT_H_
#define OZONE_QT_PLATFORM_PLATFORM_WINDOW_QT_H_

#include <QtGui/QWindow>

#include "base/macros.h"
#include "ui/events/platform/platform_event_dispatcher.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/transform.h"
#include "ui/platform_window/platform_window.h"

class QWindow;

namespace ui {

class Event;
class PlatformEventSourceQt;
class PlatformWindowDelegate;

class PlatformWindowQt
    : public QWindow
    , public PlatformWindow
    , public PlatformEventDispatcher {
  Q_OBJECT

 public:
  PlatformWindowQt(PlatformEventSourceQt&,
                   PlatformWindowDelegate*,
                   const gfx::Rect&);
  ~PlatformWindowQt() override;

  // PlatformWindow:
  void Show() override;
  void Hide() override;
  void Close() override;
  void SetBounds(const gfx::Rect& bounds) override;
  gfx::Rect GetBounds() override;
  void SetTitle(const base::string16& title) override;
  void SetCapture() override;
  void ReleaseCapture() override;
  void ToggleFullscreen() override;
  void Maximize() override;
  void Minimize() override;
  void Restore() override;
  void SetCursor(PlatformCursor cursor) override;
  void MoveCursorTo(const gfx::Point& location) override;
  void ConfineCursorToBounds(const gfx::Rect& bounds) override;
  PlatformImeController* GetPlatformImeController() override;

  // PlatformEventDispatcher:
  bool CanDispatchEvent(const PlatformEvent& event) override;
  uint32_t DispatchEvent(const PlatformEvent& event) override;

 private slots:
  void OnWindowStateChanged(Qt::WindowState);

 private:
  // QWindow:
  bool event(QEvent*) override;
  void exposeEvent(QExposeEvent*) override;
  void keyPressEvent(QKeyEvent*) override;
  void keyReleaseEvent(QKeyEvent*) override;
  void resizeEvent(QResizeEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void touchEvent(QTouchEvent*) override;
  void wheelEvent(QWheelEvent*) override;

  void PlatformInit();

  // Not owned:
  PlatformEventSourceQt& event_source_;
  PlatformWindowDelegate* delegate_;
  bool need_swap_on_first_expose_;
  bool support_pointer_capture_;

  // Should we simulate touch events when using mouse.
  bool simulate_touch_events_;

  DISALLOW_COPY_AND_ASSIGN(PlatformWindowQt);
};

} // namespace ui

#endif // !OZONE_QT_PLATFORM_PLATFORM_WINDOW_QT_H_
