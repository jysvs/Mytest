#include <xc.h>
#include <stdint.h>

#include "adc_pic16.h"

#define _XTAL_FREQ  1000000


void ADCInit()
{
    //ADC Control Register 1 Configuration
    ADCON1bits.ADFM = 1;     // ADC result is right justified
    ADCON1bits.ADNREF = 0;   // Vref- = VSS
    ADCON1bits.ADPREF0 = 0;  //
    ADCON1bits.ADPREF1 = 0;  // ADPREF = 00 --> Vref+ = VDD
    ADCON1bits.ADCS = 0b111; // FCR (Clock supplied from an internal RC oscillator)

    //ADC Control Register 0 Configuration
    ADCON0bits.CHS1 = 1;     // Select ADC Input Channel (AN1)
    
    //Turn on ADC Module
//    ADCON0bits.ADON=1;
}

uint16_t ReadADC(uint8_t ch)
{
  if(ch>7) return 0;
  ADCON0bits.CHS=ch;

  //Wait for aquisition
  __delay_us(100);

  //Start Conversion
  ADCON0bits.GO=1;

  //Wait for the conversion to complete
  while(ADCON0bits.GO);

  return ((ADRESH<<8)|ADRESL);
}