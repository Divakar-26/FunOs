[BITS 16]

global disk_load 

disk_load:
    pusha

    ; Save input parameters (drive # in DL, # of sectors in DH)
    push dx

    ; Print "Reading disk..."
    mov si, startMessage
    call print
    call print_nl

    mov ah, 0x02    ; BIOS Read Sectors function
    mov al, dh      ; Number of sectors to read
    mov cl, 0x02    ; Sector number (start from sector 2)
    mov ch, 0x00    ; Cylinder number
    mov dh, 0x00    ; Head number
    int 0x13        ; Call BIOS Disk interrupt

    jc disk_error   ; Jump to error if the Carry Flag is set (disk read failed)

    pop dx          ; Restore original sector count (in DH)
    cmp al, dh      ; Compare actual read sectors with requested sectors
    jne sectors_error ; If mismatch, print error

    popa            ; Restore registers
    ret             ; Return to caller

disk_error:
    mov si, diskErrorMessage
    call print
    jmp $

sectors_error:
    mov si, sectorsErrorMessage
    call print
    jmp $

startMessage db "Reading disk...", 0
diskErrorMessage db "Disk read error!", 0
sectorsErrorMessage db "Incorrect number of sectors read!", 0
