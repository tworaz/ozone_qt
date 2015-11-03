// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_COMPONENTS_SCREEN_SCREEN_POSITION_CLIENT_QT_H_
#define OZONE_QT_COMPONENTS_SCREEN_SCREEN_POSITION_CLIENT_QT_H_

#include "ui/aura/client/screen_position_client.h"

class QWindow;

namespace aura {
namespace client {

class ScreenPositionClientQt : public ScreenPositionClient {
 public:
  ScreenPositionClientQt(QWindow&);
  ~ScreenPositionClientQt() override;

  // aura::client::ScreenPositionClient:
  void ConvertPointToScreen(const aura::Window* window,
                            gfx::Point* point) override;
  void ConvertPointFromScreen(const aura::Window* window,
                              gfx::Point* point) override;
  void ConvertHostPointToScreen(aura::Window* root_window,
                                gfx::Point* point) override;
  void SetBounds(aura::Window* window,
                 const gfx::Rect& bounds,
                 const gfx::Display& display) override;

 private:
  QWindow& window_;

  DISALLOW_COPY_AND_ASSIGN(ScreenPositionClientQt);
};

} // namespace client
} // namespace aura


#endif // !OZONE_QT_COMPONENTS_SCREEN_SCREEN_POSITION_CLIENT_QT_H_
