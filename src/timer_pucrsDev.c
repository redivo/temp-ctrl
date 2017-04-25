/**************************************************************************************************/
/**
 * \brief  Timing functions
 */
/**************************************************************************************************/

#include <arch/nxp/lpc23xx.h>
#include "timer.h"

#define CCLK 12000000

/**************************************************************************************************/

void timeInit()
{
    // Turn on power of Timer0
    PCONP |= 2;

    PCLKSEL0 = (PCLKSEL0 & (~0x0c)) | 0x04;	/* Divide CCLK por 1 no T0 */
    T0TCR = 0;              /* Desabilita T0 */
    T0PR = CCLK/1000 - 1; /* Incrementa T0TC 1000 vezes por segundo */
    T0TCR = 2;      /* Inicializa T0 */
    T0TCR = 1;      /* Habilita T0  */
}

/**************************************************************************************************/

void ms_sleep(uint32_t t)
{
    uint32_t tf;
    tf = T0TC + t;          /* tf = Valor futuro do T0TC */
    while(tf != T0TC);      /* ms_sleep ate que tf==T0TC */
}

/**************************************************************************************************/