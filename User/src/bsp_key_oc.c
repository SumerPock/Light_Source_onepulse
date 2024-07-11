#include "bsp.h"

double linearInterpolation(double x, DataPoint* data, size_t dataSize) {
    // Ѱ������������֪��
    size_t i = 0;
    double lowerBound = 0.00; // ָ����ֵ�������
    double upperBound = 25.00; // ָ����ֵ�������

    // �����ֵ�� x ���������޷�Χ���򷵻�һ���ض���ֵ������Ը�����Ҫ�޸�
    if (x < lowerBound || x > upperBound) {
        return -1.0; // ����һ����ʾ��Чֵ�ı�־
    }

    while (i < dataSize && x > data[i].x) {
        i++;
    }

    if (i == 0) 
    {
        // x С�ڵ��ڵ�һ����֪��� x ����
        return data[0].y;
    }
    else if (i == dataSize) 
    {
        // x ���ڵ������һ����֪��� x ����
        return data[dataSize - 1].y;
    }
    else 
    {
        // ���Բ�ֵ��ʽ��y = y1 + (x - x1) * (y2 - y1) / (x2 - x1)
        double x1 = data[i - 1].x;
        double y1 = data[i - 1].y;
        double x2 = data[i].x;
        double y2 = data[i].y;

        return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
    }
}

