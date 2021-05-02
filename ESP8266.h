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
#include "app_timer.h"

typedef enum
{
    ESP_OK = 0,
    ESP_TIMEOUT,
    ESP_WAIT
} esp_state_t;

#define ESP_TIMEOUT_MS  10000UL
#define ESP_RESPONSE    "OK\r\n"

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


esp_state_t ESP_send_command (const uint8_t* cmd);
void ESP_connect_WIFI(void);
void ESP_connect_MQTT(void);
void ESP_send_beacon (void);

#endif