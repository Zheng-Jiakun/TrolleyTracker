#include "ESP8266.h"
#include "uart.h"
#include "beacon_scanner.h"

void ESP_connect_WIFI(void)
{
    printf("AT+CWMODE=1\r\n");  //AT+CWMODE=1
    while(strcmp(uart_get_rx_string(), ESP_OK));
    uart_clear_rx_buf();
    
    printf("AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);  //AT+CWJAP="ISD4223","1234567abc"  Thunderbolt, fuckrobomaster
    while(strcmp(uart_get_rx_string(), ESP_OK));
    uart_clear_rx_buf();
}

void ESP_connect_MQTT (void)
{
    printf("AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);  //AT+MQTTUSERCFG=0,1,"Trolley1","aaj","isdn3002",0,0,""
    while(strcmp(uart_get_rx_string(), ESP_OK));
    uart_clear_rx_buf();
    
    printf("AT+MQTTCONN=0,\"%s\",%s,1\r\n", MQTT_SERVER, MQTT_PORT);  //AT+MQTTCONN=0,"team-aaj.duckdns.org",1883,1
    while(strcmp(uart_get_rx_string(), ESP_OK));
    uart_clear_rx_buf();
}

void ESP_send_beacon (void)
{
//AT+MQTTPUB=0,"trackerShort","{\"M\":\"FD\"\,\"A0\":0\,\"I0\":0\,\"T0\":0\,\"R0\":-128\,\"A1\":0\,\"I1\":0\,\"T1\":0\,\"R1\":-128\,\"A2\":0\,\"I2\":0\,\"T2\":0\,\"R2\":-128\,\"A3\":0\,\"I3\":0\,\"T3\":0\,\"R3\":-128}",0,0
    //printf("AT+MQTTPUB=0,\"trackerShort\",\"{\\\"M\\\":\\\"FD\\\"\\,\\\"A0\\\":%d\\,\\\"I0\\\":%d\\,\\\"T0\\\":%d\\,\\\"R0\\\":%d\\,\\\"A1\\\":%d\\,\\\"I1\\\":%d\\,\\\"T1\\\":%d\\,\\\"R1\\\":%d\\,\\\"A2\\\":%d\\,\\\"I2\\\":%d\\,\\\"T2\\\":%d\\,\\\"R2\\\":%d\\,\\\"A3\\\":%d\\,\\\"I3\\\":%d\\,\\\"T3\\\":%d\\,\\\"R3\\\":%d}\",0,0\r\n", 
    //get_beacon_info()[0].major, get_beacon_info()[0].minor, get_beacon_info()[0].txpower, get_beacon_info()[0].rssi, 
    //get_beacon_info()[1].major, get_beacon_info()[1].minor, get_beacon_info()[1].txpower, get_beacon_info()[1].rssi, 
    //get_beacon_info()[2].major, get_beacon_info()[2].minor, get_beacon_info()[2].txpower, get_beacon_info()[2].rssi, 
    //get_beacon_info()[3].major, get_beacon_info()[3].minor, get_beacon_info()[3].txpower, get_beacon_info()[3].rssi);
    //AT+MQTTPUB=0,"test","test62345",1,0
    printf("AT+MQTTPUB=0,\"%s\",\"{\\\"M\\\":\\\"%s\\\"\\,", MQTT_TOPIC, MAC_ADDRESS);
    for (uint8_t i = 0; i < BEACON_NUM; i++)
    {
        printf("\\\"%s%d\\\":%d\\,\\\"%s%d\\\":%d\\,\\\"%s%d\\\":%d\\,\\\"%s%d\\\":%d", 
        MAJOR_KEY, i, get_beacon_info()[i].major,
        MINOR_KEY, i, get_beacon_info()[i].minor, 
        TXPOWER_KEY, i, get_beacon_info()[i].txpower, 
        RSSI_KEY, i, get_beacon_info()[i].rssi);
        if (i != BEACON_NUM - 1)
        {
            printf("\\,");
        }
    }
    printf("}\",%s,0\r\n", MQTT_QOS);
    while(strcmp(uart_get_rx_string(), ESP_OK));
    uart_clear_rx_buf();
    reset_beacon_info();
}