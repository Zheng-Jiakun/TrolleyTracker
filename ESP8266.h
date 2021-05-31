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
    ESP_WAIT,
    ESP_ON = 10,
    ESP_OFF
} esp_state_t;

#define ESP_EN_PIN      5

#define ESP_TX_PERIOD   3000

#define ESP_TIMEOUT_MS  10000UL
#define ESP_RESPONSE    "OK\r\n"
#define ESP_ERROR       "ERROR\r\n"

//esp8266 configuration
#define WIFI_SSID       "ISD_Mesh"
#define WIFI_PASSWORD   "ISDworks!"
#define MQTT_SERVER     "dmh488.resnet.ust.hk"
#define MQTT_PORT       "1883"
#define MQTT_USERNAME   "mqttuser"
#define MQTT_PASSWORD   "mqttpassword"
//#define MQTT_CLIENT_ID  "Trolley2"
#define MQTT_TOPIC      "trolley"
#define MQTT_QOS        "2"

//json data key names
//#define MAC_ADDRESS     "FD"
#define MAC_ADDRESS_KEY "M"
#define BEACON_KEY      "B"
#define MAJOR_KEY       "M"
#define MINOR_KEY       "m"
#define TXPOWER_KEY     "T"
#define RSSI_KEY        "R"


esp_state_t ESP_send_command (const uint8_t* cmd);
void ESP_connect_WIFI(void);
void ESP_connect_MQTT(void);
void ESP_send_beacon (void);
void ESP_switch(esp_state_t);
void ESP_restart(void);
void ESP_init (void);

#endif