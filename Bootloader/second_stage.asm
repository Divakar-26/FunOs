[BITS 16]
[ORG 0x0000]

second_stage:
    mov ax, 0x8000
    mov ds, ax

    mov si, secondMessage
    call print

hang:
    jmp hang  ; Infinite loop

secondMessage db "Second stage Loaded!", 0

%include "boot_sect_print.asm"

times 256 dw 0xface  ; Ensure exactly 1 sector