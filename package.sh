#bash
pwd=$PWD

cd $1
if [ "$1" == "" ]; then
        exit 1
fi

if [ "$2" == "" ]; then
	echo "No package config : CAM ? BMC"
        exit 1
fi

#Patch replace ...
find $1/. -name  '*.patch' | while read filename; do mv -v "${filename}" "`echo "${filename}" | sed -e 's/.patch//'`"; done

#BMC
if [ "$2" == "bmc" ]; then
        find $1/. -name  '*.bmc' | while read filename; do mv -v "${filename}" "`echo "${filename}" | sed -e 's/.bmc//'`"; done
	find $1/. -name  '*.cam' | while read filename; do rm -f "${filename}"; done
	rm -rf $1/arch/m68k/cpu/aspeed
	rm -f $1/drivers/spi/fmc_spi.c
	rm -f $1/arch/arm/cpu/ast-common/spl.c
	rm -f $1/arch/arm/include/asm/arch-aspee/spl.h
fi

#CAM
if [ "$2" == "cam" ]; then
	find $1/. -name  '*.cam' | while read filename; do mv -v "${filename}" "`echo "${filename}" | sed -e 's/.cam//'`"; done
	find $1/. -name  '*.bmc' | while read filename; do rm -f "${filename}"; done
fi

rm -f $1/arch/arm/include/asm/arch-aspeed/ast1010_platform.h
rm -f $1/arch/arm/include/asm/arch-aspeed/ast3200_platform.h
rm -f $1/arch/arm/cpu/arm1176/u-boot-spl.lds
rm -f $1/arch/arm/cpu/arm1176/u-boot-spl.lds.org
rm -rf $1/board/aspeed/ast1010
rm -rf $1/board/aspeed/ast1400
rm -f $1/board/aspeed/ast-g5/platform3200.S
rm -f $1/board/aspeed/ast-g5/fast.S
rm -f $1/board/aspeed/ast-g5/platform1520.S
rm -f $1/board/aspeed/ast-g5/platform.uart1
rm -f $1/drivers/gpio/ast_gpio.c
rm -f $1/drivers/mmc/ast_sdhci.c
rm -f $1/drivers/spi/ast_spi.c
rm -f $1/drivers/usb/host/ehci-ast.c
rm -f $1/include/configs/ast1010.h
rm -f $1/include/configs/ast1300.h
rm -f $1/include/configs/ast1400.h
rm -f $1/include/configs/ast2509.h
rm -rf $1/bdi-ice
rm -f $1/package.sh
