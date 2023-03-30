main:
    mov d, 0
    jmp increase
    mov d, 0xff

increase:
    inc d
    cmp d, 5
    jo increase

decrease:
    dec d
    cmp d, 0
    jnz decrease
    jz increase

    hlt
