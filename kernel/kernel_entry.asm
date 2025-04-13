[bits 32]
[extern main]  ; Ensure linker finds this symbol
global _start
_start:
    call main  ; Call kernel's main function
    jmp $
