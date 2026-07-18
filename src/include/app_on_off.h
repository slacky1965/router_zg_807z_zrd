#ifndef SRC_INCLUDE_APP_ON_OFF_H_
#define SRC_INCLUDE_APP_ON_OFF_H_

typedef struct {
    uint8_t onoff_status;
} app_onoff_status_t;

extern app_onoff_status_t app_onoff_status;

void cmdOnOff(uint8_t command);
int32_t app_repeatCmdOnOff(void *args);
void app_deferredCmdOnOff();
nv_sts_t onoffStatus_save(void);
nv_sts_t onoffStatus_restore(void);

#endif /* SRC_INCLUDE_APP_ON_OFF_H_ */
