# Release bundle build instructions for Windows 64-bit

- [Prepare script](#prepare-script)
- [Install Inno Setup](#install-inno-setup)
- [Clone source code and prepare libraries](#clone-source-code-and-prepare-libraries)
- [Build the project](#build-the-project)
- [Qt Visual Studio Tools](#qt-visual-studio-tools)

## Prepare script

Open **bundle_release.py** in preferred code editor and change needed variables in **config** dict.

## Install Inno Setup

Download [Inno Setup][inno-setup], install it, then go to installation path, copy it, add to PATH, reboot, and viola!

## Bundle release

Run **bundle_release.py**. You will find result files in:

**rabbitgramdesktop/out/Release/releases/rtgdrelease-X.XX.X-XXXXXXXX**

[inno-setup]: https://jrsoftware.org/isdl.php
