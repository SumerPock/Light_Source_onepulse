#ifndef __FSMC_H
#define __FSMC_H

#include "stm32f4xx.h" 

/**
*�궨������FSMC_WR
*���������offset
*ʵ�ֹ��ܣ�����FSMC_Bank1_ADDRΪ����ַ��offsetΪƫ�Ƶ�ַ��offset����ƫ�Ƶ�ַȡ��ȥ��Ҫ���õļĴ���
����꺯���������Ǹ��ݴ���� offset ������ⲿ�洢��������ĵ�ַ��
Ȼ�󷵻�һ��ָ��õ�ַ�� volatile uint16_t ���͵�ָ�롣����������16λ��ʽ��д�õ�ַ�����ݣ�
ͨ���������ⲿ�豸����ͨ�Ż�����
**/
#define FSMC_WR(offset) *((volatile uint16_t*) 	(FSMC_Bank1_BaseADDR + (offset<<1)))
#define FSMC_Bank1_BaseADDR    									((uint32_t)(0x60000000))		

//��������	
void FSMC_GPIO_Config(void);	//��ʼ��FSMC_GPIO
void FSMC_Init(void);					//��ʼ���ⲿSRAM
void CycleCode_Renew(uint32_t CodeTime);

#endif
