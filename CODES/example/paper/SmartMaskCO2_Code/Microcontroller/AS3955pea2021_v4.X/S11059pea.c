/*
 * File:   S11059pea.c
 * Author: P. Escobedo
 */


#include <xc.h>
#include "S11059pea.h"

/* Defines for S11059 */
#define I2C_SCL         LATCbits.LATC5  // SCL  
#define I2C_SDA         PORTAbits.RA5   // SDA

#define device_address      0x2A      // S11059 I2C Device Address ( 0 1 0 1 0 1 0 ) R/W bit not included
#define ctrlByte_write      0x54      // S11059 I2C Device Write Address ( 0 1 0 1 0 1 0 0 ) R/W = 0
#define ctrlByte_read       0x55      // S11059 I2C Device Read Address ( 0 1 0 1 0 1 0 1 ) R/W = 1
#define control_reg         0x00      // ADCreset | Standby func | Standby mon | Gain sel | Int mode | Int time set
#define timing_reg_H        0x01
#define timing_reg_L        0x02

char red_msb = 0x00, red_lsb = 0x00;
char green_msb = 0x00, green_lsb = 0x00;
char blue_msb = 0x00, blue_lsb = 0x00;
char IR_msb = 0x00, IR_lsb = 0x00;

void I2C_SendZero(){
	I2C_SCL = 0;    //Should already be low
	I2C_SDA = 0;	//zero
	I2C_SCL = 1;	//clock it out
	__delay_us(10);
	I2C_SCL = 0;	//clock idle
}

void I2C_SendOne(){
	I2C_SCL = 0;    //Should already be low
	I2C_SDA = 1;	//one
	I2C_SCL = 1;	//clock it out
	__delay_us(10);
	I2C_SCL = 0;	//clock idle
}

void I2C_Start(){
	I2C_SDA = 1;   //prepare
	I2C_SCL = 1;	//prepare
	__delay_us(10);
	I2C_SDA = 0;	//Start signal
	__delay_us(10);
	I2C_SCL = 0;	//Clock low to prepare for transmission
	__delay_us(10);
}

void I2C_Stop(){
	I2C_SCL = 0;	//prepare
	I2C_SDA = 0;	//prepare
	__delay_us(10);
	I2C_SCL = 1;	//Stop signal
	__delay_us(10);
	I2C_SDA = 1;	//Raise SDA to prepare for next Start signal
	__delay_us(10);
}

void I2C_WaitForSlave(){
	while(I2C_SCL == 0);	//Loop while the SCL line is low
}

void I2C_WriteByte(char theByte){
	char temp = theByte;
	//Send the bits, MSB first (mask them off with 0x80)
	for(int i=0;i<8;i++){
		if((temp & 0x80) == 0x80)
            I2C_SendOne();
		else
            I2C_SendZero();
        temp = temp << 1;
    }//for i
	I2C_SendOne(); //Send an ack clock bit (and ignore the result)
}

/*
void I2C_WriteToAddress(uint8_t deviceAddress){
	uint8_t temp;
	temp = deviceAddress << 1;	//Shift it over one bit
	temp = temp & 0xFE;		//Ensure LSB == 0 -> R/W bit = 0 (Write)
	I2C_WriteByte(temp);
}
 */

char I2C_ReadResult(){
	//Device already addressed in Read mode; just clock in the data...
	char readback = 0x00;
	//Send eight clock pulses, reading the data bus after each one
	I2C_SDA = 1;   //Just to be sure
	for(int i=0;i<8;i++){
		readback = (readback << 1) & 0xFE;	//Mask off the new LSB
		I2C_SCL = 1;
		I2C_WaitForSlave();                 //Check if line is high
		if(I2C_SDA)
            readback++;
        __delay_us(10);
		I2C_SCL = 0;
        __delay_us(10);
		} //for i
    // Quitamos el stop 	
    return(readback);
}

/*
void I2C_ReadFromAddress(uint8_t deviceAddress){
	I2C_WriteByte((deviceAddress << 1) | 0x01); // 7-bit address, then a 1 -> R/W bit = 1 (Read)
}
 */

//Set manual timing register to 0x0C30 (3120) so that int. time is 175us*3120=546ms/ch
void setExposureTime() {
    I2C_Start();
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54) --> Device address (0x2A) and W bit (0)
    I2C_WriteByte(timing_reg_H);    // calls timing register H
    I2C_WriteByte(0x0C);    
    I2C_Start();
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54) --> Device address (0x2A) and W bit (0)
    I2C_WriteByte(timing_reg_L);    // calls timing register L
    I2C_WriteByte(0x03);
    I2C_Stop();
}

//Fixed exposure mode settings
    //  LowGain mode
    //  0x00 = 87.5us
    //  0x01 = 1.4ms
    //  0x02 = 22.4ms
    //  0x03 = 179.2ms 
    //  HighGain Mode
    //  0x08 = 87.5us
    //  0x09 = 1.4ms
    //  0x0A = 22.4ms
    //  0x0B = 179.2ms 

