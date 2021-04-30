#include "beacon_scanner.h"
#include "uart.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing the timer. */
static void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details Handles any pending log operations, then sleeps until the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}


int main(void)
{
    // Initialize.
    //log_init();
    //timer_init();
    power_management_init();
    ble_stack_init();
    scan_init();

    bsp_board_init(BSP_INIT_LEDS);
    uart_init();

    // Start execution.
    //NRF_LOG_RAW_INFO(  " ----------------\r\n");
    //NRF_LOG_RAW_INFO(	 "| Beacon scanner |");
    //NRF_LOG_RAW_INFO("\r\n ----------------\r\n");

    printf("Beacon Scanner\n");
    
    //ESP_init();
    //bsp_board_led_on(BSP_BOARD_LED_0);

    scan_start();
    bsp_board_led_on(BSP_BOARD_LED_1);


    while(1)
    {
        //ESP_send();
        //bsp_board_led_invert(BSP_BOARD_LED_4);
        //memcpy(&beacon, &beacon_reset, sizeof(beacon));
        //nrf_delay_ms(1000);
        idle_state_handle();
    }
}
