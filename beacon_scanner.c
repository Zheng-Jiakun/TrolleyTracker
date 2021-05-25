#include "beacon_scanner.h"
#include "nrf_log.h"

#define APP_BLE_CONN_CFG_TAG        1                                   /**< Tag that identifies the BLE configuration of the SoftDevice. */
#define APP_BLE_OBSERVER_PRIO       3                                   /**< BLE observer priority of the application. There is no need to modify this value. */
#define APP_SOC_OBSERVER_PRIO       1                                   /**< SoC observer priority of the application. There is no need to modify this value. */

#define SCAN_INTERVAL               200U                              /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                 6200U                              /**< Determines scan window in units of 0.625 millisecond. */
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

/**@brief 128-bit UUID base List. */
ble_uuid128_t const m_base_uuid128 =
{
   {
        //0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,   //nrf
        //0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0
       
        //0xAB, 0x81, 0x90, 0xD5, 0xD1, 0x1E, 0x49, 0x41,    //studio
        //0xAC, 0xC4, 0x42, 0xF3, 0x05, 0x10, 0xB4, 0x08

        0xfd, 0xa5, 0x06, 0x93, 0xa4, 0xe2, 0x4f, 0xb1,   //4223 beacon
        0xaf, 0xcf, 0xc6, 0xeb, 0x07, 0x64, 0x78, 0x25
   }
};

//ble_uuid_t m_beacon_uuid =
//{
//    .uuid = 0x0510,
//    .type = BLE_UUID_TYPE_VENDOR_BEGIN
//};


 beacon_t beacon;

 int8_t find_min_rssi (beacon_t *beacon, uint8_t *min_index)
 {
    int8_t min = 127;
    for (uint8_t i = 0; i < BEACON_MAX_NUM; i++)
    {
        if (beacon->data[i].rssi < min)
        {
            min = beacon->data[i].rssi;
            *min_index = i;
        }
    }
    return min;
 }

 int8_t find_beacon_index (beacon_t *beacon, uint16_t major, uint16_t minor)
 {
    int8_t index = -1;
    for (uint8_t i = 0; i < BEACON_MAX_NUM; i++)
    {
        if (beacon->data[i].major == major && beacon->data[i].minor == minor)
        {
            index = i;
        }
    }
    return index;
 }

 void update_beacon_count (beacon_t *beacon)
 {
    beacon->count = 0;
    for (uint8_t i = 0; i < BEACON_MAX_NUM; i++)
    {
        beacon->count += beacon->dirty_flag[i];
    }
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
            //if (m_scan.scan_buffer.p_data[9] == m_base_uuid128.uuid128[0] && m_scan.scan_buffer.p_data[10] == m_base_uuid128.uuid128[1] && m_scan.scan_buffer.p_data[11] == m_base_uuid128.uuid128[2] && m_scan.scan_buffer.p_data[12] == m_base_uuid128.uuid128[3])
            if (memcmp(m_base_uuid128.uuid128, m_scan.scan_buffer.p_data+9, 16) == 0)
            {
                uint8_t min_index;
                if (p_ble_evt->evt.gap_evt.params.adv_report.rssi > find_min_rssi(&beacon, &min_index))
                {
                    uint16_t major = (m_scan.scan_buffer.p_data[25] * 256) + m_scan.scan_buffer.p_data[26];
                    uint16_t minor = (m_scan.scan_buffer.p_data[27] * 256) + m_scan.scan_buffer.p_data[28];
                    int8_t beacon_index = find_beacon_index(&beacon, major, minor);
                    if (beacon_index == -1)   //new beacon
                    {
                        beacon.data[min_index].major = major;
                        beacon.data[min_index].minor = minor;
                        beacon.data[min_index].txpower = (int8_t)m_scan.scan_buffer.p_data[29];
                        beacon.data[min_index].rssi = p_ble_evt->evt.gap_evt.params.adv_report.rssi;
                        beacon.count++;
                        beacon.dirty_flag[min_index] = 1;
                        update_beacon_count(&beacon);
                    }
                    else      //update existing beacon
                    {
                        beacon.data[beacon_index].txpower = (int8_t)m_scan.scan_buffer.p_data[29];
                        beacon.data[beacon_index].rssi = 0.5f*(p_ble_evt->evt.gap_evt.params.adv_report.rssi + beacon.data[beacon_index].rssi);
                    }
                }


                //printf ("\r\n-----------Beacon Found-----------\r\n");
                //for (uint8_t i = 0; i < BEACON_MAX_NUM; i++)
                //    printf("beacon %d:\nmajor: %d\tminor: %d\t txPower: %d\t rssi: %d\n", i, beacon.data[i].major, beacon.data[i].minor, beacon.data[i].txpower, beacon.data[i].rssi);
                //printf ("----------------------------------\r\n");

                //NRF_LOG_RAW_INFO ("\r\n-----------Beacon Found-----------\r\n");
                //for (uint8_t i = 0; i < BEACON_MAX_NUM; i++)
                //    NRF_LOG_RAW_INFO("beacon %d:\nmajor: %d\tminor: %d\t txPower: %d\t rssi: %d\n", i, beacon.data[i].major, beacon.data[i].minor, beacon.data[i].txpower, beacon.data[i].rssi);
                //NRF_LOG_RAW_HEXDUMP_INFO (m_scan.scan_buffer.p_data, m_scan.scan_buffer.len);
                //NRF_LOG_RAW_INFO ("\r\n%d\r\n", p_ble_evt->evt.gap_evt.params.adv_report.rssi);
                //NRF_LOG_RAW_INFO ("----------------------------------\r\n");
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
    if (nrf_fstorage_is_busy(NULL))
    {
        m_memory_access_in_progress = true;
        return;
    }

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
    const beacon_t beacon_reset = {0, {0, 0, 0, 0}, {{0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}, {0, 0, 0, -128}}};
    memcpy(&beacon, &beacon_reset, sizeof(beacon));
    //memset(&beacon, 0, sizeof(beacon));
}

beacon_t* get_beacon_info(void)
{
    return &beacon;
}