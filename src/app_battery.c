#include "app_router.h"

static uint32_t batteryAlarmState = 0;

// 2800..3300 mv - 0..100%
static uint8_t get_battery_level(uint16_t battery_mv) {
    /* Zigbee 0% - 0x0, 50% - 0x64, 100% - 0xc8 */
    if (battery_mv <= MIN_VBAT_MV) {
        return 0;
    }
    if (battery_mv >= MAX_VBAT_MV) {
        return 0xC8;
    }
    uint32_t level = (uint32_t)(battery_mv - MIN_VBAT_MV) * 0xC8 / (MAX_VBAT_MV - MIN_VBAT_MV);

    return (uint8_t)level;
}

int32_t app_batteryCb(void *arg) {

    zcl_powerAttr_t *powerAttr = zcl_powerAttrsGet();

    uint16_t voltage_raw = drv_get_adc_data();
    uint8_t voltage = (uint8_t)(voltage_raw/100);
    uint8_t level = get_battery_level(voltage_raw);

    batteryAlarmState = 0;
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_MIN_THRESHOLD) && voltage <= powerAttr->batteryVoltageMinThreshold) {
        batteryAlarmState |= 0x01;  // Bit 0: Alarm1
    }
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_THRESHOLD_1) && voltage <= powerAttr->batteryVoltageThreshold1) {
        batteryAlarmState |= 0x02;  // Bit 1: Alarm2
    }
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_THRESHOLD_2) && voltage <= powerAttr->batteryVoltageThreshold2) {
        batteryAlarmState |= 0x04;  // Bit 2: Alarm3
    }

     APP_DEBUG(DEBUG_BATTERY_EN, "Voltage_raw: %d\r\n", voltage_raw);
     APP_DEBUG(DEBUG_BATTERY_EN, "Voltage:     %d\r\n", voltage);
     APP_DEBUG(DEBUG_BATTERY_EN, "Level:       %d\r\n", level);
     APP_DEBUG(DEBUG_BATTERY_EN, "AlarmState:  0x%04x\r\n", batteryAlarmState);

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &voltage);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &level);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_ALARM_STATE, (uint8_t*)&batteryAlarmState);

    return 0;
}
