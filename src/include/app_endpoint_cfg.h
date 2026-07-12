#ifndef SRC_INCLUDE_APP_ENDPOINT_CFG_H_
#define SRC_INCLUDE_APP_ENDPOINT_CFG_H_

#define APP_ENDPOINT1 0x01

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct {
    uint8_t  zclVersion;
    uint8_t  appVersion;
    uint8_t  stackVersion;
    uint8_t  hwVersion;
    uint8_t  manuName[ZCL_BASIC_MAX_LENGTH];
    uint8_t  modelId[ZCL_BASIC_MAX_LENGTH];
    uint8_t  dateCode[ZCL_BASIC_MAX_LENGTH];
    uint8_t  powerSource;
    uint8_t  genDevClass;                        //attr 8
    uint8_t  genDevType;                         //attr 9
    uint8_t  deviceEnable;
    uint8_t  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
} zcl_basicAttr_t;


/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
    uint16_t identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
    uint8_t  nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
    uint8_t   sceneCount;
    uint8_t   currentScene;
    uint8_t   nameSupport;
    bool      sceneValid;
    uint16_t  currentGroup;
}zcl_sceneAttr_t;

/**
 *  @brief Defined for time cluster attributes
 */
typedef struct {
    uint32_t time_utc;
//    uint32_t time_local;
    uint8_t  time_status;
} zcl_timeAttr_t;

/**
 *  @brief Defined for custom rfConfig cluster attributes
 */
typedef struct {
    uint8_t tx_powerLevel;
} zcl_rfConfigAttr_t;

/**
 *  @brief Defined for power configuration cluster attributes
 */
typedef struct{
    uint8_t  batteryVoltage;      //0x20
    uint8_t  batteryPercentage;   //0x21
    uint8_t  batteryAlarmMask;
    uint8_t  batteryVoltageMinThreshold;
    uint8_t  batteryVoltageThreshold1;
    uint8_t  batteryVoltageThreshold2;
    uint32_t batteryAlarmState;
} zcl_powerAttr_t;

///**
// *  @brief Defined for on/off cluster attributes
// */
//typedef struct {
//    uint16_t onTime;
//    uint16_t offWaitTime;
//    uint8_t  startUpOnOff;
//    bool     onOff;
//    bool     globalSceneControl;
//    bool     key_lock;
//    uint8_t  led_control;
//} zcl_onOffAttr_t;

extern uint8_t APP_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t  g_appClusterList[];
extern const af_simple_descriptor_t app_ep_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t              g_zcl_basicAttrs;
extern zcl_identifyAttr_t           g_zcl_identifyAttrs;
extern zcl_timeAttr_t				g_zcl_timeAttrs;
extern zcl_powerAttr_t				g_zcl_powerAttrs;
extern zcl_rfConfigAttr_t           g_zcl_rfPowerAttrs;

#define zcl_timeAttrsGet()         	&g_zcl_timeAttrs
#define zcl_powerAttrsGet()         &g_zcl_powerAttrs
#define zcl_rfPowerAttrsGet()       &g_zcl_rfPowerAttrs
//#define zcl_groupAttrsGet()         &g_zcl_groupAttrs
//#define zcl_sceneAttrGet()          &g_zcl_sceneAttrs
//#define zcl_onOffAttrsGet()         &g_zcl_onOffAttrs

#endif /* SRC_INCLUDE_APP_ENDPOINT_CFG_H_ */
