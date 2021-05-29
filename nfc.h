#ifndef _NFC_H_
#define _NFC_H_

#include "bsp.h"
#include "nfc_t2t_lib.h"
#include "nfc_ndef_msg.h"
#include "nfc_text_rec.h"
#include "nrf_log_ctrl.h"
#include "nfc_launchapp_msg.h"
#include "ESP8266.h"
#include "beacon_scanner.h"

typedef struct 
{
    bool wifi;
    bool ble;
    bool mqtt;
    uint8_t ip[16];
    uint16_t bat;
    uint8_t mac;
} nfc_data_t;

void nfc_init(void);
void nfc_update(void);
void update_nfc_message(const uint8_t *text, uint16_t text_len);
nfc_data_t *get_nfc_data(void);

#endif