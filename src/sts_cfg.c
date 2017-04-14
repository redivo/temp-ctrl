/**************************************************************************************************/
/**
 * \brief  Status and configuration functions and defines
 */
/**************************************************************************************************/

#include "sts_cfg.h"

/**************************************************************************************************/

void cfgStsInit(cfg_t *cfg, sts_t *sts)
{
    cfg->targetTemp = 25;
    cfg->marginTemp = 2;
    sts->realTemp = 25;
    sts->coolerHysteresisTime = 0;
}

/**************************************************************************************************/
