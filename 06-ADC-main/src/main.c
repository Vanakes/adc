#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
#include "uart1.h"
#include "adc_helper.h"
#include "daughterboard.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    init_milis();
    init_uart1();
    
    // na pinech vypneme vstupní buffer
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL14, DISABLE); 
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL15, DISABLE); 
    // nastavíme CLOCK pro ADC2 (16MHz / 4 = 4 MHz)
    ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
    // zvolím zarovnání výsledku -- většinou doprava
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    // nastavíme multiplexer na **některý** kanál
    ADC2_Select_Channel(ADC2_CHANNEL_14);
    // rozběhnu ADC2
    ADC2_Cmd(ENABLE);
    // počkamá až se ADC2 rozběhne (~7 us)
    ADC2_Startup_Wait();

}


int main(void)
{
  
    uint32_t time = 0;
    uint16_t vref, vtemp, temp;

    init();

    while (1) {
        if (milis() - time > 1111 ) {
            time = milis();
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            vtemp = (ADC_get(CHANNEL_VTEMP) * (long)5000 + 512)/ 1023;
            vref =  ((uint32_t)ADC_get(CHANNEL_VREF) * 5000 + 512)/ 1023;
            vtemp = ((uint32_t)vtemp * 2495 + vref/2)/ vref;
            temp = (100L*vtemp - 40000L + 195/2) / 195;
            printf("%u mV %u mV T = %u,%u ˚C\n" , vtemp, vref, temp/10, temp%10);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
