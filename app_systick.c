#include "app_systick.h"

void app_systick_init(void)
{
    /* Init systick driver */
    nrf_drv_systick_init();
}

uint32_t app_systick_get(void)
{
    nrfx_systick_state_t tick;
    nrf_drv_systick_get(&tick);
    return tick.time;
}