#ifndef __ADC_H__
#define __ADC_H__   

#include "SYSTEM/sys/sys.h"
#include "SYSTEM/Delay/Delay.h"

#define ADC_REF_VOLTAGE     3.29f
#define R_UP                29400.0f
#define R_DOWM              5100.0f


void adc_init(void);
uint8_t adc_convert_to_IandV(int32_t *I, int32_t *V, int32_t *W);

#endif
