/**************************************************************************************************/
/**
 *  \brief  General definitions file
 */
/**************************************************************************************************/

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <stdbool.h>

/**************************************************************************************************/

typedef int temperature_t;

typedef struct {
	bool needApply;
	temperature_t minTemp;
	temperature_t maxTemp;
} config_t;

typedef enum {
	RED_TMP_OK = 0,
	RED_TMP_ERR_GEN = -1,
	RED_TMP_ERR_UNINPLEMENTED = -2,
} ret_temp_error_t;

#endif /* __DEFINES_H__ */
