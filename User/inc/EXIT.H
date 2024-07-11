#ifndef __EXIT_H
#define __EXIT_H

#include "main.h"
#include "bsp.h"

typedef struct 
{
	uint8_t Enable;				//按键检测使能标识，在物理管教捕获时置高
	uint8_t Long_Press;		//长按标识
	uint8_t Short_Press;	//短按标识，使用后第一时间清零
	uint32_t Count;				//滤波时间计数器
	GPIO_TypeDef* GPIOx;	//物理管脚号
	uint16_t GPIO_Pin_x;
	uint8_t Last_Value;		//之前的电平状态，用来捕获数字边沿
}Key_str;

void EXIT_Init(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x , EXTITrigger_TypeDef Edge);
uint8_t Calculate_Positon(uint16_t Indata);
void Key_Init(Key_str* key , GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x);
void Key_Start(Key_str* key);
void Key_Match(void);
void Key_Reset(Key_str* key);
void Key_filter(Key_str* key , uint8_t short_threlhold);
bool Check_ShortPress(Key_str* key);
#endif
