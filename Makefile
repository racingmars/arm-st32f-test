####### PROJECT CONFIGURATION ################################################

## Set device and project name here.
DEVICE=STM32F407xx
PROJECT=armtest

## List source files here
SRC=main.c system_stm32f4xx168MHz.c
STARTUP=startup_stm32f407xx.s
LDSCRIPT=stm32f407vg.ld

OBJS=$(SRC:.c=.o) $(STARTUP:.s=.o)

CCPREFIX=arm-none-eabi-
CC=$(CCPREFIX)gcc
OBJCOPY=$(CCPREFIX)objcopy
AS=$(CCPREFIX)gcc -x assembler-with-cpp

CFLAGS=-Wall -mcpu=cortex-m4 -mlittle-endian -mthumb \
	   -Iinclude/CMSIS -Iinclude \
	   -D$(DEVICE) -O0 \
	   -g
ASFLAGS=$(CFLAGS) -c
LDFLAGS=-mcpu=cortex-m4 -mlittle-endian -mthumb \
	   -T$(LDSCRIPT) \
	   -D$(DEVICE) -O0 -Wl,--gc-sections \
	   -g

default: all

all: $(PROJECT).hex $(PROJECT).bin $(PROJECT).elf

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -f *.o *.hex *.bin *.elf

flash: $(PROJECT).bin
	st-flash --reset write $< 0x8000000
