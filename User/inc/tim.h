#ifndef __TIM_H
#define __TIM_H	 
#include "stm32f4xx.h" 
#include "gpio.h"
#include "main.h"
#include "bsp.h"

extern unsigned int timer6_loop;

/*定时器TIM2的初始化*/
extern void TIM2_Int_Init(unsigned short arr , unsigned short psc);
extern void TIM3_Int_Init(unsigned short arr , unsigned int psc);
extern void TIM4_Int_Init(unsigned short arr , unsigned int psc);
extern void TIM6_Int_Init(unsigned short arr , unsigned int psc);
#endif
