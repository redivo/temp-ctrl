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
#define HYSTERESIS_TIME (1000 * 60 * 3) // 3 minutes

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

    // If temperature is above the limit
    if (Status.realTemp > (Config.targetTemp + Config.marginTemp)) {
        Status.coolerOffHysteresisTime = 0;
        if (!Status.coolerIsOn) {
            // Get initial hysteresis time
            if (Status.coolerOnHysteresisTime == 0) {
                Status.coolerOnHysteresisTime = getMsTimeStamp();
                hysteresisTimeWarning(true);
            }

            // If hysteresis time is reached, turn on the cooler
            if ((getMsTimeStamp() - Status.coolerOnHysteresisTime) > HYSTERESIS_TIME
                    || (getMsTimeStamp() - Status.coolerOnHysteresisTime) < 0) {
                // Turn on
                Status.coolerIsOn = true;
                Status.coolerOnHysteresisTime = 0;
                hysteresisTimeWarning(false);
                setCoolerState(COOLER_ON);
            }
        }
    }

    // If temperature reaches the bottom boundary
    else if (Status.realTemp <= (Config.targetTemp - Config.marginTemp)) {
        Status.coolerOnHysteresisTime = 0;
        if (Status.coolerIsOn) {
            // Get initial hysteresis time
            if (Status.coolerOffHysteresisTime == 0) {
                Status.coolerOffHysteresisTime = getMsTimeStamp();
                hysteresisTimeWarning(true);
            }

            // If hysteresis time is reached, turn on the cooler
            if ((getMsTimeStamp() - Status.coolerOffHysteresisTime) > HYSTERESIS_TIME
                    || (getMsTimeStamp() - Status.coolerOffHysteresisTime) < 0) {
                // Turn off
                Status.coolerIsOn = false;
                Status.coolerOffHysteresisTime = 0;
                hysteresisTimeWarning(false);
                setCoolerState(COOLER_OFF);
            }
        }
    }

    /// If temperature is inside the boundary
    else {
        Status.coolerOnHysteresisTime = 0;
        Status.coolerOffHysteresisTime = 0;
        hysteresisTimeWarning(false);
    }
}

/**************************************************************************************************/

int main (void)
{
    hwInit();
    cfgStsInit(&Config, &Status);
    clearScreen();

    // Act on the hardware according to the initial status
    if (Status.coolerIsOn) {
        setCoolerState(COOLER_ON);
    }
    else {
        setCoolerState(COOLER_OFF);
    }

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
