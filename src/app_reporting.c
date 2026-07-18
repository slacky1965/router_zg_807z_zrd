#include "app_router.h"

void app_forcedReport(uint8_t endpoint, uint16_t cluster_id, uint16_t attr_id) {

    APP_DEBUG(UART_PRINTF_MODE, "app_forcedReport(). endpoint: %d, cluster_id: 0x%04x, attr_id: 0x%04x\r\n", endpoint, cluster_id, attr_id);

    if (!zb_isDeviceJoinedNwk()) return;

    zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, cluster_id, attr_id);
    if (!pAttrEntry) return;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstEp = ENDPOINT_1;
    dstEpInfo.dstAddr.shortAddr = 0;
    zcl_sendReportCmd(endpoint, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                      cluster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    for (uint8_t i = 0; i < APS_BINDING_TABLE_NUM; i++) {
        if (bind_tbl[i].used && bind_tbl[i].srcEp == endpoint
            && bind_tbl[i].clusterId == cluster_id
            && bind_tbl[i].dstAddrMode == APS_SHORT_DSTADDR_WITHEP
            && bind_tbl[i].groupAddr != 0)
        {
            dstEpInfo.dstAddr.shortAddr = bind_tbl[i].groupAddr;
            dstEpInfo.dstEp = bind_tbl[i].dstExtAddrInfo.dstEp;
            zcl_sendReportCmd(endpoint, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              cluster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
        }
    }
}

