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
 * File:   
 * Author: Pablo Escobedo  
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ  1000000

/*
******************************************************************************
* DEFINES
******************************************************************************
*/

#define AS3955_MEM_SIZE_4KBIT    4
#define AS3955_MEM_SIZE_2KBIT    2
#define AS3955_MEM_SIZE as3955_ee_size

/* AS3955 registers */
#define AS3955_REG_IO_CONF              0x00 /*!< IO Configuration Register. */
#define AS3955_REG_IC_CONF0             0x01 /*!< IO Configuration Register 0. */
#define AS3955_REG_IC_CONF1             0x02 /*!< IO Configuration Register 1. */
#define AS3955_REG_IC_CONF2             0x03 /*!< IO Configuration Register 2. */
#define AS3955_REG_RFID_STATUS_DISP     0x04 /*!< RFID Status Display Register RO. */
#define AS3955_REG_RFID_STATUS_IC       0x05 /*!< IC Status Display Register RO. */
#define AS3955_REG_INT0_MASK            0x08 /* Interrupt register 0 mask register */
#define AS3955_REG_INT1_MASK            0x09 /* Interrupt register 1 mask register */
#define AS3955_REG_INT0                 0x0A /* Interrupt register 0 */
#define AS3955_REG_INT1                 0x0B /* Interrupt register 1 */
#define AS3955_REG_BUF_STAT2            0x0C /* Buffer status register 2 */
#define AS3955_REG_BUF_STAT1            0x0D /* Buffer status register 1 */
#define AS3955_REG_LAST_NFC_ADDR        0x0E /* Last NCF address access */
#define AS3955_REG_REV_MAJ              0x1E /* Major revision */
#define AS3955_REG_REV_MIN              0x1F /* Minor revision */

/* AS3955 register bits */
#define AS3955_REG_FIFO_STATUS_1_unf    0x02 /*!< FIFO underflow. */
#define AS3955_REG_FIFO_STATUS_1_ovr    0x01 /*!< FIFO overrun. */

/* AS3955 EEPROM Word addresses */
#define AS3955_EEPROM_USER_START_ADDR   0x04 /*!< first EEPROM user word address, RW. */
#define AS3955_EEPROM_UID_ADDR          0x00 /*!< EEPROM UID word address, RO. */
#define AS3955_EEPROM_FAB_DATA_ADDR     0x01 /*!< EEPROM Fabrication Data word address, RO. */

#define AS3955_EEPROM_CONF_DFLT_V12X    0x267e6000  /*!< Obsolete v1.2.x settings: enable wakeup and power interrupts as well as all bitrates (212, 424, 848kbit/s). */
#define AS3955_EEPROM_CONF_DFLT         0x7b7e6000  /*!< enable wakeup and power interrupts as well as all bitrates (212, 424, 848kbit/s), FSC=128, FWI=618.5ms */
#define AS3955_EEPROM_CONF_RGLTR_MASK   0x0000001F  /*!< Mask to determine current regulator settings */

#define AS3955_DIRECT_CMD_SET_DEFAULT       0x02 /* Puts IC into default state */
#define AS3955_DIRECT_CMD_CLEAR             0x04 /* Stops all activities and clears BUFFER */
#define AS3955_DIRECT_CMD_RESTART_RTX       0x06 /* Initializes RTX communication logic */
#define AS3955_DIRECT_CMD_TOGGLE_RTX        0x07 /* Toogle RTX intefrace bit */
#define AS3955_DIRECT_CMD_TRANSMIT          0x08 /* Start a transmit sequence of the buffer content */
#define AS3955_DIRECT_CMD_TX_ACK            0x09 /* Transmit NFC ACK reply */
#define AS3955_DIRECT_CMD_TX_NAK0           0x0A /* Transmit NFC NAK reply 0x00 */
#define AS3955_DIRECT_CMD_TX_NAK1           0x0B /* Transmit NFC NAK reply 0x01 */
#define AS3955_DIRECT_CMD_TX_NAK4           0x0D /* Transmit NFC NAK reply 0x04 */
#define AS3955_DIRECT_CMD_TX_NAK5           0x0C /* Transmit NFC NAK reply 0x05 */
#define AS3955_DIRECT_CMD_SLEEP             0x10 /* Put tag into SLEEP mode */
#define AS3955_DIRECT_CMD_IDLE              0x11 /* Put tag into IDLE mode */
#define AS3955_DIRECT_CMD_SLEEP_IDLE        0x12 /* Put tag into SLEEP or IDLE state (depends on tag internal state */
#define AS3955_DIRECT_CMD_EN_OTP            0x35 /* Enable write to EEPROM's OTP words (addresses 3 and 4) */
#define AS3955_DIRECT_CMD_EN_TEST_ACCESS    0x36 /* Enables R/W access to test registers */
#define AS3955_DIRECT_CMD_EN_RTX_TEST       0x38 /* Enable RTX buffer test */
#define AS3955_DIRECT_CMD_EE_RST            0x3A /* Create pulse on EEPROM's RESET_N pin. */
#define AS3955_DIRECT_CMD_EE_CLK            0x3C /* Create a pulse on EEPROM's CLK pin. */

/*
 * Special test-specific direct commands
 */

#define AS3955_NUM_REGS                 32 /*!< for continuous read we ignore the holes in the register map. */
#define AS3955_NUM_IRQ_REGS             2    /*!< main and aux irq registers. */
#define AS3955_MAIN_IRQ_REG_IDX         0
#define AS3955_AUX_IRQ_REG_IDX          1

#define AS3955_FIFO_DEPTH               32   /*!< fifo size is 32 bytes. */
//#define AS3955_FIFO_TX_REFILL           8   /*!< number of bytes in FIFO. */

#define AS3955_SILICON_VER_ESA          1   /*!< silicon version ESA (first engineering samples) */
#define AS3955_SILICON_VER_ES           2   /*!< silicon version ES  (first bugfix release) */

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

void as3955writeEEPROM(uint8_t addr, uint32_t data);

uint32_t as3955readEEPROM(uint8_t addr);

void as3955writeReg(uint8_t regaddr, uint8_t regdata);

uint8_t as3955readReg(uint8_t regaddr);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

