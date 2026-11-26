# Makefile for kacchiOS
CC = gcc
LD = ld
AS = as
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdinc -fno-builtin -fno-stack-protector -fno-leading-underscore -I.
#The most important flags for bare-metal development are -m32, -ffreestanding, -nostdinc, and -fno-builtin.
ASFLAGS = --32
LDFLAGS = -m i386pe --image-base 0

OBJS = boot.o kernel.o serial.o string.o memory.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T link.ld -o kernel.pe $^
	$(OBJCOPY) -O elf32-i386 kernel.pe $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none

run-vga: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial mon:stdio

debug: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none -s -S &
	@echo "Waiting for GDB connection on port 1234..."
	@echo "In another terminal run: gdb -ex 'target remote localhost:1234' -ex 'symbol-file kernel.elf'"

clean:
	rm -f *.o kernel.pe kernel.elf

.PHONY: all run run-vga debug clean