// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_COMPONENTS_SCREEN_SCREEN_QT_H_
#define OZONE_QT_COMPONENTS_SCREEN_SCREEN_QT_H_

#include "base/observer_list.h"
#include "ui/gfx/display.h"
#include "ui/gfx/screen.h"

namespace gfx {

class ScreenQt : public Screen {
 public:
  ScreenQt();
  ~ScreenQt() override;

  void SetRotation(Display::Rotation);

  // gfx::Screen:
  Point GetCursorScreenPoint() override;
  NativeWindow GetWindowUnderCursor() override;
  NativeWindow GetWindowAtScreenPoint(const Point& point) override;
  Display GetPrimaryDisplay() const override;
  Display GetDisplayNearestWindow(NativeView view) const override;
  Display GetDisplayNearestPoint(const Point& point) const override;
  int GetNumDisplays() const override;
  std::vector<Display> GetAllDisplays() const override;
  Display GetDisplayMatching(const Rect& match_rect) const override;
  void AddObserver(DisplayObserver* observer) override;
  void RemoveObserver(DisplayObserver* observer) override;

 private:
  Display display_;
  Rect natural_bounds_in_pixels_;
  base::ObserverList<DisplayObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(ScreenQt);
};

} // namespace gfx

#endif // OZONE_QT_COMPONENTS_SCREEN_SCREEN_QT_H_
