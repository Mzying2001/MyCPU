#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>

static const uint8_t ADDR2 = 1 << 7; // 二地址指令 1xxxaabb
static const uint8_t ADDR1 = 1 << 6; // 一地址指令 01xxxxaa
static const uint8_t ADDR2_SHIFT = 4;
static const uint8_t ADDR1_SHIFT = 2;

#define AM_INS 0 // 立即数寻址
#define AM_REG 1 // 寄存器寻址
#define AM_DIR 2 // 直接寻址
#define AM_RAM 3 // 寄存器间接寻址

static const uint8_t MOV = ADDR2 | (0 << ADDR2_SHIFT); // 数据传送
static const uint8_t ADD = ADDR2 | (1 << ADDR2_SHIFT); // 加法
static const uint8_t SUB = ADDR2 | (2 << ADDR2_SHIFT); // 减法
static const uint8_t AND = ADDR2 | (3 << ADDR2_SHIFT); // 按位与
static const uint8_t OR = ADDR2 | (4 << ADDR2_SHIFT);  // 按位或
static const uint8_t XOR = ADDR2 | (5 << ADDR2_SHIFT); // 按位异或
static const uint8_t CMP = ADDR2 | (6 << ADDR2_SHIFT); // 比较

static const uint8_t INC = ADDR1 | (0 << ADDR1_SHIFT);   // 加一
static const uint8_t DEC = ADDR1 | (1 << ADDR1_SHIFT);   // 减一
static const uint8_t NOT = ADDR1 | (2 << ADDR1_SHIFT);   // 按位非
static const uint8_t JMP = ADDR1 | (3 << ADDR1_SHIFT);   // 无条件跳转
static const uint8_t JO = ADDR1 | (4 << ADDR1_SHIFT);    // 溢出跳转
static const uint8_t JZ = ADDR1 | (5 << ADDR1_SHIFT);    // 零跳转
static const uint8_t JP = ADDR1 | (6 << ADDR1_SHIFT);    // 奇跳转
static const uint8_t JNO = ADDR1 | (7 << ADDR1_SHIFT);   // 非溢出跳转
static const uint8_t JNZ = ADDR1 | (8 << ADDR1_SHIFT);   // 非零跳转
static const uint8_t JNP = ADDR1 | (9 << ADDR1_SHIFT);   // 偶跳转
static const uint8_t PUSH = ADDR1 | (10 << ADDR1_SHIFT); // 入栈
static const uint8_t POP = ADDR1 | (11 << ADDR1_SHIFT);  // 出栈
static const uint8_t CALL = ADDR1 | (12 << ADDR1_SHIFT); // 函数调用
static const uint8_t INT = ADDR1 | (13 << ADDR1_SHIFT);  // 中断

static const uint8_t NOP = 0;    // 空指令
static const uint8_t RET = 1;    // 函数返回
static const uint8_t IRET = 2;   // 中断返回
static const uint8_t STI = 3;    // 开中断
static const uint8_t CLI = 4;    // 关中断
static const uint8_t HLT = 0x3f; // 暂停

#endif //_ASM_H_
