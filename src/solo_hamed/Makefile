# Compiler and linker settings
CC = i686-unknown-linux-gnu-gcc
AS = i686-unknown-linux-gnu-as
LINKER = i686-unknown-linux-gnu-gcc

# Compiler flags
CFLAGS = -std=gnu99 -ffreestanding -O0 -Wall -Wextra -g

# Source and build directories
SRC_DIR = src
OBJ_DIR = obj
BOOT_DIR = isodir

# Source files
C_SOURCES = $(shell find $(SRC_DIR) -name '*.c')
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.s)

# Object files
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
ALL_OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Final kernel binary and ISO
KERNEL_BIN = hamed.bin
KERNEL_ISO = hamed.iso

# QEMU command
QEMU = qemu-system-i386
QEMU_FLAGS = -m 128M -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0

# GDB command
GDB = gdb

# Default target
.PHONY: all
all: clean prepare $(KERNEL_BIN) iso

# Create necessary directories
.PHONY: prepare
prepare:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/song
	mkdir -p $(BOOT_DIR)/boot/grub

# Build C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build assembly source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $< -o $@

# Link the kernel
$(KERNEL_BIN): $(ALL_OBJECTS) linker.ld
	$(LINKER) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $(ALL_OBJECTS) -lgcc
	@echo "Kernel build complete!"
	@if grub-file --is-x86-multiboot $@; then \
		echo "Multiboot confirmed."; \
	else \
		echo "ERROR: The file is not multiboot compliant!"; \
		false; \
	fi

# Create bootable ISO
.PHONY: iso
iso: $(KERNEL_BIN)
	cp $(KERNEL_BIN) $(BOOT_DIR)/boot/
	cp grub.cfg $(BOOT_DIR)/boot/grub/
	grub-mkrescue -o $(KERNEL_ISO) $(BOOT_DIR)
	@echo "ISO created: $(KERNEL_ISO)"

# Run the kernel in QEMU
.PHONY: run
run: iso
	$(QEMU) $(QEMU_FLAGS) -cdrom $(KERNEL_ISO)

# Run the kernel directly (without ISO)
.PHONY: run-direct
run-direct: $(KERNEL_BIN)
	$(QEMU) $(QEMU_FLAGS) -kernel $(KERNEL_BIN)

# Debug the kernel with GDB
.PHONY: debug
debug: $(KERNEL_BIN)
	$(QEMU) $(QEMU_FLAGS) -kernel $(KERNEL_BIN) -S -s &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_BIN)"

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(KERNEL_BIN) $(KERNEL_ISO)
	rm -rf $(BOOT_DIR)

# Clean everything, including the ISO
.PHONY: distclean
distclean: clean
	rm -f $(KERNEL_ISO)
