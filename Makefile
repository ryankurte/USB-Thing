
.PHONY: firmware software setup

all: firmware software library

setup:
	mkdir -p build;

firmware: setup
	cd firmware; mkdir build; cd build; cmake ..; make -j4; cp usb-thing.bin ../../build

flash: setup firmware
	cd firmware/build; make flash; cd ..

software: setup
	cd software; mkdir build; cd build; cmake ..; make -j4; cp usb-thing ../../build

library: setup
	cd library; mkdir build; cd build; cmake ..; make -j4; cp libusbthing.* ../../build
