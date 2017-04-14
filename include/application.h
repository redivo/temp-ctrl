/**************************************************************************************************/
/**
 *  \brief  Application layer
 */
/**************************************************************************************************/

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "defines.h"

/**************************************************************************************************/

ret_temp_error_t getControledTemperature(temperature_t *controledTemp);

ret_temp_error_t displayInfo(config_t cfg, temperature_t controledTemp);

ret_temp_error_t getInput(config_t *cfg);

ret_temp_error_t setConfig(config_t cfg);

#endif /* __APPLICATION_H__ */
