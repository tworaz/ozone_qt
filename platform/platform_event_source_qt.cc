// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/platform_event_source_qt.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QTouchDevice>

#include "base/command_line.h"
#include "ozone_qt/platform/ozone_qt_switches.h"
#include "ozone_qt/platform/platform_window_qt.h"
#include "ozone_qt/util/type_conversion.h"
#include "ui/events/devices/device_data_manager.h"
#include "ui/events/event.h"
#include "ui/events/event_utils.h"
#include "ui/events/gesture_detection/motion_event.h"
#include "ui/events/keycodes/dom/dom_code.h"
#include "ui/events/keycodes/dom/dom_key.h"
#include "ui/events/keycodes/dom/keycode_converter.h"
#include "ui/events/keycodes/keyboard_code_conversion.h"
#include "ui/gfx/geometry/safe_integer_conversions.h"

namespace ui {

namespace {

const char kFakeTouchScreenName[] = "OzoneQt fake touch screen";

class KeyEventQt : public KeyEvent {
 public:
  KeyEventQt(EventType type,
             KeyboardCode key_code,
             DomCode code,
             int flags,
             DomKey key,
             base::char16 character,
             base::TimeDelta time_stamp)
      : KeyEvent(type, key_code, code, flags, key, time_stamp) {
    set_character(character);
  }

