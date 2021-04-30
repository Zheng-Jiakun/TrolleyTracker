#include "beacon_scanner.h"
#include "nrf_log.h"

#define APP_BLE_CONN_CFG_TAG        1                                   /**< Tag that identifies the BLE configuration of the SoftDevice. */
#define APP_BLE_OBSERVER_PRIO       3                                   /**< BLE observer priority of the application. There is no need to modify this value. */
#define APP_SOC_OBSERVER_PRIO       1                                   /**< SoC observer priority of the application. There is no need to modify this value. */

#define SCAN_INTERVAL               0x07D0                              /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                 0x07D0                              /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION               0x0000                              /**< Duration of the scanning (timeout) in units of 10 milliseconds. If set to 0x0000, scanning continues until it is explicitly disabled. */

NRF_BLE_SCAN_DEF(m_scan);                                   /**< Scanning Module instance. */

bool                  m_memory_access_in_progress;   /**< Flag to keep track of ongoing operations on persistent memory. */

ble_gap_scan_params_t m_scan_param =                 /**< Scan parameters requested for scanning and connection. */
{
    .active        = 0x00,
    .interval      = SCAN_INTERVAL,
    .window        = SCAN_WINDOW,
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
    .timeout       = SCAN_DURATION,
//    .scan_phys     = BLE_GAP_PHY_CODED,                                 // Choose only one of the following scan_phys
    .scan_phys     = BLE_GAP_PHY_1MBPS,
//    .scan_phys     = BLE_GAP_PHY_2MBPS,
    .extended      = 1,
};

///**@brief 128-bit UUID base List. */
//ble_uuid128_t const m_base_uuid128 =
//{
//   {
//       //0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
//       //0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
       
//       0xAB, 0x81, 0x90, 0xD5, 0xD1, 0x1E, 0x49, 0x41,
//       0xAC, 0xC4, 0x42, 0xF3, 0x00, 0x00, 0x00, 0x00
//       //, 0x05, 0x10, 0xB4, 0x08
//       //0xAB, 0x81, 0x90, 0xD5, 0xD1, 0x1E, 0x49, 0x41,
//       //0xAC, 0xC4, 0x42, 0xF3, 0x05, 0x10, 0xB4, 0x08
//   }
//};

