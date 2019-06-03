/**************************************************************************************************/
/**
 * \brief  Serial interface for PUCRS Devkit
 */
/**************************************************************************************************/

#include "devkit_pucrs/serial.h"
#include "devkit_pucrs/lpc23xx.h"

/**************************************************************************************************/

void serialInit()
{
    PINSEL0 |= 0x50;  /* Select TxD0 and RxD0 pins */
    U0FCR = 0x7;      /* Enable FIFOs and reset */
    U0LCR = 0x83;     /* Enable access to baud-rate divisor (DLAB) */
    //U0DLL = ((CRYSTALFREQ/BAUDRATE + 8) >> 4) & 0xff;
    //U0DLM = ((CRYSTALFREQ/BAUDRATE) >>12) & 0xff;
    U0LCR = 3;        /* Disable DLAB */
}
