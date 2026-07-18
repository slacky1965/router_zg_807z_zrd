#ifndef SRC_INCLUDE_APP_BATTERY_H_
#define SRC_INCLUDE_APP_BATTERY_H_

#define MAX_VBAT_MV                     3300                /* 3100 mV - > battery = 100%   */
#define MIN_VBAT_MV                     2800                /* 2800 mV - > battery = 0%     */
#define BATTERY_TIMER_INTERVAL          TIMEOUT_5SEC        //TIMEOUT_15MIN
#define ALARM_MASK_MIN_THRESHOLD        0x01
#define ALARM_MASK_THRESHOLD_1          0x02
#define ALARM_MASK_THRESHOLD_2          0x04
#define BATTERY_VOLTAGE_MIN_THRESHOLD   28                  /* 100mV = 2.8V                 */
#define BATTERY_VOLTAGE_THRESHOLD1      29                  /* 2.9V                         */
#define BATTERY_VOLTAGE_THRESHOLD2      30                  /* 3.0V                         */

int32_t app_batteryCb(void *arg);

#endif /* SRC_INCLUDE_APP_BATTERY_H_ */
