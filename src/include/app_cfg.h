#ifndef SRC_INCLUDE_APP_CFG_H_
#define SRC_INCLUDE_APP_CFG_H_

#include "app_types.h"

#define ON                      1
#define OFF                     0

/* for reporting */
#define REPORTING_MIN           60              /* 1 min            */
#define REPORTING_MAX           300             /* 5 min            */
#define REPORTING_BATTERY_MIN   5               /* 5 sec            */
#define REPORTING_BATTERY_MAX   3600            /* 1 hour           */
#define REPORTING_MAX_MAX       65000           /* 18 hour          */

#define MY_RF_POWER_INDEX       RF_POWER_INDEX_P10p46dBm

/**********************************************************************
 * Product Information
 * max 24 symbols
 */

#define ZCL_BASIC_MFG_NAME     {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID     {18,'R','o','u','t','e','r','-','Z','G','-','8','0','7','Z','-','S','l','D'}

/**********************************************************************
 * Version configuration
 */

#include "version_cfg.h"

/* Debug mode config */
#ifndef UART_PRINTF_MODE
#define UART_PRINTF_MODE                ON
#endif
#define DEBUG_SAVE_EN                   ON
#define DEBUG_BUTTON_EN                 OFF
#define DEBUG_CONFIG_EN                 OFF
#define DEBUG_ONOFF_EN                  ON
#define DEBUG_REPEAT_EN                 ON
#define DEBUG_TIME_EN                   OFF
#define DEBUG_BATTERY_EN				OFF
#define DEBUG_REPORTING_EN              OFF
#define DEBUG_OTA_EN                    OFF
#define DEBUG_STA_STATUS_EN             OFF
#define DEBUG_ZCL_CB_EN                 ON

#define USB_PRINTF_MODE                 OFF

/* PM */
#define PM_ENABLE                       OFF

/* PA */
#define PA_ENABLE                       ON

/* BDB */
#define TOUCHLINK_SUPPORT               ON
#define FIND_AND_BIND_SUPPORT           OFF

/* Board define */
#if defined(MCU_CORE_8258)
#if (CHIP_TYPE == TLSR_8258_1M)
    #define FLASH_CAP_SIZE_1M           1
    /********************** For 1M Flash only *********************************/
    /* Flash map:
        0x00000  Firmware
        0x40000  OTA Image
        0x80000  NV
        0x96000  Reserved       // User Area - saved energy
        0xFC000  U_Cfg_Info
        0xFE000  F_Cfg_Info
        0xFF000  MAC address
        0x100000 End Flash
     */
//    #define BEGIN_USER_DATA             0x96000   // begin address for saving energy
//    #define END_USER_DATA               0xFC000   // end address
//    #define USER_DATA_SIZE              (END_USER_DATA - BEGIN_USER_DATA)
#else
    /********************** For 1M Flash only *********************************/
    /* Flash map:
        0x00000  Firmware
        0x34000  NV_1
        0x40000  OTA Image
        0x72000  Custom User Area - saved energy
        0x76000  MAC address
        0x77000  F_Cfg_Info
        0x78000  U_Cfg_Info
        0x7A000  NV_2
        0x80000  End Flash
     */
//    #define BEGIN_USER_DATA             0x73000   // begin address for saving energy
//    #define END_USER_DATA               0x76000   // end address
//    #define USER_DATA_SIZE              (END_USER_DATA - BEGIN_USER_DATA)
#endif
    #define CLOCK_SYS_CLOCK_HZ          48000000
    #define NV_ITEM_APP_USER_CFG        (NV_ITEM_APP_GP_TRANS_TABLE + 1)        // see sdk/proj/drivers/drv_nv.h
    #define NV_ITEM_ZCL_ON_OFF_CFG      (NV_ITEM_APP_USER_CFG+1)
#else
    #error "MCU is undefined!"
#endif

/* Board include */
#include "board_zg_807z.h"


/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * 1) if MCU_CORE_826x is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * and there is no need to configure the detection IO port;
 * 2) if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 * 3) if MCU_CORE_B91 is defined, the DRV_ADC_BASE_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be connected to the target under test,
 * such as VCC.
 */
#define VOLTAGE_DETECT_ENABLE                       ON

#if defined(MCU_CORE_826x)
    #define VOLTAGE_DETECT_ADC_PIN                  0
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    #define VOLTAGE_DETECT_ADC_PIN                  GPIO_PC5
#elif defined(MCU_CORE_B91)
    #define VOLTAGE_DETECT_ADC_PIN                  ADC_GPIO_PB0
#endif


/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE                      ON

/* UART module */
#define MODULE_UART_ENABLE                          OFF

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
    #define ZBHCI_EN                                1
#endif

/**********************************************************************
 * ZCL cluster support setting
 */
#define ZCL_GROUP_SUPPORT                           OFF
#define ZCL_SCENE_SUPPORT                           OFF
#define ZCL_OTA_SUPPORT                             ON
#define ZCL_GP_SUPPORT                              ON
#define ZCL_TIME_SUPPORT                            ON
#define ZCL_POWER_CFG_SUPPORT						ON
#define ZCL_ON_OFF_SUPPORT                          ON
#define ZCL_ON_OFF_SWITCH_CFG_SUPPORT               ON
#define ZCL_ALARMS_SUPPORT                          ON

/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
    EV_POLL_ED_DETECT,
    EV_POLL_HCI,
    EV_POLL_IDLE,
    EV_POLL_MAX,
}ev_poll_e;

#endif /* SRC_INCLUDE_APP_CFG_H_ */
