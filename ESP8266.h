#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "nordic_common.h"
#include "app_util.h"

#define ESP_OK          "OK\r\n"

//esp8266 configuration
#define WIFI_SSID       "thinkpad"
#define WIFI_PASSWORD   "12345677"
#define MQTT_SERVER     "team-aaj.duckdns.org"
#define MQTT_PORT       "1883"
#define MQTT_USERNAME   "aaj"
#define MQTT_PASSWORD   "isdn3002"
#define MQTT_CLIENT_ID  "Trolley1"
#define MQTT_TOPIC      "trackerShort"
#define MQTT_QOS        "0"

//json data key names
#define MAC_ADDRESS     "FD"
#define MAJOR_KEY       "A"
#define MINOR_KEY       "I"
#define TXPOWER_KEY     "T"
#define RSSI_KEY        "R"

void ESP_connect_WIFI(void);
void ESP_connect_MQTT(void);
void ESP_send_beacon (void);
void uart_clear_rx_buf (void);
//uint8_t *uart_get_rx_string (void);

#endif