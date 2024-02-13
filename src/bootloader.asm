bits 16

; the boot sector is loaded at 0x7c00
org 0x7c00

start:
    ; disable interrupts
    cli

    ; load gdt
    xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, gdt_null
    mov [gdtr + 2], eax
    mov eax, gdt_end
    sub eax, gdt_null
    mov [gdtr], ax
    lgdt [gdtr]

    ; set segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; set up stack
    mov sp, 0x9000

    ; read kernel
    mov bx, 0x1000
    mov ah, 2
    mov al, 32
    mov ch, 0
    mov dh, 0
    mov cl, 2
    int 0x13

    ; enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; jump to kernel
    jmp 0x08:0x1000

; gdt register
gdtr:
    dw 0  ; limit
    dd 0  ; base

; gdt entries
gdt_null:
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0

gdt_kernel_code:
    db 0xff
    db 0xff
    db 0
    db 0
    db 0
    db 0x9a
    db 0xcf
    db 0

gdt_kernel_data:
    db 0xff
    db 0xff
    db 0
    db 0
    db 0
    db 0x92
    db 0xcf
    db 0

gdt_user_code:
    db 0xff
    db 0xff
    db 0
    db 0
    db 0
    db 0xfa
    db 0xcf
    db 0

gdt_user_data:
    db 0xff
    db 0xff
    db 0
    db 0
    db 0
    db 0xf2
    db 0xcf
    db 0

gdt_end:

; boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
