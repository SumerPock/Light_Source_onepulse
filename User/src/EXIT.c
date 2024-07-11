#include "bsp.h"

uint32_t Filter_Start_Time = 0;
//extern uint32_t Sys_Count;
extern unsigned int Sys_Count;
//�ⲿ�жϳ�ʼ������
void EXIT_Init(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x , EXTITrigger_TypeDef Edge)
{
	uint8_t EXTI_PortSourceGPIOx = 0 , EXTIx_IRQn = 0;
	uint16_t EXTI_PinSourcex = 0;
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	if(GPIOx == GPIOA)	EXTI_PortSourceGPIOx = 0;
	else if(GPIOx == GPIOB)	EXTI_PortSourceGPIOx = 1;
	else if(GPIOx == GPIOC)	EXTI_PortSourceGPIOx = 2;
	else if(GPIOx == GPIOD)	EXTI_PortSourceGPIOx = 3;
	else if(GPIOx == GPIOE)	EXTI_PortSourceGPIOx = 4;
	else if(GPIOx == GPIOF)	EXTI_PortSourceGPIOx = 5;
	else if(GPIOx == GPIOG)	EXTI_PortSourceGPIOx = 6;
	else if(GPIOx == GPIOH)	EXTI_PortSourceGPIOx = 7;
	else if(GPIOx == GPIOI)	EXTI_PortSourceGPIOx = 8;
	else if(GPIOx == GPIOJ)	EXTI_PortSourceGPIOx = 9;
	else if(GPIOx == GPIOK)	EXTI_PortSourceGPIOx = 10;
	
	EXTI_PinSourcex = Calculate_Positon(GPIO_Pin_x);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);//PA1 ���ӵ��ж���0
		
  EXTI_InitStructure.EXTI_Line = GPIO_Pin_x;//EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = Edge;//EXTI_Trigger_Falling; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����

	if(EXTI_PinSourcex < 5)	EXTIx_IRQn = EXTI_PinSourcex + 6;
	else if(EXTI_PinSourcex < 10)	EXTIx_IRQn = 23;
	else if(EXTI_PinSourcex < 16)	EXTIx_IRQn = 40;
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}

//�����������Ӳ���ܽ�ƥ��
void Key_Match(void)
{
	
}
//��GPIO_Pin_xת��ΪEXTI_PinSourcex
uint8_t Calculate_Positon(uint16_t Indata) 
{
	if(Indata == 0x0001)	return 0;
	else if(Indata == 0x0002)	return 1;
	else if(Indata == 0x0004)	return 2;
	else if(Indata == 0x0008)	return 3;
	else if(Indata == 0x0010)	return 4;
	else if(Indata == 0x0020)	return 5;
	else if(Indata == 0x0040)	return 6;
	else if(Indata == 0x0080)	return 7;
	else if(Indata == 0x0100)	return 8;
	else if(Indata == 0x0200)	return 9;
	else if(Indata == 0x0400)	return 10;
	else if(Indata == 0x0800)	return 11;
	else if(Indata == 0x1000)	return 12;
	else if(Indata == 0x2000)	return 13;
	else if(Indata == 0x4000)	return 14;
	else if(Indata == 0x8000)	return 15;
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	

void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
//	Key_Start(&Rotate_Right);
}	

void EXTI2_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line2);
//	Key_Start(&F2);
}	

void EXTI3_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line3);
//	Key_Start(&F1);
}	

void EXTI4_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line4);
//	Key_Start(&F3);
}	

void EXTI9_5_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_ClearITPendingBit(EXTI_Line8);
	EXTI_ClearITPendingBit(EXTI_Line9);
//	Key_Start(&Rotate_Down);	
}
void EXTI15_10_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line10);
	EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_ClearITPendingBit(EXTI_Line12);
	EXTI_ClearITPendingBit(EXTI_Line14);
	EXTI_ClearITPendingBit(EXTI_Line15);
	Position_In_Zero();
}	   

//�����ṹ���ʼ��
void Key_Init(Key_str* key , GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x)
{
	key->Enable = 0;
	key->Count = 0;
	key->Long_Press = 0;
	key->Short_Press = 0;
	key->GPIOx = GPIOx;
	key->GPIO_Pin_x = GPIO_Pin_x;
}

//�����˲����ڱ��ؼ���ж���ʹ��
void Key_Start(Key_str* key)
{
	key->Enable = 1;
	key->Count = Sys_Count;
	key->Long_Press = 0;
	key->Short_Press = 0;
}
//�����˲�
void Key_filter(Key_str* key , uint8_t short_threlhold)
{
	uint16_t long_threlhold = 1500;
	if (key->Enable)
	{
		if (IO_Read(key->GPIOx , key->GPIO_Pin_x) == 0)
		{
			key->Long_Press = ((Sys_Count - key->Count) > long_threlhold);
			if ((Sys_Count - key->Count) > short_threlhold)
			{
				if (key->Last_Value)
				{
					key->Last_Value = 0;
					key->Short_Press = 1;	
				}
			}
		}
		else
		{
			key->Last_Value = 1;	
			key->Enable = 0;
			key->Long_Press = 0;
		}
	}
}
//������λ
void Key_Reset(Key_str* key)
{
	key->Enable = 0;
	key->Count = 0;
	key->Long_Press = 0;
	key->Short_Press = 0;
	key->Last_Value = 1;
}
//����Ӧkey�Ķ̰��Ƿ���
bool Check_ShortPress(Key_str* key)
{
	if (key->Short_Press == 1)
	{
		key->Short_Press = 0;
		return true;
	}
	return false;
}


