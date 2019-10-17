/* 
 * File:   
 * Author:      dortega
 * Comments:
 * Revision history: 
 */

#ifndef I2CSW_H
#define	I2CSW_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

#define ACK   0
#define NACK  1

extern void RestartI2C(void);
extern void InitI2C(void);
extern void StartI2C(void);
extern void StopI2C(void);
extern uint8_t SendByteI2C(uint8_t data);
extern uint8_t ReceiveByteI2C(uint8_t nAck);

#endif	/* XC_HEADER_TEMPLATE_H */