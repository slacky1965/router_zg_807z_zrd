#ifndef SRC_INCLUDE_VERSION_CFG_H_
#define SRC_INCLUDE_VERSION_CFG_H_

#define BOOT_LOADER_MODE            0

/* Boot loader address. */
#define BOOT_LOADER_IMAGE_ADDR      0x0

/* APP image address. */
#if (BOOT_LOADER_MODE)
    #define APP_IMAGE_ADDR          0x8000
#else
    #define APP_IMAGE_ADDR          0x0
#endif


/* Chip IDs */
#define TLSR_8267                   0x00
#define TLSR_8269                   0x01
#define TLSR_8258_512K              0x02
#define TLSR_8258_1M                0x03
#define TLSR_8278                   0x04
#define TLSR_B91                    0x05

/* Image types */
#if (BOOT_LOADER_MODE)
#define IMAGE_TYPE_BOOT_FLAG        1
#else
#define IMAGE_TYPE_BOOT_FLAG        0
#endif

#ifndef MCU_CORE_8258
#define MCU_CORE_8258               1
#endif

#define CHIP_TYPE                   TLSR_8258_1M //TLSR_8258_512K //

#define APP_RELEASE                 0x10        //app release 1.0
#define APP_BUILD                   0x01        //app build 01, full version - v1.0.01
#define STACK_RELEASE               0x30        //stack release 3.0
#define STACK_BUILD                 0x01        //stack build 01
#define HW_VERSION                  0x01

#ifndef ZCL_BASIC_MFG_NAME
    #define ZCL_BASIC_MFG_NAME      {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
    #define ZCL_BASIC_MODEL_ID      {8,'T','L','S','R','8','2','6','x'}
#endif

#ifndef BUILD_DATE
#define BUILD_DATE                  {8,'2','0','2','5','0','7','2','6'}
#endif

#ifndef ZCL_BASIC_DATE_CODE
#define ZCL_BASIC_DATE_CODE         BUILD_DATE
#endif

#ifndef ZCL_BASIC_LOC_DESC
    #define ZCL_BASIC_LOC_DESC      {7,'U','N','K','N','O','W','N'}
#endif
#ifndef ZCL_BASIC_BUILD_ID
    #define ZCL_BASIC_BUILD_ID      {10,'0','1','2','2','0','5','2','0','1','7'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID //max 16 chars v1.3.02
    #define ZCL_BASIC_SW_BUILD_ID   {7,'v',(APP_RELEASE>>4)+0x30,'.',(APP_RELEASE&0xf)+0x30,'.',(APP_BUILD>>4)+0x30,(APP_BUILD&0xf)+0x30,0}
#endif

/*
 * 0x04 - Watermeter
 * 0x07 - ElectricityMeter
 * 0x08 - Template
 * 0x09 - Remote Control
 * 0x0a - Livolo_switch_2keys
 * 0x0b - Livolo_switch_1key
 * 0x0c - Livolo_switch_1key_dimmable
 * 0x0d - Smoke_sensor
 * 0x0e - Livolo_curtain_control
 * 0x0f - Livolo_thermostat
 * 0x10 - EKF_switch_2keys_battery
 * 0x11 - Tuya Thermostat
 * 0x12 - ECM_DIN1_counter
 * 0x13 - Tuya CO2 Sensor
 * 0x14 - Air monitor
 * 0x15 - Tuya Temperature and Humidity sensors ts0201_tz3000
 * 0x16 - Tuya Temperature and Humidity sensors ts0601_tze200 ZG-227Z
 * 0x17 - Tuya water leak sensor ZG-222ZA, ZTU or tlsr825x with 1M
 * 0x18 - Tuya water leak sensor ZG-222ZA, tlsr825x with 512K
 * 0x19 - Tuya water leak sensor SNZB-05, tlsr8656 with 512K
 * 0x1A - ElectricityMeter_ABC
 * 0x1B - Test Remote Constol
 * 0x1C - Tuya mini relay control _TZ3000_kqvb5akv
 * 0x1D - Tuya relay QS-Zigbee-SEC02 _TZ3000_m8f3z8ju
 * 0x1E - Tuya relay QS-Zigbee-SEC01 _TZ3000_hdc8bbha
 * 0x1F - Tuya plug TS011F with monitoring
 * 0x20 - Tuya door/window sensor TS0203
 * 0x21 - Tuya Temperature and Humidity sensors (board_ihseno_ic_v0.h)
 * 0x22 - Tuya Temperature and Humidity sensors (board_zbeacon.h)
 * 0x23 - Tuya Temperature and Humidity sensors (board_zg_227zs.h)
 * 0x24 - Tuya Temperature and Humidity sensors (board_th01_zbeacon.h)
 * 0x25 - Tuya mini switch ZG-301Z Hobean
 * 0x26 - LoraTap wireless battery switch (TS0041-TS0046)
 * 0x27 - Tuya wireless battery switch (1 ... 6 button)
 * 0x28 - Tuya Temperature and Humidity sensors (board_z_wxd.h)
 * 0x29 - Tuya Temperature and Humidity sensors (*.h)
 * 0x2A - EKF Socket Stockholm TS011F _TZ321A_arrqgd67
 *
 */

#define IMAGE_TYPE_APP              (0x2A | (IMAGE_TYPE_BOOT_FLAG << 7))

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define SLACKY_MANUF_CODE           0x6565
#define MANUFACTURER_CODE_TELINK    SLACKY_MANUF_CODE//Custom ID
//#define MANUFACTURER_CODE_TELINK   0x1141//Telink ID
#define IMAGE_TYPE                  ((CHIP_TYPE << 8) | IMAGE_TYPE_APP)
#define FILE_VERSION                ((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)


/* Pre-compiled link configuration. */
#define IS_BOOT_LOADER_IMAGE        0
#define RESV_FOR_APP_RAM_CODE_SIZE  0
#define IMAGE_OFFSET                APP_IMAGE_ADDR


#endif /* SRC_INCLUDE_VERSION_CFG_H_ */
