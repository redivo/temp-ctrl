# RED Temperature Controller #
[![Build Status](https://api.travis-ci.org/redivo/temp-ctrl.svg?branch=master)](https://travis-ci.org/redivo/temp-ctrl)

## Introduction ##
The RED Temperature Controller is a basic software that reads a temperature and try to maintain it within the boundaries defined by user.

## Requirements ##
It's a pretty simple project that requires a few other packages:
 - [CMake](https://cmake.org/)
 - [Goolge Tests](https://github.com/google/googletest)
 - arm-none-eabi for **devkit_pucrs** platform (you can install it with `apt-get install gcc-arm-none-eabi` or access [Arm Developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) to learn about that)
 - lpc21isp for **devkit_pucrs** platform in `make install` (you can install it with `apt-get install lpc21isp`)

## Compilation ##
The usage of **CMake** enable us to easily compile the project out of the box, so:
- Go to the path where you want to compile the project
- Type `cmake -D PLATFORM=<desired platform> <other options> <root path of this repository>`
- Type `make`
- Type `make install` (only for **devkit_pucrs**)

### Supported Platforms ###
You can compile it for the following targets:
 - PC, passing `-D PLATFORM=pc` in `cmake` command (actually it doesn't do nothing, but it's usefull for testing)
 - PUCRS DevKit (a devboard with an ARM LPC2378 created by PUCRS researchers), passing `-D PLATFORM=pucrs_devboard -D TTY=<serial path>` in `cmake` command, where `<serial path>` is the path of serial interface connected to devekit (it is used on `make install`)

### Other Options ###
 - CLI, passing `-D CLI=enabled` in `cmake` command
