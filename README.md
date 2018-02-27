# USB-Thing

Generic USB interface device for driver development and hardware testing.  

This consists of a hardware device with a number of common embedded interfaces exposed (see status section for more details) paired with a LibUSB based driver library that provides a set of fairly standard set of functions to interface with embedded peripherals.

![Example](https://raw.github.com/ryankurte/usb-thing/gh-pages/assets/01.jpg)

## Status
Very early alpha. Firmware working, library has restricted functionality.

[![Build Status](https://api.travis-ci.org/ryankurte/USB-Thing.svg)](https://travis-ci.org/ryankurte/USB-Thing)

| Module | Status                   |
|--------|--------------------------|
|GPIO    |Working - no interrupts   |
|ADC     |Working - single sample   |
|SPI     |Working                   |
|DAC     |In Progress               |
|I2C     |In Progress               |
|UART    |Unimplemented             |

## Goals

 - Generic device to allow control and interaction with embedded hardware allowing native driver development and hardware unit testing
 - LibUSB based support for I2C, SPI, GPIO (with interrupts), ADC and PWM
 - Immediately usable, information printed on device so there is no need to consult the manual

## Hardware

![Example](https://raw.github.com/ryankurte/usb-thing/gh-pages/assets/03.png)

### Schematics
Board schematics are available [here](https://raw.github.com/ryankurte/usb-thing/gh-pages/assets/usb-thing-v0.2.0.pdf) and will be updated as new hardware is released.


## Requirements

* arm-none-eabi-gcc (from launchpad or use `source armenv.sh` for a local instance)
* cmake
* make
* clang (or gcc)

## Getting Started

First, initialize the project with the following:

1. Checkout this repository
2. Initialize submodules with `git submodule update --init --recursive`
3. Build all components with `make all`

### Library installation

1. Switch to the `firmware/library` directory
2. Install with `make install`

### Firmware development

1. Switch to the `firmware/build` directory
2. Build the firmware with `make`
3. Flash the firmware to the device with `make flash`

### Software Development

1. Switch to the `software/build` directory
2. Build the software with `make`
3. Run with `./usb-thing`


## Folders

* firmware - device firmware sources and build tools
* library - usbthing shared library, this includes all libusb calls
* software - usbthing utility, provides common (simple) interface methods
* common - common protocol definitions shared between the firmware and library modules

## Licensing
TODO

--------
If you have any questions, comments, or suggestions, feel free to open an issue or a pull request.  
