section .data
    msg db "Hello, World!", 0

section .text
    mov ebx, msg
    call print_string_pm

%include "print_32.asm"