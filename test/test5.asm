;stack operate

mov SS, 1
mov SP, 0x10

mov D, 10
push D
push 1

pop C
pop B
mov A, C

add A, B
mov D, A

hlt
