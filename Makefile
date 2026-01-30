TOOLCHAIN = /home/siddarth/tools/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/bin
CC      = $(TOOLCHAIN)/arm-none-eabi-gcc
OBJCOPY = $(TOOLCHAIN)/arm-none-eabi-objcopy
OBJDUMP = $(TOOLCHAIN)/arm-none-eabi-objdump
TARGET  = firmware
CPU     = cortex-m3
CFLAGS  = -mcpu=$(CPU) -mthumb -Wall -Wextra -O0 -g \
          -ffreestanding -nostdlib -nostartfiles

LDFLAGS = -T linker.ld -nostdlib

# Sources here
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