/* Posibilidades para el registro de control:
 * High gain, manual period mode: 0x8C (ADC reset) ; 0x0C (operation)
 * High gain, fixed period time 87.5 us ms/ch: 0x88 (ADC reset) ; 0x08 (operation)
 * High gain, fixed period time 1.4 ms/ch: 0x89 (ADC reset) ; 0x09 (operation)
 * High gain, fixed period time 22.4 ms/ch: 0x8A (ADC reset) ; 0x0A (operation)
 * High gain, fixed period time 179.2 ms/ch: 0x8B (ADC reset) ; 0x0B (operation)
 */

// Color Sensor Readings
void getRGB() {
    //Initial setting:    
    I2C_Start();
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(control_reg);     // calls control byte (0x00)
//    I2C_WriteByte(0x8C);            // ADC reset wakeup (High gain, manual setting mode, Tint=00 (175us))
//    I2C_WriteByte(0x8A);           // ADC reset wakeup (High gain, fixed period time 22.4 ms/ch) 
    I2C_WriteByte(0x8B);            // ADC reset wakeup (High gain, fixed period time 179.2 ms/ch) 
//    I2C_WriteByte(0x89);           // ADC reset wakeup (High gain, fixed period time 1.4 ms/ch) 
    
    I2C_Start();                    // restart
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(control_reg);     // calls control byte (0x00)
//    I2C_WriteByte(0x0C);            // ADC reset disabled, bus release (High gain, manual setting mode, Tint=00 (175us))
//    I2C_WriteByte(0x0A);           // ADC reset disabled, bus release (High gain, fixed period time 22.4 ms/ch)
    I2C_WriteByte(0x0B);           // ADC reset disabled, bus release (High gain, fixed period time 179.2 ms/ch)
//    I2C_WriteByte(0x09);           // ADC reset disabled, bus release (High gain, fixed period time 1.4 ms/ch)
    
    I2C_Stop();
    
//    __delay_ms(2200); // wait longer than integration time (546 ms/ch * 4 channels = 2184 ms)
//    __delay_ms(100); // wait longer than integration time (22.4 ms/ch * 4 channels = 89.6 ms)
    __delay_ms(800); // wait longer than integration time (22.4 ms/ch * 4 channels = 716.8 ms)
//    __delay_ms(10); // wait longer than integration time (1.4 ms/ch * 4 channels = 5.6 ms) 
    
    
    I2C_Start(); 
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(0x03);            // calls output data byte

    I2C_Start(); 
    I2C_WriteByte(ctrlByte_read);   // changes to read mode (0x55)
  
    red_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    red_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    green_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    green_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    blue_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    blue_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    IR_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    IR_lsb = I2C_ReadResult();
    I2C_SendOne(); // nack
    
    I2C_Stop();
    
    __delay_ms(1);
    
}

// Color Sensor Readings
void getRGB_546ms() {
    //Initial setting:    
    I2C_Start();
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(control_reg);     // calls control byte (0x00)
    I2C_WriteByte(0x8C);            // ADC reset wakeup (High gain, manual setting mode, Tint=00 (175us))
//    I2C_WriteByte(0x8A);           // ADC reset wakeup (High gain, fixed period time 22.4 ms/ch) 
//    I2C_WriteByte(0x8B);            // ADC reset wakeup (High gain, fixed period time 179.2 ms/ch) 
//    I2C_WriteByte(0x89);           // ADC reset wakeup (High gain, fixed period time 1.4 ms/ch) 
    
    I2C_Start();                    // restart
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(control_reg);     // calls control byte (0x00)
    I2C_WriteByte(0x0C);            // ADC reset disabled, bus release (High gain, manual setting mode, Tint=00 (175us))
//    I2C_WriteByte(0x0A);           // ADC reset disabled, bus release (High gain, fixed period time 22.4 ms/ch)
//    I2C_WriteByte(0x0B);           // ADC reset disabled, bus release (High gain, fixed period time 179.2 ms/ch)
//    I2C_WriteByte(0x09);           // ADC reset disabled, bus release (High gain, fixed period time 1.4 ms/ch)
    
    I2C_Stop();
    
    __delay_ms(2200); // wait longer than integration time (546 ms/ch * 4 channels = 2184 ms)
//    __delay_ms(100); // wait longer than integration time (22.4 ms/ch * 4 channels = 89.6 ms)
//    __delay_ms(800); // wait longer than integration time (22.4 ms/ch * 4 channels = 716.8 ms)
//    __delay_ms(10); // wait longer than integration time (1.4 ms/ch * 4 channels = 5.6 ms) 
    
    
    I2C_Start(); 
    I2C_WriteByte(ctrlByte_write);  // write device address (0x54)
    I2C_WriteByte(0x03);            // calls output data byte

    I2C_Start(); 
    I2C_WriteByte(ctrlByte_read);   // changes to read mode (0x55)
  
    red_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    red_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    green_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    green_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    blue_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    blue_lsb = I2C_ReadResult();
    I2C_SendZero(); // ack
    
    IR_msb = I2C_ReadResult();
    I2C_SendZero(); // ack
    IR_lsb = I2C_ReadResult();
    I2C_SendOne(); // nack
    
    I2C_Stop();
    
    __delay_ms(1);
    
}

