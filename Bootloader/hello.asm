[BITS 16]

mov si, message

print:
    mov al, [si]
    cmp al, 0
    je done

    mov ah, 0x0e
    int 0x10

    add si, 1
    jmp print

done:   
    hlt
    jmp done
    
message db "hello world ", 0

times 510-($-$$) db 0
dw 0xaa55