#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk
ifneq ($(OBJTREE),$(SRCTREE))
$(shell mkdir -p $(obj)../common)
endif

LIB	= $(obj)lib$(BOARD).o

#DDR_1600MHZ, DDR_1333MHZ, DDR_1066MHZ, DDR_800MHZ,
#DDR_500MHZ, DDR_350_400MHZ, DDR_200_250MHZ, DDR_75_125MHZ
DDR_FREQ	= DDR_1600MHZ
ENV			= HW

AFLAGS+= -DASIC_DUT
CFLAGS+= -DASIC_DUT
dut = a
env = h
AFLAGS += -DHW_ENV
CFLAGS += -DHW_ENV

COBJS-y	:= board.o ddr_options.o seccode.o sw_reset_modules.o gic.o pilot_timer.o
COBJS-$(CONFIG_BOMBER_PWM_WORKAROUND) += bomber_pwm.o
COBJS-$(CONFIG_USE_SSP_RESET) += ssp_reset_wa.o
#COBJS-y	:= board.o ddr_options.o seccode.o sw_reset_modules.o print.o
SOBJS	:= lowlevel_init.o MP_GIC.o

CFLAGS+=-DASIC_DUT -DHW_ENV -g  -Os -fno-common -ffixed-r8 -msoft-float  -gdwarf-2 -D__KERNEL__ -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv7-a -Wall -Wstrict-prototypes -fno-stack-protector

ifeq ($(DDR_FREQ),)
        CFLAGS+=-DDDR_1600MHZ
        AFLAGS+=-DDDR_1600MHZ
endif

ifeq ($(DDR_FREQ), DDR_1600MHZ)
        CFLAGS+=-DDDR_1600MHZ
        AFLAGS+=-DDDR_1600MHZ
endif
ifeq ($(DDR_FREQ), DDR_75_125MHZ)
        CFLAGS+=-DDDR_75_125MHZ
        AFLAGS+=-DDDR_75_125MHZ
endif
ifeq ($(DDR_FREQ), DDR_200_250MHZ)
        CFLAGS+=-DDDR_200_250MHZ
        AFLAGS+=-DDDR_200_250MHZ
endif
ifeq ($(DDR_FREQ), DDR_350_400MHZ)
        CFLAGS+=-DDDR_350_400MHZ
        AFLAGS+=-DDDR_350_400MHZ
endif
ifeq ($(DDR_FREQ), DDR_500MHZ)
        CFLAGS+=-DDDR_500MHZ
        AFLAGS+=-DDDR_500MHZ
endif
ifeq ($(DDR_FREQ), DDR_800MHZ)
        CFLAGS+=-DDDR_800MHZ
        AFLAGS+=-DDDR_800MHZ
endif
ifeq ($(DDR_FREQ), DDR_1066MHZ)
        CFLAGS+=-DDDR_1066MHZ
        AFLAGS+=-DDDR_1066MHZ
endif
ifeq ($(DDR_FREQ), DDR_1333MHZ)
        CFLAGS+=-DDDR_1333MHZ
        AFLAGS+=-DDDR_1333MHZ
endif

CFLAGS+="-DLOWER_ECC_ADDRESS=0x80800000"
CFLAGS+="-DUPPER_ECC_ADDRESS=0x80ffffff"

ifeq ($(ECC), 1)
CFLAGS+=-DECC

ifneq ($(LECC), )
CFLAGS+="-DLOWER_ECC_ADDRESS=$(LECC)"
endif

ifneq ($(UECC), )
CFLAGS+="-DUPPER_ECC_ADDRESS=$(UECC)"
endif
endif

CFLAGS+="-DDDROPVERSION=0xFFFFFFFF"
ifneq ($(DOVERSION), )
CFLAGS+="-DDDROPVERSION=$(DOVERSION)"
endif

COBJS	:= $(sort $(COBJS-y))

SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

$(LIB):	$(obj).depend $(OBJS) $(SOBJS)
	$(call cmd_link_o_target, $(OBJS) $(SOBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
