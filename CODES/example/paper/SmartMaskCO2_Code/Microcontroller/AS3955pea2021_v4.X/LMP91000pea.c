/*
 * Fecha creacion: 16/11/2017
 * Autor: pabloescobedo@ugr.es 
 * En este caso el I2C lo hago por bit banging para poder usar el puerto SPI con
 * el AS3955. Y funciona de pm!!!
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <xc.h>
#include "mcc_generated_files/spi.h"
#include "i2c_bb_pic16.h"
#include "LMP91000pea.h"

/******************************************************************************
 * Defines
 ******************************************************************************/

#define LMP91000_MENB   LATAbits.LATA5  // LMP91000 Module Enable (active low)

/******************************************************************************
 * Global functions
 ******************************************************************************/

//.............................................................................
// Writes data to an address
//.............................................................................
uint8_t LMP91000_write(uint8_t addr, uint8_t data)
{
    LMP91000_MENB = 0;              // Module enable (active low)
    i2c_start();                    // Start condition  
    i2c_wr(LMP91000_I2C_ADDRESS_W); // Slave Address + W
    i2c_wr(addr);                   // Address pointer (register to be written) 
    i2c_wr(data);                   // Data to be written
    i2c_stop();                     // Stop condition
    LMP91000_MENB = 1;      // Module disable (active low)
    
    // Read back the value of the register
    return LMP91000_read(addr);
}

//.............................................................................
// Reads a register
//.............................................................................
uint8_t LMP91000_read(uint8_t addr)
{
    uint8_t data;
    LMP91000_MENB = 0;              // Module enable (active low)
    i2c_start();                    // Start condition
    i2c_wr(LMP91000_I2C_ADDRESS_W); // Slave Address + W
    i2c_wr(addr);                   // Address pointer (register to be read)
    i2c_start();                    // repeated start
    i2c_wr(LMP91000_I2C_ADDRESS_R); // Slave Address + R
    data=i2c_rd(NACK);              // Read data from buffer
    i2c_stop();                     // Stop condition
    LMP91000_MENB = 1;              // Module disable (active low)
    return data;                 // return data from I2C buffer
}

uint8_t LMP91000_status(void) {
      return LMP91000_read(LMP91000_STATUS_REG);
}     

uint8_t LMP91000_lock(){ // this is the default state
      return LMP91000_write(LMP91000_LOCK_REG, LMP91000_WRITE_LOCK);
}

uint8_t LMP91000_unlock(){ 
      return LMP91000_write(LMP91000_LOCK_REG, LMP91000_WRITE_UNLOCK);
}

uint8_t LMP91000_configure(uint8_t _tiacn, uint8_t _refcn, uint8_t _modecn){
      if(LMP91000_status() == LMP91000_READY){
            LMP91000_unlock();
            LMP91000_write(LMP91000_TIACN_REG, _tiacn);
            LMP91000_write(LMP91000_REFCN_REG, _refcn);
            LMP91000_write(LMP91000_MODECN_REG, _modecn);
            LMP91000_lock();
            return 1;
      }
      return 0;
}

/*
void I2C_Start_Bit()
{
    PIR1bits.SSP1IF = 0;        // clear SSP interrupt bit
    SSP1CON2bits.SEN = 1;       // send start bit
//    while(!SSP1STATbits.S);
//    while(SSP1CON2bits.SEN); // Wait for the SEN bit to go back low before we load the data buffer
    while(!PIR1bits.SSP1IF)     // Wait for the SSPIF bit to go back high before we load the data buffer
        {
        int i = 1;
        }
    PIR1bits.SSP1IF=0;
}

void I2C_Stop_Bit()
{
    PIR1bits.SSP1IF=0;          // clear SSP interrupt bit
    SSP1CON2bits.PEN=1;         // send stop bit
    while(!PIR1bits.SSP1IF)
    {
        int i = 1;
        // Wait for interrupt flag to go high indicating transmission is complete
    }
}

void I2C_Control_Write()
{
    PIR1bits.SSP1IF=0;                  // clear SSP interrupt bit
    SSP1BUF = LMP91000_I2C_ADDRESS_W;   // send the device WRITE address
    while(!PIR1bits.SSP1IF)             // Wait for interrupt flag to go high indicating transmission is complete
        {
        int i = 1;
          // place to add a breakpoint if needed
        }
    PIR1bits.SSP1IF=0;
}

void I2C_Control_Read()
{
    PIR1bits.SSP1IF=0;                  // clear SSP interrupt bit
    SSP1BUF = LMP91000_I2C_ADDRESS_R;   // send the device READ address
    while(!PIR1bits.SSP1IF)             // Wait for interrupt flag to go high indicating transmission is complete
        {
        int i = 1;
          // place to add a breakpoint if needed
        }
    PIR1bits.SSP1IF=0;
   }

void Send_I2C_Data(uint8_t byte)
{
    PIR1bits.SSP1IF=0;          // clear SSP interrupt bit
    SSP1BUF = byte;             // send databyte
    while(!PIR1bits.SSP1IF);    // Wait for interrupt flag to go high indicating transmission is complete
}

uint8_t LMP91000_read(uint8_t reg){
    uint8_t chr = 0;
    LMP91000_MENB = 0;      // Module enable (active low)
    I2C_Start_Bit();        // Start condition  
    I2C_Control_Write();    // Slave Address + W
    Send_I2C_Data(reg);     // Address pointer (register to be read)         
    I2C_Start_Bit();        // repeated start
    I2C_Control_Read();     // Slave Address + R
    RCEN = 1;
    while(RCEN) continue;
    ACKDT = 0;              // Clear the Acknowledge Data Bit - this means we are sending an 'ACK'
    ACKEN = 1;              // Set the ACK enable bit to initiate transmission of the ACK bit 
    chr = SSP1BUF;          // Read data
    I2C_Stop_Bit();         // Stop condition
    LMP91000_MENB = 1;      // Module disable (active low)
    return chr;
}

uint8_t LMP91000_write(uint8_t reg, uint8_t data) {
    LMP91000_MENB = 0;      // Module enable (active low)
    I2C_Start_Bit();        // Start condition  
    I2C_Control_Write();    // Slave Address + W
    Send_I2C_Data(reg);     // Address pointer (register to be written) 
    Send_I2C_Data(data);    // Data to be written 
    I2C_Stop_Bit();         // Stop condition
    LMP91000_MENB = 1;      // Module disable (active low)
    
    // Read back the value of the register
    return LMP91000_read(reg);
}

 */