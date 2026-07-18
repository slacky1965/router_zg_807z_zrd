#ifndef SRC_ZCL_ZCL_CUSTOM_RF_POWER_CFG_H_
#define SRC_ZCL_ZCL_CUSTOM_RF_POWER_CFG_H_

#define ZCL_CLUSTER_CUSTOM_RF_POWER_CFG     0xFF65

#define ZCL_ATTRID_CUSTOM_TX_POWER          0x0000

enum {
    TX_POWER_0_DBM = 0,
    TX_POWER_5_DBM,
    TX_POWER_10_DBM,
    TX_POWER_MAX_NUM,
};

extern uint8_t rx_power_config[TX_POWER_MAX_NUM];

status_t zcl_rfPowerCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif /* SRC_ZCL_ZCL_CUSTOM_RF_POWER_CFG_H_ */
