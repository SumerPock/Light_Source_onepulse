/**
******************************************************************************
* ��    ����	main.c 
* ��    �ߣ�	gaoyao
* �̼��汾��	V1.8.0
* �������ڣ� 2020-7-11
* ��Ҫ˵���� ����ģ���Դ���Ƴ���STM32F407VGT6
* ����汾��	V1.0
* ǰһ�汾��	��
* �޸�ʱ�䣺	2020-4-23
* �汾����˵���� 
							����ͬ��������Ӧ�����˴�����Ҫ�����¼���
									1. ��������ͬ���ֿ�
									2. ������bool b_Ext_Syncȫ�ֱ���������������
									3. main������whileѭ����������b_Ext_Sync�����ĸ���
									4. �޸���ui.c��Menu_0_1��Menu_0_2��Menu_0_3�����Ľ�����ʾ
									5. �޸���ui.c��MenuTrig_OK��������Ӧ������ͬ����Чʱǰ�����˵�����OK����Ӧ
							2022-12-26
							����Ҫ������������ԣ����Ų��ʱ���ִ�����ھ޴�ȱ�����ڱ��룬�����������������֣�����ָ��ص���һ��������
							��Ҳ����֮ǰ�İ汾����û���ǹ����ݻ��Թ��ܣ�����������޸�
							
							2023 3 18 
							Dong.Zhe ��ӻ��ζ������ݽ⹹��֮ǰ�����ݴ���ȫ���ŵ������ж����ʵ�ڿ�����ȥ�ˣ�����������ṹ�����DMA�Ʊػ����
							����Bug�����Լ������
							
							2023.9.6
							���ӹ�о�����ݣ���͸������о��Ӧ��ID͸��������λ���� ID + 60��Ϊ ID + 80
							
* ����汾��	V2.10
* ǰһ�汾��	V2.01
* �޸�ʱ�䣺	2023-8-21
* �汾����˵�����ڼ��⿪���¼����������жϣ�����α���ģʽ�����ø��±��룻����ˢ�±��롣

* ����汾��	V2.11
* ǰһ�汾��	V2.10
* �޸�ʱ�䣺	2024-5-6
* ���ߣ�			shiyongfu
* �汾����˵����
* �������������ģʽ����λ��ָ��0x51��Ӧ��ID 0xd1��bsp.c ��930��
* �����������Ϊuint16_t ���65535����λ���������500.��bsp.c ��389��
* ��������ָ���޸�Ϊ�������65535�����塣��bsp.c ��449��
* �޸�LCD���濪��������룬������������·�CPLD��main.c ��274��
******************************************************************************
*/

#include "main.h"
#include "bsp.h"

RCC_ClocksTypeDef RCC_Clocks;
static __IO uint32_t uwTimingDelay;

struct Uart_DataPacket RemoteDataPacket;

bool Packet_flag = false;
bool b_Ext_Sync = true;
bool b_Ext_Sync_old = false;

FLAG_TIMER flag_timer2 = { 0 };
control_number cn = { 0 };

//���ζ��л�����
unsigned char uart1_rxbuff[4096]; 
unsigned char uart1_txbuff[64]; 
unsigned char uart3_rxbuff[1024];
unsigned char uart3_txbuff[64];

//�������λ��������
ring_buffer rb_uart1_rx;					
ring_buffer rb_uart1_tx;
ring_buffer rb_uart3_rx;
ring_buffer rb_uart3_tx;	

unsigned short RandomNumber, RandomBaseFreq, RandomMinValue, RandomMaxValue;
unsigned int RandomCodeTime;	// α�������
// ��ΪSTM32F407�Ҳ���2Kҳ����ָ�ֻ������������������˱���Ϊ�������ݸ�һ��Buff
unsigned int u32_FlashBuff[2048];	

// α���������������飬���е�һ������Ԫ�ش�ŵ���α���λ��������ռ��ŵ��Ǽ�ȥ��Ƶ����ֵ
unsigned short int u16_RandomData[1024];	

signed int Number_Pulse = 00100;
signed int Tempnumber_Pulse = 100;

