### PROJECT CONFIGURATION

DEVICE=STM32F407xx
PROJECT=armtest

SRC=main.c system_stm32f4xx168MHz.c
STARTUP=startup_stm32f407xx.s
OBJS=$(SRC:.c=.o) $(STARTUP:.s=.o)

STMCUBE=/Users/mwilson/tmp/STM32Cube_FW_F4_V1.5.0
CCPREFIX=arm-none-eabi-
CC=$(CCPREFIX)gcc
CP=$(CCPREFIX)objcopy
AS=$(CCPREFIX)gcc -x assembler-with-cpp

CFLAGS=-Wall -mcpu=cortex-m4 -mlittle-endian -mthumb \
	   -I$(STMCUBE)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	   -I$(STMCUBE)/Drivers/CMSIS/Include \
	   -D$(DEVICE) -Os \
	   -g -gdwarf-2
ASFLAGS=$(CFLAGS) -c
LDFLAGS=-mcpu=cortex-m4 -mlittle-endian -mthumb \
	   -T$(STMCUBE)/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/STM32F4-Discovery/STM32F407VG_FLASH.ld \
	   -D$(DEVICE) -Os -Wl,--gc-sections \
	   -g -gdwarf-2

default: $(PROJECT).hex

$(PROJECT).hex: $(PROJECT).elf
	$(CP) -Oihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).bin: $(PROJECT).elf
	$(CP) -Obinary -S $(PROJECT) $(PROJECT).bin

%.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -f *.o *.hex *.bin *.elf

flash: $(PROJECT).hex
	openocd -f /usr/local/share/openocd/scripts/board/stm32f4discovery.cfg -c "init" -c "reset halt" -c "sleep 100" -c "wait_halt 2" -c "flash write_image erase $(PROJECT).hex" -c "sleep 100" -c "verify_image $(PROJECT).hex" -c "sleep 100" -c "reset run" -c shutdown
