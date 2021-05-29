#ifndef _ADC_H_
#define _ADC_H_

#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nfc.h"

#define ADC2mV(x)   ((x-3)*7)//((x-3)/288.0f*1000)

void adc_init(void);

#endif