#include "fsmc.h"

/**
*��������	 SRAM_GPIO_Config
*���������void
*���������void
*ʵ�ֹ��ܣ���ʼ��FSMC
*�������ã�		
		FSMC_CLK:		PD3		FSMC_NE:		PD7		FSMC_NOE:		PD4		FSMC_NWE:		PD5		FSMC_NWAIT:	PD6
		FSMC_NADV:	PB7		FSMC_AD:
								0->	PD14	1->	PD15	 2->	PD0		3->	PD1		4->	PE7		  5->	PE8   6->	PE9		7->	PE10		
								8->	PE11	9->	PE12	10->PE13		11->PE14	12->PE15    13->PD8		14->PD9		15->PD10
**/
void FSMC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  /* ʹ��SRAM��ص�GPIOʱ�� */
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE); 
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
	 /*-- GPIO ���� -----------------------------------------------------*/

  /* ͨ�� GPIO ���� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
  /*DQ�����ź���AD0-AD15 �����������*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  
  /*�����ź���*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//FSMC_CLK
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource3,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//FSMC_NOE
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//FSMC_NWE
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//FSMC_NWAIT
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_FSMC); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FSMC_NE1
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FSMC_NL
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_FSMC); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//FSMC_NBL1
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//FSMC_NBL0
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_FSMC); 
}	

/**
*��������		FSMC_Init
*��������� void
*���������	void
*ʵ�ֹ��ܣ� ��ʼ���ⲿSRAM
**/
void FSMC_Init(void)
{	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	
	FSMC_GPIO_Config();
	
 	readWriteTiming.FSMC_AddressSetupTime = 8;	 ////��ַ����ʱ�䣨ADDSET��Ϊ8��HCLK,8/168M=1/21M=47ns
	readWriteTiming.FSMC_AddressHoldTime = 8;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	readWriteTiming.FSMC_DataSetupTime = 8;		 ////���ݱ���ʱ�䣨DATAST��Ϊ9��HCLK 6*9=54ns	 	 

	readWriteTiming.FSMC_BusTurnAroundDuration = 200;	//��������ת�����ڣ������ڸ���ģʽ��NOR����,
  readWriteTiming.FSMC_CLKDivision = 0x00;	//����ʱ�ӷ�Ƶ��������ͬ�����͵Ĵ洢��
  readWriteTiming.FSMC_DataLatency = 0x00;	//���ݱ���ʱ�䣬������ͬ���͵�NOR
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_B;	 //ѡ��ƥ��SRAM��ģʽ
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1; //ѡ��FSMCӳ��Ĵ洢���� Bank1 sram1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable; 	//���õ�ַ���������������Ƿ���
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_NOR; //����Ҫ���ƵĴ洢������ΪFSMC_MemoryType_SRAM,FSMC_MemoryType_NOR
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//�洢�����ݿ�ȣ�16λ
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;	//�����Ƿ�ʹ��ͻ������ģʽ��������ͬ�����͵Ĵ洢��
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //���õȴ��źŵ���Ч���ԣ�������ͬ�����͵Ĵ洢��
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;	//�����Ƿ�ʹ�ܵȴ��źţ�������ͬ�����͵Ĵ洢��
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   	//�����Ƿ�֧�ְѷǶ����ͻ��������������ͬ�����͵Ĵ洢��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;    //���õȴ��źŲ����ʱ�䣬������ͬ�����͵Ĵ洢��
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	  //�洢��дʹ�� 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;    //��ʹ�õȴ��ź�
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;   //��ʹ����չģʽ����дʹ����ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  	//ͻ��д����
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;	//��дʱ������
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;	//��дͬ��ʱ��ʹ����չģʽʱ������ò���Ч
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����
 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	  //ʹ��BANK1����1								  								
}



/**
 * @brief ��Ҫ��������������
 *
 * ��������ṩ���ں����ĸ߼�������������������������
 *
 * @param[in]  ����1  ����1�������������������ͺͺ���
 * @param[out] ����2  ����2�������������������ͺͺ���
 *
 * @return ��������ֵ�������������������ͺͺ��壬������õĻ���
 *
 * @note �κ�����ע��������÷�ʾ����������������ӡ�
 */
void CycleCode_Renew(uint32_t CodeTime)
{
		double Temp_double;
		Temp_double = (double)(CodeTime - ((CodeTime / 1000) * (0.0089)))*14.7456;		//������ۼ�������΢��
		//Temp_double = (double)CodeTime*14.7456;		//������ۼ�������΢��
		FSMC_WR(1) = (uint16_t)((uint32_t)(Temp_double-0.5) & 0xFFFF);	// �������ڵ�16λ
		FSMC_WR(2) = (uint16_t)((uint32_t)(Temp_double-0.5) >> 16);		// �������ڸ�8λ��ʣ��8λ����
//		FSMC_WR(3) = 19169;	// ������ 1.3ms
		FSMC_WR(3) = 14746;	// ������ 1ms
}	

