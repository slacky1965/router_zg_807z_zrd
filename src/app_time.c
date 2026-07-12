#include "app_router.h"

int32_t app_uptimeCb(void *args) {

	zcl_timeAttr_t *timeAtts = zcl_timeAttrsGet();

	timeAtts->time_utc++;

	return 0;
}

