// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/events/gesture_detection/gesture_configuration.h"

#include "base/command_line.h"
#include "base/memory/singleton.h"
#include "ui/events/event_switches.h"

namespace ui {
namespace {

class GestureConfigurationQt : public GestureConfiguration {
 public:
  ~GestureConfigurationQt() override {
  }

  static GestureConfigurationQt* GetInstance() {
    return base::Singleton<GestureConfigurationQt>::get();
  }

 private:
  GestureConfigurationQt() : GestureConfiguration() {
    set_double_tap_enabled(true);
    set_double_tap_timeout_in_ms(300);
    set_max_distance_between_taps_for_double_tap(40);
    set_gesture_begin_end_types_enabled(true);
    set_min_gesture_bounds_length(default_radius());
    set_min_scaling_touch_major(default_radius() * 2);
    set_span_slop(max_touch_move_in_pixels_for_click() * 2);
    set_swipe_enabled(true);
    set_two_finger_tap_enabled(true);

    set_min_pinch_update_span_delta(
        base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kCompensateForUnstablePinchZoom) ? 5 : 0);
  }

  friend struct base::DefaultSingletonTraits<GestureConfigurationQt>;
  DISALLOW_COPY_AND_ASSIGN(GestureConfigurationQt);
};

} // namespace

GestureConfiguration* GestureConfiguration::GetPlatformSpecificInstance() {
  return GestureConfigurationQt::GetInstance();
}

} // namespace ui

