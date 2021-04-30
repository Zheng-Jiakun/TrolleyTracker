#include <uart.h>


uart_rx_string_t uart_rx;

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
        app_uart_get(uart_rx.string + uart_rx.index);
        if (uart_rx.string[uart_rx.index] == '\n')
        {
            uart_rx.done_flag = 1;
            uart_rx.string[uart_rx.index+1] = '\0';
            uart_rx.index = 0;
        }
        else
        {
            uart_rx.index++;
            uart_rx.done_flag = 0;
        }
    }
    else if (p_event->evt_type == APP_UART_TX_EMPTY)
    {
        bsp_board_led_invert(BSP_BOARD_LED_3);
    }
}

void uart_init(void)
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

    //printf("UART started.\r\n");
}

void uart_clear_rx_buf (void)
{
    memset(uart_rx.string, '\0', UART_RX_BUF_SIZE);
}

uint8_t *uart_get_rx_string (void)
{
    return uart_rx.string;
}