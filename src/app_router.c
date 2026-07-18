#include "app_router.h"
#include "factory_reset.h"

static bool boot_announce_sent = false;
uint8_t resp_time = false;

app_ctx_t g_appCtx = {
        .timerFactoryReset = NULL,
};

#ifdef ZCL_OTA
extern ota_callBack_t app_otaCb;

//running code firmware information
ota_preamble_t app_otaInfo = {
    .fileVer            = FILE_VERSION,
    .imageType          = IMAGE_TYPE,
    .manufacturerCode   = MANUFACTURER_CODE_TELINK,
};
#endif

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,//start device cnf cb
    NULL,//reset cnf cb
    NULL,//device announce indication cb
    app_leaveIndHandler,//leave ind cb
    app_leaveCnfHandler,//leave cnf cb
    app_nwkUpdateIndicateHandler,//nwk update ind cb
    NULL,//permit join ind cb
    NULL,//nlme sync cnf cb
    NULL,//tc join ind cb
    NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key = (uint8_t *)tcLinkKeyCentralDefault,             //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key = (uint8_t *)linkKeyDistributedMaster,    //use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key = (uint8_t *)touchLinkKeyMaster,               //use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
    .touchlinkEnable = 1,                                               /* enable touch-link */
#else
    .touchlinkEnable = 0,                                               /* disable touch-link */
#endif
    .touchlinkChannel = DEFAULT_CHANNEL,                                /* touch-link default operation channel for target */
    .touchlinkLqiThreshold = 0xA0,                                      /* threshold for touch-link scan req/resp command */
};



static void afApsAckCb(void *args) {

    apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)args;
    repeat_cmd_t *r_cmd = app_find_repeat_cmd(pApsDataCnf->clusterId,
                                              pApsDataCnf->srcEndpoint,
                                              pApsDataCnf->dstEndpoint,
                                              pApsDataCnf->dstAddrMode,
                                              (tl_zb_addr_t*)&pApsDataCnf->dstAddr);
#if UART_PRINTF_MODE
    APP_DEBUG(DEBUG_REPEAT_EN, "afApsAckCb() - status: 0x%02x, clId: 0x%04x, src_ep: %d, dst_ep: %d, ",
            pApsDataCnf->status, pApsDataCnf->clusterId, pApsDataCnf->srcEndpoint, pApsDataCnf->dstEndpoint);
    if (pApsDataCnf->dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
        APP_DEBUG(DEBUG_REPEAT_EN, "short_addr: 0x%04x, ", pApsDataCnf->dstAddr.addr_short);
    } else {
        APP_DEBUG(DEBUG_REPEAT_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                pApsDataCnf->dstAddr.addr_long[0], pApsDataCnf->dstAddr.addr_long[1],
                pApsDataCnf->dstAddr.addr_long[2], pApsDataCnf->dstAddr.addr_long[3],
                pApsDataCnf->dstAddr.addr_long[4], pApsDataCnf->dstAddr.addr_long[5],
                pApsDataCnf->dstAddr.addr_long[6], pApsDataCnf->dstAddr.addr_long[7]);

        APP_DEBUG(DEBUG_REPEAT_EN, "cmp_addr: %d, ", ZB_64BIT_ADDR_CMP(pApsDataCnf->dstAddr.addr_long, pApsDataCnf->dstAddr.addr_long));
    }
    APP_DEBUG(DEBUG_REPEAT_EN, "r_cmd: %s\r\n", r_cmd?"true":"false");
#endif

    if (r_cmd) {
        if (pApsDataCnf->status != APS_STATUS_SUCCESS) {
            if (pApsDataCnf->dstAddrMode != APS_SHORT_GROUPADDR_NOEP) {
                if (pApsDataCnf->clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
                    TL_ZB_TIMER_SCHEDULE(app_repeatCmdOnOff, r_cmd, TIMEOUT_250MS);
                }
            }
        }
        r_cmd->used = false;
    }
}

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
    /* Initialize ZB stack */
    zb_init();

    /* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
    af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

    /* Initialize ZCL layer */
    /* Register Incoming ZCL Foundation command/response messages */
    zcl_init(app_zclProcessIncomingMsg);

    /* Register endPoint */
    af_endpointRegister(APP_ENDPOINT1, (af_simple_descriptor_t *)&app_ep_simpleDesc, zcl_rx_handler, afApsAckCb);

    zcl_reportingTabInit();

    start_message();
    router_settings_restore();
    zcl_onOffCfgAttr_restore();
    onoffStatus_restore();

    /* Register ZCL specific cluster information */
    zcl_register(APP_ENDPOINT1, APP_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_appClusterList);


