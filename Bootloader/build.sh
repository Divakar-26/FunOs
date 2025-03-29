#!/bin/bash

set -e  # Exit if any command fails

# Assemble the bootloader (Stage 1)
nasm -f bin bootloader.asm -o bootloader.bin

# Assemble the second stage loader (Stage 2)
nasm -f bin second_stage.asm -o second_stage.bin

# Create a 1440 KB (1.44MB) floppy disk image
dd if=/dev/zero of=os.img bs=512 count=2880 status=none

# Write the bootloader (Stage 1) to the first sector (512 bytes)
dd if=bootloader.bin of=os.img bs=512 count=1 conv=notrunc status=none

# Write the second stage loader (Stage 2) starting at sector 2
dd if=second_stage.bin of=os.img bs=512 seek=1 conv=notrunc status=none

echo "Build successful! Image created: os.img"

# Run in QEMU
qemu-system-x86_64 -drive format=raw,file=os.img
