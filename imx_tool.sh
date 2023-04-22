#!/bin/sh
echo "$1"
imx_tool -e 0x7e1000 -b $1 -o flash.bin
chmod +x flash.bin
