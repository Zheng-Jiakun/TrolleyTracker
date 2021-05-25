#include "nfc.h"
#include "ESP8266.h"

#define MAX_REC_COUNT      1     /**< Maximum records count. */

/* Text message in English with its language code. */
/** @snippet [NFC text usage_1] */
static const uint8_t en_payload[] = "hello";
//{
//    'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'
//};
static const uint8_t en_code[] = {'e', 'n'};
/** @snippet [NFC text usage_1] */


/* Buffer used to hold an NFC NDEF message. */
uint8_t m_ndef_msg_buf[256];

/**
 * @brief Callback function for handling NFC events.
 */
static void nfc_callback(void * p_context, nfc_t2t_event_t event, const uint8_t * p_data, size_t data_length)
{
    (void)p_context;

    switch (event)
    {
        case NFC_T2T_EVENT_FIELD_ON:
            bsp_board_led_on(BSP_BOARD_LED_0);
            break;
        case NFC_T2T_EVENT_FIELD_OFF:
            bsp_board_led_off(BSP_BOARD_LED_0);
            break;
        default:
            break;
    }
}


/**
 * @brief Function for encoding the NDEF text message.
 */
static ret_code_t welcome_msg_encode(uint8_t * p_buffer, uint32_t * p_len)
{
    /** @snippet [NFC text usage_2] */
    ret_code_t err_code;

    /* Create NFC NDEF text record description in English */
    NFC_NDEF_TEXT_RECORD_DESC_DEF(nfc_en_text_rec,
                                  UTF_8,
                                  en_code,
                                  sizeof(en_code),
                                  en_payload,
                                  sizeof(en_payload));
    /** @snippet [NFC text usage_2] */

    /* Create NFC NDEF message description, capacity - MAX_REC_COUNT records */
    /** @snippet [NFC text usage_3] */
    NFC_NDEF_MSG_DEF(nfc_text_msg, MAX_REC_COUNT);
    /** @snippet [NFC text usage_3] */

    /* Add text records to NDEF text message */
    /** @snippet [NFC text usage_4] */
    err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(nfc_text_msg),
                                       &NFC_NDEF_TEXT_RECORD_DESC(nfc_en_text_rec));
    VERIFY_SUCCESS(err_code);


    /** @snippet [NFC text usage_5] */
    err_code = nfc_ndef_msg_encode(&NFC_NDEF_MSG(nfc_text_msg),
                                   p_buffer,
                                   p_len);
    return err_code;
    /** @snippet [NFC text usage_5] */
}



/**
 * @brief Function for application main entry.
 */
void nfc_init(void)
{
    uint32_t  len = sizeof(m_ndef_msg_buf);
    uint32_t  err_code;

    /* Set up NFC */
    err_code = nfc_t2t_setup(nfc_callback, NULL);
    APP_ERROR_CHECK(err_code);

    /* Encode welcome message */
    err_code = welcome_msg_encode(m_ndef_msg_buf, &len);
    APP_ERROR_CHECK(err_code);

    /* Set created message as the NFC payload */
    err_code = nfc_t2t_payload_set(m_ndef_msg_buf, len);
    APP_ERROR_CHECK(err_code);

    /* Start sensing NFC field */
    err_code = nfc_t2t_emulation_start();
    APP_ERROR_CHECK(err_code);
}

void update_nfc_message(const uint8_t *text, uint16_t text_len)
{
    uint32_t  len = sizeof(m_ndef_msg_buf);
    uint32_t  err_code;

    err_code = nfc_t2t_emulation_stop();
    APP_ERROR_CHECK(err_code);

    NFC_NDEF_TEXT_RECORD_DESC_DEF(nfc_en_text_rec, UTF_8, en_code, sizeof(en_code), text, text_len);
    NFC_NDEF_MSG_DEF(nfc_text_msg, MAX_REC_COUNT);

    err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(nfc_text_msg), &NFC_NDEF_TEXT_RECORD_DESC(nfc_en_text_rec));
    APP_ERROR_CHECK(err_code);

    err_code = nfc_ndef_msg_encode(&NFC_NDEF_MSG(nfc_text_msg), m_ndef_msg_buf, &len);
    APP_ERROR_CHECK(err_code);

    err_code = nfc_t2t_payload_set(m_ndef_msg_buf, len);
    APP_ERROR_CHECK(err_code);

    err_code = nfc_t2t_emulation_start();
    APP_ERROR_CHECK(err_code);
}

void nfc_service(void)
{
    uint8_t nfc_msg[512];
    uint32_t len;
    static uint16_t bat = 420;
    len = sprintf(nfc_msg, "{\"MAC\":\"%s\",\"IP\":\"%s\",\"BAT\":\"%dmV\",\"BLE\":\"%s\",\"WIFI\":\"%s\"}", MAC_ADDRESS, "192.168.1.100", bat, "OK", "OK");
    update_nfc_message(nfc_msg, len);
    bat--;
    if (bat < 300) bat = 420;
}
