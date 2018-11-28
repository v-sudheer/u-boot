#
# (C) Copyright 2000-2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2018
# Shivah Shankar, Aspeed Technology, <shivahshankar.shankarnarayanrao@aspeedtech.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

DDR_FREQ	= DDR_1600MHZ
ENV			= HW

asflags-y += -DASIC_DUT
ccflags-y += -DASIC_DUT
dut = a
env = h
asflags-y += -DHW_ENV
ccflags-y += -DHW_ENV

asflags-y  +=-DASIC_DUT -DHW_ENV -g  -Os -fno-common -ffixed-r8 -msoft-float  -gdwarf-2 -D__KERNEL__ -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv7-a -mcpu=cortex-a9 -Wall -Wstrict-prototypes -fno-stack-protector
ccflags-yi +=-DASIC_DUT -DHW_ENV -g  -Os -fno-common -ffixed-r8 -msoft-float  -gdwarf-2 -D__KERNEL__ -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv7-a -mcpu=cortex-a9 -Wall -Wstrict-prototypes -fno-stack-protector

ifeq ($(DDR_FREQ),)
        ccflags-y +=-DDDR_1600MHZ
        asflags-y +=-DDDR_1600MHZ
endif

ifeq ($(DDR_FREQ), DDR_1600MHZ)
        ccflags-y +=-DDDR_1600MHZ
        asflags-y +=-DDDR_1600MHZ
endif
ifeq ($(DDR_FREQ), DDR_75_125MHZ)
        ccflags-y +=-DDDR_75_125MHZ
        asflags-y +=-DDDR_75_125MHZ
endif
ifeq ($(DDR_FREQ), DDR_200_250MHZ)
        ccflags-y +=-DDDR_200_250MHZ
        asflags-y +=-DDDR_200_250MHZ
endif
ifeq ($(DDR_FREQ), DDR_350_400MHZ)
        ccflags-y +=-DDDR_350_400MHZ
        asflags-y +=-DDDR_350_400MHZ
endif
ifeq ($(DDR_FREQ), DDR_500MHZ)
        ccflags-y +=-DDDR_500MHZ
        asflags-y +=-DDDR_500MHZ
endif
ifeq ($(DDR_FREQ), DDR_800MHZ)
        ccflags-y +=-DDDR_800MHZ
        asflags-y +=-DDDR_800MHZ
endif
ifeq ($(DDR_FREQ), DDR_1066MHZ)
        ccflags-y +=-DDDR_1066MHZ
        asflags-y +=-DDDR_1066MHZ
endif
ifeq ($(DDR_FREQ), DDR_1333MHZ)
        ccflags-y +=-DDDR_1333MHZ
        asflags-y +=-DDDR_1333MHZ
endif

ccflags-y +="-DLOWER_ECC_ADDRESS=0x80800000"
ccflags-y +="-DUPPER_ECC_ADDRESS=0x80ffffff"

ifeq ($(ECC), 1)
ccflags-y +=-DECC

ifneq ($(LECC), )
ccflags-y +="-DLOWER_ECC_ADDRESS=$(LECC)"
endif

ifneq ($(UECC), )
ccflags-y +="-DUPPER_ECC_ADDRESS=$(UECC)"
endif
endif

ccflags-y +="-DDDROPVERSION=0xFFFFFFFF"
ifneq ($(DOVERSION), )
ccflags-y +="-DDDROPVERSION=$(DOVERSION)"
endif
obj-y   += timer.o
obj-y   += flash.o
obj-y   += lowlevel_init.o
obj-y	+= ddr_options.o seccode.o sw_reset_modules.o
#obj-y	+= MP_GIC.o 

obj-y   += startup_secure.o
obj-y   += Remap.o
obj-y   += monitor_core0.o
obj-y   += monitor_core1.o
obj-y	+= MP_SCU.o 
obj-y	+= GIC.o 
obj-y	+= gic.o 
obj-(CONFIG_BOMBER_PWM_WORKAROUND)	+= bomber_pwm.o
obj-(CONFIG_USE_SSP_RESET)	+= ssp_reset_wa.o
