
all: bootloader

.PHONY: shim loader

clean:
	rm -rf build/

shim:
	mkdir -p build/shim
	cd build/shim; cmake -DFLASH_START=0x00000000 ../..; make; cd ../..

loader:
	mkdir -p build/loader
	cd build/loader; cmake -DFLASH_START=0x0000400 ../..; make; cd ../..

bootloader: shim loader
	./concat.sh build/shim/shim.hex build/loader/bootloader.hex build/bootloader.hex
	arm-none-eabi-objcopy -I ihex -O binary build/bootloader.bin build/bootloader.hex


