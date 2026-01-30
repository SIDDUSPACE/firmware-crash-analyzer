CROSS_COMPILE ?= arm-none-eabi-

CC      := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

TARGET  = firmware
CPU     = cortex-m3

CFLAGS  = -mcpu=$(CPU) -mthumb -Wall -Wextra -O0 -g \
          -ffreestanding -nostdlib -nostartfiles

LDFLAGS = -T linker.ld -nostdlib

SRCS = startup.s main.c fault.c uart.c crash_record.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

all: $(TARGET).elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

dump:
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).dump

clean:
	rm -f *.o *.elf *.dump
