/* 
 * File:   mcp9700_pic16.h
 * Author: P. Escobedo
 */

#ifndef MCP9700_PIC16_H
#define	MCP9700_PIC16_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MCP9700_ADC_CH 3 // Connected to RA4/AN3

float MCP9700ReadTemp();


#ifdef	__cplusplus
}
#endif

#endif	/* MCP9700_PIC16_H */

