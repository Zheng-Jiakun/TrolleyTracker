#include "ESP8266.h"

void ESP_init(void)
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