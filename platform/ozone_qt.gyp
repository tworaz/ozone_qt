# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'include_dirs': [
      '<(DEPTH)',
    ],
  },

  'targets': [
    {
      'target_name': 'ozone_platform_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/ui/events/devices/events_devices.gyp:events_devices',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtcore',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
        '<(DEPTH)/third_party/khronos/khronos.gyp:khronos_headers',
      ],
      'sources': [
        'client_native_pixmap_factory_qt.cc',
        'client_native_pixmap_factory_qt.h',
        'cursor_factory_ozone_qt.cc',
        'cursor_factory_ozone_qt.h',
        'media_ozone_platform_qt.cc',
        'overlay_manager_ozone_qt.cc',
        'overlay_manager_ozone_qt.h',
        'ozone_platform_qt.cc',
        'ozone_platform_qt.h',
        'ozone_qt_switches.cc',
        'ozone_qt_switches.h',
        'platform_event_source_qt.cc',
        'platform_event_source_qt.h',
        'platform_window_qt.cc',
        'platform_window_qt.h',
        'surface_factory_qt.cc',
        'surface_factory_qt.h',
        'surface_ozone_egl_qt.cc',
        'surface_ozone_egl_qt.h',

        # Sources generated ozone_platform_qt_moc.
        '<(moc_gen_dir)/moc_platform_window_qt.cc',
      ],
    }, # ozone_platform_qt
    {
      'target_name': 'ozone_platform_qt_moc',
      'type': 'none',
      'sources': [
        'platform_window_qt.h',
      ],
      'rules': [{
        'rule_name': 'generate_ozone_platform_qt_moc',
        'includes': [ '../build/moc_rule.gypi' ],
      }],
    }, # ozone_platform_qt_moc
  ],
}
