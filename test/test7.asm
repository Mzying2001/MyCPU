; int & iret

    mov ss, 1
    mov sp, 0x20
    jmp start

show:
    mov d, 255
    iret

start:
    mov c, 0

increase:
    inc c
    mov d, c
    jp disable

enable:
    sti
    jmp int

disable:
    cli

int:
    int show
    jmp increase

    hlt
