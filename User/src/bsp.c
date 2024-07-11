#include "bsp.h"

extern uint32_t MOTO_COUNT;

extern unsigned int Sys_Count;
LIGHTSOURCE Lightsoutce = {0};

RETDATA retdata = {0 , 0 , {0} , {0}};

DISPLAY display = {0 , 0};

union Union_TypeConversion Byte_TypeConversion;

unsigned short PauseTime = 1000;	// �ж�ʱ��Ԥ��1s

bool b_Random_EN = false;	// α�������ʹ��

union Union_Byte LaserControl;	// �����壬����������λ

/*�˵�����ֵ*/
struct Menu_Index Menu_Indexs = {0, 0, 0};
/**/
/*������ƽṹ��*/
MOTODATA Motodata = {0 , 0 , 0};

DataPoint podata[] = {
	{0,  					  250},
	{0.02891452  ,  240},
	{0.16146636  ,  230},
	{0.442551271 ,  220},
	{0.601794173 ,  210},
	{0.719223551 ,  200},
	{0.938945656 ,  195},
	{1.036625334 ,  190},
	{1.347509743 ,  185},
	{1.367205672 ,  180},
	{1.669658915 ,  175},
	{1.926867782 ,  170},
	{2.312190572 , 	165},
	{2.661570632 , 	160},
	{3.012606047 , 	155},
	{3.410933038 , 	150},
	{3.882506223 , 	145},
	{4.236120229 , 	140},
	{4.610255274 , 	135},
	{5.200064168 , 	130},
	{5.452512122 , 	127},
	{5.970743035 , 	125},
	{6.142763337 , 	123},
	{6.703922756 , 	120},
	{6.73370616  , 	117},
	{6.861915397 , 	115},
	{6.919662555 , 	113},
	{7.520902366 , 	110},
	{8.646678819 , 	107},
	{9.494562942 , 	105},
	{10.2889414  , 	103},
	{11.05128508 , 100},
	{11.8901956  , 97},
	{11.35144179 , 95},
	{11.75515272 , 93},
	{14.09771194 , 90},
	{16.98250139 , 87},
	{19.83876504 , 85},
	{20.17044867 , 83},
	{20.85392608 , 80},
};

unsigned char podatanum = sizeof(podata) / sizeof(podata[0]);

/**
 * @brief
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note �������ղ�ֵ
 */
double lagrangeInterpolate(DataPoint* data, int n, double x) {
    double result = 0.0;
    for (int i = 0; i < n; i++) 
		{
        double term = data[i].y;
        for (int j = 0; j < n; j++) 
				{
            if (j != i) 
						{
                term = term * (x - data[j].x) / (data[i].x - data[j].x);
            }
        }
        result += term;
    }
    return result;
}


/**
 * @brief
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note �ָ��
 */
float splitNumber(signed short int input) {
    // ��������Ƿ����99
    if (input > 99) {
        // ֻ����ʮλ�͸�λ
        int remainder = input % 100;
        // ת��Ϊ������������С�������λ
        float result = remainder / 10.0; // �޸�������� 10.0 ������ 100.0
        result += (result / 0.01) * 0.0001;
        return result;
    }
    else {
        // ����С�ڵ���99��ֱ�ӷ���
        return input;
    }
}


/**
 * @brief
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note ������㹫ʽ
 */
unsigned int pulse_width_formula(unsigned int pw_xus)
{
	unsigned int uireturn = 0;
	double dosc = 0.067816840277;
	if(pw_xus > 0 && pw_xus < 2000)
	{
			uireturn = pw_xus / dosc;
	}
	else
	{
		uireturn = 0;
		return uireturn;
	}
	return uireturn;
}

