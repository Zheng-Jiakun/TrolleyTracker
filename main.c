#include "beacon_scanner.h"
#include "uart.h"
#include "ESP8266.h"
#include "app_timer.h"
#include "app_systick.h"
#include "nfc.h"
#include "math.h"
#include "adc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
//#include "nordic_common.h"
//#include "nrf_sdm.h"
//#include "ble_hci.h"
//#include "ble_db_discovery.h"
//#include "ble_conn_state.h"
//#include "ble_dis_c.h"
//#include "ble_rscs_c.h"
//#include "bsp_btn_ble.h"
//#include "peer_manager.h"
//#include "peer_manager_handler.h"
#include "nrf_pwr_mgmt.h"

/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

APP_TIMER_DEF(m_timer);
static void timer_handler(void * p_context)
{
    //printf("current tick is %d\n", app_timer_cnt_get());
}

/**@brief Function for initializing the timer. */
static void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_timer, APP_TIMER_MODE_REPEATED, timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_timer, APP_TIMER_TICKS(1000), NULL);
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
    //timer_init(); //RTC1
    //power_management_init();
    //app_systick_init();

    ble_stack_init();
    scan_init();
    bsp_board_init(BSP_INIT_LEDS);
    ESP_init();
    uart_init();
    nfc_init();
    adc_init();


    nfc_update();


    ESP_connect_WIFI();
    get_nfc_data()->wifi = true;
    nfc_update();
    //bsp_board_led_on(BSP_BOARD_LED_0);

    ESP_connect_MQTT();
    get_nfc_data()->mqtt = true;
    nfc_update();
    //bsp_board_led_on(BSP_BOARD_LED_1);

    scan_start();
    get_nfc_data()->ble = true;
    nfc_update();

    bsp_board_led_on(BSP_BOARD_LED_2);

    while(1)
    {
        bsp_board_led_on(BSP_BOARD_LED_3);
        ESP_send_beacon();
        nrf_delay_ms(ESP_TX_PERIOD-100);
        bsp_board_led_off(BSP_BOARD_LED_3);
        nrf_delay_ms(ESP_TX_PERIOD);

        nfc_update();
        //idle_state_handle();
    }
}
