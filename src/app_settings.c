#include "app_router.h"

static app_settings_t app_settings;
app_settings_t *settings = &app_settings;

static uint8_t checksum(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len - 1; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

nv_sts_t router_settings_save(void) {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    APP_DEBUG(DEBUG_SAVE_EN, "Saved settings\r\n");

    settings->crc = checksum((uint8_t*)settings, sizeof(app_settings_t));
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(app_settings_t), (uint8_t*)settings);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t router_settings_restore(void) {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    app_settings_t temp_app_settings;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(app_settings_t), (uint8_t*)&temp_app_settings);

    if (st == NV_SUCC && temp_app_settings.crc == checksum((uint8_t*)&temp_app_settings, sizeof(app_settings_t))) {

        APP_DEBUG(DEBUG_SAVE_EN, "Restored settings\r\n");

        memcpy(settings, &temp_app_settings, (sizeof(app_settings_t)));
    } else {
        /* default settings */
        APP_DEBUG(DEBUG_SAVE_EN, "Default settings\r\n");
        settings->tx_power = TX_POWER_5_DBM;
        settings->batteryAlarmMask = ALARM_MASK_MIN_THRESHOLD | ALARM_MASK_THRESHOLD_1 | ALARM_MASK_THRESHOLD_2;
        settings->batteryVoltageMinThreshold = BATTERY_VOLTAGE_MIN_THRESHOLD;
        settings->batteryVoltageThreshold1 = BATTERY_VOLTAGE_THRESHOLD1;
        settings->batteryVoltageThreshold2 = BATTERY_VOLTAGE_THRESHOLD2;
    }

    g_zcl_powerAttrs.batteryAlarmMask = settings->batteryAlarmMask;
    g_zcl_powerAttrs.batteryVoltageMinThreshold = settings->batteryVoltageMinThreshold;
    g_zcl_powerAttrs.batteryVoltageThreshold1 = settings->batteryVoltageThreshold1;
    g_zcl_powerAttrs.batteryVoltageThreshold2 = settings->batteryVoltageThreshold2;
    g_zcl_rfPowerAttrs.tx_powerLevel = settings->tx_power;

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

void router_default_settings() {
    /* default settings */
    APP_DEBUG(DEBUG_SAVE_EN, "Default settings\r\n");
    settings->tx_power = TX_POWER_5_DBM;
    settings->batteryAlarmMask = ALARM_MASK_MIN_THRESHOLD | ALARM_MASK_THRESHOLD_1 | ALARM_MASK_THRESHOLD_2;
    settings->batteryVoltageMinThreshold = BATTERY_VOLTAGE_MIN_THRESHOLD;
    settings->batteryVoltageThreshold1 = BATTERY_VOLTAGE_THRESHOLD1;
    settings->batteryVoltageThreshold2 = BATTERY_VOLTAGE_THRESHOLD2;

    g_zcl_powerAttrs.batteryAlarmMask = settings->batteryAlarmMask;
    g_zcl_powerAttrs.batteryVoltageMinThreshold = settings->batteryVoltageMinThreshold;
    g_zcl_powerAttrs.batteryVoltageThreshold1 = settings->batteryVoltageThreshold1;
    g_zcl_powerAttrs.batteryVoltageThreshold2 = settings->batteryVoltageThreshold2;
    g_zcl_rfPowerAttrs.tx_powerLevel = settings->tx_power;

    rf_setTxPower(rx_power_config[settings->tx_power]);
}