#if ZCL_GP_SUPPORT
    /* Initialize GP */
    gp_init(APP_ENDPOINT1);
#endif

#if ZCL_OTA_SUPPORT
    /* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&app_ep_simpleDesc, &app_otaInfo, &app_otaCb);
#endif

#if ZCL_WWAH_SUPPORT
    /* Initialize WWAH server */
    wwah_init(WWAH_TYPE_SERVER, (af_simple_descriptor_t *)&app_simpleDesc);
#endif

    app_batteryCb(NULL);

#if DEBUG_BATTERY_EN
    g_appCtx.timerBatteryEvt = TL_ZB_TIMER_SCHEDULE(app_batteryCb, NULL, 5000);
#else
    g_appCtx.timerBatteryEvt = TL_ZB_TIMER_SCHEDULE(app_batteryCb, NULL, BATTERY_TIMER_INTERVAL);
#endif

    TL_ZB_TIMER_SCHEDULE(app_uptimeCb, NULL, TIMEOUT_1SEC);

    APP_DEBUG(UART_PRINTF_MODE, "zb_getLocalShortAddr: 0x%04x\r\n", zb_getLocalShortAddr());

    if (zb_getLocalShortAddr() >= 0xFFF8) {
        light_blink_start(90, 250, 750);
    }

//    APP_DEBUG(UART_PRINTF_MODE, "FLASH_ADDR_OF_OTA_IMAGE: 0x%08x\r\n", FLASH_ADDR_OF_OTA_IMAGE);
}

/*********************************************************************
 * @fn      app_task
 *
 * @brief   main application task — sends device announce, handles
 *          factory reset, buttons, bl0937, monitoring, and reporting
 *
 * @param   None
 *
 * @return  None
 */
void app_task(void) {

    if (!boot_announce_sent && zb_isDeviceJoinedNwk()) {
        zb_zdoSendDevAnnance();
        boot_announce_sent = true;
    }

//    factoryRst_handler();
    button_handler();

    if (BDB_STATE_GET() == BDB_STATE_IDLE && !button_idle()) {
        report_handler();
    }
}


extern volatile u16 T_evtExcept[4];

/*********************************************************************
 * @fn      app_sysException
 *
 * @brief   handle system exceptions — log the fault and reset
 *
 * @param   None
 *
 * @return  None
 */
static void app_sysException(void) {

    APP_DEBUG(UART_PRINTF_MODE, "app_sysException, line: %d, event: %d, reset\r\n", T_evtExcept[0], T_evtExcept[1]);

#if 1
    SYSTEM_RESET();
#else
    led_on(LED_STATUS);
    while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
    (void)isRetention;

    /* Initialize LEDs*/
    light_init();

//    factoryRst_init();

    /* Initialize Stack */
    stack_init();

    /* Initialize user application */
    user_app_init();

    /* Register except handler for test */
    sys_exceptHandlerRegister(app_sysException);


    /* User's Task */
#if ZBHCI_EN
    zbhciInit();
    ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
    ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
    if(bdb_preInstallCodeLoad(&g_appCtx.tcLinkKey.keyType, g_appCtx.tcLinkKey.key) == RET_OK){
        g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_appCtx.tcLinkKey.keyType;
        g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_appCtx.tcLinkKey.key;
    }

    /* Set default reporting configuration */
    /*  Battery */
    uint8_t reportableChange = 0x01;
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG,
            ZCL_ATTRID_BATTERY_VOLTAGE, REPORTING_BATTERY_MIN, REPORTING_BATTERY_MAX, (uint8_t *)&reportableChange);
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG,
            ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, REPORTING_BATTERY_MIN, REPORTING_BATTERY_MAX, (uint8_t *)&reportableChange);

    /* Uptime */
    uint32_t reportableChange_u32 = 15;
    bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_TIME,
    		ZCL_ATTRID_TIME, 60, 65000, (uint8_t *)&reportableChange_u32);

    /* Initialize BDB */
    bdb_init((af_simple_descriptor_t *)&app_ep_simpleDesc, &g_bdbCommissionSetting, &g_zbBdbCb, 1);

    rf_setTxPower(rx_power_config[settings->tx_power]);
}

