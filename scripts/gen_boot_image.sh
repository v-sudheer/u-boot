#!/bin/sh
truncate -s +64k ./spl/u-boot-spl.bin
dd if=./spl/u-boot-spl.bin of=64Kfile bs=1k count=64
cat 64Kfile ./u-boot.bin > u-boot-fpga.bin
rm 64Kfile
