#include <stdio.h>
#include <stdint.h>

const uint16_t WE_A = 1 << 0;
const uint16_t CS_A = 1 << 1;

const uint16_t WE_B = 1 << 2;
const uint16_t CS_B = 1 << 3;

const uint16_t WE_C = 1 << 4;
const uint16_t CS_C = 1 << 5;

const uint16_t ALU_ADD = 0;
const uint16_t ALU_SUB = 1 << 6;
const uint16_t ALU_OUT = 1 << 7;

const uint16_t WE_M = 1 << 8;
const uint16_t CS_M = 1 << 9;

const uint16_t WE_PC = 1 << 10;
const uint16_t EN_PC = 1 << 11;
const uint16_t CS_PC = 1 << 12;

const uint16_t HLT = 1 << 15;

int main()
{
    // 微程序
    uint16_t microprog[] = {
        CS_M | CS_A | WE_A | CS_PC | EN_PC | WE_PC,
        CS_M | CS_B | WE_B | CS_PC | EN_PC | WE_PC,
        ALU_ADD | ALU_OUT | CS_C | WE_C,
        CS_C | CS_M | WE_M | CS_PC | EN_PC | WE_PC,
        HLT,
    };

    FILE *pf = fopen("data.bin", "wb");
    fwrite(microprog, sizeof(microprog), 1, pf);
    fclose(pf);
    return 0;
}
