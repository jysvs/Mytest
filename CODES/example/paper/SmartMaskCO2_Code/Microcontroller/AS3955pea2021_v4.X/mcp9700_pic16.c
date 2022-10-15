/* 
 * File:   mcp9700_pic16.c
 * Author: P. Escobedo
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "adc_pic16.h"
#include "mcp9700_pic16.h"

#define _XTAL_FREQ  1000000

float MCP9700ReadTemp()
{
    //Temperature
    float t;

    //Read ADC
    uint16_t adc_value=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    adc_value+=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    adc_value+=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    //Promedio de 3 lecturas
    adc_value=adc_value/3;

    //Convert to degree Centrigrade
    t=((adc_value/1023.00)*330.00); // Vdd = 3.3V
    t = t - 50.00;

    return t;
}

uint16_t MCP9700ReadTempHex()
{
    //Read ADC
    uint16_t adc_value=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    adc_value+=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    adc_value+=ReadADC(MCP9700_ADC_CH);
    __delay_us(100);

    //3 readings average
    adc_value=adc_value/3;

    return adc_value;
}
