# Variables
CC=i386-elf-gcc
AS=nasm
SRC_DIR=src
ARCH=arch/i386
LDSCRIPT=$(SRC_DIR)/$(ARCH)/linker.ld
# DO_I_SPEED=-O2
DO_I_SPEED=
CFLAGS=-std=gnu99 -ffreestanding ${DO_I_SPEED} -Wall -Wextra -Werror\
		 -fno-builtin -nodefaultlibs -Isrc/include\
		 -mno-red-zone -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2\
		 -fno-stack-protector -fno-omit-frame-pointer

ASFLAGS=-f elf32
LDFLAGS=-T $(LDSCRIPT) -ffreestanding -nostdlib -lgcc

CSRCS=$(shell find $(SRC_DIR) -type f -name "*.c")
SSRCS=$(shell find $(SRC_DIR) -type f -name "*.s")

COBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(CSRCS:.c=.o))
SOBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(SSRCS:.s=.o))
OBJECTS= ${COBJECTS} ${SOBJECTS}
TARGET=yona.bin
ISO=yona.iso

# Default target
all: $(ISO)

# Rule to make the iso
$(ISO): $(TARGET)
	mkdir -p isodir/boot/grub
	cp $(TARGET) isodir/boot/$(TARGET)
	cp $(SRC_DIR)/$(ARCH)/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

# Rule to make the binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
run:
	qemu-system-i386 -cdrom yona.iso\
	 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker\
	 -serial file:serialLogs.txt -no-reboot -no-shutdown

# Rule to make the object files
build/%.o: $(SRC_DIR)/%.c
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) -c $< -o $@

build/%.o: $(SRC_DIR)/%.s
			mkdir -p $(dir $@)
			$(AS) $(ASFLAGS) $< -o $@

# Clean rule
clean:
	rm -rf $(OBJECTS) $(TARGET) isodir

# re rule
re: clean all

# fclean rule
fclean: clean
	rm -rf $(ISO)