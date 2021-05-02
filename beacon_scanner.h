#ifndef _BEACON_SCANNER_H_
#define _BEACON_SCANNER_H_

#include "nordic_common.h"
//#include "nrf_sdm.h"
#include "ble.h"
//#include "ble_hci.h"
//#include "ble_db_discovery.h"
#include "ble_srv_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "app_util.h"
#include "app_error.h"
//#include "ble_dis_c.h"
//#include "ble_rscs_c.h"
//#include "bsp_btn_ble.h"
//#include "peer_manager.h"
//#include "peer_manager_handler.h"
//#include "fds.h"
//#include "nrf_fstorage.h"
//#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_scan.h"


typedef struct {
    uint16_t major;
    uint16_t minor;
    int8_t txpower;
    int8_t rssi;
 } beacon_t;

void scan_init(void);
void scan_start(void);
void ble_stack_init(void);
void reset_beacon_info(void);
beacon_t* get_beacon_info(void);

#define BEACON_NUM  4


#endif