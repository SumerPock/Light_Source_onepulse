#include "bsp.h"


extern unsigned char flag_lightN_F;
/**
 * @brief 向上按键
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
void MenuTrig_Up(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		/*上翻选择功能*/
		case 0:
			Menu_Indexs.Next_Index++;
			if(Menu_Indexs.Next_Index >= 7)
			{
				Menu_Indexs.Next_Index = 1;
			}else if(Menu_Indexs.Next_Index == 2){
				//跳过伪随机
				Menu_Indexs.Next_Index = 3;
			}else if(Menu_Indexs.Next_Index == 5){
				//跳过仿真模式
				Menu_Indexs.Next_Index = 6;
			}		
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			break;
			
		/*上翻选择周期设定*/
		case 1:
			switch(Menu_Indexs.Next_Index)// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
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
			/**限制周期**/			
			if(Temp_CycleCodeTime > 220000){
				Temp_CycleCodeTime = 220000;
			}
			else if(Temp_CycleCodeTime < 10000){ 
				//2024.3.26	,	加入限制周期设定
				Temp_CycleCodeTime = 10000;
			}	
			Menu_Refresh();
			break;
			
		case 2:
			if(Menu_Indexs.Prior_Index < 10)// 当小于10，说明在进行伪随机编码选择，可执行下面的代码
			{	
				Menu_Indexs.Next_Index++;
				if(Menu_Indexs.Next_Index > 4)
					Menu_Indexs.Next_Index = 1;
				Menu_Refresh();
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			}
			break;
			
		case 3:
						if(flag_lightN_F == false)//只有在不出激光的情况下才去设定脉冲次数
			{
				switch(Menu_Indexs.Next_Index)	// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
				{	
					case 1:		//个位
						Tempnumber_Pulse = Tempnumber_Pulse + 1;
						break;

					case 2:		//十位
						Tempnumber_Pulse = Tempnumber_Pulse + 10;
						break;

					case 3:		//百位
						Tempnumber_Pulse = Tempnumber_Pulse + 100;
						break;
					
					case 4:	//千位
						Tempnumber_Pulse = Tempnumber_Pulse + 1000;
						break;						

					case 5:	//万位
						Tempnumber_Pulse = Tempnumber_Pulse + 10000;
						break;	
					
					default:
					break;		
				}	
				/**脉冲次数限定**/
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
		
		/*上翻选择能量设定*/
		case 4:
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)	// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
				{	
					case 1:		//个位
					Temp_dB_Loss = Temp_dB_Loss + 1;
					break;

					case 2:		//十位
					Temp_dB_Loss = Temp_dB_Loss + 10;
					break;

					case 3:		//百位
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
 * @brief 向下按键
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
void MenuTrig_Down(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		
		/*下翻选择功能*/
		case 0:
			Menu_Indexs.Next_Index--;			
			if(Menu_Indexs.Next_Index <= 0)
			{
				Menu_Indexs.Next_Index = 6;
			}else if(Menu_Indexs.Next_Index == 2){
				//跳过伪随机
				Menu_Indexs.Next_Index = 1;
			}else if(Menu_Indexs.Next_Index == 5){
				//跳过仿真模式
				Menu_Indexs.Next_Index = 4;			
			}	
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;			
			break;
			
		/*下翻周期设定*/
		case 1:
			switch(Menu_Indexs.Next_Index)// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
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
			if(Menu_Indexs.Prior_Index < 10)// 当小于10，说明在进行伪随机编码选择，可执行下面的代码
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
			/*下翻脉冲次数设定*/
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
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
		
		/*下翻能量设定*/
		case 4:
			if(flag_lightN_F == false)
			{
				switch(Menu_Indexs.Next_Index)	// 因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
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


