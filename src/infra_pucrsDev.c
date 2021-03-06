/**************************************************************************************************/
/**
 * \brief  Infrastructure for PUCRS DevKit
 */
/**************************************************************************************************/

#include "infra.h"
#include "timer.h"
#include "io.h"
#include "temperature.h"

/**************************************************************************************************/

void hwInit()
{
    timeInit();
    ioInit();

    /* Now it's possible to print, so do it! */
    print("Initializing...\r");

    tempInit();
}

/**************************************************************************************************/

