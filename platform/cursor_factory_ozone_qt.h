// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_PLATFORM_CURSOR_FACTORY_OZONE_QT_H_
#define OZONE_QT_PLATFORM_CURSOR_FACTORY_OZONE_QT_H_

#include "ui/ozone/public/cursor_factory_ozone.h"

namespace ui {

class CursorFactoryOzoneQt : public CursorFactoryOzone {
 public:
  CursorFactoryOzoneQt();
  ~CursorFactoryOzoneQt() override;

  // CursorFactoryOzone overrides
  PlatformCursor GetDefaultCursor(int type) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(CursorFactoryOzoneQt);
};


} // namespace ui

#endif // OZONE_QT_PLATFORM_CURSOR_FACTORY_OZONE_QT_H_