//����˥�����������������FPGA���������ں�,����
signed short int Set_Energy(uint16_t show)
{
	signed short int ssiretrurn = 0;
	if(show < GEAR_7)
	{
		if(show < GEAR_2)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_0);
			#endif
			ssiretrurn = show;		
			return ssiretrurn;
		}
		else if(show >= GEAR_2 && show < GEAR_3)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_10);
			#endif
			ssiretrurn = show - GEAR_2;
			return ssiretrurn;
		}
		else if(show >= GEAR_3 && show < GEAR_4)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_20);
			#endif
			ssiretrurn = show - GEAR_3;
			return ssiretrurn;
		}
		else if(show >= GEAR_4 && show < GEAR_5)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_30);
			#endif
			ssiretrurn = show - GEAR_4;
			return ssiretrurn;
		}
		else if(show >= GEAR_5 && show < GEAR_6)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_40);
			#endif
			ssiretrurn = show - GEAR_5;
			return ssiretrurn;		
		}
		else if(show >= GEAR_6 && show < GEAR_7)
		{
			#ifdef SEDMOTOR
				Motor_Move_To(DB_50);
			#endif
			ssiretrurn = show - GEAR_6;
			return ssiretrurn;	
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//20kj���֧��20db��̬��Χ
		return 0;
	}
}

/**
 * @brief
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note
 */
void Set_DB(uint16_t DB_Data)
{
	switch (DB_Data)
	{
		case 0:
			Motor_Move_To(DB_0);
		break;
	
		case 10:
			Motor_Move_To(DB_10);
		break;
		
		case 20:
			Motor_Move_To(DB_20);
		break;
		
		case 30:
			Motor_Move_To(DB_30);
		break;

		case 40:
			Motor_Move_To(DB_40);
		break;

		case 50:
			Motor_Move_To(DB_50);
		break;

		case 60:
			Motor_Move_To(DB_60);
		break;		
		
		default:
		break;
	}
}

/**
 * @brief				
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note			Motor_Move_To
 */
void Motor_Move_To(unsigned short position)
{
		unsigned short data_1 = 586 * 2;	
//	unsigned short data_1 = 1026;
//	unsigned short data_1 = 1172;
	
	//unsigned short data_1 = 1050;
	
	if (Motodata.Motor_Current_Positon == position) //δ�ı�
		return;
	if (Motodata.Motor_End_count)			
		return;
	if (position >= 12)
		return;
	Motodata.Motor_Next_Positon = position; //��ֵ
	//0 -> any
	if(Motodata.Motor_Next_Positon != 0 && Motodata.Motor_Current_Positon == 0)
	{
		#ifdef DEBUGPR
			SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_YELLOW " now = %d " RTT_CTRL_TEXT_BRIGHT_YELLOW" old = %d\r\n" ,position, Motodata.Motor_Current_Positon);		
		#endif
		Motodata.Motor_End_count = MOTO_COUNT + (position) * data_1; // + 400
		Dir_Up;		
	}
	/*any -> any */
	else if(Motodata.Motor_Next_Positon != 0 && Motodata.Motor_Current_Positon != 0)
	{
		if (Motodata.Motor_Current_Positon > position)		//��ǰλ�ÿ�ǰ�ˣ���Ҫ������
		{
			#ifdef DEBUGPR
				SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_YELLOW " now = %d " RTT_CTRL_TEXT_BRIGHT_YELLOW" old = %d\r\n" ,position, Motodata.Motor_Current_Positon);	
			#endif
			Motodata.Motor_End_count = MOTO_COUNT + abs(Motodata.Motor_Current_Positon - position) * data_1;
			Dir_Down;
		}
		else 
		{
			#ifdef DEBUGPR
				SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_YELLOW " now = %d " RTT_CTRL_TEXT_BRIGHT_YELLOW" old = %d\r\n" ,position, Motodata.Motor_Current_Positon);
			#endif			
			Motodata.Motor_End_count = MOTO_COUNT + abs(position - Motodata.Motor_Current_Positon) * data_1;
			Dir_Up;
		}		
	}
	/*any -> 0*/	
	else if(Motodata.Motor_Next_Positon == 0 && Motodata.Motor_Current_Positon != 0)
	{
		#ifdef DEBUGPR
			SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_RED "x to zero \r\n");	
			SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_YELLOW " now = %d " RTT_CTRL_TEXT_BRIGHT_YELLOW" old = %d\r\n" ,position, Motodata.Motor_Current_Positon);
		#endif		
		Motodata.Motor_End_count = MOTO_COUNT + abs(position - Motodata.Motor_Current_Positon) * data_1; //+ 400
		Dir_Down;		
	}
	else
	{
		#ifdef DEBUGPR
			SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_RED "does not exist \r\n");	
		#endif			
	}

	Motodata.Motor_Current_Positon = position;
	Motor_Power_On;
}


