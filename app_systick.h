#ifndef _APP_SYSTICK_H_
#define _APP_SYSTICK_H_


#include "system_nrf52.h"
#include "nrf_drv_systick.h"

#define APP_US2TICK(us)  (us * ((SystemCoreClock) / 1000UL))
#define APP_MS2TICK(ms)  (ms * ((SystemCoreClock) / 1000000UL))
#define APP_TICK2US(tick)  (tick / SystemCoreClock * 1000UL)
#define APP_TICK2MS(tick)  (tick / SystemCoreClock * 1000000UL)

void app_systick_init(void);
uint32_t app_systick_get(void);

#endif