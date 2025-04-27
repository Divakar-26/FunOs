[BITS 16]
switch_to_pm:
    cli        ; 1 diable interupts

    mov si, switchingMsg
    call print  ; Print this BEFORE switching


    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    

    jmp CODE_SEG:init_pm


[BITS 32]


init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM

switchingMsg db "Switching to 32-bit mode...", 0


; entering videomo
; [BITS 16]
; switch_to_pm:
;     cli                        ; Disable interrupts
; 
;     ; Switch to video mode 13h (320x200, 256 colors)
;     mov ah, 0x00               ; BIOS function: Set video mode
;     mov al, 0x13               ; Video mode 13h (320x200, 256 colors)
;     int 0x10                   ; Call BIOS video interrupt
; 
;     ; Print message before switching to PM
;     mov si, switchingMsg
;     call print                 ; Print the message
; 
;     lgdt [gdt_descriptor]      ; Load the GDT descriptor
; 
;     ; Enable Protected Mode by setting the PE (Protection Enable) bit in CR0
;     mov eax, cr0
;     or eax, 0x1                ; Set PE bit to 1 (enable protected mode)
;     mov cr0, eax
; 
;     ; Jump to the 32-bit protected mode code
;     jmp CODE_SEG:init_pm
; 
; 
; [BITS 32]
; 
; init_pm:
;     mov ax, DATA_SEG
;     mov ds, ax                 ; Initialize data segments
;     mov ss, ax
;     mov es, ax
;     mov fs, ax
;     mov gs, ax
; 
;     mov ebp, 0x90000           ; Set up stack pointer for PM
;     mov esp, ebp
; 
;     call BEGIN_PM              ; Call the 32-bit code
; 
; switchingMsg db "Switching to 32-bit mode...", 0
; 