#include "app_router.h"

void app_forcedReport(uint8_t endpoint, uint16_t cluster_id, uint16_t attr_id) {

    status_t st = ZCL_STA_SUCCESS;

    APP_DEBUG(UART_PRINTF_MODE, "app_forcedReport(). endpoint: %d, cluster_id: 0x%04x, attr_id: 0x%04x\r\n", endpoint, cluster_id, attr_id);

    if(zb_isDeviceJoinedNwk()) {

        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = APP_ENDPOINT1;
        dstEpInfo.dstAddr.shortAddr = 0;

        zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, cluster_id, attr_id);

        if (pAttrEntry) {
            st = zcl_sendReportCmd(endpoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                        cluster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

            APP_DEBUG(UART_PRINTF_MODE, "st from zcl_sendReportCmd: %d\r\n", st);
        }
    }
}

