#ifndef _PIN_H_
#define _PIN_H_

#include <stdint.h>

enum
{
    MSR = 1,
    MAR,
    MDR,
    RAM,
    IR,
    DST,
    SRC,
    A,
    B,
    C,
    D,
    DI,
    SI,
    SP,
    BP,
    CS,
    DS,
    SS,
    ES,
    VEC,
    T1,
    T2,
};

static const uint32_t MSR_OUT = MSR;
static const uint32_t MAR_OUT = MAR;
static const uint32_t MDR_OUT = MDR;
static const uint32_t RAM_OUT = RAM;
static const uint32_t IR_OUT = IR;
static const uint32_t DST_OUT = DST;
static const uint32_t SRC_OUT = SRC;
static const uint32_t A_OUT = A;
static const uint32_t B_OUT = B;
static const uint32_t C_OUT = C;
static const uint32_t D_OUT = D;
static const uint32_t DI_OUT = DI;
static const uint32_t SI_OUT = SI;
static const uint32_t SP_OUT = SP;
static const uint32_t BP_OUT = BP;
static const uint32_t CS_OUT = CS;
static const uint32_t DS_OUT = DS;
static const uint32_t SS_OUT = SS;
static const uint32_t ES_OUT = ES;
static const uint32_t VEC_OUT = VEC;
static const uint32_t T1_OUT = T1;
static const uint32_t T2_OUT = T2;

#define _DST_SHIFT 5

static const uint32_t MSR_IN = MSR << _DST_SHIFT;
static const uint32_t MAR_IN = MAR << _DST_SHIFT;
static const uint32_t MDR_IN = MDR << _DST_SHIFT;
static const uint32_t RAM_IN = RAM << _DST_SHIFT;
static const uint32_t IR_IN = IR << _DST_SHIFT;
static const uint32_t DST_IN = DST << _DST_SHIFT;
static const uint32_t SRC_IN = SRC << _DST_SHIFT;
static const uint32_t A_IN = A << _DST_SHIFT;
static const uint32_t B_IN = B << _DST_SHIFT;
static const uint32_t C_IN = C << _DST_SHIFT;
static const uint32_t D_IN = D << _DST_SHIFT;
static const uint32_t DI_IN = DI << _DST_SHIFT;
static const uint32_t SI_IN = SI << _DST_SHIFT;
static const uint32_t SP_IN = SP << _DST_SHIFT;
static const uint32_t BP_IN = BP << _DST_SHIFT;
static const uint32_t CS_IN = CS << _DST_SHIFT;
static const uint32_t DS_IN = DS << _DST_SHIFT;
static const uint32_t SS_IN = SS << _DST_SHIFT;
static const uint32_t ES_IN = ES << _DST_SHIFT;
static const uint32_t VEC_IN = VEC << _DST_SHIFT;
static const uint32_t T1_IN = T1 << _DST_SHIFT;
static const uint32_t T2_IN = T2 << _DST_SHIFT;

static const uint32_t PIN_SRC_R = 1 << 10; // 读SRC寄存器的值所指向的寄存器
static const uint32_t PIN_SRC_W = 1 << 11; // 写SRC寄存器的值所指向的寄存器
static const uint32_t PIN_DST_R = 1 << 12; // 读DST寄存器的值所指向的寄存器
static const uint16_t PIN_DST_W = 1 << 13; // 写DST寄存器的值所指向的寄存器

static const uint32_t PIN_PC_WE = 1 << 14; // 程序计数器写使能
static const uint32_t PIN_PC_CS = 1 << 15; // 程序计数器片选
static const uint32_t PIN_PC_EN = 1 << 16; // 程序计数器开启（自增），此时写无效

static const uint32_t PC_OUT = PIN_PC_CS;                         // 输出程序计数器的值
static const uint32_t PC_IN = PIN_PC_CS | PIN_PC_WE;              // 写程序计数器
static const uint32_t PC_INC = PIN_PC_CS | PIN_PC_WE | PIN_PC_EN; // 程序计数器自增

#define _OP_SHIFT 17

static const uint32_t OP_ADD = 0 << _OP_SHIFT; // 加法
static const uint32_t OP_SUB = 1 << _OP_SHIFT; // 减法
static const uint32_t OP_INC = 2 << _OP_SHIFT; // 加一
static const uint32_t OP_DEC = 3 << _OP_SHIFT; // 减一
static const uint32_t OP_AND = 4 << _OP_SHIFT; // 按位与
static const uint32_t OP_OR = 5 << _OP_SHIFT;  // 按位或
static const uint32_t OP_XOR = 6 << _OP_SHIFT; // 按位异或
static const uint32_t OP_NOT = 7 << _OP_SHIFT; // 按位非

static const uint32_t PIN_ALU_OUT = 1 << 20;   // ALU输出
static const uint32_t PIN_ALU_PSW = 1 << 21;   // ALU开PSW
static const uint32_t PIN_ALU_INT_W = 1 << 22; // 中断写入
static const uint32_t PIN_ALU_INT = 1 << 23;   // 中断无效

static const uint32_t ALU_STI = PIN_ALU_INT_W;               // 中断允许
static const uint32_t ALU_CLI = PIN_ALU_INT_W | PIN_ALU_INT; // 关中断

static const uint32_t PIN_HLT = 1 << 31; // 程序停止位
static const uint32_t PIN_CYC = 1 << 30; // CYC清零位，执行下一条指令

#endif //_PIN_H_