/**
 * @brief				
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note	�ⲿ��紫������⵽λ�õ�����λ
 */
void Position_In_Zero(void)
{
	Motor_Power_Off;
}


/**
 * @brief				
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note	�������λ
 */
void Motor_Return_Zero(void)
{
	//��ס�����£�������������0��
	//û��ס����1�����Թ������½���
	if(IO_Read(Zero_Position) == 1)		
	{
		Dir_Down;						//���÷���λ���㷽��
		Motor_Power_On;			//ʹ���˶�
	}
}


uint8_t IO_Read(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x)
{
  return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x);
}

extern DataPoint podata[];

/**
 * @brief ����ָ��Ӧ��
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
unsigned char UART_RemoteCMD_Process(void)
{
	unsigned char Temp_u8 = 0, CheckSum = 0;
	unsigned short i = 0 , Temp_int16 = 0;
	unsigned int *p_u32 = 0;
	static unsigned short  	RandomIndex = 0;
	static unsigned char 		RandomRxError = 0;	// α����������ʱ�ĳ��������
	uint8_t temp_energy_int = 0;
	
	/*�����������*/
	unsigned char uc_sbuffer[] = {0x5a , 0xa5 , 0x03 ,0x00};
	/*���� �������������� Ĭ��Ϊ1 */ 
	uint16_t PulseNumber = 1;
	
	if(RemoteDataPacket.RX_BUF[0] > 2)
	{	
		for(i = 1 ; i < RemoteDataPacket.RX_BUF[0] ; i++)
		{
			CheckSum = CheckSum + RemoteDataPacket.RX_BUF[i];
		}
		if(CheckSum == RemoteDataPacket.RX_BUF[RemoteDataPacket.RX_BUF[0]])	// ������ʾУ�����ȷ
		{
			/*��λ���·����ݹ�о*/
			if((RemoteDataPacket.RX_BUF[2] >= 0x43) & (RemoteDataPacket.RX_BUF[2] <= 0x59))
			{
				memcpy(Uart_Tx_Buffer3, RemoteDataPacket.RX_BUF, Number_Rx_Buffer1);
				Uart_Tx_Buffer3[0] = 0XEA;
				Uart_Tx_Buffer3[Number_Rx_Buffer1 - 1] = 0XED;
				DMA_Tx(USART3 , Number_Rx_Buffer1);
			}
			/*��о���ݱ�����λ��*/
			else if((RemoteDataPacket.RX_BUF[2] >= 0xA3) & (RemoteDataPacket.RX_BUF[2] <= 0xB0))
			{
				#if (VERSION_SELECTION == Ver_2_1_0)
					//У�����
					RemoteDataPacket.RX_BUF[2] += 0x20;	//2023��9��6���޸� ��о��Ӧ��ID͸��������λ���� ID + 60��Ϊ ID + 80
					//����У��,2023��9��6���޸� ��о��Ӧ��ID͸��������λ���� ID + 60��Ϊ ID + 80
					RemoteDataPacket.RX_BUF[Number_Rx_Buffer3 - 2] =  RemoteDataPacket.RX_BUF[Number_Rx_Buffer3 - 2] + 0x20;						
				#elif (VERSION_SELECTION == Ver_2_1_1)
					
				#elif (VERSION_SELECTION == Ver_2_2_0)
				
				#elif (VERSION_SELECTION == Ver_2_3_0)
				
				#endif
				memcpy(Uart_Tx_Buffer1, RemoteDataPacket.RX_BUF, Number_Rx_Buffer3);
				Uart_Tx_Buffer1[0] = 0XEA;
				Uart_Tx_Buffer1[Number_Rx_Buffer3 - 1] = 0XED;				
				DMA_Tx(USART1 , Number_Rx_Buffer3);
			}
			
			switch(RemoteDataPacket.RX_BUF[2])
			{
				/*�̿�ģʽͨѶ����*/
				case 0x00:	
				if(RemoteDataPacket.RX_BUF[0] == 3)//���ȼ��
				{
					retdata.Mode = 0x80;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;
				
				/*��λ���˿� ������������
				*��Ϊ������ȷ����Ҫ�����⣬�����Ǵ���λ���·���ָ�ʵ�尴����������仯��
				*�������ﲻ��ʹ��ϵͳ�������е��ã���Ϊû��ʵ�尴������������Ҳ���ᷢ���仯�ģ��������Ҫ��һ��ȷ����ֵ	
				*/
				case 0x01:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x81;
					UART_Answer_LightData(USART1 , &retdata);	
					Laser_StateTurn(DISABLE);	
					/*2024.05.06���� ����������� Ĭ��65535*/
					PulseNumber = 65535;
					FSMC_WR(5) = PulseNumber;
					RemoteDataPacket.RX_BUF[1]++;
				}						
				break;
						
				/*��λ���˿� ��������ر�*/	
				case 0x2:				
				if(RemoteDataPacket.RX_BUF[0] == 3)	
				{	
					retdata.Mode = 0x82;
					UART_Answer_LightData(USART1 , &retdata);
					Laser_StateTurn(ENABLE);
				}
				break;
				
				/*��ɫʮ�ֹ����*/
				case 0x3:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x83;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(DISABLE);
				}
				break;
					
				/*��ɫʮ�ֹ�ر�*/
				case 0x4:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x84;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(ENABLE);
				}
				break;
				
				/*����ģ�ⰴ���Ϸ�����*/
				case 0x6:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Up();
				}
				break;
				
				/*����ģ�ⰴ���·�����*/
				case 0x7:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Down();
				}
				break;
				
				/*����ģ�ⰴ��OK����*/
				case 0x8:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_OK();
				}
				break;
				
				/*����ģ�ⰴ��Cancel����*/
				case 0x9:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Cancel();
				}
				break;
				
				/*�ж�1��*/						
				case 0x10:	
				if(RemoteDataPacket.RX_BUF[0] == 5)
				{// 
					Byte_TypeConversion.TC_uint64_t = 0;
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					PauseTime = Byte_TypeConversion.TC_uint16_t;
					Pulse_StateTurn(DISABLE);		//�ж�1s��ʱ��������
					TIM_Cmd(TIM2 , ENABLE); 		//ʹ�ܶ�ʱ��2,����1ms�Ķ̴�������,����ʹ�ö�ʱ����Ҫ��Ϊ���Ժ����������ʱ�ȽϷ��㣬
																			//��Ϊֻ����һ���жϣ�Ҳ����̫ռ����Դ						
					retdata.Mode = 0x90;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;	
					
				/*�̶���������װ��*/
				case 0x11:	
				if(RemoteDataPacket.RX_BUF[0] == 6)	// ���Ƚ�����������ȼ��
				{		
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[2] = RemoteDataPacket.RX_BUF[3];						
					CycleCodeTime = Byte_TypeConversion.TC_uint32_t;
					if(CycleCodeTime <= 120000 && CycleCodeTime >= 50000)
					{
//						if(CycleCodeTime > 120000) 
//						{
//						CycleCodeTime = 120000;
//						}else if(CycleCodeTime < 50000)
//						{ 
//						//2024.3.26	,	�������������趨
//						CycleCodeTime = 50000;
//						}
						
						if(b_Random_EN)	
						{//�����ǰ����α���ģʽ�����˳�α���ģʽ
							b_Random_EN = false;	
						}	
						if(LaserControl.bits.b0 == 1)		
						{//���֮ǰ�Ѿ��ǿ�����״̬�������¸���һ�ο���ָ�
							//���֮ǰ�ǹؼ���״̬����ʲôҲ���䣬ֻ�������ݣ�����һ���ֶ��򿪼��⼴��
							Laser_StateTurn(DISABLE);			//�򿪼���
						}		
						unsigned int my_ms = 0;			/*ӦС��Ҫ�����������ֵ��ʾ*/
						my_ms = RemoteDataPacket.RX_BUF[3] << 16 | RemoteDataPacket.RX_BUF[4] << 8 | RemoteDataPacket.RX_BUF[5];
						//FSMC_WR(4) = (uint16_t)((double)(my_ms * 14.7456 * 2));		/*Ӧ�ϰ�Ҫ����λ����ms�����Ϊ��cpld������������*/
						retdata.cycle[0] =  RemoteDataPacket.RX_BUF[3];
						retdata.cycle[1] =  RemoteDataPacket.RX_BUF[4];
						retdata.cycle[2] =  RemoteDataPacket.RX_BUF[5];
						if(cn.Program_control_mode_2 == 0xff)/*����ֵ����ֵ����������*/
						{
							/*�˴�������ʾ���ߴ����� �����ݴ���99.999ʱΪ100.000ʱ��7λʱ���ڵ�ms��λ����ȥ��ms��λ*/
							if(my_ms > 0x1869F)
							{
								display.discycle = 1;
								display.discycleshow = my_ms;
							}
							else
							{			
								display.discycle = 1;
								display.discycleshow = my_ms;						
							}
						}
						retdata.Mode = 0x91;
						UART_Answer_LightData(USART1 , &retdata);
					}

				}
				break;

				/*α��������������*/
				case 0x20:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					#if (VERSION_SELECTION != Ver_2_3_0)
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					u16_RandomData[7] = Byte_TypeConversion.TC_uint16_t;					// ����λ��
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					u16_RandomData[6] = Byte_TypeConversion.TC_uint16_t;					// ��Ƶ
					RandomIndex = 0;
					RandomRxError = 0;
					if(u16_RandomData[7] > 0 && u16_RandomData[7] < 1017)	
					{		//������Ч��Χ1~1016
						UART_Answer_RandomNumber(USART1, u16_RandomData[7]);
					}
					else if(u16_RandomData[7] >= 1017 || u16_RandomData[7] == 0)
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);		//�ϱ�0xAE������
					}
					#endif	
				}
				break;

				/*α�����������*/
				case 0x21:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					#if (VERSION_SELECTION != Ver_2_3_0)
					Byte_TypeConversion.TC_uint64_t = 0;
					Temp_u8 = RemoteDataPacket.RX_BUF[3];
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = Temp_u8 & 0x0F;			// ȡ��4λ������������ϰ�д�ģ���Ҳ��֪��Ϊʲô��Ҫ��ôд��
					Temp_int16 = Byte_TypeConversion.TC_uint16_t;					// ��ȡα����������			
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];		
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[2] = (Temp_u8 >> 4) & 0x0F;	// ȡ��4λ��	
					u16_RandomData[Temp_int16 + 7] = (uint16_t)(Byte_TypeConversion.TC_uint32_t - u16_RandomData[6]);	// ��α������������Ӧ��ŵ�������
					
					// Ϊ��ʡ�ռ䣬���ݰ���16Bit�洢������С����Ϊ��Ƶ�����ݼ���Ƶ�洢
					if((Temp_int16 != (RandomIndex + 1)) || (Byte_TypeConversion.TC_uint32_t > 120000) ||(Byte_TypeConversion.TC_uint32_t < 40000))
					{
						RandomRxError++;
						UART_AnswerError(USART1, RandomIndex);
					}
					RandomIndex = Temp_int16;
					//Ϊ����ٶȣ����ݲ��ֲ�Ӧ�𣬵�ȫ���������Ӧ��
					//UART_Answer_RandomData(USART1, Temp_int16, Int32_RandomData[Temp_int16]); 
					#endif					
				}
				break;
					
				/*α������뷢����ϼ��*/
				case 0x22:
				if(RandomRxError == 0)	// =0˵�����յ���������Ŷ���
				{//α����������з�����Ϻ�ļ��
					#if (VERSION_SELECTION != Ver_2_3_0)
					if(RandomCode_Check())
					{				
						retdata.Mode = 0xA2;
						UART_Answer_LightData(USART1 , &retdata);				
						Laser_StateTurn(ENABLE);	//�رռ����������ָ����λ�����·�һ�Σ�Ϊ�������������Ҳ��һ�δ���
						//��������whileѭ��������b_Random_EN״̬�Զ�����α���״̬
						b_Random_EN = true;		//����α���ģʽ
					}
					else
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);	//�ϱ��������				
					}
					#endif
				}
				else
				{
					UART_AnswerError(USART1, RandomRxError);
				}
				break;

				/*α�������洢*/
				case 0x23:	
				if(RemoteDataPacket.RX_BUF[0] == 11)
				{
					#if (VERSION_SELECTION != Ver_2_3_0)
					uint8_t SaveCH = RemoteDataPacket.RX_BUF[3];			
					/*2������Ϊ�人�ߵº��⣬4���趨Ϊͨ�ð汾*/
					if((SaveCH > 0) && (SaveCH < 5))		//V3.01 ��α������ȣ�ԭ���룺if((SaveCH >0) && (SaveCH <5))
					{
						Byte_TypeConversion.TC_uint64_t = 0;						
						Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
						Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
						Byte_TypeConversion.TC_byte[2] = RemoteDataPacket.RX_BUF[6];
						Byte_TypeConversion.TC_byte[3] = RemoteDataPacket.RX_BUF[7];
						Byte_TypeConversion.TC_byte[4] = RemoteDataPacket.RX_BUF[8];
						Byte_TypeConversion.TC_byte[5] = RemoteDataPacket.RX_BUF[9];
						Byte_TypeConversion.TC_byte[6] = RemoteDataPacket.RX_BUF[10];
						for(i = 0 ; i < 4 ; i++)
						{
							u16_RandomData[i] = Byte_TypeConversion.TC_u16[i];
						}
						p_u32 = (uint32_t *)&u16_RandomData;						
						for(i = 0; i < 512; i++)
						{
							u32_FlashBuff[512*(SaveCH-1)+i] = *(p_u32+i);
						}												
						STMFLASH_Write(FLASH_USER_START_ADDR, u32_FlashBuff, 2048*4);							
						//UART_Answer_NoData(USART1);
						retdata.Mode = 0xA3;  
						UART_Answer_LightData(USART1 , &retdata);	   
						//V3.01 ��α������ȣ��洢Ӧ��ԭ���룺UART_Answer_NoData(USART1);
					}
					#endif
				}
				break;					
				
				/*α�������ɾ��*/
				case 0x24:	
				if(RemoteDataPacket.RX_BUF[0] == 4)
				{
					#if (VERSION_SELECTION != Ver_2_3_0)
					uint8_t SaveCH = RemoteDataPacket.RX_BUF[3];					
					if((SaveCH >0) && (SaveCH <5))
					{
						for(i=0; i<8; i++)
						{
							u16_RandomData[i] = 0x0;
						}
						p_u32 = (uint32_t *)&u16_RandomData;						
						for(i=0; i<512; i++)
						{
							u32_FlashBuff[512*(SaveCH-1)+i] = *(p_u32+i);
						}													
						STMFLASH_Write(FLASH_USER_START_ADDR, u32_FlashBuff, 2048*4);						
						//UART_Answer_NoData(USART1);
						retdata.Mode = 0xA4;  
						UART_Answer_LightData(USART1 , &retdata); //V3.01 ��α������ȣ�ɾ��Ӧ��ԭ���룺UART_Answer_NoData(USART1);
					}
					#endif
				}
				break;
					
				/*��������*/
				case 0x41:				
				if(RemoteDataPacket.RX_BUF[0] == 7 && flag_lightN_F == false)
				{
					RemoteDataPacket.TX_BUF[0] = 0;
					for(int i = 1 ; i < 7 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[i]);	
					}
					
					#if (VERSION_SELECTION == Ver_2_1_0)
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);	
					#elif (VERSION_SELECTION == Ver_2_1_1)
						static unsigned short temp = 0;			/*2023 9 7 �� Ҫ�����ӹ̶�DB 2000*/
						static unsigned char SendRemotBuff[16] = {0};
						memset(SendRemotBuff , 0 , sizeof(SendRemotBuff));
						memcpy(SendRemotBuff , RemoteDataPacket.TX_BUF , 16);
						temp = RemoteDataPacket.TX_BUF[5] << 8 | RemoteDataPacket.TX_BUF[6];
						temp += 2000;
						SendRemotBuff[6]= temp;					//�ȸߺ��
						SendRemotBuff[5]= temp >> 8;				
						UART_RemoteEncode(USART3, SendRemotBuff);
					#elif (VERSION_SELECTION == Ver_2_2_0)
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
						RemoteDataPacket.RX_BUF[1]++;
						RemoteDataPacket.TX_BUF[0] = 0;	
						retdata.Mode = 0xc1;

						retdata.energy[0] =  RemoteDataPacket.RX_BUF[5];
						retdata.energy[1] =  RemoteDataPacket.RX_BUF[6];
						/*����ʱ���֣���һ�����ǲ�����λ���ظ�˥��ֵ������ǿ�Ƹ���һ��*/
						retdata.energy[0] = RemoteDataPacket.TX_BUF[5];
						retdata.energy[1] = RemoteDataPacket.TX_BUF[6];

						UART_Answer_LightData(USART1 , &retdata);	

						attenuation_number(&cn , &RemoteDataPacket);		/*2022 8 3 ӦС��Ҫ�����˥����ֵ��ʾ*/			
						unsigned short show = 0;				
						unsigned char bit_point = 0;				//��ʾ��λ��					
						unsigned char decimal_point = 0;		//��ʾ��С����λ��					
						show = cn.control_number_hight | cn.control_number_low << 8;
						if(show >= 0x3E8 )									/*��ֵ������λ��*/
						{
							bit_point = 3;
							decimal_point = 1;
							show = show / 10;	
						}
						else
						{
							bit_point = 3;
							decimal_point = 1;
							show = show / 10;	
						}	
						//��Ҫ����С����ʾ
						if(cn.Program_control_mode_1 == 0xff)
						{
							display.disenergy = 1;
							display.bit_point = bit_point;
							display.decimal_point = decimal_point;
							display.disenergyshow = show;
						}						
					#elif (VERSION_SELECTION == Ver_2_3_0) //��λ��������������
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
						RemoteDataPacket.RX_BUF[1]++;
						RemoteDataPacket.TX_BUF[0] = 0;	
						retdata.Mode = 0xc1;

						retdata.energy[0] =  RemoteDataPacket.RX_BUF[5];
						retdata.energy[1] =  RemoteDataPacket.RX_BUF[6];
						/*����ʱ���֣���һ�����ǲ�����λ���ظ�˥��ֵ������ǿ�Ƹ���һ��*/
						retdata.energy[0] = RemoteDataPacket.TX_BUF[5];
						retdata.energy[1] = RemoteDataPacket.TX_BUF[6];
						
						//UART_Answer_LightData(USART1 , &retdata);	
						
						attenuation_number(&cn , &RemoteDataPacket);		/*2022 8 3 ӦС��Ҫ�����˥����ֵ��ʾ*/			
						unsigned short show = 0;				
						unsigned char bit_point = 0;				//��ʾ��λ��					
						unsigned char decimal_point = 0;		//��ʾ��С����λ��					
						show = cn.control_number_hight | cn.control_number_low << 8;
							
						TIM_Cmd(TIM4 , ENABLE);
						static unsigned short getdata = 0;
						getdata	= (show / 10); //�����1553�汾�޸�
						
						#ifdef DEBUGPR
							SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_GREEN "getdata = %d\r\n" , getdata);
						#endif						
						signed short int temp_dB_Loss = Set_Energy(getdata);		/*��е����*/
						
						#ifdef SETPW
						#endif
						
						if(getdata < GEAR_2)
						{
							show = GEAR_1;
							if(getdata == GEAR_1)
								UART_Answer_LightData(USART1 , &retdata);
						}
						else if(getdata >= GEAR_2 && getdata < GEAR_3)
						{
							show = GEAR_2 * 10;
								if(getdata == GEAR_2)
									UART_Answer_LightData(USART1 , &retdata);
						}
						else if(getdata >= GEAR_3 && getdata < GEAR_4)
						{
							show = GEAR_3 * 10;
								if(getdata == GEAR_3)
									UART_Answer_LightData(USART1 , &retdata);
						}
						else if(getdata >= GEAR_4 && getdata < GEAR_5)
						{
							show = GEAR_4 * 10;
							if(getdata == GEAR_4)
								UART_Answer_LightData(USART1 , &retdata);
						}
						else if(getdata >= GEAR_5 && getdata < GEAR_6)
						{	
							show = GEAR_5 * 10;
								if(getdata == GEAR_5)
									UART_Answer_LightData(USART1 , &retdata);
						}
						else if(getdata >= GEAR_6 && getdata < GEAR_7)
						{
							show = GEAR_6 * 10;
								if(getdata == GEAR_6)
									UART_Answer_LightData(USART1 , &retdata);
						}
						else
						{
							return 0;
						}
						
						if(show >= 0x3E8 )		/*��ֵ������λ��*/
						{
							bit_point = 3;
							decimal_point = 1;
							show = show / 10;	
						}else{
							bit_point = 3;
							decimal_point = 1;
							show = show / 10;	
						}	
						//��Ҫ����С����ʾ
						if(cn.Program_control_mode_1 == 0xff)
						{
							display.disenergy = 1;
							display.bit_point = bit_point;
							display.decimal_point = decimal_point;
							display.disenergyshow = show;
						}						
					#endif
				}				
				break;
					
				/*���������趨*/
				case 0x42:	
				if(RemoteDataPacket.RX_BUF[0] == 4)
				{
					#if (VERSION_SELECTION != Ver_2_3_0)
					RemoteDataPacket.TX_BUF[0] = 0;	
					for(int i = 1 ; i < 4 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[i]);	
					}
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
						RemoteDataPacket.RX_BUF[1]++;
						RemoteDataPacket.TX_BUF[0] = 0;
					#endif
				}
				break;
				
				/*��������趨������ֻ�����20kw��100kw�������*/
				case 0x43:
					#if (VERSION_SELECTION == Ver_2_3_0)
					/*��оָ��� 5A A5 02 ����*/
					uc_sbuffer[3] = RemoteDataPacket.RX_BUF[3];
					memset(Uart_Tx_Buffer3, 0, sizeof(Uart_Tx_Buffer3));
					memcpy(Uart_Tx_Buffer3, uc_sbuffer, sizeof(uc_sbuffer));
					DMA_Tx(USART3 , sizeof(uc_sbuffer));
					#endif
				break;
				
				/*2024.05.06��������  �������������� */
				case 0x51:
					/*������λ������*/
					PulseNumber = RemoteDataPacket.RX_BUF[4];
					if(PulseNumber == 0)
					{
						/* �رռ��� */
						Laser_StateTurn(ENABLE);
						FSMC_WR(5) = 0;
					}
					else
					{
					/* �������� */
					Laser_StateTurn(DISABLE);
					/* ����������д��CPLD*/
					FSMC_WR(5) = PulseNumber;
					}
					/* ָ��Ӧ�� */
					retdata.Mode = 0xd1;
					UART_Answer_LightData(USART1 , &retdata);
				break;
					
				default:
				break;
				
			}
			return 0;
		}
		else
		{
			return 0x1;	//�˳�������У��ʹ���
		}
	}
	else
	{
		return 0x2;	//�˳����������ݳ��Ȳ���
	}
}




