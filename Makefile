
.PHONY: firmware software library bootloader setup clean

VERSION=`git describe --dirty`

all: firmware software library bootloader

package: all
	tar -cvf build.tgz build/*

setup:
	mkdir -p build;

firmware: setup
	cd firmware; mkdir -p build; cd build; cmake -DWITH_BOOTLOADER=1 ..; make; cp usb-thing.bin ../../build/

flash: setup firmware
	cd firmware/build; make flash; cd ..

software: setup
	cd software; mkdir -p build; cd build; cmake ..; make; cp usb-thing ../../build/

library: setup
	cd library; mkdir -p build; cd build; cmake ..; make; cp libusbthing.* ../../build/

bootloader: setup
	cd bootloader; mkdir -p build; cd build; cmake ..; make; cp bootloader.bin ../../build/

clean: 
	rm -rf firmware/build software/build library/build bootloader/build build/
