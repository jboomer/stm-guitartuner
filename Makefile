# put your *.o targets here, make should handle the rest!

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c usart.c pitch.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=tuner

# Make sure st-flash is in your current path

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -Wall -Tstm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -O0

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc 
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 
CFLAGS += -Ilib/inc/cmsis
CFLAGS += -specs=nosys.specs
CFLAGS += -DARM_MATH_CM4

SRCS += lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

again: clean all

# Flash the STM32F4
burn:
	st-flash write $(PROJ_NAME).bin 0x8000000

# Create tags; assumes ctags exists
ctags:
	ctags -R --exclude=*cm0.h --exclude=*cm3.h .

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f4 -larm_cortexM4lf_math
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o *.i
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
