
.PHONY: firmware software setup

all: firmware software

setup:
	mkdir -p build;

firmware: setup
	cd firmware; mkdir build; cd build; cmake ..; make -j4; cp usb-thing.bin ../../build

software: setup
	cd software; mkdir build; cd build; cmake ..; make -j4; cp usb-thing ../../build


