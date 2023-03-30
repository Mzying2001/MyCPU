; call & ret

    mov ss, 1
    mov sp, 0x20
    jmp start

show:
    mov d, 255
    ret

start:
    mov c, 0

increase:
    inc c
    mov d, c
    call show
    jmp increase

    hlt
