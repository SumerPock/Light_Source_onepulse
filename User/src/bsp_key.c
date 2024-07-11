#include "bsp.h"


extern unsigned char flag_lightN_F;
/**
 * @brief ���ϰ���
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
void MenuTrig_Up(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		/*�Ϸ�ѡ����*/
		case 0:
			Menu_Indexs.Next_Index++;
			if(Menu_Indexs.Next_Index >= 7)
			{
				Menu_Indexs.Next_Index = 1;
			}else if(Menu_Indexs.Next_Index == 2){
				//����α���
				Menu_Indexs.Next_Index = 3;
			}else if(Menu_Indexs.Next_Index == 5){
				//��������ģʽ
				Menu_Indexs.Next_Index = 6;
			}		
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			break;
			
		/*�Ϸ�ѡ�������趨*/
		case 1:
			switch(Menu_Indexs.Next_Index)// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 1;
					break;
				case 2:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 10;
					break;
				case 3:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 100;
					break;
				case 4:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 1000;
					break;
				case 5:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 10000;
					break;
				case 6:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 100000;
					break;
				default:
					break;		
			}
			/**��������**/			
			if(Temp_CycleCodeTime > 220000){
				Temp_CycleCodeTime = 220000;
			}
			else if(Temp_CycleCodeTime < 10000){ 
				//2024.3.26	,	�������������趨
				Temp_CycleCodeTime = 10000;
			}	
			Menu_Refresh();
			break;
			
		case 2:
			if(Menu_Indexs.Prior_Index < 10)// ��С��10��˵���ڽ���α�������ѡ�񣬿�ִ������Ĵ���
			{	
				Menu_Indexs.Next_Index++;
				if(Menu_Indexs.Next_Index > 4)
					Menu_Indexs.Next_Index = 1;
				Menu_Refresh();
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			}
			break;
			
		case 3:
						if(flag_lightN_F == false)//ֻ���ڲ������������²�ȥ�趨�������
			{
				switch(Menu_Indexs.Next_Index)	// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
				{	
					case 1:		//��λ
						Tempnumber_Pulse = Tempnumber_Pulse + 1;
						break;

					case 2:		//ʮλ
						Tempnumber_Pulse = Tempnumber_Pulse + 10;
						break;

					case 3:		//��λ
						Tempnumber_Pulse = Tempnumber_Pulse + 100;
						break;
					
					case 4:	//ǧλ
						Tempnumber_Pulse = Tempnumber_Pulse + 1000;
						break;						

					case 5:	//��λ
						Tempnumber_Pulse = Tempnumber_Pulse + 10000;
						break;	
					
					default:
					break;		
				}	
				/**��������޶�**/
				if(Tempnumber_Pulse > 60001)
				{
					Tempnumber_Pulse = 60000;
				}else if(Tempnumber_Pulse < 1)
				{
					Tempnumber_Pulse = 1;
				}
				Menu_Refresh();				
			}
			break;
		
		/*�Ϸ�ѡ�������趨*/
		case 4:
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)	// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
				{	
					case 1:		//��λ
					Temp_dB_Loss = Temp_dB_Loss + 1;
					break;

					case 2:		//ʮλ
					Temp_dB_Loss = Temp_dB_Loss + 10;
					break;

					case 3:		//��λ
					Temp_dB_Loss = Temp_dB_Loss + 100;
					break;

					default:
					break;		
				}
				
				if(Temp_dB_Loss > 0 && Temp_dB_Loss <= ENERGYGEAR_1)
				{
					Temp_dB_Loss = ENERGYGEAR_1;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_1 && Temp_dB_Loss <= ENERGYGEAR_2)
				{
					Temp_dB_Loss = ENERGYGEAR_2;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_2 && Temp_dB_Loss <= ENERGYGEAR_3)
				{
					Temp_dB_Loss = ENERGYGEAR_3;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_3 && Temp_dB_Loss <= ENERGYGEAR_4)
				{
					Temp_dB_Loss = ENERGYGEAR_4;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_4 && Temp_dB_Loss <= MAX_Attenuation)
				{
					Temp_dB_Loss = MAX_Attenuation;
				}
				else 
				{
					Temp_dB_Loss = MAX_Attenuation;
				}
				Menu_Refresh();
			}
			else
			{				
			}
			break;
			
		case 5:
			break;
		
		case 6:
			break;
		
		default:
			break;
	}
}

