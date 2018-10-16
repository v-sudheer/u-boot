CTPATH = /home/pilot/Cappuccino/crosstools/bin
CC = $(CTPATH)/arm-none-eabi-gcc
LD = $(CTPATH)/arm-none-eabi-ld
OBJCOPY = $(CTPATH)/arm-none-eabi-objcopy

CFLAGS = -Wall -mlittle-endian -mthumb -march=armv7-m

LDSCRIPT   = ld0_cm3.scr
LDFLAGS = -T $(LDSCRIPT) -EL -Map cm3_reset_wa.lst 

#SRC = startup.c uart.c timer.c test.c
#OBJ = startup.o uart.o timer.o test.o
SRC = startup.c uart.c test.c timer.c
OBJ = startup.o uart.o test.o timer.o

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(LD) $(LDFLAGS) -o cm3_reset_wa.elf $(OBJ)
	$(OBJCOPY) -O binary cm3_reset_wa.elf cm3_reset_wa.bin

clean:
	@rm -f *.o *.lst *.elf *.bin