signed int CycleCodeTime = 50000;			// ����Ԥ��50ms
signed int Temp_CycleCodeTime = 0;		// ��ʱ��������ֵ��ͨ�����ٿظ��ĵ��������ֵ����ȷ�Ϻ������ֵ�ű����µ�CycleCodeTime��
signed short int Temp_dB_Loss = 0;	//��ʱ����ֵ
//signed short int dB_Loss = 600;		// ��������˥��Ԥ��60dB
signed short int dB_Loss = 0;				// ��������˥��Ԥ��0dB
extern uint32_t MOTO_COUNT;
unsigned char findzero_bigin = 0;



unsigned char flag_lightN_F = false;
//unsigned char auto_lightMode = 0;

//unsigned char Skill_cooldown = 1;

LASER_AUTO_CONTROL LaAutoCon = {0 , 0 , 0 , 1 , 0};

extern void SetTimer6Flag(unsigned int *p);

/**
 * @brief ������
 * @param[in]  
 * @param[out] 
 * @return 
 * @note 
 */
int main(void)
{
	static unsigned short Random_Index = 0;	
	unsigned short i = 0;
	unsigned int *p;		
	union Union_Byte KeyState, KeyState_Old, Hbyte_KeyState;	// �����壬����״̬
	union Union_TypeConversion TypeConversion;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemInit();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);  /* SysTick end of count event each 10ms */
	Delay(5);  /* Insert 50 ms delay */
	//��ʼ��tim�������ж�ʱ��1ms��������ʱ�ж�1s���ܵĴ����źſ��
	//TIM2_Int_Init(2-1 , 42000-1);		
	/*Tout= ((arr+1)*(psc+1))/Tclk = 2 S*/
	TIM3_Int_Init(2000-1 , 42000-1);
	/*Tout= ((arr+1)*(psc+1))/Tclk = 84*1000/42000 000 = 0.002 S*/
	TIM4_Int_Init(84-1 , 1000-1);
	/*Tout= ((arr+1)*(psc+1))/Tclk = 42000*1000/42000 000 = 1 S*/
	TIM6_Int_Init(42000 - 1 , 2000 - 1);
	
	TIM_Cmd(TIM6 , DISABLE); 
	FSMC_Init();									//��ʼ��FSMC����
	User_GPIO_Init();
 	delay_init(168);							//��ʱ������ʼ��	
	LCD_Init();										//���2.4��LCD����

	KeyState.All = (uint8_t)FSMC_WR(8);
	KeyState_Old.All = KeyState.All;
	LaserControl.All = 0x0;

	Read_Memory(FLASH_USER_START_ADDR, 2048, u32_FlashBuff);	// ��Flash�ж�ȡ����4��α�����
	
	p = (uint32_t *)&u16_RandomData;
	
	for(i=0; i<=100; i++)
	{	// �ѵ�һ��α������ݼ��ص�u16_RandomData������
		*(p+i) = u32_FlashBuff[i];
	}
	UI_Init();
	bsp_InitI2C();
	double ddtemp = 0.00;
	RB_Init(&rb_uart1_rx , uart1_rxbuff , BUFFER_SIZE);	//��ʼ�����λ���������
	RB_Init(&rb_uart1_tx , uart1_txbuff , BUFFER_SIZE);		
	RB_Init(&rb_uart3_rx , uart3_rxbuff , BUFFER_SIZE);		
	RB_Init(&rb_uart3_tx , uart3_txbuff , BUFFER_SIZE);
	Uart_Init(USART1 , 115200 , GPIOA , GPIO_PinSource9 , GPIOA , GPIO_PinSource10 , 0 , 8 , 0);
	Uart_Init(USART3 , 115200 , GPIOB , GPIO_PinSource10 , GPIOB , GPIO_PinSource11 , 0 , 4 , 0);
	
	#if (VERSION_SELECTION == Ver_2_3_0)
	User_Motor_GPIO_Init();/*���GPIO���� begin*/
	GPIO_SetBits(EN5);
	GPIO_SetBits(EN6);
	Motor_Power_Off;
	EXIT_Init(Zero_Position, EXTI_Trigger_Falling);
	
	#ifdef	DEBUGPR
		/* ����ͨ��0����������*/
		SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
		/* ����ͨ��0����������*/	
		SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
		SEGGER_RTT_SetTerminal(0);
		SEGGER_RTT_printf(0,RTT_CTRL_TEXT_BRIGHT_GREEN "SEGGER_RTT is begin \r\n");	
	#endif
	
	/*��оָ��� 5A A5 02 00*/
	/*���ù�Դ��ʱͨ��*/
	unsigned char uc_sbuffer[] = {0x5a , 0xa5 , 0x02 ,0x00};
	memset(Uart_Tx_Buffer3 , 0 , sizeof(Uart_Tx_Buffer3));
	memcpy(Uart_Tx_Buffer3 , uc_sbuffer , sizeof(uc_sbuffer));
	DMA_Tx(USART3 , sizeof(uc_sbuffer));		/*���GPIO���� end*/	
	/* 1 / 14.7456 = 0.0678168402777 , 3687 * 0.0678168402777 = 250.04069*/
	//FSMC_WR(4) = 3687;//20kw������Ĭ��Ϊ250us����
	/* 250.5 / 0.0678168402777 = 3693.77280000*/
	//FSMC_WR(4) = pulse_width_formula(5);	// 1553������Ĭ������Ϊ						
	#endif
	
	#ifdef  DEBUGPR
		SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_RED "the motor is find zero \r\n");
	#endif
	while(1)
	{
		TypeConversion.TC_uint16_t = FSMC_WR(8);
		KeyState.All = TypeConversion.TC_byte[0];
		Hbyte_KeyState.All = TypeConversion.TC_byte[1];	
		if(KeyState.bits.b7 == 0)
		{
			b_Ext_Sync = false;
		}
		else
		{
			b_Ext_Sync = true;
		}
		
		if((b_Ext_Sync == false) && (b_Ext_Sync_old == false))
		{	//ֻ�е���ֵ�仯ʱ
			UI_Init();
			b_Ext_Sync_old = true;
		}
		else if((b_Ext_Sync == true) && (b_Ext_Sync_old == true))
		{
			UI_Init();
			b_Ext_Sync_old = false;
		}

		if(KeyState.All != KeyState_Old.All)
		{
			if((KeyState.bits.b4 == 1) && (KeyState_Old.bits.b4 == 0))
			{
				if(KeyState.bits.b5 == 1)	
				{
					MenuTrig_Down();
				}	
				else 
				{
					MenuTrig_Up();
				}		
			}
			if((KeyState.bits.b3 == 0) && (KeyState_Old.bits.b3 == 1)) 
			{
				MenuTrig_OK();
			}		
			if((KeyState.bits.b2 == 0) && (KeyState_Old.bits.b2 == 1)) 
			{
				MenuTrig_Cancel();
			}				
			if((KeyState.bits.b1 == 0) && (KeyState_Old.bits.b1 == 1)) 
			{
				Cross_StateTurn(LaserState.bits.b1);
			}		
			//LCD���濪�ؼ���			
			if((KeyState.bits.b0 == 0) && (KeyState_Old.bits.b0 == 1)) 
			{
				if(Lightsoutce.UIMode == 3)
				{
						/*����ģʽ����*/
//					LaserControl.bits.b0 = 1;					
//					FSMC_WR(0) = (uint16_t)LaserControl.All;
//					FSMC_WR(5) = Number_Pulse;
//					LaserControl.bits.b0 = 0;
//					
//					delay_xms(10);
					
//					LaserControl.bits.b0 = 0;					
//					FSMC_WR(0) = (uint16_t)LaserControl.All;
//					FSMC_WR(5) = Number_Pulse;
//					LaserControl.bits.b0 = 1;		

					Laser_StateTurn_OnePulse(LaserState.bits.b0);
					FSMC_WR(5) = Number_Pulse;
				}else{
					/*һֱ����*/
					Laser_StateTurn(LaserState.bits.b0);
				 /*2024.05.06�޶�������CPLD����������ģʽ�������������·���������������*/
					FSMC_WR(5) = 65535;					
				}

			}				
			if(b_Random_EN)// ���α�������ʹ���ˣ���̬���±���ֵ
			{	
				if((KeyState.bits.b6 == 1) && (KeyState_Old.bits.b6 == 0))
				{
					unsigned int Temp_u32 = 0;	
					if(Random_Index < (u16_RandomData[7] - 1))	
					{
						Random_Index++; 
					}					
					else	
					{
						Random_Index = 0;
					}								
					Temp_u32 = u16_RandomData[6];														// ������Ƶ
					Temp_u32 = Temp_u32 + u16_RandomData[8 + Random_Index];	// �������
					CycleCode_Renew(Temp_u32);	// ������д��CPLD		
				}
			}			
			KeyState_Old.All = KeyState.All;		
		}

		KeyState.All = (uint8_t)(FSMC_WR(8) & 0xFF);
		if((LaserState.bits.b0 != Hbyte_KeyState.bits.b0) ||(LaserState.bits.b1 != Hbyte_KeyState.bits.b1))
		{
			LaserState.bits.b0 = Hbyte_KeyState.bits.b0;	// �����������ص�״̬����ϵͳ����
			LaserState.bits.b1 = Hbyte_KeyState.bits.b1;	// �����������ص�״̬����ϵͳ����							
			if(Hbyte_KeyState.bits.b1 == 0){	
				LED2_OFF;// ������״̬
			}else{
				LED2_ON;
			}				
		}
		
		/*��λ��*/
		if(RB_Get_Length(&rb_uart1_rx) != 0){
			unsigned char getdata = 0;
			if(RB_Read_Byte(&rb_uart1_rx, &getdata) == RING_BUFFER_SUCCESS){
				if(Lightsoutce.UIMode == 6){
					UART_RemoteDecode(getdata);
				}
				else{
					//δ����̿أ����������
				}
			}
		}
		
		/*��о*/
		if(RB_Get_Length(&rb_uart3_rx) != 0){
			unsigned char getdata = 0;
			if(RB_Read_Byte(&rb_uart3_rx, &getdata) == RING_BUFFER_SUCCESS){
					UART_RemoteDecode(getdata);
			}
		}	
		
		#ifdef LM75B
			if(flag_timer2.flag_timer2_1s)				
			{
				flag_timer2.flag_timer2_1s = myfalse;
				ddtemp = LM75BTempRead();
				retdata.temp = (unsigned char)(ddtemp + 0.4);/*�������������λ��*/
				//LCD_ShowNum1(20 , 300 , ddtemp , 4 , WHITE);
				//LCD_ShowString(60, 300 , "`C", WHITE);	
			}
		#endif
			
		/*�̿�ģʽ��DBˢ��*/
		if(display.disenergy)
		{
			display.disenergy = 0;
			Number_Highlight_Show(16 + 75  , 
														36*2 + 32 + 16 + 32 + 16 , \
														(unsigned int)display.disenergyshow, \
														display.bit_point , \
														display.decimal_point , \
														0 , WHITE , BLUE , BLACK , WHITE);				
			LCD_Print(16 + 60 + 120 , 36*2 + 32 + 16 + 32 + 16 , "dB" , WHITE , BLUE);
		}
		
		/*�̿�ģʽ��MSˢ��*/
		if(display.discycle)
		{
			display.discycle = 0;
			Number_Highlight_Show(16 + 60 , 
														36*2 + 32 + 16 + 32 + 16 + 32 + 16, \
														(uint32_t)display.discycleshow, \
														6 , \
														3 , \
														0 , WHITE , BLUE , BLACK , WHITE);									
			LCD_Print(16 + 60 + 120 , 36*2 + 32 + 16 + 32 + 16 + 32 + 16 , 	"ms" , WHITE , BLUE);
		}	
	}
}

