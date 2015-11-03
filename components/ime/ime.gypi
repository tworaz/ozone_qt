# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'ime_qt',
      'type': 'static_library',
      'dependencies': [
				'<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
        '<(DEPTH)/ui/base/ime/ui_base_ime.gyp:ui_base_ime',
      ],
      'sources': [
        'input_method_qt.cc',
        'input_method_qt.h',
      ]
    }
  ],
}
