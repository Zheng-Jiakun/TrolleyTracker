#ifndef _UART_H_
#define _UART_H_

#include <stdbool.h>
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

typedef struct 
{
  uint8_t string[UART_RX_BUF_SIZE];
  uint8_t done_flag;
  uint16_t index;
} uart_rx_string_t;

void uart_init(void);
void uart_clear_rx_buf (void);
uint8_t *uart_get_rx_string (void);

#endif