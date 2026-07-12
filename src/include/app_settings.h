#ifndef SRC_INCLUDE_APP_SETTINGS_H_
#define SRC_INCLUDE_APP_SETTINGS_H_

typedef struct {
    uint8_t  tx_power;
    uint8_t  batteryAlarmMask;
    uint8_t  batteryVoltageMinThreshold;
    uint8_t  batteryVoltageThreshold1;
    uint8_t  batteryVoltageThreshold2;
    uint8_t  crc;
} app_settings_t;

extern app_settings_t *settings;

nv_sts_t router_settings_save(void);
nv_sts_t router_settings_restore(void);
void router_default_settings();

#endif /* SRC_INCLUDE_APP_SETTINGS_H_ */
