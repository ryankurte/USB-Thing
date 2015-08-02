# USB-Thing

Generic USB interface device for driver development and hardware testing

## Status
Very early alpha. Firmware working, library has restricted functionality.

## Goals

 - Generic device to allow control and interaction with embedded hardware allowing native driver development and hardware unit testing
 - LibUSB based support for I2C, SPI, GPIO (with interrupts), ADC and PWM

## Requirements

* arm-none-eabi-gcc (from launchpad or use `source armenv.sh` for a local instance)
* cmake
* make
* clang (or gcc)

## Getting Started

1. Checkout this repository
2. Initialize submodules with `git submodule update --init --recursive`
3. Build the firmware with `make firmware`
4. Build the utility with `make software`

## Folders

hardware - device hardware design files
firmware - device firmware sources and build tools
library - device support library (linux)