/**
 * @brief ok����
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
void MenuTrig_OK(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		case 0:	// ��OK��֮ǰ���ڶ����˵�
			if ((b_Ext_Sync) || (Menu_Indexs.Next_Index>3))
			{	
				Menu_Indexs.Current_Index = Menu_Indexs.Next_Index;
				switch(Menu_Indexs.Current_Index)
				{
					/*��ȷƵ�ʱ���*/
					case 1:
						Menu_Indexs.Next_Index = 0;
						Menu_Indexs.Prior_Index = 1;
						Temp_CycleCodeTime = CycleCodeTime;	// ����ʽ����������ֵ���µ���ʱ������
						UI_Level_1();
						break;
					
					/*α�������*/
					case 2:
						Menu_Indexs.Next_Index = 1;
						Menu_Indexs.Prior_Index = 1;
						#if (VERSION_SELECTION != Ver_2_3_0)
						UI_Level_2();
						#endif
						Menu_Refresh();
						break;
					
					/*����ģʽ*/
					case 3:
						Menu_Indexs.Next_Index = 0;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_3();
						#if (VERSION_SELECTION != Ver_2_3_0)
							UI_Level_3();
						#endif
						break;
					
					/*��������趨*/
					case 4:
						Menu_Indexs.Next_Index = 0;
						Menu_Indexs.Prior_Index = 1;
						Temp_dB_Loss = dB_Loss;	// ����ʽ��������˥����ֵ���µ���ʱ������
						UI_Level_4();
						break;
					
					/*����ģʽ*/
					case 5:
						Menu_Indexs.Next_Index = 50;
						Menu_Indexs.Prior_Index = 1;
						#if (VERSION_SELECTION != Ver_2_3_0)
							UI_Level_5();
						#endif
						break;
					
					/*�̿�ģʽ*/
					case 6:
						Menu_Indexs.Next_Index = 60;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_6();	
						retdata.Mode = 0x80;
						UART_Answer_LightData(USART1 , &retdata);			
						break;
					
					default:
						break;
				}
			}
			break;
			
		case 1:	// ��OK��֮ǰ���ھ�ȷƵ�ʱ������ò˵�
			if (Menu_Indexs.Prior_Index == 0xFF)		// Index=0xFF��˵������ѯ���Ƿ��˳�
			{	
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			else
			{
				if(Menu_Indexs.Next_Index == 0)
				{
					if(Temp_CycleCodeTime < 100000) 
						Menu_Indexs.Next_Index = Menu_Indexs.Next_Index + 5;
					else 
						Menu_Indexs.Next_Index = Menu_Indexs.Next_Index + 6;	//�������OK֮ǰ��0����+6�����趨ֵ�����λ����
				}
				else 
					Menu_Indexs.Next_Index--;
				Menu_Refresh();
				if(Menu_Indexs.Next_Index == 0)
				{
					CycleCodeTime = Temp_CycleCodeTime;	// ȷ�Ϻ󣬽���ʱ��������ֵͬ������ʽ�����С�
					CycleCode_Renew(CycleCodeTime);
					LCD_Print(32, 176, "�����Ѹ���", WHITE, BLUE);
					b_Random_EN = false;
				}
				else  	
					LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			break;

		case 2:	// ��OK��֮ǰ����α�������
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
			}
			else
			{
				Menu_Indexs.Prior_Index = 0x20;
				Menu_Refresh();				
			}
			break;
			
		
		/**����ģʽ���趨�������**/	
		case 3:
			// ��OK��֮ǰ���ڳ̿�ģʽ�˵�
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			else if(Menu_Indexs.Prior_Index > 10)
			{	// ˵��Ҫ����ѡ�����Ĳ˵�����
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				/**����ģʽ���趨�������**/	
				if(Menu_Indexs.Next_Index == 0){
					Menu_Indexs.Next_Index = Menu_Indexs.Next_Index + 5;	//�������OK֮ǰ��0����+3�����趨ֵ�����λ����
				}
				else{
					Menu_Indexs.Next_Index--;
				}
				Menu_Refresh();								
				if(Menu_Indexs.Next_Index == 0)
				{
					Number_Pulse = Tempnumber_Pulse;	// ȷ�Ϻ󣬽���ʱ��������ֵͬ������ʽ�����С�
//					CycleCode_Renew(CycleCodeTime);
					LCD_Print(32, 176, "�����Ѹ���", WHITE, BLUE);
//					b_Random_EN = false;
				}
				else  
				{
					LCD_Print(0, 176, "              ", WHITE, BLUE);				
				}
			}
			break;
			
		case 4:	// ��OK��֮ǰ��������˥�����ò˵�
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
					Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Current_Index = 0;
					UI_Level_0();
					Menu_Refresh();
			}
			else
			{
				if(Menu_Indexs.Next_Index == 0) 
					Menu_Indexs.Next_Index = Menu_Indexs.Next_Index+3;	//�������OK֮ǰ��0����+3�����趨ֵ�����λ����
				else 
					Menu_Indexs.Next_Index--;
				Menu_Refresh();

				if(Menu_Indexs.Next_Index == 0)
				{
					dB_Loss = Temp_dB_Loss;			//ȷ�Ϻ󣬽���ʱ��������ֵͬ������ʽ�����С�
					
					#if (VERSION_SELECTION == Ver_2_1_0)						
					#elif (VERSION_SELECTION == Ver_2_1_1)
						dB_Loss += 200; //2023.9.7��������������Ҫ�ģ��̼���20DB�������Ҫ
					#elif (VERSION_SELECTION == Ver_2_2_0)	
					#elif (VERSION_SELECTION == Ver_2_3_0)
					/*��λ���� ���������޸�*/
						TIM_Cmd(TIM4 , ENABLE);
						/*�����ȿ���*/
						static unsigned short getdata = 0;
						getdata = dB_Loss;
						signed short int temp_dB_Loss = Set_Energy(getdata);				
						#ifdef DEBUGPR
							SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_RED "temp_dB_Loss = %d \r\n" , temp_dB_Loss);
						#endif					
							#ifdef SETPW
							#endif									
					#ifdef DEBUGPR
						SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_RED "dB_Loss = %d \r\n" , dB_Loss);
						//SEGGER_RTT_printf(0 , RTT_CTRL_TEXT_BRIGHT_GREEN " number = %d \r\n" , number);
					#endif							
					#endif						
					LCD_Print(32, 176, "˥���Ѹ���", WHITE, BLUE);				
					RemoteDataPacket.RX_BUF[2] = 0x41;					/*����ͨ�����������������*/
					
					/*2023 . 9 .7 ������0��������ϣ����ӻ���ɼ�����������
					*/
					RemoteDataPacket.RX_BUF[3] = 0;
					RemoteDataPacket.RX_BUF[4] = 0;
					
					RemoteDataPacket.RX_BUF[5] = ((dB_Loss + 0)*10)/256;
					RemoteDataPacket.RX_BUF[6] = ((dB_Loss + 0)*10)%256;
					RemoteDataPacket.TX_BUF[0] = 0;	
					for(int i = 1 ; i < 7 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[i]);	
					}
					//��Ҫ��Ϊʲô�������Σ��ϰ�Ĵ����������Σ��Ҳ��޸�
					UART_RemoteEncode(USART3 , RemoteDataPacket.TX_BUF);
					RemoteDataPacket.RX_BUF[1]++; 	//�����
					//��������ʱ�����DMA�������� ����
					delay_xms(10);
					UART_RemoteEncode(USART3 , RemoteDataPacket.TX_BUF);
				}
				else 
				{
					LCD_Print(0, 176, "              ", WHITE, BLUE);
				}
			}
			break;	
		
		case 6:
			// ��OK��֮ǰ���ڳ̿�ģʽ�˵�
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				//USART3_Config(DISABLE);			//�رմ���
				//USART1_Config(DISABLE);			//�رմ���
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			else if(Menu_Indexs.Prior_Index > 10)
			{	// ˵��Ҫ����ѡ�����Ĳ˵�����
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				/* 2022 8 5
					 ��仰��������ʲô���壿���λ��ԭ�����ǿհ׵� �Ҽ���˥�������ں� ����̿�ģʽ ����ȷ����
					 �����ҵ��ַ���ס�������������ˣ���������ҷ��������������ԭ����������ϵ��
				*/
				//LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
		
		default:
			break;
	}

}



