// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_PLATFORM_PLATFORM_EVENT_SOURCE_QT_H_
#define OZONE_QT_PLATFORM_PLATFORM_EVENT_SOURCE_QT_H_

#include "ui/events/platform/platform_event_source.h"
#include "ui/gfx/transform.h"

namespace ui {

class PlatformEventSourceQt : public PlatformEventSource {
 public:
  PlatformEventSourceQt();
  ~PlatformEventSourceQt() override;

  template<class T>
  void ProcessEvent(const T&);
  template<class T>
  void ProcessEvent(const T&, const gfx::Transform&);

 private:
  void DetectDevices() const;

  DISALLOW_COPY_AND_ASSIGN(PlatformEventSourceQt);
};

} // namespace ui

#endif // !OZONE_QT_PLATFORM_PLATFORM_EVENT_SOURCE_QT_H_

