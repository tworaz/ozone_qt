// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/cursor_factory_ozone_qt.h"

#include <QtCore/QHash>
#include <QtGui/QCursor>

#include "ui/base/cursor/cursor.h"

namespace ui {

namespace {

QHash<Qt::CursorShape, QCursor*> g_cursor_hash;

QCursor* GetCursorForType(Qt::CursorShape shape) {
  if (!g_cursor_hash.contains(shape)) {
    g_cursor_hash.insert(shape, new QCursor(shape));
  }
  return g_cursor_hash[shape];
}

} // namespace

CursorFactoryOzoneQt::CursorFactoryOzoneQt()
    : CursorFactoryOzone() {
}

CursorFactoryOzoneQt::~CursorFactoryOzoneQt() {
}

PlatformCursor CursorFactoryOzoneQt::GetDefaultCursor(int type) {
  switch (type) {
    case kCursorNull:
      return nullptr;
    case kCursorPointer:
      return GetCursorForType(Qt::ArrowCursor);
    case kCursorHand:
      return GetCursorForType(Qt::PointingHandCursor);
    case kCursorIBeam:
      return GetCursorForType(Qt::IBeamCursor);
    case kCursorEastResize:
    case kCursorWestResize:
      return GetCursorForType(Qt::SizeHorCursor);
    case kCursorNorthResize:
    case kCursorSouthResize:
      return GetCursorForType(Qt::SizeVerCursor);
    case kCursorNorthWestResize:
    case kCursorSouthEastResize:
      return GetCursorForType(Qt::SizeFDiagCursor);
    case kCursorNorthEastResize:
    case kCursorSouthWestResize:
      return GetCursorForType(Qt::SizeBDiagCursor);
    default:
      NOTIMPLEMENTED() << "Handle cursor type: " << type;
      return nullptr;
  }
}

}
