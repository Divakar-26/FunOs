[BITS 16]
[org 0x7c00]

    mov si, message
    call print

    call print_nl

    mov bp, 0x8000 ; set the stack safely away from us
    mov sp, bp

    mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000
    mov dh, 1 ; read 2 sectors
    ; the bios sets 'dl' for our boot disk number
    ; if you have trouble, use the '-fda' flag: 'qemu -fda file.bin'
    call disk_load

    mov si, successMessage
    call print

    call print_nl

    jmp 0x0000:0x9000


message db "Booting....", 0
successMessage db "sector Loaded!", 0
loadingMessage db "loading sector", 0

%include "boot_sect_print.asm"
%include "boot_sect_print_hex.asm"
%include "disk_load.asm"

times 510-($-$$) db 0
dw 0xaa55

