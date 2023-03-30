#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *pf = fopen("data.bin", "wb");

    for (int i = 0; i < 32; ++i)
    {
        uint32_t d = 1 << i;
        fwrite(&d, sizeof(d), 1, pf);
    }

    fclose(pf);
    return 0;
}
