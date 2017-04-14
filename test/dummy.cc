/**************************************************************************************************/
/**
 * \file
 * \brief  Dummy test file
 */
/**************************************************************************************************/

#include <gtest/gtest.h>

extern "C" {
#include "defines.h"
#include "application.h"
}
/**************************************************************************************************/

TEST(DummyTest, Dummy)
{
	config_t cfg;
	temperature_t controledTemp;

	getControledTemperature(&controledTemp);
	displayInfo(cfg, controledTemp);
	getInput(&cfg);
	setConfig(cfg);
}
