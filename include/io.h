/**************************************************************************************************/
/**
 * \brief  In and Output interfaces
 */
/**************************************************************************************************/

#ifndef __IO_H__
#define __IO_H__

#include <stdbool.h>
#include "general.h"

/**************************************************************************************************/

typedef enum {
    COOLER_ON,
    COOLER_OFF,
} coolerState_t;

/**************************************************************************************************/

typedef enum {
    HEATER_ON,
    HEATER_OFF,
} heaterState_t;

/**************************************************************************************************/

/**
 * \brief Initialize IOs
 */
void ioInit();

/**************************************************************************************************/

/**
 * \brief  Print something on standard out
 * \return ERR_NONE in case of success, error code otherwise
 */
error_t print(char *txt);

/**************************************************************************************************/

/**
 * \brief  Clear screen
 */
void clearScreen();

/**************************************************************************************************/

/**
 * \brief  Read user input from hardware
 * \return User input code
 */
userInput_t userInput();

/**************************************************************************************************/
 /**
 * \brief  Control cooler state
 * \param  state  Cooler state to be set
 */
void setCoolerState(coolerState_t state);

/**************************************************************************************************/

 /**
 * \brief  Control heater state
 * \param  state  Heater state to be set
 */
void setHeaterState(heaterState_t state);

/**************************************************************************************************/

/**
 * \brief  Show or hide hysteresis time hot warning
 * \param  display  True to display hysteresis hot warning and false to hide
 */
void hysteresisTimeHotWarning(bool display);

/**************************************************************************************************/

/**
 * \brief  Show or hide hysteresis time cold warning
 * \param  display  True to display hysteresis cold warning and false to hide
 */
void hysteresisTimeColdWarning(bool display);

/**************************************************************************************************/

/**
 * \brief  Set cooler state warning
 * \param  isOn  Display 'cooler is on' information
 */
void coolerStateWarning(bool isOn);

/**************************************************************************************************/

/**
 * \brief  Set heater state warning
 * \param  isOn  Display 'heater is on' information
 */
void heaterStateWarning(bool isOn);

/**************************************************************************************************/

/**
 * \brief  Debug a message. If the debug is disabled it will do nothing
 * \param  msg  Message to debug
 */
void debug(char *msg);

/**************************************************************************************************/

#endif /* __IO_H__ */
