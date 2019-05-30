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
    INIT_COLD_HYSTERESIS,
    WAIT_COLD_HYSTERESIS,
    HEATING,
} act_state_t;

/**
 * \brief  Act based on status and config
 */
void act()
{
    static act_state_t state = IDLE;
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
            Status.hysteresisTime = 0;
            setCoolerState(COOLER_OFF);
            setHeaterState(HEATER_OFF);
            hysteresisTimeHotWarning(false);
            hysteresisTimeColdWarning(false);
            break;

        case INIT_HOT_HYSTERESIS:
            Status.hysteresisTime = getMsTimeStamp();
            hysteresisTimeHotWarning(true);
            break;

        case WAIT_HOT_HYSTERESIS:
            break;

        case COOLING:
            hysteresisTimeHotWarning(false);
            setCoolerState(COOLER_ON);
            break;

        case INIT_COLD_HYSTERESIS:
            Status.hysteresisTime = getMsTimeStamp();
            hysteresisTimeColdWarning(true);
            break;

        case WAIT_COLD_HYSTERESIS:
            break;

        case HEATING:
            hysteresisTimeColdWarning(false);
            setHeaterState(HEATER_ON);
            break;
    }

    // Transitions
    switch(state) {
        case IDLE:
            // If temperature is above the limits
            if (Status.realTemp > (Config.targetTemp + Config.marginTemp)) {
                state = INIT_HOT_HYSTERESIS;
            }

            // If temperature is below the limits
            else if (Status.realTemp < (Config.targetTemp - Config.marginTemp)) {
                state = INIT_COLD_HYSTERESIS;
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
            else if ((getMsTimeStamp() - Status.hysteresisTime) > HYSTERESIS_TIME
                    || (getMsTimeStamp() - Status.hysteresisTime) < 0) {
                state = COOLING;
            }
            break;

        case COOLING:
            // If cooling reaches the target value go back to IDLE
            if (Status.realTemp <= (Config.targetTemp - Config.marginTemp)) {
                state = IDLE;
            }
            break;

        case INIT_COLD_HYSTERESIS:
            state = WAIT_COLD_HYSTERESIS;
            break;

        case WAIT_COLD_HYSTERESIS:
            // If the temperature is within the limit, go back to IDLE
            if (Status.realTemp >= (Config.targetTemp - Config.marginTemp)) {
                state = IDLE;
            }

            // If it stills below the limit and the hysteresis is done, go to heating
            else if ((getMsTimeStamp() - Status.hysteresisTime) > HYSTERESIS_TIME
                    || (getMsTimeStamp() - Status.hysteresisTime) < 0) {
                state = HEATING;
            }
            break;

        case HEATING:
            // If warming reaches the target value go back to IDLE
            if (Status.realTemp >= (Config.targetTemp + Config.marginTemp)) {
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
