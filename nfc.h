#ifndef _NFC_H_
#define _NFC_H_

#include "bsp.h"
#include "nfc_t2t_lib.h"
#include "nfc_ndef_msg.h"
#include "nfc_text_rec.h"
#include "nrf_log_ctrl.h"
#include "nfc_launchapp_msg.h"
#include "ESP8266.h"

typedef struct 
{
    bool wifi;
    bool ble;
    uint8_t ip[16];
    uint16_t bat;
    const uint8_t *mac;
} nfc_data_t;

void nfc_init(void);
void nfc_service(void);
void update_nfc_message(const uint8_t *text, uint16_t text_len);

#endif