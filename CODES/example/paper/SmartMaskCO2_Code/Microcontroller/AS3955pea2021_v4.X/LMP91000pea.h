/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * Date: 10/11/2017    
 * Author: pabloescobedo@ugr.es 
 * Comments:
 * Revision history: v1 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LMP91000pea_H
#define	LMP91000pea_H

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <xc.h> // include processor files - each processor file is guarded.  

/* REGISTER TABLE */
#define LMP91000_I2C_ADDRESS    0x48    /* Device Address: 1 0 0 1 0 0 0 R/W bit not included */
#define LMP91000_I2C_ADDRESS_W  0x90    /* Device Write Address 1 0 0 1 0 0 0 R/W=0 */
#define LMP91000_I2C_ADDRESS_R  0x91    /* Device Read Address 1 0 0 1 0 0 0 R/W=1 */

#define LMP91000_STATUS_REG     0x00    /* Read only Status Register */
#define LMP91000_LOCK_REG		0x01    /* Protection Register */
#define LMP91000_TIACN_REG		0x10    /* TIA Control Register */
#define LMP91000_REFCN_REG		0x11    /* Reference Control Register*/
#define LMP91000_MODECN_REG		0x12    /* Mode Control Register */

/* STATUS register bitfield definition (Address 0x00) */
#define LMP91000_NOT_READY      0x00    // default
#define LMP91000_READY          0x01

/* LOCK register bitfield definition (Address 0x01) */
#define LMP91000_WRITE_LOCK     0x01    // Registers 0x10, 0x11 in read only mode (default)
#define LMP91000_WRITE_UNLOCK   0x00    // Registers 0x10, 0x11 in write mode

/* TIACN register bitfield definition (Address 0x10) */
// TIA feedback resistance selection
#define LMP91000_TIA_GAIN_EXT   0x00    // External resistance (default)
#define LMP91000_TIA_GAIN_2P75K 0x04
#define LMP91000_TIA_GAIN_3P5K  0x08
#define LMP91000_TIA_GAIN_7K    0x0C
#define LMP91000_TIA_GAIN_14K   0x10
#define LMP91000_TIA_GAIN_35K   0x14
#define LMP91000_TIA_GAIN_120K  0x18
#define LMP91000_TIA_GAIN_350K  0x1C
// RLoad selection
#define LMP91000_RLOAD_10OHM    0X00        
#define LMP91000_RLOAD_33OHM    0X01
#define LMP91000_RLOAD_50OHM    0X02
#define LMP91000_RLOAD_100OHM   0X03    // default

/* REFCN register bitfield definition (Address 0x11) */
// Reference voltage source selection
#define LMP91000_REF_SOURCE_INT 0x00    // Internal (default)
#define LMP91000_REF_SOURCE_EXT 0x80    // External
// Internal zero selection (Percentage of the source reference)
#define LMP91000_INT_Z_20PCT    0x00    // 20%
#define LMP91000_INT_Z_50PCT    0x20    // 50% (default)
#define LMP91000_INT_Z_67PCT    0x40    // 67%
#define LMP91000_INT_Z_BYPASS   0x60    // Internal zero circuitry bypassed (only in O2 ground referred measurement)
// Selection of the Bias polarity
#define LMP91000_BIAS_SIGN_NEG  0x00    // Negative (default)
#define LMP91000_BIAS_SIGN_POS  0x10    // Positive
// BIAS selection (Percentage of the source reference)
#define LMP91000_BIAS_0PCT      0x00    // default
#define LMP91000_BIAS_1PCT      0x01 
#define LMP91000_BIAS_2PCT      0x02 
#define LMP91000_BIAS_4PCT      0x03 
#define LMP91000_BIAS_6PCT      0x04 
#define LMP91000_BIAS_8PCT      0x05 
#define LMP91000_BIAS_10PCT     0x06 
#define LMP91000_BIAS_12PCT     0x07 
#define LMP91000_BIAS_14PCT     0x08 
#define LMP91000_BIAS_16PCT     0x09 
#define LMP91000_BIAS_18PCT     0x0A 
#define LMP91000_BIAS_20PCT     0x0B 
#define LMP91000_BIAS_22PCT     0x0C 
#define LMP91000_BIAS_24PCT     0x0D 

/* MODECN register bitfield definition */
// Shorting FET feature
#define LMP91000_FET_SHORT_DISABLED     0x00    // default
#define LMP91000_FET_SHORT_ENABLED      0x80
// Mode of Operation selection
#define LMP91000_OP_MODE_DEEP_SLEEP     0x00    // Deep Sleep (default)
#define LMP91000_OP_MODE_GALVANIC       0x01    // 2-lead ground referred galvanic cell
#define LMP91000_OP_MODE_STANDBY        0x02    // Standby
#define LMP91000_OP_MODE_AMPEROMETRIC   0x03    // 3-lead amperometric cell
#define LMP91000_OP_MODE_TIA_OFF        0x06    // Temperature measurement (TIA OFF)
#define LMP91000_OP_MODE_TIA_ON         0x07    // Temperature measurement (TIA ON)

/* When the LMP91000 is in Temperature measurement (TIA ON) mode, the output of the temperature sensor is
present at the VOUT pin, while the output of the potentiostat circuit is available at pin C2.*/

#define LMP91000_NOT_PRESENT    0xA8    // arbitrary library status code


/******************************************************************************
 * Methods
 ******************************************************************************/

/*
void I2C_Start_Bit();
void I2C_Stop_Bit();
void I2C_Control_Write();
void I2C_Control_Read();
void Send_I2C_Data(uint8_t byte);
*/

uint8_t LMP91000_write(uint8_t addr, uint8_t data);
uint8_t LMP91000_read(uint8_t addr);
uint8_t LMP91000_status(void);
uint8_t LMP91000_lock();
uint8_t LMP91000_unlock();
uint8_t LMP91000_configure(uint8_t _tiacn, uint8_t _refcn, uint8_t _modecn);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LMP91000pea_H */

