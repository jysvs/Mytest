/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include <xc.h>
#include "mcc_generated_files/spi.h"
#include "AS3955pea.h"

#define AS3955_SS     LATCbits.LATC3  // /SS

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
/* AS3955 command masks */
#define AS3955_REG_WRITE_MASK       0x00
#define AS3955_REG_READ_MASK        0x20
#define AS3955_DIRECT_CMD_MASK      0xC0
/* AS3955 EEPROM Access commands */
#define AS3955_EEPROM_WRITE_CMD     0x40
#define AS3955_EEPROM_READ_CMD      0x7F
/* AS3955 BUFFER Access commands */
#define AS3955_BUFFER_LOAD_CMD      0x80
#define AS3955_BUFFER_READ_CMD      0xA0

/* size of EEPROM I/O buffer is 1 byte cmd + 1 byte address + 4 bytes word size */
#define AS3955_EEPROM_IO_BUF_SIZE   6

/* timeouts */
#define AS3955_EEPROM_WR_TIMEOUT_MS 50  /* eeprom write takes approx 9ms */
#define AS3955_BUFFER_RX_TIMEOUT_MS 10
#define AS3955_BUFFER_TX_TIMEOUT_MS 100

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */
#define AS3955_DESELECT()       { AS3955_SEN_PIN = 1; }
/* AS3955 Register Access commands */
#define AS3955_REG_WRITE_CMD(addr)  (AS3955_REG_WRITE_MASK | ((addr) & 0x1F))
#define AS3955_REG_READ_CMD(addr)   (AS3955_REG_READ_MASK | ((addr) & 0x1F))
/* AS3955 Direct command */
#define AS3955_DIRECT_CMD(cmd)      (AS3955_DIRECT_CMD_MASK | ((cmd) & 0x3F))

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/

//SPI write to EEPROM 
void as3955writeEEPROM(uint8_t addr, uint32_t data)
{
    //SSP1STAT = 0x80; //1000 0000  SMP and CKE
    AS3955_SS = 0;
    addr = addr << 1;
    __delay_us(20);
    
    SPI_Exchange8bit(AS3955_EEPROM_WRITE_CMD); //mode
    SPI_Exchange8bit(addr); //address
    
    SPI_Exchange8bit(data>>24);
    SPI_Exchange8bit(data>>16);
    SPI_Exchange8bit(data>>8);
    SPI_Exchange8bit(data);
    
    __delay_us(20);
    AS3955_SS = 1;
}

//SPI read from EEPROM
uint32_t as3955readEEPROM(uint8_t addr){
    uint32_t data = 0;
    
    //SSP1STAT = 0x80; //1000 0000 SMP and CKE
    AS3955_SS = 0;
    addr = addr << 1;
    __delay_us(200); //necesaria mas espera para leer
    
    SPI_Exchange8bit(AS3955_EEPROM_READ_CMD);
    SPI_Exchange8bit(addr); //address
    
    data = SPI_Exchange8bit(0x00);
    data = (data << 8)+ SPI_Exchange8bit(0x00);
    data = (data << 8)+ SPI_Exchange8bit(0x00);
    data = (data << 8)+ SPI_Exchange8bit(0x00);
 
    __delay_us(200);
    AS3955_SS = 1;
    
    return data;
}

//Register read
uint8_t as3955readReg(uint8_t regaddr){
    uint8_t data = 0;

    AS3955_SS = 0;
    __delay_us(100);
    
    SPI_Exchange8bit(AS3955_REG_READ_CMD(regaddr));
    data=SPI_Exchange8bit(0x00);
   
    __delay_us(100);
    AS3955_SS = 1;
    
    return data;
}


/**
 End of File
*/

