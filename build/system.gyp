# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'qt_version%': '<!(pkg-config --modversion Qt5Core)',
  },
  'targets': [
    {
      'target_name': 'qtcore',
      'type': 'none',
      'direct_dependent_settings': {
        'cflags': [
          '<!@(pkg-config --cflags Qt5Core)',
        ],
        'include_dirs': [
          '/usr/include/qt5/QtCore/<(qt_version)',
          '/usr/include/qt5/QtCore/<(qt_version)/QtCore',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other Qt5Core)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l Qt5Core)',
        ],
      },
    }, # qtcore
    {
      'target_name': 'qtgui',
      'type': 'none',
      'direct_dependent_settings': {
        'cflags': [
          '<!@(pkg-config --cflags Qt5Gui)',
        ],
        'include_dirs': [
          '/usr/include/qt5/QtGui/<(qt_version)',
          '/usr/include/qt5/QtGui/<(qt_version)/QtGui',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other Qt5Gui)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l Qt5Gui)',
        ],
      },
    }, # qtgui
    {
      'target_name': 'qtopengl',
      'type': 'none',
      'direct_dependent_settings': {
        'cflags': [
          '<!@(pkg-config --cflags Qt5OpenGL)',
        ],
        'include_dirs': [
          '/usr/include/qt5/QtGui/<(qt_version)',
          '/usr/include/qt5/QtGui/<(qt_version)/QtOpenGL',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other Qt5OpenGL)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l Qt5OpenGL)',
        ],
      },
    }, # qtopengl
    {
      'target_name': 'qtqml',
      'type': 'none',
      'direct_dependent_settings': {
        'cflags': [
          '<!@(pkg-config --cflags Qt5Qml)',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other Qt5Qml)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l Qt5Qml)',
        ],
      },
    }, # qtqml
    {
      'target_name': 'qtquick',
      'type': 'none',
      'direct_dependent_settings': {
        'cflags': [
          '<!@(pkg-config --cflags Qt5Quick)',
        ],
        'include_dirs': [
          '/usr/include/qt5/QtQuick/<(qt_version)',
          '/usr/include/qt5/QtQuick/<(qt_version)/QtQuick',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other Qt5Quick)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l Qt5Quick)',
        ],
      },
    }, # qtquick
  ],
}
