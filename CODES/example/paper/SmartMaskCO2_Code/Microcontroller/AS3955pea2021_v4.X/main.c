/**
  File Name:
    main.c
 
 * Created on 24/02/2021, 12:20 by Pablo Escobedo
 
  Summary:
    This is the main file to control the communication with NFC chip, digital colour sensor and temp sensor

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC16LF1703
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 5.45
*/

// Libraries
#include "mcc_generated_files/mcc.h"
#include "AS3955pea.h"
#include "S11059pea.h"
#include "adc_pic16.h"
#include "mcp9700_pic16.h"

// Definitions for digital colour detector S11059
#define device_address  0x2A    // S11059 I2C Device Address ( 0 1 0 1 0 1 0 ) R/W bit not included
#define ctrlByte_write  0x54    // S11059 I2C Device Write Address ( 0 1 0 1 0 1 0 0 ) R/W = 0
#define ctrlByte_read   0x55    // S11059 I2C Device Read Address ( 0 1 0 1 0 1 0 1 ) R/W = 1
#define control_reg     0x00    // ADCreset | Standby func | Standby mon | Gain sel | Int mode | Int time set
#define timing_reg_H    0x01
#define timing_reg_L    0x02
// Ports
#define I2C_SCL     LATCbits.LATC5  // SCL
#define I2C_SDA     PORTAbits.RA5   // SDA

/* Variables */
char red_msb = 0x00, red_lsb = 0x00;
char green_msb = 0x00, green_lsb = 0x00;
char blue_msb = 0x00, blue_lsb = 0x00;
char IR_msb = 0x00, IR_lsb = 0x00;
short red = 0x0000, green = 0x0000, blue = 0x0000, IR = 0x0000;
short red_R = 0x0000, green_R = 0x0000, blue_R = 0x0000;
float temp = 0;
uint16_t tempHex = 0x0000;


void main(void)
{
    // Initialize the device: Oscillator, WDT, SPI, ADC
    SYSTEM_Initialize();
    
    // /SS enable active low
    AS3955_SS = 1;

    ADC_ISR();                              // clear the ADC interrupt flag
    OPTION_REGbits.INTEDG = 1;              // external INT interrupt trigger on rising edge
    INTCONbits.INTE = 1;                    // enable the INT external interrupts
    INTERRUPT_PeripheralInterruptEnable();  // enable peripheral interrupts
    INTERRUPT_GlobalInterruptEnable();      // enable global interrupts     
//    INTERRUPT_GlobalInterruptDisable();
   
    // Init LED sequence
    LED = 1;
    __delay_ms(100);
    LED = 0;
    __delay_ms(100);

    while(1) 
    {
                
        LED = 1;
        __delay_ms(10);
        
        // Get RGB values from colour detector
        getRGB();
        red = (red_msb << 8) | red_lsb;
        green = (green_msb << 8) | green_lsb;
        blue = (blue_msb << 8) | blue_lsb;
        
        // Send RGB values to EEPROM of NFC chip
        __delay_ms(100);
        as3955writeEEPROM(0x05, red);
        __delay_ms(20);
        as3955writeEEPROM(0x06, green);
        __delay_ms(20);
       as3955writeEEPROM(0x07, blue);
        __delay_ms(20);
        
        // Get temp value and send to NFC chip
        temp = MCP9700ReadTemp(); 
        tempHex = MCP9700ReadTempHex(); 
        __delay_ms(100);
        as3955writeEEPROM(0x08, tempHex); // temp =  Vdd*1000*(adc_value/1023)/10 -500/10 = Vdd*100*(adc_value/1023) - 50
        __delay_ms(100);
    
    }
   
}


/***** INTERRUPT SERVICE ROUTINE *****/
void interrupt INTERRUPT_InterruptManager (void)
{
    // external interrupt handler
    // Esta interrupcion salta cuando se escribe algo por RF en la EEPROM del chip NFC
    if(INTCONbits.INTF == 1) 
    {   
        INTCONbits.INTF = 0;    // clear interrupt flag  
    }
    
    // ADC peripheral interrupt handler
    else if(INTCONbits.PEIE == 1 && PIE1bits.ADIE == 1 && PIR1bits.ADIF == 1)
    {      
        ADC_ISR();  // Clear the ADC interrupt flag           
    }
    else
    {
        //Unhandled Interrupt
    }

}

/**
 End of File
*/