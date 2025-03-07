#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
extern void delay_xms(uint16_t nms);
#endif
