#include "app_router.h"

/*********************************************************************
 * @fn      net_steer_start_offCb
 *
 * @brief   timer callback to clear the network steering flag
 *          and stop blinking after factory reset
 *
 * @param   args - unused
 *
 * @return  -1 to stop the timer
 */
static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;

    light_blink_stop();

    return -1;
}

/*********************************************************************
 * @fn      buttonKeepPressed
 *
 * @brief   handle long press (5s) — trigger factory reset and
 *          start network steering with blink indication
 *
 * @param   btNum - button index (1-based)
 *
 * @return  None
 */
static void buttonKeepPressed(u8 btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
        APP_DEBUG(DEBUG_BUTTON_EN, "The button was keep pressed for 5 seconds\r\n");

        zb_factoryReset();

        g_appCtx.net_steer_start = true;
        TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
        light_blink_start(90, 250, 750);
    }
}


/*********************************************************************
 * @fn      buttonSinglePressed
 *
 * @brief   handle single press — toggle relay on/off
 *
 * @param   btNum - button index (1-based)
 *
 * @return  None
 */
static void buttonSinglePressed(u8 btNum) {

    switch (btNum) {
        case VK_SW1:
            APP_DEBUG(DEBUG_BUTTON_EN, "Button push 1 time\r\n");
            app_batteryCb(NULL);
            app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
            app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
            break;
        default:
            break;
    }
}

//static void buttonDoublePressed(u8 btNum) {
//    APP_DEBUG(DEBUG_BUTTON_EN, "Command double click\r\n");
//}
//
//static void buttonTriplePressed(u8 btNum) {
//    APP_DEBUG(DEBUG_BUTTON_EN, "Command triple click\r\n");
//}
//
//static void buttonQuadruplePressed(u8 btNum) {
//    APP_DEBUG(DEBUG_BUTTON_EN, "Command quadruple click\r\n");
//}


/*********************************************************************
 * @fn      buttonCheckCommand
 *
 * @brief   evaluate click count after release and dispatch
 *          the corresponding action (single press only)
 *
 * @param   btNum - button index (1-based)
 *
 * @return  None
 */
static void buttonCheckCommand(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_STATE_NORMAL;

    if (g_appCtx.button[btNum-1].ctn == 1) {
        buttonSinglePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 2) {
//        buttonDoublePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 3) {
//        buttonTriplePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 4) {
//        buttonQuadruplePressed(btNum);
    }

    g_appCtx.button[btNum-1].ctn = 0;
}


/*********************************************************************
 * @fn      keyScan_keyPressedCB
 *
 * @brief   callback when a key is pressed — record timestamp,
 *          increment click count, and provide feedback blink
 *
 * @param   kbEvt - keyboard event with keycode data
 *
 * @return  None
 */
void keyScan_keyPressedCB(kb_data_t *kbEvt) {

    u8 keyCode = kbEvt->keycode[0];

    if(keyCode != 0xff) {
        g_appCtx.button[keyCode-1].pressed_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_FACTORY_NEW_SET_CHECK;
        g_appCtx.button[keyCode-1].ctn++;
        light_blink_start(1, 30, 1);
        if (zb_isDeviceJoinedNwk()) {
        }
    }
}


/*********************************************************************
 * @fn      keyScan_keyReleasedCB
 *
 * @brief   callback when a key is released — record release
 *          timestamp and set state to released
 *
 * @param   keyCode - released key code
 *
 * @return  None
 */
void keyScan_keyReleasedCB(u8 keyCode){
    if (keyCode != 0xff) {
        g_appCtx.button[keyCode-1].released_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_STATE_RELEASE;

//        g_appCtx.button[keyCode-1].state = APP_STATE_NORMAL;
    }
}

/*********************************************************************
 * @fn      button_handler
 *
 * @brief   periodic button task — detects long press (5s) and
 *          release timeout (250ms), dispatches key events from
 *          the keyboard scanner
 *
 * @param   None
 *
 * @return  None
 */
void button_handler(void) {
    static u8 valid_keyCode = 0xff;

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].state == APP_FACTORY_NEW_SET_CHECK) {
            if(clock_time_exceed(g_appCtx.button[i].pressed_time, TIMEOUT_TICK_5SEC)) {
                buttonKeepPressed(i+1);
            }
        }

        if (g_appCtx.button[i].state == APP_STATE_RELEASE) {
            if(clock_time_exceed(g_appCtx.button[i].released_time, TIMEOUT_TICK_250MS)) {
                buttonCheckCommand(i+1);
            }

        }
    }

    if(kb_scan_key(0, 1)){
        if(kb_event.cnt){
            g_appCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if(kb_event.cnt == 1){
                valid_keyCode = kb_event.keycode[0];
            }
        }else{
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_appCtx.keyPressed = 0;
        }
    }
}

/*********************************************************************
 * @fn      button_idle
 *
 * @brief   check if any button activity is still pending
 *          (key pressed or unprocessed clicks)
 *
 * @param   None
 *
 * @return  true if busy, false if idle
 */
u8 button_idle(void) {

    if (g_appCtx.keyPressed) {
        return true;
    }

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].ctn) return true;
    }

    return false;
}
