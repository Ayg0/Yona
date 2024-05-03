# Variables
CC=i386-elf-gcc
AS=i386-elf-as
LDSCRIPT=src/linker.ld
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS=
LDFLAGS=-T $(LDSCRIPT) -ffreestanding -O2 -nostdlib -lgcc

CSRCS=src/kernel.c
SSRCS=src/boot.s

COBJECTS = ${CSRCS:.c=.o}
SOBJECTS = ${SSRCS:.s=.o}
OBJECTS= ${COBJECTS} ${SOBJECTS}
TARGET=myos.bin
ISO=myos.iso

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

# Rule to make the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Clean rule
clean:
	rm -rf $(OBJECTS) $(TARGET) $(ISO) isodir