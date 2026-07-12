#ifndef SRC_INCLUDE_BOARD_ZG_807Z_H_
#define SRC_INCLUDE_BOARD_ZG_807Z_H_

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON                  GPIO_PD4
#define PD4_INPUT_ENABLE        ON
#define PD4_DATA_OUT            OFF
#define PD4_OUTPUT_ENABLE       OFF
#define PD4_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PD4     PM_PIN_PULLUP_10K

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON}

/************************** Configure LED ****************************************/

#define LED_ON                  0
#define LED_OFF                 1

#define LED1_GPIO               GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       ON
#define PA0_INPUT_ENABLE        OFF

#define LED1                    LED1_GPIO

/********************* Configure printf UART ***************************/

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PB1    //GPIO_SWS    //printf
#define DEBUG_BAUDRATE          115200

#endif /* UART_PRINTF_MODE */



#endif /* SRC_INCLUDE_BOARD_ZG_807Z_H_ */
