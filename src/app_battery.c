#include "app_router.h"

static uint32_t batteryAlarmState = 0;
static uint32_t prevAlarmState = 0;
static bool first_run = true;
static uint16_t vbat_buf[3];

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

    if (first_run) {
        vbat_buf[0] = vbat_buf[1] = vbat_buf[2] = voltage_raw;
    } else {
        vbat_buf[2] = vbat_buf[1];
        vbat_buf[1] = vbat_buf[0];
        vbat_buf[0] = voltage_raw;
    }

    /* median of 3 ends up in b */
    uint16_t a = vbat_buf[0], b = vbat_buf[1], c = vbat_buf[2];
    if (a > b) { uint16_t t = a; a = b; b = t; }
    if (b > c) { uint16_t t = b; b = c; c = t; }
    if (a > b) { uint16_t t = a; a = b; b = t; }

    uint8_t voltage = (uint8_t)(b/100);
    uint8_t level = get_battery_level(b);

    batteryAlarmState = 0;
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_MIN_THRESHOLD) && voltage <= powerAttr->batteryVoltageMinThreshold) {
        batteryAlarmState |= BIT(0);
    }
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_THRESHOLD_1) && voltage <= powerAttr->batteryVoltageThreshold1) {
        batteryAlarmState |= BIT(1);
    }
    if ((powerAttr->batteryAlarmMask & ALARM_MASK_THRESHOLD_2) && voltage <= powerAttr->batteryVoltageThreshold2) {
        batteryAlarmState |= BIT(2);
    }

    uint8_t cmd = 0xFF;
    if (batteryAlarmState != 0) {
        switch (g_zcl_onOffSwitchCfgAttrs.switchActions) {
            case ZCL_SWITCH_ACTION_ON_OFF:  cmd = ZCL_CMD_ONOFF_ON;    break;
            case ZCL_SWITCH_ACTION_OFF_ON:  cmd = ZCL_CMD_ONOFF_OFF;   break;
            case ZCL_SWITCH_ACTION_TOGGLE:
                if (prevAlarmState == 0) cmd = ZCL_CMD_ONOFF_TOGGLE;
                break;
        }
    } else {
        switch (g_zcl_onOffSwitchCfgAttrs.switchActions) {
            case ZCL_SWITCH_ACTION_ON_OFF:  cmd = ZCL_CMD_ONOFF_OFF;   break;
            case ZCL_SWITCH_ACTION_OFF_ON:  cmd = ZCL_CMD_ONOFF_ON;    break;
            case ZCL_SWITCH_ACTION_TOGGLE:
                if (prevAlarmState != 0) cmd = ZCL_CMD_ONOFF_TOGGLE;
                break;
        }
    }

    if (first_run) {
        first_run = false;
    } else if (cmd != 0xFF && cmd != app_onoff_status.onoff_status) {
        cmdOnOff(cmd);
        app_onoff_status.onoff_status = (cmd == ZCL_CMD_ONOFF_TOGGLE)
                                        ? app_onoff_status.onoff_status ^ 1 : cmd;
        onoffStatus_save();
    }

    prevAlarmState = batteryAlarmState;

     APP_DEBUG(DEBUG_BATTERY_EN, "Voltage_raw: %d\r\n", voltage_raw);
     APP_DEBUG(DEBUG_BATTERY_EN, "Voltage:     %d\r\n", voltage);
     APP_DEBUG(DEBUG_BATTERY_EN, "Level:       %d\r\n", level);
     APP_DEBUG(DEBUG_BATTERY_EN, "AlarmState:  0x%04x\r\n", batteryAlarmState);

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &voltage);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &level);
    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_ALARM_STATE, (uint8_t*)&batteryAlarmState);

    return 0;
}
