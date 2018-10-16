include $(TOPDIR)/config.mk

LIB     = $(obj)libpilot4_tests.o

#COBJS-$(CONFIG_PILOT4_DDR_TEST) += ddr_options.o
COBJS-y += ddr_options.o
COBJS-y += cmd_mdiord.o
COBJS-y += cmd_mdiowr.o

COBJS   := $(sort $(COBJS-y))
XCOBJS  := $(sort $(XCOBJS-y))
SRCS    := $(COBJS:.o=.c) $(XCOBJS:.o=.c)
OBJS    := $(addprefix $(obj),$(COBJS))
XOBJS   := $(addprefix $(obj),$(XCOBJS))

CPPFLAGS += -I..

all:    $(LIB) $(XOBJS)

$(LIB): 
	echo $(OBJS)
	echo "2"$(obj)
	echo "3"
	$(obj).depend $(OBJS)
	echo $(strip $(OBJS))
#        $(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################

