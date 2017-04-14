/**************************************************************************************************/
/**
 * \brief  General definitions
 */
/**************************************************************************************************/

#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <stdint.h>

/**************************************************************************************************/

typedef enum {
    ERR_NONE = 0,
    ERR_UNKNOWN = -1,
    ERR_MAX_SIZE_EXCEEDED = -2,
} error_t;

/**************************************************************************************************/

typedef enum {
    USER_INPUT_NONE = 0,
    USER_INPUT_TARGET_TEMP_STEP_UP,
    USER_INPUT_TARGET_TEMP_STEP_DOWN,
} userInput_t;

/**************************************************************************************************/

/**
 * \brief  Translate a digit to an ASCII char
 * \param  digit  Digit to be translated
 * \return translated ASCII char
 */
char digitToChar(uint8_t digit);

/**************************************************************************************************/

/**
 * \brief  Translate a float value to ASCII
 * \param  c     Pointer to me stored the ASCII value
 * \param  size  Allocated size of c
 * \param  f     Float value to be translated
 */
void ftoa(char *c, uint8_t size, float f);

/**************************************************************************************************/

/**
 * \brief  Translate a 2-complement value in a float
 * \param  intPart    Integer part of 2-complement value
 * \param  floatPart  Floating point part of 2-complement value
 * \param  mantissaSize  Size of the mantissa
 * \return Translated value
 */
float two_complement_to_float(uint8_t intPart, uint8_t floatPart, uint8_t mantissaSize);

/**************************************************************************************************/

#endif /* __GENERAL_H__ */