void SetTimer6Flag(unsigned int *p)
{
	*p = 0;
}
/**
 * @brief �����弤��
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
void Laser_StateTurn_OnePulse(FunctionalState OldState)
{
	if(OldState){
		LaserControl.bits.b0 = 0;
	}
	else{
		if(b_Random_EN == false){
			CycleCode_Renew(CycleCodeTime);
		}
		else{
		}
		LaserControl.bits.b0 = 1;		
	}
	FSMC_WR(0) = (uint16_t)LaserControl.All;
}




/**
 * @brief ���ؼ���
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
void Laser_StateTurn(FunctionalState OldState)
{
	if(OldState){
		LaserControl.bits.b0 = 0;
		Uart_Tx_Buffer3[0] = 0XEA;	/*ͨ�����濪�ؼ��⺯��*/	
		Uart_Tx_Buffer3[1] = 0X01;
		Uart_Tx_Buffer3[2] = 0X02;
		Uart_Tx_Buffer3[3] = 0X03;
		Uart_Tx_Buffer3[4] = 0XED;
		DMA_Tx(USART3 , 5);
	}
	else{
		/*V2.01 �����α���ģʽ������
		����ģʽ���������ڱ��롣 Դ���룺CycleCode_Renew(CycleCodeTime);*/
		if(b_Random_EN == false){
			CycleCode_Renew(CycleCodeTime);
		}
		else{
		}
		LaserControl.bits.b0 = 1;
		Uart_Tx_Buffer3[0] = 0XEA;		/*ͨ�����濪�ؼ��⺯��*/	
		Uart_Tx_Buffer3[1] = 0X01;
		Uart_Tx_Buffer3[2] = 0X01;
		Uart_Tx_Buffer3[3] = 0X02;
		Uart_Tx_Buffer3[4] = 0XED;
		DMA_Tx(USART3 , 5);			
	}
	FSMC_WR(0) = (uint16_t)LaserControl.All;
}

