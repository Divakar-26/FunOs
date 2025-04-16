[BITS 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl
    mov bp, 0x8000
    mov sp, bp

    mov si, message
    call print
    call print_nl

    call load_kernel
    call switch_to_pm
    jmp $


%include "boot_sect_print.asm"
%include "boot_sect_print_hex.asm"
%include "disk_load.asm"
%include "32bit_gdt.asm"
%include "32bit_switch.asm"
%include "print_32.asm"

[BITS 16]
load_kernel:
    mov si, loadingMessage
    call print
    call print_nl

    mov ax, 0x100
    mov es, ax
    mov bx, 0x0000

    mov dh, 53
    mov dl, [BOOT_DRIVE]
    

    call disk_load

    mov si, successMessage
    call print
    call print_nl
    ret


[BITS 32]
BEGIN_PM:

    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET
    jmp $



MSG_PROT_MODE db "", 0
loadingMessage db "Loading kernel...", 0
message db "B..", 0
successMessage db "Kernel Loaded", 0
errorMessage db "''", 0


BOOT_DRIVE db 0         ;store boot drive number

times 510-($-$$) db 0
dw 0xaa55