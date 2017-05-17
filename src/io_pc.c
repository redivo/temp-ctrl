/**************************************************************************************************/
/**
 * \brief  In and Output interfaces
 */
/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

/**************************************************************************************************/

void ioInit()
{
    // Do nothing
}

/**************************************************************************************************/

error_t print(char *txt)
{
    printf("%s", txt);
    return ERR_NONE;
}

/**************************************************************************************************/

void clearScreen()
{
    system("clear");
}

/**************************************************************************************************/

userInput_t userInput()
{
    int option;

    clearScreen();

    printf("What do you wanna do?\r\n");
    printf("1 - Increase target temperature\r\n");
    printf("2 - Decrease target temperature\r\n");
    printf("\r\n");
    printf("Your choice: ");

    scanf("%d", &option);
}

/**************************************************************************************************/

void setCoolerState(coolerState_t state)
{
    /* TODO
     * This function is not implemented yet, so do nothing
     */
}

/**************************************************************************************************/

void hysteresisTimeWarning(bool display)
{
    /* TODO
     * This function is not implemented yet, so do nothing
     */
}

/**************************************************************************************************/

void coolerStateWarning(bool isOn)
{
    /* TODO
     * This function is not implemented yet, so do nothing
     */
}

/**************************************************************************************************/
