#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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


#define UART_TX_BUF_SIZE 512 /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 512 /**< UART RX buffer size. */

/* When UART is used for communication with the host do not use flow control.*/
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

uint8_t uart_rx_string[UART_RX_BUF_SIZE];
uint8_t uart_rx_string_done = 0;

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
        static uint16_t string_index = 0;
        app_uart_get(uart_rx_string + string_index);
        if (uart_rx_string[string_index] == '\n')
        {
            uart_rx_string_done = 1;
            uart_rx_string[string_index+1] = '\0';
            string_index = 0;
        }
        else
        {
            string_index++;
            uart_rx_string_done = 0;
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

void ESP_init()
{
    //while(uart_rx_string_done == 0);
    printf("AT+CWMODE=1\r\n");  //AT+CWJAP="ISD4223","1234567abc"
    while(strcmp(uart_rx_string, "OK\r\n"));
    memset(uart_rx_string, '\0', sizeof(uart_rx_string));
    
    printf("AT+CWJAP=\"thinkpad\",\"12345677\"\r\n");  //AT+CWJAP="ISD4223","1234567abc"  Thunderbolt, fuckrobomaster
    while(strcmp(uart_rx_string, "OK\r\n"));
    memset(uart_rx_string, '\0', sizeof(uart_rx_string));

    bsp_board_led_on(BSP_BOARD_LED_2);
    
    printf("AT+MQTTUSERCFG=0,1,\"Trolley1\",\"aaj\",\"isdn3002\",0,0,\"\"\r\n");  //AT+MQTTUSERCFG=0,1,"Trolley1","aaj","isdn3002",0,0,""
    while(strcmp(uart_rx_string, "OK\r\n"));
    memset(uart_rx_string, '\0', sizeof(uart_rx_string));
    
    printf("AT+MQTTCONN=0,\"team-aaj.duckdns.org\",1883,1\r\n");  //AT+MQTTCONN=0,"team-aaj.duckdns.org",1883,1
    while(strcmp(uart_rx_string, "OK\r\n"));
    memset(uart_rx_string, '\0', sizeof(uart_rx_string));
}

void ESP_send (void)
{
    printf("AT+MQTTPUB=0,\"trackerShort\",\"{\\\"M\\\":\\\"FD\\\"\\,\\\"A0\\\":%d\\,\\\"I0\\\":%d\\,\\\"T0\\\":%d\\,\\\"R0\\\":%d\\,\\\"A1\\\":%d\\,\\\"I1\\\":%d\\,\\\"T1\\\":%d\\,\\\"R1\\\":%d\\,\\\"A2\\\":%d\\,\\\"I2\\\":%d\\,\\\"T2\\\":%d\\,\\\"R2\\\":%d\\,\\\"A3\\\":%d\\,\\\"I3\\\":%d\\,\\\"T3\\\":%d\\,\\\"R3\\\":%d}\",0,0\r\n", 
    beacon[0].major, beacon[0].minor, beacon[0].txpower, beacon[0].rssi, 
    beacon[1].major, beacon[1].minor, beacon[1].txpower, beacon[1].rssi, 
    beacon[2].major, beacon[2].minor, beacon[2].txpower, beacon[2].rssi, 
    beacon[3].major, beacon[3].minor, beacon[3].txpower, beacon[3].rssi);
    //AT+MQTTPUB=0,"test","test62345",1,0
    while(strcmp(uart_rx_string, "OK\r\n"));
    memset(uart_rx_string, '\0', sizeof(uart_rx_string));
}