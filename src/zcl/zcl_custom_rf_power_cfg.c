#include "app_router.h"

uint8_t rx_power_config[TX_POWER_MAX_NUM] = {
    RF_POWER_INDEX_P0p04dBm,
    RF_POWER_INDEX_P5p13dBm,
    RF_POWER_INDEX_P0p04dBm,
};

_CODE_ZCL_ status_t zcl_rfPowerCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_CUSTOM_RF_POWER_CFG, manuCode, attrNum, attrTbl, NULL, cb);
}
