#!/bin/sh
truncate -s +64k ./spl/u-boot-spl.bin
dd if=./spl/u-boot-spl.bin of=64Kfile bs=1k count=64
cat 64Kfile ./u-boot.bin > u-boot-fpga.bin
rm 64Kfile

if [ ! -z $1 ]
then
	echo $1
	cp u-boot-fpga.bin u-boot-fpga.bin_tmp
	truncate -s +400k ./u-boot-fpga.bin_tmp
	dd if=./u-boot-fpga.bin_tmp of=400Kfile bs=1k count=400
	cat ./400Kfile $1 > uboot-fpga-testapp.bin
else
	echo "No testapp ??"
fi

