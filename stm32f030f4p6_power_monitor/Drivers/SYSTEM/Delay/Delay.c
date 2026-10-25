#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/Delay/Delay.h"

static uint32_t g_fac_us = 0;       

  
void Delay_Init(uint16_t sysclk)
{
    g_fac_us = sysclk;                                 
}

void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        
    ticks = nus * g_fac_us;                 
    
    told = SysTick->VAL;                    
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      
            }
        }
    }

}

void Delay_ms(uint16_t nms)
{
    Delay_us((uint32_t)(nms * 1000));                   
}










