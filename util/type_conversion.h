// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_UTIL_TYPE_CONVERSION_H_
#define OZONE_QT_UTIL_TYPE_CONVERSION_H_

#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtGui/QVector2D>

#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/vector2d_f.h"
#include "url/gurl.h"

inline QString ToQt(const base::string16& string) {
  return QString::fromUtf16(string.data());
}

inline QString ToQt(const std::string &string) {
    return QString::fromStdString(string);
}

inline base::string16 toString16(const QString& qString) {
  return base::string16(qString.utf16());
}

inline QUrl ToQt(const GURL& url) {
  return QUrl(QString::fromStdString(url.spec()));
}

inline GURL toGurl(const QUrl& url) {
  return GURL(url.toString().toStdString());
}

inline QPoint ToQt(const gfx::Point& point) {
  return QPoint(point.x(), point.y());
}

inline gfx::Point ToGfx(const QPoint& point) {
  return gfx::Point(point.x(), point.y());
}

inline gfx::PointF ToGfx(const QPointF& point) {
  return gfx::PointF(point.x(), point.y());
}

inline QPointF ToQt(const gfx::PointF& point) {
  return QPointF(point.x(), point.y());
}

inline QRect ToQt(const gfx::Rect& rect) {
  return QRect(rect.x(), rect.y(), rect.width(), rect.height());
}

inline QRectF ToQt(const gfx::RectF& rect) {
  return QRectF(rect.x(), rect.y(), rect.width(), rect.height());
}

inline QSize ToQt(const gfx::Size& size) {
  return QSize(size.width(), size.height());
}

inline gfx::SizeF ToGfx(const QSizeF& size) {
  return gfx::SizeF(size.width(), size.height());
}

inline gfx::Rect ToGfx(const QRect& rect) {
  return gfx::Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

inline gfx::RectF ToGfx(const QRectF& rect) {
  return gfx::RectF(rect.x(), rect.y(), rect.width(), rect.height());
}

inline QSizeF ToQt(const gfx::SizeF& size) {
  return QSizeF(size.width(), size.height());
}

inline gfx::Size ToGfx(const QSize& size) {
  return gfx::Size(size.width(), size.height());
}

inline QVector2D ToQt(const gfx::Vector2dF& vec) {
  return QVector2D(vec.x(), vec.y());
}

#endif // !OZONE_QT_UTIL_TYPE_CONVERSION_H_
