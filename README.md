## Introduction

OzoneQt is a Qt based implementation of Chromium Ozone platform abstraction layer.
It makes it possible to run Chromuim/Blink web engine on EGL based Qt platforms.

## Getting the code

The OzonQt layer should be checked out in ozone_qt directory under main chromuim
source tree.

For instructions regarding Chromuim/Blink checkout please see:
https://www.chromium.org/developers/how-tos/get-the-code

After checking the sources please make sure ozone_qt patches are applied to chromium
source tree.

$ git am ozone_qt/patches/*.patch

## Building
```
$ ./build/gyp_chromuim -Duse_ozone=1 -Dozone_platform_qt=1 -Dtoolkit_views=0
$ ninja -C out/Debug content_shell
```
## Running
```
$ ./out/Debug/content_shell --no-sandbox --ozone-platfom=qt
```
## Documents

https://www.chromium.org/developers/design-documents/ozone

## License

OzoneQt code uses the 3-clause BSD license, see `LICENSE` file.
