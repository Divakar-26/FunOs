BUILD_DIR = build

# Define the cross-compiler toolchain
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld
GDB = i686-elf-gdb

all: $(BUILD_DIR) $(BUILD_DIR)/os-image.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Bootloader
$(BUILD_DIR)/bootloader.bin: Bootloader/bootloader.asm
	$(AS) -f bin -I Bootloader/ Bootloader/bootloader.asm -o $(BUILD_DIR)/bootloader.bin

# Kernel entry (Assembly)
$(BUILD_DIR)/kernel_entry.o: kernel/kernel_entry.asm
	$(AS) -f elf kernel/kernel_entry.asm -o $(BUILD_DIR)/kernel_entry.o

# Kernel (C)
$(BUILD_DIR)/kernel.o: kernel/kernel.c drivers/screen.h cpu/ports.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c kernel/kernel.c -o $(BUILD_DIR)/kernel.o

# Ports (C)
$(BUILD_DIR)/ports.o: cpu/ports.c cpu/ports.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c cpu/ports.c -o $(BUILD_DIR)/ports.o

# Screen (C)
$(BUILD_DIR)/screen.o: drivers/screen.c drivers/screen.h cpu/ports.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c drivers/screen.c -o $(BUILD_DIR)/screen.o

#UTILS (C)
$(BUILD_DIR)/util.o: libc/util.c libc/util.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c libc/util.c -o $(BUILD_DIR)/util.o

$(BUILD_DIR)/idt.o: cpu/idt.c cpu/idt.h cpu/types.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c cpu/idt.c -o $(BUILD_DIR)/idt.o

# ISR (C)
$(BUILD_DIR)/isr.o: cpu/isr.c cpu/isr.h cpu/idt.h drivers/screen.h libc/util.h
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c cpu/isr.c -o $(BUILD_DIR)/isr.o

$(BUILD_DIR)/timer.o: cpu/timer.c cpu/timer.h 
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c cpu/timer.c -o $(BUILD_DIR)/timer.o

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c drivers/keyboard.h 
	$(CC) -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -g -c drivers/keyboard.c -o $(BUILD_DIR)/keyboard.o



$(BUILD_DIR)/interrupt.o: cpu/interrupt.asm
	$(AS) -f elf cpu/interrupt.asm -o $(BUILD_DIR)/interrupt.o

# Link Kernel
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/ports.o $(BUILD_DIR)/screen.o $(BUILD_DIR)/util.o $(BUILD_DIR)/interrupt.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/isr.o   $(BUILD_DIR)/timer.o  $(BUILD_DIR)/keyboard.o  
	$(LD) -m elf_i386 -Ttext 0x1000 --oformat binary $^ -o $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/ports.o $(BUILD_DIR)/screen.o $(BUILD_DIR)/util.o $(BUILD_DIR)/interrupt.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/isr.o  $(BUILD_DIR)/timer.o  $(BUILD_DIR)/keyboard.o  
	$(LD) -m elf_i386 -Ttext 0x1000 $^ -o $(BUILD_DIR)/kernel.elf

# Create OS Image
$(BUILD_DIR)/os-image.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	dd if=/dev/zero of=$(BUILD_DIR)/os-image.img bs=512 count=2880
	dd if=$(BUILD_DIR)/bootloader.bin of=$(BUILD_DIR)/os-image.img conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/os-image.img seek=1 conv=notrunc

# Run in QEMU
run: all
	qemu-system-i386 -drive format=raw,file=$(BUILD_DIR)/os-image.img

# Debug with GDB
debug: all
	qemu-system-i386 -s -S -drive format=raw,file=$(BUILD_DIR)/os-image.img & 
	$(GDB) $(BUILD_DIR)/kernel.elf -ex "target remote localhost:1234"

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR)