/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.26
        Device            :  PIC16LF1703
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/

#include <xc.h>
#include "pin_manager.h"
#include "stdbool.h"



void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */   
    LATA = 0x00;    
    LATC = 0x00;    

    /**
    TRISx registers
    */    
    TRISA = 0x17;   // 0001 0111 (RA5->S11059_SDA ; RA4-> ANALOG INPUT: TEMP SENSOR ; RA2->AS3955_IRQ) 
    TRISC = 0x02;   // 0000 0010 (RC5->S11059_SCL; RC4->RedLED ; RC3->AS3955_/SS; RC2->AS3955_MOSI; RC1->AS3955_MISO; RC0->AS3955_SCLK)

    /**
    ANSELx registers
    */   
    ANSELC = 0x00;  // (RC3, RC4, RC5 digital pins) 
    ANSELA = 0x11;  // RA0/AN0 analog input pin, RA4/AN3 analog input -> 0001 0001

    /**
    WPUx registers
    */ 
    WPUA = 0x3F;
    WPUC = 0x3F;
    //OPTION_REGbits.nWPUEN = 0;
    OPTION_REGbits.nWPUEN = 0x1;

    /**
    ODx registers
    */   
    ODCONA = 0x00;
    ODCONC = 0x00;

    bool state = (unsigned char)GIE;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RC2PPSbits.RC2PPS = 0x0012;   //RC2->MSSP:SDO;
    SSPDATPPSbits.SSPDATPPS = 0x0011;   //RC1->MSSP:SDI;
    SSPCLKPPSbits.SSPCLKPPS = 0x0010;   //RC0->MSSP:SCK;
    RC0PPSbits.RC0PPS = 0x10;   //RC0->MSSP:SCK;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

    GIE = state;
    
}       

void PIN_MANAGER_IOC(void)
{   

}

/**
 End of File
*/