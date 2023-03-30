/**
 * 微程序生成
 */

#include <stdio.h>
#include "asm.h"
#include "pin.h"

// 取数组长度宏
#define ARR_LEN(_arr) (sizeof(_arr) / sizeof(*_arr))

// 取指
static const uint32_t FETCH[6] = {
    PC_OUT | MAR_IN,
    RAM_OUT | IR_IN | PC_INC,

    PC_OUT | MAR_IN,
    RAM_OUT | DST_IN | PC_INC,

    PC_OUT | MAR_IN,
    RAM_OUT | SRC_IN | PC_INC,
};

/// @brief 根据指令和状态字写微指令
/// @param buf 写微指令的位置
/// @param ir 指令
/// @param psw 程序状态字
/// @return
static int Write(uint32_t *buf, uint8_t ir, uint8_t psw)
{
    uint8_t op;
    uint8_t amd; // 目标地址寻址方式
    uint8_t ams; // 源地址寻址方式

    int isok = 1;

    if (ir & ADDR2)
    {
        // 二地址指令
        op = ir & 0xf0;
        amd = ((ir >> 2) & 0x3);
        ams = (ir & 0x3);

        switch (op)
        {
        case MOV:
        {
            // MOV指令
            if (amd == AM_REG && ams == AM_INS) // e.g MOV A, 1
            {
                *buf = PIN_DST_W | SRC_OUT | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g MOV A, B
            {
                *buf = PIN_DST_W | PIN_SRC_R | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_DIR) // e.g MOV A, [5]
            {
                *buf = MAR_IN | SRC_OUT;
                *++buf = PIN_DST_W | RAM_OUT | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_RAM) // e.g MOV A, [B]
            {
                *buf = MAR_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | RAM_OUT | PIN_CYC;
            }
            else if (amd == AM_DIR && ams == AM_INS) // e.g MOV [5], 1
            {
                *buf = MAR_IN | DST_OUT;
                *++buf = RAM_IN | SRC_OUT | PIN_CYC;
            }
            else if (amd == AM_DIR && ams == AM_REG) // e.g MOV [5], A
            {
                *buf = MAR_IN | DST_OUT;
                *++buf = RAM_IN | PIN_SRC_R | PIN_CYC;
            }
            else if (amd == AM_RAM && ams == AM_INS) // e.g MOV [A], 1
            {
                *buf = MAR_IN | PIN_DST_R;
                *++buf = RAM_IN | SRC_OUT | PIN_CYC;
            }
            else if (amd == AM_RAM && ams == AM_REG) // e.g MOV [A], B
            {
                *buf = MAR_IN | PIN_DST_R;
                *++buf = RAM_IN | PIN_SRC_R | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case ADD:
        {
            // ADD指令
            if (amd == AM_REG && ams == AM_INS) // e.g ADD A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = PIN_DST_W | OP_ADD | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g ADD A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | OP_ADD | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case SUB:
        {
            // SUB指令
            if (amd == AM_REG && ams == AM_INS) // e.g SUB A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = PIN_DST_W | OP_SUB | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g SUB A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | OP_SUB | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case AND:
        {
            // AND指令
            if (amd == AM_REG && ams == AM_INS) // e.g AND A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = PIN_DST_W | OP_AND | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g AND A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | OP_AND | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case OR:
        {
            // OR指令
            if (amd == AM_REG && ams == AM_INS) // e.g OR A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = PIN_DST_W | OP_OR | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g OR A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | OP_OR | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case XOR:
        {
            // XOR指令
            if (amd == AM_REG && ams == AM_INS) // e.g XOR A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = PIN_DST_W | OP_XOR | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g XOR A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = PIN_DST_W | OP_XOR | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case CMP:
        {
            // CMP指令
            if (amd == AM_REG && ams == AM_INS) // e.g CMP A, 1
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | SRC_OUT;
                *++buf = OP_SUB | PIN_ALU_PSW | PIN_CYC;
            }
            else if (amd == AM_REG && ams == AM_REG) // e.g CMP A, B
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = B_IN | PIN_SRC_R;
                *++buf = OP_SUB | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        default:
            isok = 0;
            break;
        }
    }
    else if (ir & ADDR1)
    {
        // 一地址指令
        op = ir & 0xfc;
        amd = (ir & 0x3);

        switch (op)
        {
        case INC:
        {
            // INC指令
            if (amd == AM_REG)
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = PIN_DST_W | OP_INC | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case DEC:
        {
            // DEC指令
            if (amd == AM_REG)
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = PIN_DST_W | OP_DEC | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case NOT:
        {
            // NOT指令
            if (amd == AM_REG)
            {
                *buf = A_IN | PIN_DST_R;
                *++buf = PIN_DST_W | OP_NOT | PIN_ALU_OUT | PIN_ALU_PSW | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case JMP:
        case JO:
        case JZ:
        case JP:
        case JNO:
        case JNZ:
        case JNP:
        {
            // 转移指令
            int overflow = psw & 1;
            int zero = psw & 2;
            int parity = psw & 4;
            int jump = op == JMP                 // 无条件跳转
                       || op == JO && overflow   // 溢出跳转
                       || op == JZ && zero       // 零跳转
                       || op == JP && parity     // 奇跳转
                       || op == JNO && !overflow // 非溢出跳转
                       || op == JNZ && !zero     // 非零跳转
                       || op == JNP && !parity;  // 偶跳转
            if (amd == AM_INS)
                *buf = jump ? (PC_IN | DST_OUT | PIN_CYC)
                            : (PIN_CYC);
            else
                isok = 0;
        }
        break;

        case PUSH:
        {
            // PUSH指令
            if (amd == AM_INS)
            {
                *buf = A_IN | SP_OUT;
                *++buf = SP_IN | OP_DEC | PIN_ALU_OUT;
                *++buf = MAR_IN | SP_OUT;
                *++buf = MSR_IN | SS_OUT;
                *++buf = RAM_IN | DST_OUT;
                *++buf = MSR_IN | CS_OUT | PIN_CYC;
            }
            else if (amd == AM_REG)
            {
                *buf = A_IN | SP_OUT;
                *++buf = SP_IN | OP_DEC | PIN_ALU_OUT;
                *++buf = MAR_IN | SP_OUT;
                *++buf = MSR_IN | SS_OUT;
                *++buf = RAM_IN | PIN_DST_R;
                *++buf = MSR_IN | CS_OUT | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case POP:
        {
            // POP指令
            if (amd == AM_REG)
            {
                *buf = MAR_IN | SP_OUT;
                *++buf = MSR_IN | SS_OUT;
                *++buf = PIN_DST_W | RAM_OUT;
                *++buf = A_IN | SP_OUT;
                *++buf = SP_IN | OP_INC | PIN_ALU_OUT;
                *++buf = MSR_IN | CS_OUT | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case CALL:
        {
            // CALL指令
            if (amd == AM_INS)
            {
                // push pc
                *buf = A_IN | SP_OUT;
                *++buf = SP_IN | OP_DEC | PIN_ALU_OUT;
                *++buf = MAR_IN | SP_OUT;
                *++buf = MSR_IN | SS_OUT;
                *++buf = RAM_IN | PC_OUT;
                *++buf = MSR_IN | CS_OUT;
                // mov pc, dst
                *++buf = PC_IN | DST_OUT | PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        case INT:
        {
            // INT指令
            int interrupt = psw & 8;
            if (amd == AM_INS)
            {
                if (interrupt)
                {
                    *buf = A_IN | SP_OUT;
                    *++buf = SP_IN | OP_DEC | PIN_ALU_OUT;
                    *++buf = MAR_IN | SP_OUT;
                    *++buf = MSR_IN | SS_OUT;
                    *++buf = RAM_IN | PC_OUT;
                    *++buf = MSR_IN | CS_OUT;
                    *++buf = PC_IN | DST_OUT | PIN_ALU_PSW | ALU_CLI | PIN_CYC;
                }
                else
                    *buf = PIN_CYC;
            }
            else
            {
                isok = 0;
            }
        }
        break;

        default:
            isok = 0;
            break;
        }
    }
    else
    {
        // 零地址指令
        op = ir;

        switch (op)
        {
        case NOP:
            *buf = PIN_CYC;
            break;

        case RET:
        {
            // pop pc
            *buf = MAR_IN | SP_OUT;
            *++buf = MSR_IN | SS_OUT;
            *++buf = PC_IN | RAM_OUT;
            *++buf = A_IN | SP_OUT;
            *++buf = SP_IN | OP_INC | PIN_ALU_OUT;
            *++buf = MSR_IN | CS_OUT | PIN_CYC;
        }
        break;

        case IRET:
        {
            *buf = MAR_IN | SP_OUT;
            *++buf = MSR_IN | SS_OUT;
            *++buf = PC_IN | RAM_OUT;
            *++buf = A_IN | SP_OUT;
            *++buf = SP_IN | OP_INC | PIN_ALU_OUT;
            *++buf = MSR_IN | CS_OUT | PIN_ALU_PSW | ALU_STI | PIN_CYC;
        }
        break;

        case STI:
            *buf = PIN_ALU_PSW | ALU_STI;
            break;

        case CLI:
            *buf = PIN_ALU_PSW | ALU_CLI;
            break;

        case HLT:
            *buf = PIN_HLT;
            break;

        default:
            isok = 0;
            break;
        }
    }

    if (!isok)
        *buf = PIN_HLT; // 未定义指令直接HLT
    return isok;
}

int main(int argc, char *argv[])
{
    const char *output = "micro.bin";
    static uint32_t micro[0x10000];

    if (argc == 2)
        output = argv[1];

    for (int i = 0; i < ARR_LEN(micro); i += 0x10)
    {
        uint8_t ir = (i >> 8) & 0xff;
        uint8_t psw = (i >> 4) & 0xf;
        uint8_t cyc = 0;

        while (cyc < ARR_LEN(FETCH))
        {
            micro[i + cyc] = FETCH[cyc];
            ++cyc;
        }

        Write(&micro[i + cyc], ir, psw);
    }

    FILE *pf = fopen(output, "wb");
    fwrite(micro, sizeof(micro), 1, pf);
    fclose(pf);

    return 0;
}
