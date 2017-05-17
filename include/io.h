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
 * \brief  Show or hide hysteresis time acting
 * \param  display  True to display hysteresis warning and false to hide
 */
void hysteresisTimeWarning(bool display);

/**************************************************************************************************/

/**
 * \brief  Set cooler state warning
 * \param  isOn  Display 'cooler is on' information
 */
void coolerStateWarning(bool isOn);

/**************************************************************************************************/

#endif /* __IO_H__ */
