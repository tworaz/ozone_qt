# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'mce',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/build/linux/system.gyp:dbus',
        '<(DEPTH)/dbus/dbus.gyp:dbus',
      ],
      'sources': [
        'memory_pressure_monitor_mce.cc',
        'memory_pressure_monitor_mce.h',
      ]
    }
  ],
}

