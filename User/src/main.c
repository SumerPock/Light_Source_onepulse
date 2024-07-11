/**
******************************************************************************
* 文    件：	main.c 
* 作    者：	gaoyao
* 固件版本：	V1.8.0
* 创建日期： 2020-7-11
* 简要说明： 激光模拟光源控制程序，STM32F407VGT6
* 软件版本：	V1.0
* 前一版本：	无
* 修改时间：	2020-4-23
* 版本更新说明： 
							对外同步界面响应进行了处理，主要有以下几处
									1. 更新了外同步字库
									2. 增加了bool b_Ext_Sync全局变量的声明和引用
									3. main函数的while循环中增加了b_Ext_Sync变量的更新
									4. 修改了ui.c中Menu_0_1、Menu_0_2、Menu_0_3函数的界面显示
									5. 修改了ui.c中MenuTrig_OK函数的响应，当外同步有效时前三条菜单不对OK键响应
							2022-12-26
							刘辉要求增加命令回显，但排查的时候发现代码存在巨大缺陷周期编码，激光脉冲启动，握手，三种指令回的是一样的命令
							，也许是之前的版本根本没考虑过数据回显功能，现如今将命令修改
							
							2023 3 18 
							Dong.Zhe 添加环形队列数据解构，之前的数据处理全都放到串口中断里，我实在看不下去了，但介于软件结构，添加DMA势必会造成
							其他Bug，所以加入队列
							
							2023.9.6
							增加关芯的数据，并透传，光芯的应答ID透传后由下位机从 ID + 60改为 ID + 80
							
* 软件版本：	V2.10
* 前一版本：	V2.01
* 修改时间：	2023-8-21
* 版本更新说明：在激光开启事件处，新增判断，进入伪随机模式，禁用更新编码；否则，刷新编码。

* 软件版本：	V2.11
* 前一版本：	V2.10
* 修改时间：	2024-5-6
* 作者：			shiyongfu
* 版本更新说明：
* 新增单脉冲输出模式，上位机指令0x51，应答ID 0xd1（bsp.c 行930）
* 脉冲个数类型为uint16_t 最大65535，上位机限制最大500.（bsp.c 行389）
* 开启激光指令修改为持续输出65535个脉冲。（bsp.c 行449）
* 修改LCD界面开启激光代码，新增脉冲个数下发CPLD（main.c 行274）
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

//环形队列缓冲区
unsigned char uart1_rxbuff[4096]; 
unsigned char uart1_txbuff[64]; 
unsigned char uart3_rxbuff[1024];
unsigned char uart3_txbuff[64];

//创建环形缓冲区句柄
ring_buffer rb_uart1_rx;					
ring_buffer rb_uart1_tx;
ring_buffer rb_uart3_rx;
ring_buffer rb_uart3_tx;	

unsigned short RandomNumber, RandomBaseFreq, RandomMinValue, RandomMaxValue;
unsigned int RandomCodeTime;	// 伪随机周期
// 因为STM32F407找不到2K页擦除指令，只能整个扇区擦除，因此必须为所有数据搞一个Buff
unsigned int u32_FlashBuff[2048];	

// 伪随机编码的数据数组，其中第一个数组元素存放的是伪随机位数，其余空间存放的是减去基频的数值
unsigned short int u16_RandomData[1024];	

signed int Number_Pulse = 00100;
signed int Tempnumber_Pulse = 100;

signed int CycleCodeTime = 50000;			// 周期预设50ms
signed int Temp_CycleCodeTime = 0;		// 临时的周期数值，通过面板操控更改的是这个数值，当确认后，这个数值才被更新到CycleCodeTime中
signed short int Temp_dB_Loss = 0;	//临时能量值
//signed short int dB_Loss = 600;		// 激光能量衰减预设60dB
signed short int dB_Loss = 0;				// 激光能量衰减预设0dB
extern uint32_t MOTO_COUNT;
unsigned char findzero_bigin = 0;



unsigned char flag_lightN_F = false;
//unsigned char auto_lightMode = 0;

//unsigned char Skill_cooldown = 1;

LASER_AUTO_CONTROL LaAutoCon = {0 , 0 , 0 , 1 , 0};

extern void SetTimer6Flag(unsigned int *p);

/**
 * @brief 主函数
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
	union Union_Byte KeyState, KeyState_Old, Hbyte_KeyState;	// 共用体，开关状态
	union Union_TypeConversion TypeConversion;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemInit();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);  /* SysTick end of count event each 10ms */
	Delay(5);  /* Insert 50 ms delay */
	//初始化tim函数，中断时间1ms，用来计时中断1s功能的触发信号宽度
	//TIM2_Int_Init(2-1 , 42000-1);		
	/*Tout= ((arr+1)*(psc+1))/Tclk = 2 S*/
	TIM3_Int_Init(2000-1 , 42000-1);
	/*Tout= ((arr+1)*(psc+1))/Tclk = 84*1000/42000 000 = 0.002 S*/
	TIM4_Int_Init(84-1 , 1000-1);
	/*Tout= ((arr+1)*(psc+1))/Tclk = 42000*1000/42000 000 = 1 S*/
	TIM6_Int_Init(42000 - 1 , 2000 - 1);
	
	TIM_Cmd(TIM6 , DISABLE); 
	FSMC_Init();									//初始化FSMC总线
	User_GPIO_Init();
 	delay_init(168);							//延时函数初始化	
	LCD_Init();										//针对2.4寸LCD配置

	KeyState.All = (uint8_t)FSMC_WR(8);
	KeyState_Old.All = KeyState.All;
	LaserControl.All = 0x0;

	Read_Memory(FLASH_USER_START_ADDR, 2048, u32_FlashBuff);	// 从Flash中读取所有4组伪随机数
	
	p = (uint32_t *)&u16_RandomData;
	
	for(i=0; i<=100; i++)
	{	// 把第一组伪随机数据加载到u16_RandomData数组中
		*(p+i) = u32_FlashBuff[i];
	}
	UI_Init();
	bsp_InitI2C();
	double ddtemp = 0.00;
	RB_Init(&rb_uart1_rx , uart1_rxbuff , BUFFER_SIZE);	//初始化环形缓冲区参数
	RB_Init(&rb_uart1_tx , uart1_txbuff , BUFFER_SIZE);		
	RB_Init(&rb_uart3_rx , uart3_rxbuff , BUFFER_SIZE);		
	RB_Init(&rb_uart3_tx , uart3_txbuff , BUFFER_SIZE);
	Uart_Init(USART1 , 115200 , GPIOA , GPIO_PinSource9 , GPIOA , GPIO_PinSource10 , 0 , 8 , 0);
	Uart_Init(USART3 , 115200 , GPIOB , GPIO_PinSource10 , GPIOB , GPIO_PinSource11 , 0 , 4 , 0);
	
	#if (VERSION_SELECTION == Ver_2_3_0)
	User_Motor_GPIO_Init();/*电机GPIO配置 begin*/
	GPIO_SetBits(EN5);
	GPIO_SetBits(EN6);
	Motor_Power_Off;
	EXIT_Init(Zero_Position, EXTI_Trigger_Falling);
	
	#ifdef	DEBUGPR
		/* 配置通道0，上行配置*/
		SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
		/* 配置通道0，下行配置*/	
		SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
		SEGGER_RTT_SetTerminal(0);
		SEGGER_RTT_printf(0,RTT_CTRL_TEXT_BRIGHT_GREEN "SEGGER_RTT is begin \r\n");	
	#endif
	
	/*光芯指令发送 5A A5 02 00*/
	/*设置光源外时通道*/
	unsigned char uc_sbuffer[] = {0x5a , 0xa5 , 0x02 ,0x00};
	memset(Uart_Tx_Buffer3 , 0 , sizeof(Uart_Tx_Buffer3));
	memcpy(Uart_Tx_Buffer3 , uc_sbuffer , sizeof(uc_sbuffer));
	DMA_Tx(USART3 , sizeof(uc_sbuffer));		/*电机GPIO配置 end*/	
	/* 1 / 14.7456 = 0.0678168402777 , 3687 * 0.0678168402777 = 250.04069*/
	//FSMC_WR(4) = 3687;//20kw激光器默认为250us脉宽
	/* 250.5 / 0.0678168402777 = 3693.77280000*/
	//FSMC_WR(4) = pulse_width_formula(5);	// 1553激光器默认脉宽为						
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
		{	//只有当键值变化时
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
			//LCD界面开关激光			
			if((KeyState.bits.b0 == 0) && (KeyState_Old.bits.b0 == 1)) 
			{
				if(Lightsoutce.UIMode == 3)
				{
						/*脉冲模式发送*/
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
					/*一直发送*/
					Laser_StateTurn(LaserState.bits.b0);
				 /*2024.05.06修订，由于CPLD新增单脉冲模式，输出激光必须下发脉冲个数才能输出*/
					FSMC_WR(5) = 65535;					
				}

			}				
			if(b_Random_EN)// 如果伪随机编码使能了，则动态更新编码值
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
					Temp_u32 = u16_RandomData[6];														// 读出基频
					Temp_u32 = Temp_u32 + u16_RandomData[8 + Random_Index];	// 计算编码
					CycleCode_Renew(Temp_u32);	// 将编码写入CPLD		
				}
			}			
			KeyState_Old.All = KeyState.All;		
		}

		KeyState.All = (uint8_t)(FSMC_WR(8) & 0xFF);
		if((LaserState.bits.b0 != Hbyte_KeyState.bits.b0) ||(LaserState.bits.b1 != Hbyte_KeyState.bits.b1))
		{
			LaserState.bits.b0 = Hbyte_KeyState.bits.b0;	// 将激光器返回的状态赋给系统变量
			LaserState.bits.b1 = Hbyte_KeyState.bits.b1;	// 将激光器返回的状态赋给系统变量							
			if(Hbyte_KeyState.bits.b1 == 0){	
				LED2_OFF;// 红光输出状态
			}else{
				LED2_ON;
			}				
		}
		
		/*上位机*/
		if(RB_Get_Length(&rb_uart1_rx) != 0){
			unsigned char getdata = 0;
			if(RB_Read_Byte(&rb_uart1_rx, &getdata) == RING_BUFFER_SUCCESS){
				if(Lightsoutce.UIMode == 6){
					UART_RemoteDecode(getdata);
				}
				else{
					//未进入程控，报错误代码
				}
			}
		}
		
		/*光芯*/
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
				retdata.temp = (unsigned char)(ddtemp + 0.4);/*四舍五入后传入上位机*/
				//LCD_ShowNum1(20 , 300 , ddtemp , 4 , WHITE);
				//LCD_ShowString(60, 300 , "`C", WHITE);	
			}
		#endif
			
		/*程控模式下DB刷新*/
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
		
		/*程控模式下MS刷新*/
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
 * @brief 单脉冲激光
 *
 * 这里可以提供关于函数的高级概述，包括它的输入和输出。
 *
 * @param[in]  参数1  参数1的描述，包括数据类型和含义
 * @param[out] 参数2  参数2的描述，包括数据类型和含义
 *
 * @return 函数返回值的描述，包括数据类型和含义，如果适用的话。
 *
 * @note 任何特殊注意事项或用法示例都可以在这里添加。
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
 * @brief 开关激光
 *
 * 这里可以提供关于函数的高级概述，包括它的输入和输出。
 *
 * @param[in]  参数1  参数1的描述，包括数据类型和含义
 * @param[out] 参数2  参数2的描述，包括数据类型和含义
 *
 * @return 函数返回值的描述，包括数据类型和含义，如果适用的话。
 *
 * @note 任何特殊注意事项或用法示例都可以在这里添加。
 */
void Laser_StateTurn(FunctionalState OldState)
{
	if(OldState){
		LaserControl.bits.b0 = 0;
		Uart_Tx_Buffer3[0] = 0XEA;	/*通过界面开关激光函数*/	
		Uart_Tx_Buffer3[1] = 0X01;
		Uart_Tx_Buffer3[2] = 0X02;
		Uart_Tx_Buffer3[3] = 0X03;
		Uart_Tx_Buffer3[4] = 0XED;
		DMA_Tx(USART3 , 5);
	}
	else{
		/*V2.01 如果是伪随机模式，禁用
		正常模式，更新周期编码。 源代码：CycleCode_Renew(CycleCodeTime);*/
		if(b_Random_EN == false){
			CycleCode_Renew(CycleCodeTime);
		}
		else{
		}
		LaserControl.bits.b0 = 1;
		Uart_Tx_Buffer3[0] = 0XEA;		/*通过界面开关激光函数*/	
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
	LaserControl.bits.pulse_time = PauseTime / 10;			//PauseTime时ms为单位的，输出给CPLD时用10ms为单位
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
