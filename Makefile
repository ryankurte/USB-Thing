
.PHONY: firmware software library bootloader setup

VERSION=`git describe --dirty`

all: firmware software library bootloader

setup:
	mkdir -p build;

firmware: setup
	cd firmware; mkdir build; cd build; cmake -DWITH_BOOTLOADER=1 ..; make; cp usb-thing.bin ../../build/

flash: setup firmware
	cd firmware/build; make flash; cd ..

software: setup
	cd software; mkdir build; cd build; cmake ..; make; cp usb-thing ../../build/

library: setup
	cd library; mkdir build; cd build; cmake ..; make; cp libusbthing.* ../../build/

bootloader: setup
	cd bootloader; mkdir build; cd build; cmake ..; make; cp bootloader.bin ../../build/