/**
 * @brief ȡ������
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
void MenuTrig_Cancel(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		/*0��ȡ�� ˢ��*/
		case 0:
			LCD_Init();
			UI_Level_0();
			Menu_Refresh();
			break;
		
		/*1��ȡ��*/
		case 1:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Prior_Index = 1;
				LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			else if(Menu_Indexs.Next_Index == 0)	//�������C֮ǰ��0���˳�
			{
					LCD_Print(32, 176, "ȷ�Ϸ���? ", WHITE, BLUE);
					Menu_Indexs.Prior_Index = 0xFF;
			}
			else if(Menu_Indexs.Next_Index == 6)
			{
				Menu_Indexs.Next_Index = 0;
				Menu_Indexs.Prior_Index = 1;
				Temp_CycleCodeTime = CycleCodeTime;	// ���˵�0����ǰ����ʱ�����ĸ��Ķ����ϣ����¼�����ʱ��������ֵ��
				LCD_Print(32, 176, "�޸���ȡ��", WHITE, BLUE);
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				Menu_Indexs.Next_Index++;
			}
			Menu_Refresh();
			break;
		
		/*2��ȡ��*/
		case 2:
			if(Menu_Indexs.Prior_Index == 0x20)
			{
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_2_x(true, 1);
				Menu_2_x(true, 2);
				Menu_2_x(true, 3);
				Menu_2_x(true, 4);
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			break;
						
		case 4:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Prior_Index = 1;
				LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			else if(Menu_Indexs.Next_Index == 0)	//�������C֮ǰ��0���˳�
			{
					LCD_Print(32, 176, "ȷ�Ϸ���? ", WHITE, BLUE);
					Menu_Indexs.Prior_Index = 0xFF;
			}
			else if(Menu_Indexs.Next_Index == 3)
			{
				Menu_Indexs.Next_Index = 0;
				Menu_Indexs.Prior_Index = 1;
				Temp_dB_Loss = dB_Loss;	// ���˵�0����ǰ����ʱ�����ĸ��Ķ����ϣ����¼�����ʱ��������ֵ��
				LCD_Print(32, 176, "�޸���ȡ��", WHITE, BLUE);
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				Menu_Indexs.Next_Index++;
			}
			Menu_Refresh();
			break;

		case 6:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				/*�ٴν���̿�ģʽ*/
				LCD_Print(0, 176, "              ", WHITE, BLUE);
				
				LCD_Init();		
				UI_Level_6();
				Menu_Refresh();
				
				Menu_Indexs.Prior_Index = 1;
			}
			else
			{
				LCD_Print(32, 176, "ȷ�Ϸ���? ", WHITE, BLUE);
				cn.Program_control_mode_1 = false;
				cn.Program_control_mode_2 = false;
				Menu_Indexs.Prior_Index = 0xFF;
			}
			break;
			
		case 3:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				/*�ٴν��������ģʽ*/
				LCD_Print(0, 176, "              ", WHITE, BLUE);
				
				LCD_Init();		
				UI_Level_3();
				Menu_Refresh();				
				
				Menu_Indexs.Prior_Index = 1;
			}
			else
			{
				LCD_Print(32, 176, "ȷ�Ϸ���? ", WHITE, BLUE);
				cn.Program_control_mode_1 = false;
				cn.Program_control_mode_2 = false;
				Menu_Indexs.Prior_Index = 0xFF;
			}			
//			Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
//			Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
//			Menu_Indexs.Current_Index = 0;
//			UI_Level_0();
//			Menu_Refresh();
			break;
		
		case 5:
			Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
			Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
			Menu_Indexs.Current_Index = 0;
			UI_Level_0();
			Menu_Refresh();
			break;
		
		default:
			break;
	}
}




