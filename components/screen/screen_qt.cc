// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/components/screen/screen_qt.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

#include "ozone_qt/util/type_conversion.h"
#include "ui/gfx/display.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/geometry/size_conversions.h"

namespace gfx {

namespace {

bool IsRotationPortrait(Display::Rotation rotation) {
  return rotation == Display::ROTATE_90 ||
         rotation == Display::ROTATE_270;
}

} // namespace

ScreenQt::ScreenQt() {
  QGuiApplication* app = static_cast<QGuiApplication*>(
      QGuiApplication::instance());
  const QScreen* screen = app->primaryScreen();
  Display display(0);

  const float device_scale_factor =
      Display::HasForceDeviceScaleFactor() ?
      Display::GetForcedDeviceScaleFactor() : screen->devicePixelRatio();

  Display::SetInternalDisplayId(1);
  display_.set_id(1);
  display_.set_rotation(Display::ROTATE_0);
  natural_bounds_in_pixels_ = ToGfx(screen->availableGeometry());
  display_.SetScaleAndBounds(device_scale_factor, natural_bounds_in_pixels_);
}

ScreenQt::~ScreenQt() {
}

void ScreenQt::SetRotation(Display::Rotation rotation) {
  if (display_.rotation() != rotation) {
    Rect bounds_in_pixels;
    if (IsRotationPortrait(rotation)) {
      bounds_in_pixels = Rect(natural_bounds_in_pixels_.width(),
                              natural_bounds_in_pixels_.height());
    } else {
      bounds_in_pixels = Rect(natural_bounds_in_pixels_.height(),
                              natural_bounds_in_pixels_.width());
    }

    display_.set_rotation(rotation);
    display_.SetScaleAndBounds(display_.device_scale_factor(),
                               bounds_in_pixels);
    FOR_EACH_OBSERVER(DisplayObserver, observer_list_,
        OnDisplayMetricsChanged(display_,
            DisplayObserver::DISPLAY_METRIC_ROTATION));
  }
}

Point ScreenQt::GetCursorScreenPoint() {
  return Point();
}

NativeWindow ScreenQt::GetWindowUnderCursor() {
  NOTIMPLEMENTED();
  return NULL;
}

NativeWindow ScreenQt::GetWindowAtScreenPoint(const Point& point) {
  NOTIMPLEMENTED();
  return NULL;
}

Display ScreenQt::GetPrimaryDisplay() const {
  return display_;
}

Display ScreenQt::GetDisplayNearestWindow(NativeView view) const {
  return GetPrimaryDisplay();
}

Display ScreenQt::GetDisplayNearestPoint(const Point& point) const {
  return GetPrimaryDisplay();
}

int ScreenQt::GetNumDisplays() const {
  return 1;
}

std::vector<Display> ScreenQt::GetAllDisplays() const {
  return std::vector<Display>(1, GetPrimaryDisplay());
}

Display ScreenQt::GetDisplayMatching(const Rect& match_rect) const {
  return GetPrimaryDisplay();
}

void ScreenQt::AddObserver(DisplayObserver* observer) {
  observer_list_.AddObserver(observer);
}

void ScreenQt::RemoveObserver(DisplayObserver* observer) {
  observer_list_.RemoveObserver(observer);
}

}  // namespace gfx