/**
 * @brief ���ڵ��ֽ�ָ��ͣ�ʺɽ���룬�����Լ����鷳�ͱ���
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
void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd)
{	
	unsigned char cmode = rd->Mode;
	unsigned char ctmmp = rd->temp;
	unsigned char ccyle[5] 		= {rd->cycle[0] , rd->cycle[1] , rd->cycle[2]};
	unsigned char cenergy[5] 	= {rd->energy[0] , rd->energy[1] , rd->energy[2]};
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[1]);		/*���ض�Ӧ�İ����*/

	switch(cmode)
	{
		case 0x80://����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;
		
		case 0x81://���⿪
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;

		case 0x82://�����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x83://�����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x84://�غ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x89://�̿�ģʽ
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x90://��ͣ
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x91://Ƶ��װ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[2]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xc1://����װ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[2]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		case 0xd1://�������װ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		case 0xA2://α����ɹ�
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xAE://α�������
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA3://V3.01 ��α������ȣ�α����洢�ɹ�   
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA4://V3.01 ��α������ȣ�α���ɾ���ɹ�
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;			
		default:
			break;
	}	
	UART_RemoteEncode(USART1 , RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}

/**
 * @brief				
 * @param[in] 	 
 * @param[out] 
 * @return 
 * @note	
 */
void attenuation_number(control_number* ap , struct Uart_DataPacket* rp)
{
	unsigned short num = 0;
	num = (rp->RX_BUF[5] << 8) | (rp->RX_BUF[6] );
	if( num <= 7500){
		ap->control_number_low = rp->RX_BUF[5];
		ap->control_number_hight = rp->RX_BUF[6];
		//showflag
		//ap->showflag = mytrue;
	}
}

