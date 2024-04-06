# rabbitGram Desktop – Telegram Desktop based messenger with special features

[![Windows.](https://github.com/rabbitgramdesktop/rabbitgramdesktop/actions/workflows/win.yml/badge.svg)](https://github.com/rabbitgramdesktop/rabbitgramdesktop/actions/workflows/win.yml)

This is the complete source code and the build instructions for the app based on the official [Telegram][telegram] messenger desktop client.

![image](https://github.com/xmdnx/exteraGramDesktop/assets/72883689/082fe7d6-eeba-4198-83ab-843d08ac909c)

The source code is published under GPLv3 with OpenSSL exception, the license is available [here][license].

## Features

* Sticker size setting
* Userpic corner radius
* Auto-hiding notifications
* Side menu editor
* Hide phone number in profile
* Show user ID in profile

## Supported systems

The latest version is available for

* Windows 7 and above (x64)

You can build rabbitGram Desktop yourself using [build instructions][build_instructions]

## Third-party

* Qt 6 ([LGPL](http://doc.qt.io/qt-6/lgpl.html)) and Qt 5.15 ([LGPL](http://doc.qt.io/qt-5/lgpl.html)) slightly patched
* OpenSSL 3.2.1 ([Apache License 2.0](https://www.openssl.org/source/apache-license-2.0.txt))
* WebRTC ([New BSD License](https://github.com/desktop-app/tg_owt/blob/master/LICENSE))
* zlib 1.2.11 ([zlib License](http://www.zlib.net/zlib_license.html))
* LZMA SDK 9.20 ([public domain](http://www.7-zip.org/sdk.html))
* liblzma ([public domain](http://tukaani.org/xz/))
* Google Breakpad ([License](https://chromium.googlesource.com/breakpad/breakpad/+/master/LICENSE))
* Google Crashpad ([Apache License 2.0](https://chromium.googlesource.com/crashpad/crashpad/+/master/LICENSE))
* GYP ([BSD License](https://github.com/bnoordhuis/gyp/blob/master/LICENSE))
* Ninja ([Apache License 2.0](https://github.com/ninja-build/ninja/blob/master/COPYING))
* OpenAL Soft ([LGPL](https://github.com/kcat/openal-soft/blob/master/COPYING))
* Opus codec ([BSD License](http://www.opus-codec.org/license/))
* FFmpeg ([LGPL](https://www.ffmpeg.org/legal.html))
* Guideline Support Library ([MIT License](https://github.com/Microsoft/GSL/blob/master/LICENSE))
* Range-v3 ([Boost License](https://github.com/ericniebler/range-v3/blob/master/LICENSE.txt))
* Open Sans font ([Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html))
* Vazir font ([SIL Open Font License 1.1](https://github.com/rastikerdar/vazir-font/blob/master/OFL.txt))
* Emoji alpha codes ([MIT License](https://github.com/emojione/emojione/blob/master/extras/alpha-codes/LICENSE.md))
* xxHash ([BSD License](https://github.com/Cyan4973/xxHash/blob/dev/LICENSE))
* QR Code generator ([MIT License](https://github.com/nayuki/QR-Code-generator#license))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))
* Hunspell ([LGPL](https://github.com/hunspell/hunspell/blob/master/COPYING.LESSER))

## Build instructions

* Windows [(32-bit)][win32] [(64-bit)][win64]
* [macOS][macos]
* [GNU/Linux using Docker][linux]

## Special thanks to

* [Kotatogram][kotatogram]
* [64Gram][64gram]

[//]: # (LINKS)
[telegram]: https://telegram.org
[license]: LICENSE
[etg_themes_repo]: https://github.com/xmdnx/exteraThemes/
[win32]: docs/building-win.md
[win64]: docs/building-win-x64.md
[linux]: docs/building-linux.md
[macos]: docs/building-mac.md
[build_instructions]: https://github.com/rabbitGramDesktop/rabbitGramDesktop#build-instructions
[kotatogram]: https://github.com/kotatogram/kotatogram-desktop
[64gram]: https://github.com/TDesktop-x64/tdesktop
