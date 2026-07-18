#ifndef SRC_INCLUDE_APP_ROUTER_H_
#define SRC_INCLUDE_APP_ROUTER_H_

#include "tl_common.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "gp.h"

#include "zcl_custom_rf_power_cfg.h"
#include "zcl_onoffSwitchCfg.h"
#include "app_utility.h"
#include "app_settings.h"
#include "app_endpoint_cfg.h"
#include "app_led.h"
#include "app_button.h"
#include "app_battery.h"
#include "app_time.h"
#include "app_reporting.h"
#include "app_on_off.h"
#include "app_repeat_cmd.h"


typedef struct{
    uint8_t keyType; /* CERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
                   SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
    uint8_t key[16]; /* the key used */
}app_linkKey_info_t;


typedef struct{
    ev_timer_event_t *timerFactoryReset;
    ev_timer_event_t *timerLedEvt;
    ev_timer_event_t *timerBatteryEvt;

    uint16_t ledOnTime;
    uint16_t ledOffTime;
    uint8_t  oriSta;     //original state before blink
    uint8_t  sta;        //current state in blink
    uint8_t  times;      //blink times

    bool net_steer_start;

    button_t button[MAX_BUTTON_NUM];
    u8  keyPressed;

    app_linkKey_info_t tcLinkKey;
} app_ctx_t;

extern app_ctx_t g_appCtx;
extern bdb_commissionSetting_t g_bdbCommissionSetting;
extern bdb_appCb_t g_zbBdbCb;

status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_timeCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_rfPowerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_alarmsCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
//status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);

void app_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void app_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void app_otaProcessMsgHandler(uint8_t evt, uint8_t status);
bool app_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);
void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);


#endif /* SRC_INCLUDE_APP_ROUTER_H_ */
