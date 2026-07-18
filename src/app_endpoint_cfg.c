#include "app_router.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     		{6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   		{8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID     	{10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RWR             ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_REPORTABLE

#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_UINT24      ZCL_DATA_TYPE_UINT24
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_UINT48      ZCL_DATA_TYPE_UINT48
#define ZCL_INT8        ZCL_DATA_TYPE_INT8
#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_ENUM16      ZCL_DATA_TYPE_ENUM16
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_BITMAP32    ZCL_DATA_TYPE_BITMAP32
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_OCTET_STR   ZCL_DATA_TYPE_OCTET_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
#define ZCL_IEEE_ADDR   ZCL_DATA_TYPE_IEEE_ADDR
#define ZCL_SINGLE_PREC ZCL_DATA_TYPE_SINGLE_PREC

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_TIME,
    ZCL_CLUSTER_GEN_POWER_CFG,
    ZCL_CLUSTER_CUSTOM_RF_POWER_CFG,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep_outClusterList[] = {
    ZCL_CLUSTER_OTA,
    ZCL_CLUSTER_GEN_ON_OFF,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_EP_IN_CLUSTER_NUM      (sizeof(app_ep_inClusterList)/sizeof(app_ep_inClusterList[0]))
#define APP_EP_OUT_CLUSTER_NUM     (sizeof(app_ep_outClusterList)/sizeof(app_ep_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_ep_simpleDesc =
{
    HA_PROFILE_ID,                          /* Application profile identifier */
	HA_DEV_RANGE_EXTENDER,					/* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    1,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP_IN_CLUSTER_NUM,                  /* Application input cluster count */
    APP_EP_OUT_CLUSTER_NUM,                 /* Application output cluster count */
    (uint16_t *)app_ep_inClusterList,       /* Application input cluster list */
    (uint16_t *)app_ep_outClusterList,      /* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_BATTERY,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion      },
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.appVersion      },
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion    },
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion       },
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.manuName         },
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.modelId          },
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.dateCode         },
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,      R,  (uint8_t*)&g_zcl_basicAttrs.powerSource     },
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,    RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable    },
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR,   R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_BASIC_ATTR_NUM    sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)

/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,     RW, (uint8_t*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_IDENTIFY_ATTR_NUM    sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

/* power */
zcl_powerAttr_t g_zcl_powerAttrs =
{
    .batteryVoltage    = 30,   //in 100 mV units, 0xff - unknown
    .batteryPercentage = 0x64, //in 0,5% units, 0xff - unknown
    .batteryAlarmMask = (ALARM_MASK_MIN_THRESHOLD | ALARM_MASK_THRESHOLD_1 | ALARM_MASK_THRESHOLD_2),
    .batteryVoltageMinThreshold = BATTERY_VOLTAGE_MIN_THRESHOLD,
    .batteryVoltageThreshold1 = BATTERY_VOLTAGE_THRESHOLD1,
    .batteryVoltageThreshold2 = BATTERY_VOLTAGE_THRESHOLD2,
    .batteryAlarmState = 0,
};

const zclAttrInfo_t powerCfg_attrTbl[] =
{
    { ZCL_ATTRID_BATTERY_VOLTAGE,               ZCL_UINT8,    RR, (uint8_t*)&g_zcl_powerAttrs.batteryVoltage            },
    { ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,  ZCL_UINT8,    RR, (uint8_t*)&g_zcl_powerAttrs.batteryPercentage         },
    { ZCL_ATTRID_BATTERY_ALARM_MASK,            ZCL_UINT8,    RW, (uint8_t*)&g_zcl_powerAttrs.batteryAlarmMask          },
    { ZCL_ATTRID_BATTERY_VOLTAGE_MIN_THRESHOLD, ZCL_UINT8,    RW, (uint8_t*)&g_zcl_powerAttrs.batteryVoltageMinThreshold},
    { ZCL_ATTRID_BATTERY_VOLTAGE_THRESHOLD_1,   ZCL_UINT8,    RW, (uint8_t*)&g_zcl_powerAttrs.batteryVoltageThreshold1  },
    { ZCL_ATTRID_BATTERY_VOLTAGE_THRESHOLD_2,   ZCL_UINT8,    RW, (uint8_t*)&g_zcl_powerAttrs.batteryVoltageThreshold2  },
    { ZCL_ATTRID_BATTERY_ALARM_STATE,           ZCL_BITMAP32, RR, (uint8_t*)&g_zcl_powerAttrs.batteryAlarmState         },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision            },
};

#define ZCL_POWER_CFG_ATTR_NUM       sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)

/* On/Off Config */

zcl_onOffSwitchCfgAttr_t g_zcl_onOffSwitchCfgAttrs = {
    .switchType     = ZCL_SWITCH_TYPE_TOGGLE,
    .switchActions  = ZCL_SWITCH_ACTION_OFF_ON,
};

const zclAttrInfo_t onoff_switch_cfg_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,   R,   (u8*)&g_zcl_onOffSwitchCfgAttrs.switchType    },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,   RWR, (u8*)&g_zcl_onOffSwitchCfgAttrs.switchActions },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,  R,   (u8*)&zcl_attr_global_clusterRevision         },
};

