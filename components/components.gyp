# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'include_dirs': [
      '<(DEPTH)',
    ],
  },

  'includes': [
    'mce/mce.gypi',
    'aura_client/aura_client.gypi',
    'ime/ime.gypi',
    'screen/screen.gypi',
  ],
}
