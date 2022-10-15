/* 
 * File:   adc_pic16.h
 */

#ifndef ADC_PIC16_H
#define	ADC_PIC16_H

#include <xc.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void ADCInit();
uint16_t ReadADC(uint8_t ch);




#ifdef	__cplusplus
}
#endif

#endif	/* ADC_PIC16_H */

