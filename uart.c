#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"

#include "app_uart.h"
#if defined(UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined(UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#define UART_TX_BUF_SIZE 256 /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256 /**< UART RX buffer size. */

/* When UART is used for communication with the host do not use flow control.*/
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

void uart_event_handle(app_uart_evt_t *p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
    else if (p_event->evt_type == APP_UART_DATA_READY)
    {
        uint8_t cr;
        app_uart_get(&cr);
        // printf("%c", cr);
        app_uart_put(cr);
        if (cr == 'k')
            bsp_board_led_on(BSP_BOARD_LED_2);
        else if (cr == 'g')
            bsp_board_led_off(BSP_BOARD_LED_2);
    }
    else if (p_event->evt_type == APP_UART_TX_EMPTY)
    {
        bsp_board_led_invert(BSP_BOARD_LED_3);
    }
}

/**
 * @brief Function for main application entry.
 */
void uart_task_setup(void)
{
    uint32_t err_code;

    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        UART_HWFC,
        false,
#if defined(UART_PRESENT)
        NRF_UART_BAUDRATE_115200
#else
        NRF_UARTE_BAUDRATE_115200
#endif
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);

    APP_ERROR_CHECK(err_code);

    printf("UART started.\r\n");
}

void uart_task_loop()
{
    // uint8_t cr;
    // while (app_uart_get(&cr) != NRF_SUCCESS)
    //     ;
    // while (app_uart_put(cr) != NRF_SUCCESS)
    //     ;
}

/** @} */
