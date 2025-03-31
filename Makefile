all:
	nasm -f bin -I Bootloader/ Bootloader/bootloader.asm -o bootloader.bin
	nasm -f elf kernel/kernel_entry.asm -o kernel/kernel_entry.o
	gcc -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -c kernel/kernel.c -o kernel/kernel.o
	ld -m elf_i386 -Ttext 0x1000 --oformat binary kernel/kernel_entry.o kernel/kernel.o -o kernel.bin
	dd if=/dev/zero of=os-image.img bs=512 count=2880
	dd if=bootloader.bin of=os-image.img conv=notrunc
	dd if=kernel.bin of=os-image.img seek=1 conv=notrunc
	qemu-system-i386 -drive format=raw,file=os-image.img
