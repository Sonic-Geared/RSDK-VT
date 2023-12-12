#include "RetroEngine.hpp"
#include <math.h>

int sin1024LookupTable[0x400];
int cos1024LookupTable[0x400];
int tan1024LookupTable[0x400];
int asin1024LookupTable[0x400];
int acos1024LookupTable[0x400];

int sinM7LookupTable[0x200];
int cosM7LookupTable[0x200];

int sin512LookupTable[0x200];
int cos512LookupTable[0x200];
int tan512LookupTable[0x200];
int asin512LookupTable[0x200];
int acos512LookupTable[0x200];

int sin256LookupTable[0x100];
int cos256LookupTable[0x100];
int tan256LookupTable[0x100];
int asin256LookupTable[0x100];
int acos256LookupTable[0x100];

byte arcTan256LookupTable[0x100 * 0x100];

uint randSeed = 0;

void ClearTrigLookupTables()
{
    memset(sin256LookupTable, 0, sizeof(sin256LookupTable));
    memset(cos256LookupTable, 0, sizeof(cos256LookupTable));
    memset(tan256LookupTable, 0, sizeof(tan256LookupTable));
    memset(asin256LookupTable, 0, sizeof(asin256LookupTable));
    memset(acos256LookupTable, 0, sizeof(acos256LookupTable));
    memset(sin512LookupTable, 0, sizeof(sin512LookupTable));
    memset(cos512LookupTable, 0, sizeof(cos512LookupTable));
    memset(tan512LookupTable, 0, sizeof(tan512LookupTable));
    memset(asin512LookupTable, 0, sizeof(asin512LookupTable));
    memset(acos512LookupTable, 0, sizeof(acos512LookupTable));
    memset(sin1024LookupTable, 0, sizeof(sin1024LookupTable));
    memset(cos1024LookupTable, 0, sizeof(cos1024LookupTable));
    memset(tan1024LookupTable, 0, sizeof(tan1024LookupTable));
    memset(asin1024LookupTable, 0, sizeof(asin1024LookupTable));
    memset(acos1024LookupTable, 0, sizeof(acos1024LookupTable));
    memset(arcTan256LookupTable, 0, sizeof(arcTan256LookupTable));
    randSeed = 0;
}

void CalculateTrigAngles()
{
    srand((uint)time(NULL));
    randSeed = rand();

    for (int i = 0; i < 0x400; ++i) {
        sin1024LookupTable[i]  = (int)(sinf((i / 512.f) * RSDK_PI) * 1024.f);
        cos1024LookupTable[i]  = (int)(cosf((i / 512.f) * RSDK_PI) * 1024.f);
        tan1024LookupTable[i]  = (int)(tanf((i / 512.f) * RSDK_PI) * 1024.f);
        asin1024LookupTable[i] = (int)((asinf(i / 1023.f) * 512.f) / RSDK_PI);
        acos1024LookupTable[i] = (int)((acosf(i / 1023.f) * 512.f) / RSDK_PI);
    }

    cos1024LookupTable[0x000] = 0x400;
    cos1024LookupTable[0x100] = 0;
    cos1024LookupTable[0x200] = -0x400;
    cos1024LookupTable[0x300] = 0;

    sin1024LookupTable[0x000] = 0;
    sin1024LookupTable[0x100] = 0x400;
    sin1024LookupTable[0x200] = 0;
    sin1024LookupTable[0x300] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        sin512LookupTable[i]  = (int)(sinf((i / 256.f) * RSDK_PI) * 512.f);
        cos512LookupTable[i]  = (int)(cosf((i / 256.f) * RSDK_PI) * 512.f);
        tan512LookupTable[i]  = (int)(tanf((i / 256.f) * RSDK_PI) * 512.f);
        asin512LookupTable[i] = (int)((asinf(i / 511.f) * 256.f) / RSDK_PI);
        acos512LookupTable[i] = (int)((acosf(i / 511.f) * 256.f) / RSDK_PI);
    }

    cos512LookupTable[0x00]  = 0x200;
    cos512LookupTable[0x80]  = 0;
    cos512LookupTable[0x100] = -0x200;
    cos512LookupTable[0x180] = 0;

    sin512LookupTable[0x00]  = 0;
    sin512LookupTable[0x80]  = 0x200;
    sin512LookupTable[0x100] = 0;
    sin512LookupTable[0x180] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sin256LookupTable[i]  = (int)((sin512LookupTable[i * 2] >> 1));
        cos256LookupTable[i]  = (int)((cos512LookupTable[i * 2] >> 1));
        tan256LookupTable[i]  = (int)((tan512LookupTable[i * 2] >> 1));
        asin256LookupTable[i] = (int)((asinf(i / 255.f) * 128.f) / RSDK_PI);
        acos256LookupTable[i] = (int)((acosf(i / 255.f) * 128.f) / RSDK_PI);
    }

    for (int y = 0; y < 0x100; ++y) {
        byte *arcTan = (byte *)&arcTan256LookupTable[y];

        for (int x = 0; x < 0x100; ++x) {
            // 40.743664 = 0x100 / (2 * M_PI) (roughly)
            *arcTan = (int)(float)((float)atan2((float)y, x) * 40.743664f);
            arcTan += 0x100;
        }
    }
}

byte ArcTanLookup(int X, int Y)
{
    int x = abs(X);
    int y = abs(Y);

    if (x <= y) {
        while (y > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    else {
        while (x > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            return arcTan256LookupTable[(x << 8) + y] + 0x80;
        else
            return 0x80 - arcTan256LookupTable[(x << 8) + y];
    }
    else if (Y <= 0)
        return -arcTan256LookupTable[(x << 8) + y];
    else
        return arcTan256LookupTable[(x << 8) + y];
}