/**
 * @brief       auto laser
 * @param[in]
 * @param[out]   
 * @return 
 * @note 
 */
void AutoLaser(unsigned char mode){
	union Union_Byte LaserControl_auto;
	if(mode){
		if(b_Random_EN == false) {
			CycleCode_Renew(CycleCodeTime);
		}
		LaserControl_auto.bits.b0 = 1;
		Uart_Tx_Buffer3[0] = 0X5a;		/*open*/	
		Uart_Tx_Buffer3[1] = 0Xa5;
		Uart_Tx_Buffer3[2] = 0X01;
		Uart_Tx_Buffer3[3] = 0X01;;
		DMA_Tx(USART3 , 4);
		LaAutoCon.auto_lightMode = 0;
		flag_lightN_F = true;
	}
	else{
		LaserControl_auto.bits.b0 = 0;		
		Uart_Tx_Buffer3[0] = 0X5a;	/*close*/	
		Uart_Tx_Buffer3[1] = 0Xa5;
		Uart_Tx_Buffer3[2] = 0X01;
		Uart_Tx_Buffer3[3] = 0X00;;
		DMA_Tx(USART3 , 4);
		LaAutoCon.auto_lightMode = 1;
		flag_lightN_F = false;
	}
	FSMC_WR(0) = (uint16_t)LaserControl_auto.All;
}



/**
 * @brief 
 * @param[in]
 * @param[out]   
 * @return 
 * @note 
 */
void Cross_StateTurn(FunctionalState OldState)
{
	if(OldState)
	{
		LaserControl.bits.b2 = 0;
	}
	else
	{
		LaserControl.bits.b2 = 1;
	}
	FSMC_WR(0) = (uint16_t)LaserControl.All;
}

/**
 * @brief
 * @param[in]
 * @param[out]
 * @return 
 * @note 
 */
void Pulse_StateTurn(FunctionalState OldState)
{
	if(OldState)
	{
		LaserControl.bits.b1 = 0;
	}
	else
	{
		LaserControl.bits.b1 = 1;
	}
	LaserControl.bits.pulse_time = PauseTime / 10;			//PauseTimeʱmsΪ��λ�ģ������CPLDʱ��10msΪ��λ
	FSMC_WR(0) = (uint16_t)LaserControl.All;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
	uwTimingDelay = nTime;
	while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if (uwTimingDelay != 0x00)
	{ 
		uwTimingDelay--;
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	while (1)
	{
	}
}
#endif

/**** END OF FILE ****/
