/**************************************************************************************************/
/**
 * \brief  Temperature-related functions
 */
/**************************************************************************************************/

#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

/**************************************************************************************************/

/**
 * \brief  Initialize temperature sensor
 */
void tempInit();

/**************************************************************************************************/

/**
 * \brief  Read temperature from sensor
 * \return Read temperature
 */
float readTemperature();

/**************************************************************************************************/

#endif /* __TEMPERATURE_H__ */