  void SetIsChar(bool is_char) {
    set_is_char(is_char);
  }
};

struct {
  int qt_key;
  KeyboardCode key_code;
} kQtToKeyboardCodeMap[] = {
  { Qt::Key_Space,             VKEY_SPACE },
  { Qt::Key_Left,              VKEY_LEFT },
  { Qt::Key_Right,             VKEY_RIGHT },
  { Qt::Key_Up,                VKEY_UP },
  { Qt::Key_Down,              VKEY_DOWN },
  { Qt::Key_Escape,            VKEY_ESCAPE },
  { Qt::Key_Tab,               VKEY_TAB },
  { Qt::Key_Backtab,           VKEY_BACKTAB },
  { Qt::Key_Backspace,         VKEY_BACK },
  { Qt::Key_Return,            VKEY_RETURN },
  { Qt::Key_Enter,             VKEY_RETURN },
  { Qt::Key_Insert,            VKEY_INSERT },
  { Qt::Key_Delete,            VKEY_DELETE },
  { Qt::Key_Pause,             VKEY_PAUSE },
  { Qt::Key_Print,             VKEY_PRINT },
  { Qt::Key_Clear,             VKEY_CLEAR },
  { Qt::Key_Home,              VKEY_HOME },
  { Qt::Key_End,               VKEY_END },
  { Qt::Key_Shift,             VKEY_LSHIFT },
  { Qt::Key_Control,           VKEY_LCONTROL },
  { Qt::Key_AltGr,             VKEY_ALTGR },
  { Qt::Key_NumLock,           VKEY_NUMLOCK },
  { Qt::Key_CapsLock,          VKEY_CAPITAL },
  { Qt::Key_Alt,               VKEY_LMENU },
  { Qt::Key_Meta,              VKEY_LWIN },
  { Qt::Key_PageUp,            VKEY_PRIOR },
  { Qt::Key_PageDown,          VKEY_NEXT },
  { Qt::Key_ScrollLock,        VKEY_SCROLL },
  { Qt::Key_Super_R,           VKEY_OEM_8 },
  { Qt::Key_Super_L,           VKEY_OEM_8 },
  { Qt::Key_Plus,              VKEY_OEM_PLUS },
  { Qt::Key_Comma,             VKEY_OEM_COMMA },
  { Qt::Key_Minus,             VKEY_OEM_MINUS },
  { Qt::Key_Period,            VKEY_OEM_PERIOD },
  { Qt::Key_Slash,             VKEY_DIVIDE },
  { Qt::Key_Help,              VKEY_HELP },
  { Qt::Key_MonBrightnessUp,   VKEY_BRIGHTNESS_UP },
  { Qt::Key_MonBrightnessDown, VKEY_BRIGHTNESS_DOWN },
  { Qt::Key_WLAN,              VKEY_WLAN },
  { Qt::Key_PowerOff,          VKEY_POWER },
  { Qt::Key_PowerDown,         VKEY_POWER },
  { Qt::Key_VolumeMute,        VKEY_VOLUME_MUTE },
  { Qt::Key_VolumeDown,        VKEY_VOLUME_DOWN },
  { Qt::Key_VolumeUp,          VKEY_VOLUME_UP },
  { Qt::Key_MediaPlay,         VKEY_MEDIA_PLAY_PAUSE },
  { Qt::Key_MediaStop,         VKEY_MEDIA_STOP },
  { Qt::Key_MediaPrevious,     VKEY_MEDIA_PREV_TRACK },
  { Qt::Key_MediaNext,         VKEY_MEDIA_NEXT_TRACK },
  { Qt::Key_MediaPause,        VKEY_MEDIA_PLAY_PAUSE },
  { Qt::Key_HomePage,          VKEY_BROWSER_HOME },
  { Qt::Key_Favorites,         VKEY_BROWSER_FAVORITES },
  { Qt::Key_Search,            VKEY_BROWSER_SEARCH },
  { Qt::Key_Back,              VKEY_BROWSER_BACK },
  { Qt::Key_Forward,           VKEY_BROWSER_FORWARD },
  { Qt::Key_Stop,              VKEY_BROWSER_STOP },
  { Qt::Key_Refresh,           VKEY_BROWSER_REFRESH },
  { Qt::Key_LaunchMail,        VKEY_MEDIA_LAUNCH_MAIL },
  { Qt::Key_LaunchMedia,       VKEY_MEDIA_LAUNCH_MEDIA_SELECT },
  { Qt::Key_Zoom,              VKEY_ZOOM },
  { Qt::Key_Sleep,             VKEY_SLEEP },
  { Qt::Key_Hangul,            VKEY_HANGUL },
};

KeyboardCode KeyboardKodeFromQt(const QKeyEvent& event) {
  if (event.key() >= Qt::Key_A && event.key() <= Qt::Key_Z) {
    return static_cast<KeyboardCode>(VKEY_A + event.key() - Qt::Key_A);
  }
  if (event.key() >= Qt::Key_0 && event.key() <= Qt::Key_9) {
    return static_cast<KeyboardCode>(VKEY_0 + event.key() - Qt::Key_0);
  }
  if (event.key() >= Qt::Key_F1 && event.key() <= Qt::Key_F24) {
    return static_cast<KeyboardCode>(VKEY_F1 + event.key() - Qt::Key_F1);
  }

  for (int i = 0; i < arraysize(kQtToKeyboardCodeMap); ++i) {
    if (kQtToKeyboardCodeMap[i].qt_key == event.key()) {
      return kQtToKeyboardCodeMap[i].key_code;
    }
  }

  LOG(WARNING) << "No translation for key: " << event.key();

  return VKEY_UNKNOWN;
}

base::char16 CharacterFromQEvent(const QKeyEvent& event) {
  if (event.text().length() != 1)
    return 0;

  switch (event.key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Tab:
      return 0;
    default:
      return event.text().at(0).unicode();
  }

  return 0;
}

inline DomCode DomCodeFromQt(const QKeyEvent& event) {
  int keycode = event.nativeScanCode();
  return KeycodeConverter::NativeKeycodeToDomCode(keycode);
}

EventType EventTypeFromQt(const QEvent& event) {
  switch (event.type()) {
    case QEvent::KeyPress:
      return ET_KEY_PRESSED;
    case QEvent::KeyRelease:
      return ET_KEY_RELEASED;
    case QEvent::MouseMove:
    case QEvent::HoverMove:
      return ET_MOUSE_MOVED;
    case QEvent::MouseButtonPress:
      return ET_MOUSE_PRESSED;
    case QEvent::MouseButtonRelease:
      return ET_MOUSE_RELEASED;
    case QEvent::Wheel:
      return ET_MOUSEWHEEL;
    default:
      NOTREACHED() << "Unhandled event type: " << event.type();
      return ET_UNKNOWN;
  }
}

int MouseEventChangedFlagsFromQt(const QMouseEvent& event) {
  switch (event.button()) {
    case Qt::NoButton:
      return EF_NONE;
    case Qt::LeftButton:
      return EF_LEFT_MOUSE_BUTTON;
    case Qt::RightButton:
      return EF_RIGHT_MOUSE_BUTTON;
    case Qt::MidButton:
      return EF_MIDDLE_MOUSE_BUTTON;
    case Qt::BackButton:
      return EF_BACK_MOUSE_BUTTON;
    case Qt::ForwardButton:
      return EF_FORWARD_MOUSE_BUTTON;
    default:
      LOG(WARNING) << "Unhandled mouse button: " << event.button();
      return EF_NONE;
  }
}

template<class T>
int MouseEventFlagsFromQt(const T& event) {
  int flags = 0;
  Qt::MouseButtons buttons = event.buttons();

  if (buttons & Qt::LeftButton)
    flags |= EF_LEFT_MOUSE_BUTTON;
  if (buttons & Qt::RightButton)
    flags |= EF_RIGHT_MOUSE_BUTTON;
  if (buttons & Qt::MidButton)
    flags |= EF_MIDDLE_MOUSE_BUTTON;
  if (buttons & Qt::BackButton)
    flags |= EF_BACK_MOUSE_BUTTON;
  if (buttons & Qt::ForwardButton)
    flags |= EF_FORWARD_MOUSE_BUTTON;

  return flags;
}

int KeyEventFlagsFromQt(const QInputEvent& event) {
  int flags = 0;

  Qt::KeyboardModifiers modifiers = event.modifiers();
  if (modifiers & Qt::ShiftModifier)
    flags |= EF_SHIFT_DOWN;
  if (modifiers & Qt::ControlModifier)
    flags |= EF_CONTROL_DOWN;
  if (modifiers & Qt::AltModifier)
    flags |= EF_ALT_DOWN;
  if (modifiers & Qt::MetaModifier)
    flags |= EF_COMMAND_DOWN; // XXX: Is this correct mapping?

  return flags;
}

template<class T>
inline base::TimeDelta EventTimeStampFromQt(const T& event) {
  //return base::TimeDelta::FromMilliseconds(event.timestamp());
  return EventTimeForNow();
}

} // namespace

PlatformEventSourceQt::PlatformEventSourceQt() {
  DeviceDataManager::CreateInstance();
  DetectDevices();
}

PlatformEventSourceQt::~PlatformEventSourceQt() {
}

template void PlatformEventSourceQt::ProcessEvent(const QTouchEvent&);
template void PlatformEventSourceQt::ProcessEvent(const QMouseEvent&);
template void PlatformEventSourceQt::ProcessEvent(const QWheelEvent&);

template<class T>
void PlatformEventSourceQt::ProcessEvent(const T& event) {
  static gfx::Transform transform;
  ProcessEvent(event, transform);
}

template<>
void PlatformEventSourceQt::ProcessEvent(
    const QMouseEvent& event, const gfx::Transform& transform) {
  EventType type = EventTypeFromQt(event);
  gfx::Point location = ToGfx(event.localPos().toPoint());
  base::TimeDelta time_stamp = EventTimeStampFromQt(event);
  int flags = MouseEventFlagsFromQt(event);
  int changed_flags = MouseEventChangedFlagsFromQt(event);

  transform.TransformPoint(&location);

  MouseEvent mouse_event(type, location, location,
                         time_stamp, flags, changed_flags);
  DispatchEvent(&mouse_event);
}

template<>
void PlatformEventSourceQt::ProcessEvent(const QKeyEvent& event) {
  EventType type = EventTypeFromQt(event);
  KeyboardCode key_code = KeyboardKodeFromQt(event);
  DomCode dom_code = DomCodeFromQt(event);
  DomKey dom_key = DomKey::NONE;
  base::char16 character = CharacterFromQEvent(event);
  base::TimeDelta time_stamp = EventTimeStampFromQt(event);
  int flags = KeyEventFlagsFromQt(event);

  if (key_code != VKEY_UNKNOWN && dom_code == DomCode::NONE) {
    dom_code = UsLayoutKeyboardCodeToDomCode(key_code);
    if (dom_code == DomCode::NONE) {
      return;
    }
  }

  KeyEventQt key_event(type, key_code, dom_code, flags,
                       dom_key, character, time_stamp);
  key_event.SetIsChar(character != 0);

  DispatchEvent(&key_event);
}

template<>
void PlatformEventSourceQt::ProcessEvent(
    const QTouchEvent& event, const gfx::Transform& transform) {

  const QList<QTouchEvent::TouchPoint>& touchPoints = event.touchPoints();
  QList<QTouchEvent::TouchPoint>::const_iterator it = touchPoints.begin();
  for (; it != touchPoints.end(); ++it) {
    ui::EventType type;
    if (event.type() == QEvent::TouchCancel) {
      type = ui::ET_TOUCH_CANCELLED;
      break;
    } else if (it->state() == Qt::TouchPointPressed) {
      type = ui::ET_TOUCH_PRESSED;
    } else if (it->state() == Qt::TouchPointReleased) {
      type = ui::ET_TOUCH_RELEASED;
    } else if (it->state() == Qt::TouchPointMoved) {
      type = ui::ET_TOUCH_MOVED;
    } else if (it->state() == Qt::TouchPointStationary) {
      continue;
    } else {
      NOTREACHED();
    }

    gfx::Point point = ToGfx(it->pos().toPoint());
    transform.TransformPoint(&point);

    // Chromium gesture detection expects pointer IDs to be
    // in <0, MotionEvent::MAX_POINTER_ID) range.
    int id = it->id() % MotionEvent::MAX_POINTER_ID;

    ui::TouchEvent touch_event(type, point, 0, id,
        EventTimeStampFromQt(event),
        it->rect().width() / 2, it->rect().height() / 2,
        180.0f,
        it->pressure());

    DispatchEvent(&touch_event);
  }
}

template<>
void PlatformEventSourceQt::ProcessEvent(
    const QWheelEvent& event, const gfx::Transform& transform) {
  QPoint delta = event.angleDelta() / 4;
  gfx::Vector2d offset(delta.x(), delta.y());
  gfx::Point location = ToGfx(event.pos());
  base::TimeDelta time_stamp = EventTimeStampFromQt(event);
  int flags = MouseEventFlagsFromQt(event);

  transform.TransformPoint(&location);

  MouseWheelEvent wheel_event(offset, location, location,
                              time_stamp, flags, 0);
  DispatchEvent(&wheel_event);
}

template<>
void PlatformEventSourceQt::ProcessEvent(
    const QHoverEvent& event, const gfx::Transform& transform) {
  EventType type = EventTypeFromQt(event);
  gfx::Point location = ToGfx(event.pos());
  base::TimeDelta time_stamp = EventTimeStampFromQt(event);
  int flags = 0;
  int changed_flags = 0;

  transform.TransformPoint(&location);

  MouseEvent mouse_event(type, location, location,
                         time_stamp, flags, changed_flags);
  DispatchEvent(&mouse_event);
}

void PlatformEventSourceQt::DetectDevices() const {
  std::vector<TouchscreenDevice> touchscreen_devices;
  std::vector<InputDevice> touchpad_devices;
  int unique_id = 0;

  for (const QTouchDevice* device : QTouchDevice::devices()) {
    DVLOG(1) << "Adding touch device: " << qPrintable(device->name());
    switch (device->type()) {
      case QTouchDevice::TouchScreen:
        touchscreen_devices.push_back(
            TouchscreenDevice(++unique_id, INPUT_DEVICE_INTERNAL,
                              device->name().toStdString(),
                              gfx::Size(), device->maximumTouchPoints()));
        break;
      case QTouchDevice::TouchPad:
        touchpad_devices.push_back(
            InputDevice(++unique_id, INPUT_DEVICE_INTERNAL,
                        device->name().toStdString()));
        break;
      default:
        NOTREACHED() << "Unhandled touch device type: " << device->type();
        break;
    }
  }

  if (touchscreen_devices.empty() && base::CommandLine::ForCurrentProcess()->
          HasSwitch(switches::kSimulateTouchScreenWithMouse)) {
    touchscreen_devices.push_back(
        TouchscreenDevice(++unique_id, INPUT_DEVICE_INTERNAL,
                          kFakeTouchScreenName, gfx::Size(), 1));
  }

  // XXX: What about keyboard and mouse?
  DeviceHotplugEventObserver* observer = DeviceDataManager::GetInstance();
  observer->OnTouchscreenDevicesUpdated(touchscreen_devices);
  observer->OnTouchpadDevicesUpdated(touchpad_devices);
  observer->OnDeviceListsComplete();
}

} // namespace ui
