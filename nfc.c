#include "nfc.h"

#define MAX_REC_COUNT      3     /**< Maximum records count. */

/** @snippet [NFC Launch App usage_0] */
/* nRF Toolbox Android application package name */
static const uint8_t m_android_package_name[] = {'c', 'o', 'm', '.', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '.', 't', 'r', 'o', 'l', 'l' ,'e', 'y'};
//{'n', 'o', '.', 'n', 'o', 'r', 'd', 'i', 'c', 's',
//                                                 'e', 'm', 'i', '.', 'a', 'n', 'd', 'r', 'o', 'i',
//                                                 'd', '.', 'n', 'r', 'f', 't', 'o', 'o', 'l', 'b',
//                                                 'o', 'x'};

/* nRF Toolbox application ID for Windows phone */
static const uint8_t m_windows_application_id[] = {'{', 'e', '1', '2', 'd', '2', 'd', 'a', '7', '-',
                                                   '4', '8', '8', '5', '-', '4', '0', '0', 'f', '-',
                                                   'b', 'c', 'd', '4', '-', '6', 'c', 'b', 'd', '5',
                                                   'b', '8', 'c', 'f', '6', '2', 'c', '}'};

uint8_t m_ndef_msg_buf[256];
/** @snippet [NFC Launch App usage_0] */

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
 * @brief Function for application main entry.
 */
void nfc_init(void)
{

    /** @snippet [NFC Launch App usage_1] */
    uint32_t len;
    uint32_t err_code;
    /** @snippet [NFC Launch App usage_1] */

    /* Set up NFC */
    err_code = nfc_t2t_setup(nfc_callback, NULL);
    APP_ERROR_CHECK(err_code);

    /** @snippet [NFC Launch App usage_2] */
    /*  Provide information about available buffer size to encoding function. */
    len = sizeof(m_ndef_msg_buf);

    /* Encode launchapp message into buffer */
    err_code = nfc_launchapp_msg_encode(m_android_package_name,
                                        sizeof(m_android_package_name),
                                        m_windows_application_id,
                                        sizeof(m_windows_application_id),
                                        m_ndef_msg_buf,
                                        &len);

    APP_ERROR_CHECK(err_code);
    /** @snippet [NFC Launch App usage_2] */

    /* Set created message as the NFC payload */
    err_code = nfc_t2t_payload_set(m_ndef_msg_buf, len);
    APP_ERROR_CHECK(err_code);

    /* Start sensing NFC field */
    err_code = nfc_t2t_emulation_start();
    APP_ERROR_CHECK(err_code);

}

