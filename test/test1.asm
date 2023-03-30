;test

mov a, 123 ;aaaaa
mov b, 123

mov c, [0]
mov d, [1]

mov [0xf0], 200
mov [0xf1], a

mov a, 0xe0
mov [a], 0xff
mov [a], b

mov d, [a]

hlt
