# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'conditions': [
      ['sailfish==1', {
        'target_conditions': [

          ['_target_name=="gpu_config"', {
            'sources/': [
              [ 'exclude', 'config/gpu_info_collector_linux\\.cc$' ],
            ],
            'sources': [
              'gpu/config/gpu_info_collector_sailfish.cc',
            ],
          }], # gpu_config

          ['_target_name=="base"', {
            'sources': [
              'base/sys_info_sailfish.cc',
            ],
          }], # base

				], # target_conditions
      }], # sailfish==1
    ], # conditions

    'target_conditions': [
      ['_target_name=="gesture_detection"', {
        'sources/': [
          [ 'exclude', 'gesture_detection/gesture_configuration_aura\\.cc$' ],
        ],
        'sources': [
          'ui/events/gesture_detection/gesture_configuration_qt.cc',
        ],
      }], # gesture_detection
    ],
  },
}
