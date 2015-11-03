// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/components/aura_client/screen_position_client_qt.h"

#include <QtCore/QPoint>
#include <QtGui/QWindow>

#include "base/logging.h"
#include "ozone_qt/util/type_conversion.h"

namespace aura {
namespace client {

ScreenPositionClientQt::ScreenPositionClientQt(QWindow& window)
    : window_(window) {
}

ScreenPositionClientQt::~ScreenPositionClientQt() {
}

void ScreenPositionClientQt::ConvertPointToScreen(const aura::Window* window,
                                                  gfx::Point* point) {
  QPoint q_point = ToQt(*point);
  q_point = window_.mapToGlobal(q_point);
  *point = ToGfx(q_point);
}

void ScreenPositionClientQt::ConvertPointFromScreen(const aura::Window* window,
                                                    gfx::Point* point) {
  QPoint q_point = ToQt(*point);
  q_point = window_.mapFromGlobal(q_point);
  *point = ToGfx(q_point);
}
void ScreenPositionClientQt::ConvertHostPointToScreen(aura::Window* root_window,
                                                      gfx::Point* point) {
  NOTIMPLEMENTED();
}

void ScreenPositionClientQt::SetBounds(aura::Window* window,
                                       const gfx::Rect& bounds,
                                       const gfx::Display& display) {
  NOTIMPLEMENTED();
}

} // namespace client
} // namespace aura
