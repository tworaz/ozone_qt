# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'aura_client_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/ui/aura/aura.gyp:aura',
      ],
      'sources': [
        'screen_position_client_qt.cc',
        'screen_position_client_qt.h',
      ]
    }
  ],
}