//ble_uuid_t m_beacon_uuid =
//{
//    .uuid = 0x0510,
//    .type = BLE_UUID_TYPE_VENDOR_BEGIN
//};


 const beacon_t beacon_reset[BEACON_NUM] = {{0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}};
 beacon_t beacon[BEACON_NUM] = {{0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}};

 int8_t find_min_rssi (beacon_t *beacon, uint8_t *min_index)
 {
    int8_t min = 127;
    for (uint8_t i = 0; i < BEACON_NUM; i++)
    {
        if (beacon[i].rssi < min)
        {
            min = beacon[i].rssi;
            *min_index = i;
        }
    }
    return min;
 }

 int8_t find_minor (uint16_t minor)
 {
    int8_t index = -1;
    for (uint8_t i = 0; i < BEACON_NUM; i++)
    {
        if (beacon[i].minor == minor)
        {
            index = i;
        }
    }
    return index;
 }

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
        {
            if (m_scan.scan_buffer.p_data[9] == 0xCB && m_scan.scan_buffer.p_data[10] == 0x5D && m_scan.scan_buffer.p_data[11] == 0xF7 && m_scan.scan_buffer.p_data[12] == 0xB3)
            //if (m_scan.scan_buffer.p_data[9] == 0xAB && m_scan.scan_buffer.p_data[10] == 0x81 && m_scan.scan_buffer.p_data[11] == 0x90 && m_scan.scan_buffer.p_data[12] == 0xD5)
            {
                uint8_t min_index;
                if (p_ble_evt->evt.gap_evt.params.adv_report.rssi > find_min_rssi(beacon, &min_index))
                {
                    uint16_t minor = (m_scan.scan_buffer.p_data[27] * 256) + m_scan.scan_buffer.p_data[28];
                    int8_t minor_index = find_minor(minor);
                    if (minor_index == -1)
                    {
                        beacon[min_index].major = (m_scan.scan_buffer.p_data[25] * 256) + m_scan.scan_buffer.p_data[26];
                        beacon[min_index].minor = minor;
                        beacon[min_index].txpower = (int8_t)m_scan.scan_buffer.p_data[29];
                        beacon[min_index].rssi = p_ble_evt->evt.gap_evt.params.adv_report.rssi;
                    }
                    else
                    {
                        //beacon[minor_index].major = (m_scan.scan_buffer.p_data[25] * 256) + m_scan.scan_buffer.p_data[26];
                        //beacon[minor_index].minor = minor;
                        beacon[minor_index].txpower = (int8_t)m_scan.scan_buffer.p_data[29];
                        beacon[minor_index].rssi = p_ble_evt->evt.gap_evt.params.adv_report.rssi;
                    }
                }


                //printf ("\r\n-----------Beacon Found-----------\r\n");
                //for (uint8_t i = 0; i < BEACON_NUM; i++)
                //    printf("beacon %d:\nmajor: %d\tminor: %d\t txPower: %d\t rssi: %d\n", i, beacon[i].major, beacon[i].minor, beacon[i].txpower, beacon[i].rssi);
                //printf ("----------------------------------\r\n");

                NRF_LOG_RAW_INFO ("\r\n-----------Beacon Found-----------\r\n");
                for (uint8_t i = 0; i < BEACON_NUM; i++)
                    NRF_LOG_RAW_INFO("beacon %d:\nmajor: %d\tminor: %d\t txPower: %d\t rssi: %d\n", i, beacon[i].major, beacon[i].minor, beacon[i].txpower, beacon[i].rssi);
                //NRF_LOG_RAW_HEXDUMP_INFO (m_scan.scan_buffer.p_data, m_scan.scan_buffer.len);
                //NRF_LOG_RAW_INFO ("\r\n%d\r\n", p_ble_evt->evt.gap_evt.params.adv_report.rssi);
                NRF_LOG_RAW_INFO ("----------------------------------\r\n");
            }
        }

        default:
            break;
    }
}

/**@brief Function for handling Scanning Module events.
 */
void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_SCAN_TIMEOUT:
        {
            NRF_LOG_INFO("Scan timed out.");
            scan_start();
        } break;

        default:
          break;
    }
}


/**@brief Function for initializing the scanning and setting the filters.
 */
void scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;

    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.connect_if_match = false;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;
    init_scan.p_scan_param     = &m_scan_param;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    APP_ERROR_CHECK(err_code);
  

    //err_code = sd_ble_uuid_vs_add(&m_base_uuid128, &m_beacon_uuid.type);
    //APP_ERROR_CHECK(err_code);

    //err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_UUID_FILTER, &m_beacon_uuid);
    //APP_ERROR_CHECK(err_code);

    //err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_UUID_FILTER, true);
    //APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting scanning.
 */
void scan_start(void)
{
    ret_code_t err_code;

    // If there is any pending write to flash, defer scanning until it completes.
    //if (nrf_fstorage_is_busy(NULL))
    //{
    //    m_memory_access_in_progress = true;
    //    return;
    //}

    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief SoftDevice SoC event handler.
 *
 * @param[in] evt_id    SoC event.
 * @param[in] p_context Context.
 */
void soc_evt_handler(uint32_t evt_id, void * p_context)
{
    switch (evt_id)
    {
        case NRF_EVT_FLASH_OPERATION_SUCCESS:
        /* fall through */
        case NRF_EVT_FLASH_OPERATION_ERROR:

            if (m_memory_access_in_progress)
            {
                m_memory_access_in_progress = false;
                scan_start();
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
  */
void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register handlers for BLE and SoC events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
    NRF_SDH_SOC_OBSERVER(m_soc_observer, APP_SOC_OBSERVER_PRIO, soc_evt_handler, NULL);
}

void reset_beacon_info(void)
{
    memcpy(&beacon, &beacon_reset, sizeof(beacon));
}

beacon_t* get_beacon_info(void)
{
    return beacon;
}