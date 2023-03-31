#ifndef _ASM_H_
#define _ASM_H_

#define ADDR2       (1 << 7) // 二地址指令 1xxxaabb
#define ADDR1       (1 << 6) // 一地址指令 01xxxxaa
#define ADDR2_SHIFT (4)
#define ADDR1_SHIFT (2)

#define AM_INS  (0) // 立即数寻址
#define AM_REG  (1) // 寄存器寻址
#define AM_DIR  (2) // 直接寻址
#define AM_RAM  (3) // 寄存器间接寻址

#define MOV     (ADDR2 | (0 << ADDR2_SHIFT))    // 数据传送
#define ADD     (ADDR2 | (1 << ADDR2_SHIFT))    // 加法
#define SUB     (ADDR2 | (2 << ADDR2_SHIFT))    // 减法
#define AND     (ADDR2 | (3 << ADDR2_SHIFT))    // 按位与
#define OR      (ADDR2 | (4 << ADDR2_SHIFT))    // 按位或
#define XOR     (ADDR2 | (5 << ADDR2_SHIFT))    // 按位异或
#define CMP     (ADDR2 | (6 << ADDR2_SHIFT))    // 比较

#define INC     (ADDR1 | (0 << ADDR1_SHIFT))    // 加一
#define DEC     (ADDR1 | (1 << ADDR1_SHIFT))    // 减一
#define NOT     (ADDR1 | (2 << ADDR1_SHIFT))    // 按位非
#define JMP     (ADDR1 | (3 << ADDR1_SHIFT))    // 无条件跳转
#define JO      (ADDR1 | (4 << ADDR1_SHIFT))    // 溢出跳转
#define JZ      (ADDR1 | (5 << ADDR1_SHIFT))    // 零跳转
#define JP      (ADDR1 | (6 << ADDR1_SHIFT))    // 奇跳转
#define JNO     (ADDR1 | (7 << ADDR1_SHIFT))    // 非溢出跳转
#define JNZ     (ADDR1 | (8 << ADDR1_SHIFT))    // 非零跳转
#define JNP     (ADDR1 | (9 << ADDR1_SHIFT))    // 偶跳转
#define PUSH    (ADDR1 | (10 << ADDR1_SHIFT))   // 入栈
#define POP     (ADDR1 | (11 << ADDR1_SHIFT))   // 出栈
#define CALL    (ADDR1 | (12 << ADDR1_SHIFT))   // 函数调用
#define INT     (ADDR1 | (13 << ADDR1_SHIFT))   // 中断

#define NOP     (0)     // 空指令
#define RET     (1)     // 函数返回
#define IRET    (2)     // 中断返回
#define STI     (3)     // 开中断
#define CLI     (4)     // 关中断
#define HLT     (0x3f)  // 暂停

#endif //_ASM_H_
