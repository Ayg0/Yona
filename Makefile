# Variables
CC=i386-elf-gcc
AS=nasm
LDSCRIPT=src/linker.ld
DO_I_SPEED=-O2
# DO_I_SPEED=
CFLAGS=-std=gnu99 -ffreestanding ${DO_I_SPEED} -Wall -Wextra -Werror\
		 -fno-builtin -nostdlib -nodefaultlibs -Isrc/includes
ASFLAGS=-f elf32
LDFLAGS=-T $(LDSCRIPT) -ffreestanding ${DO_I_SPEED} -nostdlib -lgcc

CSRCS=$(shell find src -type f -name "*.c")
SSRCS=$(shell find src -type f -name "*.s")

COBJECTS = $(patsubst src/%,build/%,$(CSRCS:.c=.o))
SOBJECTS = $(patsubst src/%,build/%,$(SSRCS:.s=.o))
OBJECTS= ${COBJECTS} ${SOBJECTS}
TARGET=yona.bin
ISO=yona.iso

# Default target
all: $(ISO)

# Rule to make the iso
$(ISO): $(TARGET)
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/$(TARGET)
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

# Rule to make the binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
run:
	qemu-system-i386 -cdrom yona.iso -serial file:serialLogs.txt

# Rule to make the object files
build/%.o: src/%.c
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.s
			mkdir -p $(dir $@)
			$(AS) $(ASFLAGS) $< -o $@

# Clean rule
clean:
	rm -rf $(OBJECTS) $(TARGET) isodir
re: clean all
# fclean rule
fclean: clean
	rm -rf $(ISO)