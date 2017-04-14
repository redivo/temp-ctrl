/**************************************************************************************************/
/**
 * \brief  General definitions
 */
/**************************************************************************************************/

#include <stdbool.h>
#include "general.h"

/**************************************************************************************************/

char digitToChar(uint8_t digit)
{
    switch (digit) {
        case 9:
            return '9';

        case 8:
            return '8';

        case 7:
            return '7';

        case 6:
            return '6';

        case 5:
            return '5';

        case 4:
            return '4';

        case 3:
            return '3';

        case 2:
            return '2';

        case 1:
            return '1';

        default:
            return '0';
    }

}

/**************************************************************************************************/

void ftoa(char *c, uint8_t size, float f)
{
    int intPart = f;
    int intOneDigit = f;
    float floatPart = f - intPart;
    int oneDigitPrev = 0;
    int divider = 1;
    int index = 0;

    /* Found max 10 divider */
    while (intOneDigit >= 10) {
        intOneDigit /= 10;
        divider *=10;
    }

    /* Spare digits converting them to char */
    while (divider > 0) {
        intOneDigit = intPart;
        intOneDigit /= divider;
        oneDigitPrev = intOneDigit;
        c[index] = digitToChar(intOneDigit);
        index++;
        intPart -= oneDigitPrev * divider;
        divider /= 10;
    }

    /* Place '.' divider */
    c[index] = '.';
    index++;
    c[index] = '0';

    /* Parse decimals */
    while (floatPart != 0) {
        int floatOneDigit = floatPart * 10;
        c[index] = digitToChar(floatOneDigit);
        index++;
        floatPart = (floatPart * 10) - floatOneDigit;
    }
}

/**************************************************************************************************/

float two_complement_to_float(uint8_t intPart, uint8_t floatPart, uint8_t mantissaSize)
{
    float f = 0;
    bool negative = false;

    /* Handle if it's a negative value.
     * If the MSb of intPart is '1' it indicates that the value is negative
     */
    if ((intPart >> 7) == 1) {
        uint8_t offset = 8 - mantissaSize;
        /* Join both int and float parts */
        uint16_t allBits = (intPart << 8) + floatPart;

        /* Ajust according to mantissa size */
        allBits = allBits >> offset;

        /* Normalize 2 complement */
        allBits = ~allBits;
        allBits = allBits << offset;
        allBits = allBits >> offset;
        allBits++;
        allBits = allBits << offset;

        /* Set to both float and int parts */
        intPart = allBits >> 8;
        floatPart = ((allBits << offset) & 0xFFFF) >> offset;
        negative = true;
    }

    int intMultiplier;
    for (intMultiplier = 1; intPart != 0; intPart = intPart >> 1, intMultiplier *= 2) {
        f += (intPart & 1) * intMultiplier;
    }

    float floatMultiplier;
    for (floatMultiplier = 0.5;
         floatPart != 0 && mantissaSize >= 0;
         floatPart = floatPart << 1, floatMultiplier /= 2, mantissaSize--) {
        f += (!!(floatPart & 0x80)) * floatMultiplier;
    }

    if (negative) {
        return (f * -1);
    }
    else {
        return f;
    }
}

/**************************************************************************************************/
