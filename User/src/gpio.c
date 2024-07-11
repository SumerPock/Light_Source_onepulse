#include "bsp.h"

void User_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
	/* Enable the GPIOA peripheral */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
	/* Configure MCO1 pin(PA8) in alternate function */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/* HSE clock selected to output on MCO1 pin(PA8)*/
	RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
	  
	/* Output SYSCLK/4 clock on MCO2 pin(PC9) ***********************************/ 
	/* Enable the GPIOACperipheral */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	  
	/* Configure MCO2 pin(PC9) in alternate function */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
	/* SYSCLK/4 clock selected to output on MCO2 pin(PC9)*/
	RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4);
	
	// ����EN5��EN6
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			//�ƻ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  				//���ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 								//��ʼ��
	
	// ����LED0�����ϵ����á�LED1~2����忪�ص�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			//�ƻ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  				//���ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 								//��ʼ��
}


void User_Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			//�ƻ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  				//���ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 								//��ʼ��	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			//�ƻ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  				//���ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 								//��ʼ��	


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  			//�ƻ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  				//���ߵ�ƽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 								//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  				//ѡ��Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		//���ö˿ڵķ�ת�ٶȼ���Ϊ2MHz 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 								//��ʼ��	
}


