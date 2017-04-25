/**************************************************************************************************/
/**
 *  \brief  Main function file
 */
/**************************************************************************************************/

#include <stdint.h>
#include <string.h>
#include "infra.h"
#include "timer.h"
#include "io.h"
#include "temperature.h"
#include "sts_cfg.h"

/**************************************************************************************************/

#define TARGET_TEMPERATURE_STEP 0.5

sts_t Status;
cfg_t Config;

/**************************************************************************************************/

/**
 * \brief  Read user input and update config
 */
static void readUserInput()
{
    switch (userInput()) {
        case USER_INPUT_TARGET_TEMP_STEP_UP:
            Config.targetTemp += TARGET_TEMPERATURE_STEP;
            break;

        case USER_INPUT_TARGET_TEMP_STEP_DOWN:
            Config.targetTemp -= TARGET_TEMPERATURE_STEP;
            break;

        case USER_INPUT_NONE:
            // Do nothing
            break;
    }
}

/**************************************************************************************************/

/**
 * \brief  Read and update board status
 */
static void updateStatus()
{
    Status.realTemp = readTemperature();
}

/**************************************************************************************************/

/**
 * \brief  Act based on status and config
 */
void act()
{
    char tempStr[7];

    // First of all show real temperature
    print("\f");
    print("Real Tmp: ");
    memset(tempStr, 0, 6);
    ftoa(tempStr, 6, Status.realTemp);
    print(tempStr);

    /* Now print target temperature */
    memset(tempStr, 0, 6);
    ftoa(tempStr, 6, Config.targetTemp);
    print("\r\n");
    print("Targ Tmp: ");
    print(tempStr);

    /* Verify temperatures and take an action if necessary */
    if (Status.realTemp > (Config.targetTemp + Config.marginTemp)) {
        // Turn on
        setCoolerState(COOLER_ON);
    }
    else if (Status.realTemp <= (Config.targetTemp - Config.marginTemp)) {
        // Turn off
        setCoolerState(COOLER_OFF);
    }
}

/**************************************************************************************************/

int main (void)
{
    hwInit();
    cfgStsInit(&Config, &Status);
    clearScreen();

    while (1) {
        readUserInput();
        updateStatus();
        act();

        ms_sleep(500);
    }

    return 0;
}

/**************************************************************************************************/

/* These routines are called by crt.S 
They must exist, but probably they're not being used */
void UNDEF_Routine(){}
void SWI_Routine(){}
void FIQ_Routine(){}