#define ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM      sizeof(onoff_switch_cfg_attrTbl) / sizeof(zclAttrInfo_t)

zcl_timeAttr_t g_zcl_timeAttrs = {
    .time_utc   = 0,
//    .time_local = 0xffffffff,
    .time_status = 0,
};

const zclAttrInfo_t time_attrTbl[] =
{
    { ZCL_ATTRID_TIME,                      ZCL_UTC,        RWR,    (uint8_t*)&g_zcl_timeAttrs.time_utc         },
//    { ZCL_ATTRID_LOCAL_TIME,                ZCL_UINT32,     R,      (uint8_t*)&g_zcl_timeAttrs.time_local       },
    { ZCL_ATTRID_TIME_STATUS,               ZCL_BITMAP8,    RW,     (uint8_t*)&g_zcl_timeAttrs.time_status      },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_TIME_ATTR_NUM    sizeof(time_attrTbl) / sizeof(zclAttrInfo_t)

zcl_rfConfigAttr_t g_zcl_rfPowerAttrs = {
    .tx_powerLevel = 0,
};

const zclAttrInfo_t rf_powerCfg_attrTbl[] = {
    { ZCL_ATTRID_CUSTOM_TX_POWER, ZCL_ENUM8, RW, (uint8_t*)&g_zcl_rfPowerAttrs.tx_powerLevel },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_UINT16, R, (uint8_t*)&zcl_attr_global_clusterRevision },
};

#define ZCL_RF_POWER_CFG_ATTR_NUM   sizeof(rf_powerCfg_attrTbl) / sizeof(zclAttrInfo_t)

/**
 *  @brief Definition for mini relay ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appClusterList[] =
{
    {ZCL_CLUSTER_GEN_BASIC,                MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,		       basic_attrTbl,            zcl_basic_register,          app_basicCb     },
    {ZCL_CLUSTER_GEN_IDENTIFY,             MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,          identify_attrTbl,         zcl_identify_register,       app_identifyCb  },
    {ZCL_CLUSTER_GEN_POWER_CFG,	           MANUFACTURER_CODE_NONE, ZCL_POWER_CFG_ATTR_NUM,	       powerCfg_attrTbl,	     zcl_powerCfg_register,       app_powerCfgCb  },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, MANUFACTURER_CODE_NONE, ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM, onoff_switch_cfg_attrTbl, zcl_onOffSwitchCfg_register, NULL            },
    {ZCL_CLUSTER_GEN_TIME,		           MANUFACTURER_CODE_NONE, ZCL_TIME_ATTR_NUM,		       time_attrTbl,             zcl_time_register,           app_timeCb      },
    {ZCL_CLUSTER_CUSTOM_RF_POWER_CFG,      SLACKY_MANUF_CODE,      ZCL_RF_POWER_CFG_ATTR_NUM,      rf_powerCfg_attrTbl,      zcl_rfPowerCfg_register,     app_rfPowerCfgCb},
};

uint8_t APP_CB_CLUSTER_NUM = (sizeof(g_appClusterList)/sizeof(g_appClusterList[0]));

