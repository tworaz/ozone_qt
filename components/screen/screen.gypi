# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'screen_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
      ],
      'sources': [
        'screen_qt.cc',
        'screen_qt.h',
      ]
    }
  ],
}