/**
 * @brief ���°���
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
void MenuTrig_Down(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		
		/*�·�ѡ����*/
		case 0:
			Menu_Indexs.Next_Index--;			
			if(Menu_Indexs.Next_Index <= 0)
			{
				Menu_Indexs.Next_Index = 6;
			}else if(Menu_Indexs.Next_Index == 2){
				//����α���
				Menu_Indexs.Next_Index = 1;
			}else if(Menu_Indexs.Next_Index == 5){
				//��������ģʽ
				Menu_Indexs.Next_Index = 4;			
			}	
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;			
			break;
			
		/*�·������趨*/
		case 1:
			switch(Menu_Indexs.Next_Index)// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 1;
					break;
				case 2:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 10;
					break;
				case 3:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 100;
					break;
				case 4:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 1000;
					break;
				
				case 5:
					if(Temp_CycleCodeTime > 19000)
						Temp_CycleCodeTime = Temp_CycleCodeTime - 10000;
					break;
				case 6:
					if(Temp_CycleCodeTime > 109000)
						Temp_CycleCodeTime = Temp_CycleCodeTime - 100000;
					break;
					
				default:
					break;		
			}
//			if(Temp_CycleCodeTime < 50000){
//				Temp_CycleCodeTime = 50000;
//			}
			if(Temp_CycleCodeTime < 10000){
				Temp_CycleCodeTime = 10000;
			}
			Menu_Refresh();
			break;
			
		
		case 2:
			if(Menu_Indexs.Prior_Index < 10)// ��С��10��˵���ڽ���α�������ѡ�񣬿�ִ������Ĵ���
			{
				Menu_Indexs.Next_Index--;
				if(Menu_Indexs.Next_Index==0)
				{
					Menu_Indexs.Next_Index = 4;
				}
				Menu_Refresh();
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			}
			break;
			
		case 3:
			/*�·���������趨*/
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
				{	
					case 1:
						Tempnumber_Pulse = Tempnumber_Pulse - 1;
						break;
					case 2:
						Tempnumber_Pulse = Tempnumber_Pulse - 10;
						break;
					case 3:
						Tempnumber_Pulse = Tempnumber_Pulse - 100;
						break;
					case 4:
						Tempnumber_Pulse = Tempnumber_Pulse - 1000;
						break;
					case 5:
						Tempnumber_Pulse = Tempnumber_Pulse - 10000;
						break;
					default:
						break;		
				}
				if(Tempnumber_Pulse < 1){
					Tempnumber_Pulse = 1;
				}
				Menu_Refresh();
			}
			break;
		
		/*�·������趨*/
		case 4:
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)	// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
				{	
					case 1:
					Temp_dB_Loss = Temp_dB_Loss - 1;
					break;

					case 2:
					if(Temp_dB_Loss > 10)
					{
//						Temp_dB_Loss = Temp_dB_Loss - 10;
						Temp_dB_Loss = Temp_dB_Loss - 50;
					}
					break;

					case 3:
					if(Temp_dB_Loss > 100)
					{
						Temp_dB_Loss = Temp_dB_Loss - 100;
					}
					break;

					default:
					break;		
				}
				
				if(Temp_dB_Loss <= 0)
				{
					Temp_dB_Loss = 0;
				}
				else if(Temp_dB_Loss > 0 && Temp_dB_Loss <= ENERGYGEAR_1)
				{
					Temp_dB_Loss = ENERGYGEAR_1;	
				}
				else if(Temp_dB_Loss > ENERGYGEAR_1 && Temp_dB_Loss <= ENERGYGEAR_2)
				{
					Temp_dB_Loss = ENERGYGEAR_2;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_2 && Temp_dB_Loss <= ENERGYGEAR_3)
				{
					Temp_dB_Loss = ENERGYGEAR_3;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_3 && Temp_dB_Loss <= ENERGYGEAR_4)
				{
					Temp_dB_Loss = ENERGYGEAR_4;
				}
				else if(Temp_dB_Loss > ENERGYGEAR_4 && Temp_dB_Loss <= MAX_Attenuation)
				{
					Temp_dB_Loss = MAX_Attenuation;
				}
				else
				{
					//Temp_dB_Loss = MAX_Attenuation;
				}	
				Menu_Refresh();
			}
			else
			{
				
			}
			break;
			
		case 5:
			break;
		
		case 6:
			break;
		
		default:
			break;
	}
}


