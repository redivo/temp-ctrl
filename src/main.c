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

typedef enum {
    IDLE,
    INIT_HOT_HYSTERESIS,
    WAIT_HOT_HYSTERESIS,
    COOLING,
} act_state_t;

/**
 * \brief  Act based on status and config
 */
void act()
{
    static state = IDLE;
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

    // Actions
    switch(state) {
        case IDLE:
            // Turn all off
            Status.coolerOffHysteresisTime = 0;
            Status.coolerOnHysteresisTime = 0;
            setCoolerState(COOLER_OFF);
            hysteresisTimeWarning(false);
            break;

        case INIT_HOT_HYSTERESIS:
            Status.coolerOnHysteresisTime = getMsTimeStamp();
            hysteresisTimeWarning(true);
            break;

        case WAIT_HOT_HYSTERESIS:
            break;

        case COOLING:
            hysteresisTimeWarning(false);
            setCoolerState(COOLER_ON);
            break;
    }

    // Transitions
    switch(state) {
        case IDLE:
            // If temperature is above the limit
            if (Status.realTemp > (Config.targetTemp + Config.marginTemp)) {
                state = INIT_HOT_HYSTERESIS;
            }
            break;

        case INIT_HOT_HYSTERESIS:
            state = WAIT_HOT_HYSTERESIS;
            break;

        case WAIT_HOT_HYSTERESIS:
            // If the temperature is below the limit, go back to IDLE
            if (Status.realTemp <= (Config.targetTemp + Config.marginTemp)) {
                state = IDLE;
            }

            // If it stills above the limit and the hysteresis is done, go to cooling
            else if ((getMsTimeStamp() - Status.coolerOnHysteresisTime) > HYSTERESIS_TIME
                    || (getMsTimeStamp() - Status.coolerOnHysteresisTime) < 0) {
                state = COOLING;
            }
            break;

        case COOLING:
            // If cooling reach the target value go back to IDLE
            if (Status.realTemp <= (Config.targetTemp - Config.marginTemp)) {
                state = IDLE;
            }
            break;
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
        ms_sleep(200);
    }

    return 0;
}

/**************************************************************************************************/

/* These routines are called by crt.S 
They must exist, but probably they're not being used */
void UNDEF_Routine(){}
void SWI_Routine(){}
void FIQ_Routine(){}
