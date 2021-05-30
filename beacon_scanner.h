#ifndef _BEACON_SCANNER_H_
#define _BEACON_SCANNER_H_

#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "app_util.h"
#include "app_error.h"
#include "fds.h"
#include "nrf_fstorage.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_scan.h"

#define BEACON_MAX_NUM  5

typedef struct {
    uint16_t major;
    uint16_t minor;
    int8_t txpower;
    int8_t rssi;
 } beacon_data_t;

 typedef struct {
    uint8_t count;
    uint8_t dirty_flag[BEACON_MAX_NUM];
    beacon_data_t data[BEACON_MAX_NUM];
 } beacon_t;

void scan_init(void);
void scan_start(void);
void ble_stack_init(void);
void reset_beacon_info(void);
beacon_t* get_beacon_info(void);
uint8_t get_ble_id(void);
uint8_t *get_ble_mac(void);



#endif