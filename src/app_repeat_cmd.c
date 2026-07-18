#include "app_router.h"

static repeat_cmd_t repeat_cmd[REPEAT_CMD_NUM];

repeat_cmd_t *app_find_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t *addr) {

    for (uint8_t i = 0; i < REPEAT_CMD_NUM; i++) {
        if (repeat_cmd[i].used) {
            if (repeat_cmd[i].clId == clId && repeat_cmd[i].srcEp == srcEp &&
                    repeat_cmd[i].dstEp == dstEp && repeat_cmd[i].dstAddrMode == addrMode) {
                if (repeat_cmd[i].dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                    if (repeat_cmd[i].dstAddr.shortAddr == addr->shortAddr) {
                        APP_DEBUG(DEBUG_REPEAT_EN, "i: %d\r\n", i);
                        return &repeat_cmd[i];
                    }
                } else {
                    if (ZB_64BIT_ADDR_CMP(repeat_cmd[i].dstAddr.extAddr, addr->extAddr)) {
                        APP_DEBUG(DEBUG_REPEAT_EN, "i: %d\r\n", i);
                        return &repeat_cmd[i];
                    }
                }
            }
        }
    }

    return NULL;
}

void app_del_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t addr) {

    for (uint8_t i = 0; i < REPEAT_CMD_NUM; i++) {
        if (repeat_cmd[i].used) {
            if (repeat_cmd[i].clId == clId && repeat_cmd[i].srcEp == srcEp &&
                    repeat_cmd[i].dstEp == dstEp && repeat_cmd[i].dstAddrMode == addrMode) {
                if (repeat_cmd[i].dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                    if (repeat_cmd[i].dstAddr.shortAddr == addr.shortAddr) {
                        repeat_cmd[i].used = false;
                    }
                } else {
                    if (ZB_64BIT_ADDR_CMP(repeat_cmd[i].dstAddr.extAddr, addr.extAddr)) {
                        repeat_cmd[i].used = false;
                    }
                }
            }
        }
    }
}

bool app_add_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t addr, uint8_t cmdId) {

    APP_DEBUG(DEBUG_REPEAT_EN, "clId: 0x%04x, srcEp: %d, dstEp: %d, addrMode: %d\r\n", clId, srcEp, dstEp, addrMode);
    for (uint8_t i = 0; i < REPEAT_CMD_NUM; i++) {
        if (!repeat_cmd[i].used) {
            repeat_cmd[i].used = true;
            repeat_cmd[i].clId = clId;
            repeat_cmd[i].srcEp = srcEp;
            repeat_cmd[i].dstEp = dstEp;
            repeat_cmd[i].dstAddrMode = addrMode;
            memcpy(&repeat_cmd[i].dstAddr, &addr, sizeof(tl_zb_addr_t));
            repeat_cmd[i].cmdId = cmdId;
            return true;
        }
    }

    return false;
}

void app_reset_repeat_cmd() {
    memset(repeat_cmd, 0, sizeof(repeat_cmd));
}
