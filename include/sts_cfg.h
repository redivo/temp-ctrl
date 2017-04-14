/**************************************************************************************************/
/**
 * \brief  Status and configuration functions and defines
 */
/**************************************************************************************************/

#ifndef __STS_CFG_H__
#define __STS_CFG_H__

/**************************************************************************************************/

typedef struct {
    float realTemp;
    int coolerHysteresisTime;
} sts_t;

/**************************************************************************************************/

typedef struct {
    float targetTemp;
    float marginTemp;
} cfg_t;

/**************************************************************************************************/

/**
 * \brief  Configuration and Status initialization
 * \param  cfg  Pointer to configuration structure to be initialized
 * \param  sts  Pointer to status structure to be initialized
 */
void cfgStsInit(cfg_t *cfg, sts_t *sts);

/**************************************************************************************************/

#endif /* __STS_CFG_H__ */
