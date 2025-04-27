BUILD_DIR = build

# Define the cross-compiler toolchain
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld
GDB = i686-elf-gdb

CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c
ASFLAGS = -f elf
LDFLAGS = -m elf_i386 -Ttext 0x1000

# Find all source files
C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
ASM_SOURCES := $(wildcard kernel/*.asm cpu/*.asm)
HEADERS := $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

# Convert source files to object file paths in $(BUILD_DIR)
C_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))
OBJ = $(C_OBJECTS) $(ASM_OBJECTS)

all: $(BUILD_DIR) $(BUILD_DIR)/os-image.img

# Create necessary directories for object files
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/kernel $(BUILD_DIR)/drivers $(BUILD_DIR)/cpu $(BUILD_DIR)/libc

# Bootloader
$(BUILD_DIR)/bootloader.bin: Bootloader/bootloader.asm | $(BUILD_DIR)
	$(AS) -f bin -I Bootloader/ Bootloader/bootloader.asm -o $@

# Compile Assembly files
$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: %.c $(HEADERS) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Link Kernel
$(BUILD_DIR)/kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) --oformat binary $^ -o $@

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

# Create OS Image
$(BUILD_DIR)/os-image.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BUILD_DIR)/bootloader.bin of=$@ seek=0 conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$@ seek=1 conv=notrunc

# Run in QEMU
run: all
	qemu-system-i386 -drive format=raw,file=$(BUILD_DIR)/os-image.img

# Debug with GDB
debug: all
	qemu-system-i386 -s -S -drive format=raw,file=$(BUILD_DIR)/os-image.img & \
	$(GDB) $(BUILD_DIR)/kernel.elf -ex "target remote localhost:1234"

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR)

full:
	qemu-system-x86_64 -drive file=build/os-image.img -display sdl,full-screen=on