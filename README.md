# USB-Thing

Generic USB interface device for driver development and hardware testing

## Status
Very early alpha. Firmware working, library has restricted functionality.

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

* hardware - device hardware design files
* firmware - device firmware sources and build tools
* library - usbthing shared library, this includes all libusb calls
* common - common protocol definitions shared between the firmware and library modules

