# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables':  {
    'moc_gen_dir': '<(SHARED_INTERMEDIATE_DIR)/qt_moc',
    'rcc_gen_dir': '<(SHARED_INTERMEDIATE_DIR)/qt_rcc',
    'qt_moc%': '/usr/lib/qt5/bin/moc',
    'qt_rcc%': '/usr/lib/qt5/bin/rcc',

    'ozone_platform_qt%': 1,
    'sailfish%': 0,
  },
  'conditions': [
    ['<(ozone_platform_qt) == 1', {
      'variables':  {
        'external_ozone_platform_deps': [
          '<(DEPTH)/ozone_qt/platform/ozone_qt.gyp:ozone_platform_qt',
        ],
        'external_ozone_platforms': [
          'qt'
        ],
        'ozone_platform%': 'qt',
      },
    }],
  ],

  'target_defaults': {
    'conditions': [
      ['sailfish==1', {
        'defines': [
          'OS_SAILFISH=1',
        ],
      }], # sailfish==1
    ], # conditions
  },

  'includes': [
    'chromium_impl/chromium_impl.gypi',
  ],
}

