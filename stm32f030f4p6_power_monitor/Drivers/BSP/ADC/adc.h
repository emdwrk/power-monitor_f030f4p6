#ifndef __ADC_H__
#define __ADC_H__   

#include "SYSTEM/sys/sys.h"
#include "SYSTEM/Delay/Delay.h"

#define ADC_REF_VOLTAGE     3.27f
#define R_UP                9910.0f
#define R_DOWM              3260.0f


void adc_init(void);
uint8_t adc_convert_to_IandV(float *I,float *V,float *W);

#endif
