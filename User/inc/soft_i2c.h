#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "main.h"

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void bsp_InitI2C(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif /* __MAIN_H */

