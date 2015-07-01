#!/bin/bash

SHIM_LINES=`cat shim.hex | wc -l`

A=$(($SHIM_LINES-1))

head -n $A shim.hex > loader.hex
cat bootloader.hex >> loader.hex

