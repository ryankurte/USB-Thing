#!/bin/bash
# Concatenate two intel hex files
# Drops the last line of the first file and adds the second

SHIM_LINES=`cat $1 | wc -l`

echo "Concatenating $1 and $2 to $3"

A=$(($SHIM_LINES-1))

head -n $A $1 > $3
cat $2 >> $3

