
#include "bsp.h"

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
void Menu_0_1(bool Show_Mode)
{
	uint16_t color_1 = 0 , color_2 = 0;
	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}
	if(b_Ext_Sync)
		LCD_Print(0, 36+32, 	"1.��ȷƵ�ʱ���", color_1, color_2);
	else 
		LCD_Print(0, 36+32, 	"1.��ͬ��      ", color_1, color_2);
}





void Menu_Refresh(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		case 0:
			switch(Menu_Indexs.Prior_Index)
			{
				case 1:
					Menu_0_1(true);
					break;
				
				case 2:
					Menu_0_2(true);
					break;
				
				case 3:
					Menu_0_3(true);
					break;
				
				case 4:
					Menu_0_4(true);
					break;
				
				case 5:
					Menu_0_5(true);
					break;
				
				case 6:
					Menu_0_6(true);
					break;
				
				default:
					break;
			}
			switch(Menu_Indexs.Next_Index)
			{
				case 1:
					Menu_0_1(false);
					break;
				
				case 2:
					Menu_0_2(false);
					break;
				
				case 3:
					Menu_0_3(false);
					break;
				
				case 4:
					Menu_0_4(false);
					break;
				
				case 5:
					Menu_0_5(false);
					break;
				
				case 6:
					Menu_0_6(false);
					break;
				
				default:
					break;
			}
			break;
			
		case 1:
			Menu_1();
			break;
		
		case 2:
			if(Menu_Indexs.Prior_Index == 0x20)
			{	// α���������ѡ��
				Menu_2_x_OK(Menu_Indexs.Next_Index);
			}
			else
			{	// ѡ����һ��α�������
				Menu_2_x(true, Menu_Indexs.Prior_Index);	// ��ǰһ���˵���ɫˢ����
				Menu_2_x(false, Menu_Indexs.Next_Index);	// ����ѡ��Ĳ˵���ɫ����				
			}
			break;

		case 3:
			Menu_3();
			break;
			
		case 4:
			Menu_4();
			break;
		
		default:
			break;
	}
}
	
/**ˢ�����������ʾˢ���趨**/
void Menu_3(void)
{
	Number_Highlight_Show(32 ,36 * 2 + 32 + 16 ,(uint32_t)Tempnumber_Pulse , 6, 0, Menu_Indexs.Next_Index % 10, WHITE, BLUE, BLACK, WHITE);
}

void Menu_1(void)	// ˢ�¾�ȷƵ������ֵ
{
	// ˢ��������ֵ��6λ���֣�С�����3λ��������λ��ΪMenu_Indexs.Next_Index�ĸ�λ����������Ҫ��10ȡ��
	Number_Highlight_Show(32 , 36*2 + 32 + 16, (uint32_t)Temp_CycleCodeTime , 6 , 3 , Menu_Indexs.Next_Index % 10 , WHITE, BLUE, BLACK, WHITE);
	// ע������λ��Ӧ��UI_Level_1�����С�us������
}


void Menu_0_2(bool Show_Mode)
{
	uint8_t line_number;
	uint16_t color_1, color_2;
	
	if(Menu_Indexs.Current_Index == 2)	line_number = 1;
	else line_number = 2;	

	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}
	
	if(b_Ext_Sync)
		LCD_Print(0, 36*line_number+32, "2.α�������  ", color_1, color_2);
	else
		LCD_Print(0, 36*line_number+32, "2.��ͬ��     ", color_1, color_2);	
}

void Menu_0_3(bool Show_Mode)
{
	uint8_t line_number;
	uint16_t color_1, color_2;
	
	if(Menu_Indexs.Current_Index == 3)	line_number = 1;
	else line_number = 3;
	
	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}

	if(b_Ext_Sync){
		LCD_Print(0, 36*line_number+32, "3.����ģʽ    ", color_1, color_2);
	}	
	else{
		LCD_Print(0, 36*line_number+32, "3.��ͬ��      ", color_1, color_2);	
	}
		
}

void Menu_0_4(bool Show_Mode)
{
	uint8_t line_number;
	uint16_t color_1, color_2;
	
	if(Menu_Indexs.Current_Index == 4)	line_number = 1;
	else line_number = 4;
	
	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}
	
	LCD_Print(0, 36*line_number+32, "4.��������趨", color_1, color_2);
}

void Menu_4(void)	// ˢ������˥����ֵ
{
	// ˢ������˥����ֵ��3λ���֣�С�����1λ��������λ��ΪMenu_Indexs.Next_Index�ĸ�λ����������Ҫ��10ȡ��
	Number_Highlight_Show(16+80, 36*2+32+16, (uint32_t)Temp_dB_Loss, 3, 1, Menu_Indexs.Next_Index % 10, WHITE, BLUE, BLACK, WHITE);
	// ע������λ��Ӧ��UI_Level_4�����С�dB������
	// LCD_Print(16, 36*2+32+16, 	"˥��:32.3dB", WHITE, BLUE);
}

void Menu_0_5(bool Show_Mode)
{
	uint8_t line_number;
	uint16_t color_1, color_2;
	
	if(Menu_Indexs.Current_Index == 5)	line_number = 1;
	else 	line_number = 5;

	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}
	
	LCD_Print(0, 36*line_number+32, "5.����ģʽ    ", color_1, color_2);
}

void Menu_0_6(bool Show_Mode)
{
	uint8_t line_number;
	uint16_t color_1, color_2;
	
	if(Menu_Indexs.Current_Index == 6)	
		line_number = 1;
	else 
		line_number = 6;

	if(Show_Mode)
	{	// δѡ��ģʽ�����װ���
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{	// ѡ��ģʽ���׵׺���
		color_1 = BLACK;
		color_2 = WHITE;
	}	
	LCD_Print(0, 36*line_number+32, "6.�̿�ģʽ    ", color_1, color_2);
}

void Menu_2_x(bool Show_Mode, uint8_t line_number)
{	// �˵�2��α�������ˢ��
	//uint8_t i;
	uint64_t Temp_u64;
	uint16_t color_1, color_2;
	char Temp_char[4] = {0x30, 0x29, 0x20, 0x0};
	//char *p_char;
	
	Temp_u64 = RandomSaveData_Check(line_number-1);
	
	if(Show_Mode)
	{
		color_1 = WHITE;
		color_2 = BLUE;
	}
	else
	{
		color_1 = BLACK;
		color_2 = WHITE;
	}
	Temp_char[0] += line_number;
	LCD_Print(32, 40*line_number+72, Temp_char, color_1, color_2);
	if(Temp_u64 == 0) LCD_Print(80, 40*line_number+72, "��     ", color_1, color_2);
	else LCD_Print(80, 40*line_number+72, (char *)&Temp_u64, color_1, color_2);
}

bool Menu_2_x_OK(uint8_t CodeNumber)
{	// �˵�2��α�������ˢ��
	uint16_t i;
	uint32_t *p_u32, Temp_u32;
	uint64_t Temp_u64;
	char Temp_char[4] = {0x30, 0x29, 0x20, 0x0};
	//char *p_char;
	char str[12];
	
	Temp_u64 = RandomSaveData_Check(CodeNumber-1);
	if(Temp_u64 != 0) 
	{
		b_Random_EN = true;	// ������ģʽ����α���״̬
		// ��α������FlashBuff[]���ص�RandomData[]��;
		p_u32 = (uint32_t *)&u16_RandomData;
		for(i=0; i<512; i++)
		{
			*(p_u32+i) = u32_FlashBuff[512*(CodeNumber-1)+i];
		}
		Temp_u32 = u16_RandomData[6];	// ������Ƶ
		Temp_u32 = Temp_u32 + u16_RandomData[8];	// �������
		CycleCode_Renew(Temp_u32);	// ������д��CPLD
		
		Temp_char[0] += CodeNumber;
		LCD_Print(32, 40+72, Temp_char, WHITE, BLUE);
		LCD_Print(80, 40+72, (char *)&Temp_u64, WHITE, BLUE);
		LCD_Print(32, 40*2+72, "Size:       ", WHITE, BLUE);
		LCD_Print(32, 40*3+72, "Tmin:       ", WHITE, BLUE);
		LCD_Print(32, 40*4+72, "Tmax:       ", WHITE, BLUE);
		
		snprintf(str, 10, "%d", RandomNumber);
		LCD_Print(32+80, 40*2+72, str, WHITE, BLUE);
		snprintf(str, 10, "%d", RandomBaseFreq + RandomMinValue);
		LCD_Print(32+80, 40*3+72, str, WHITE, BLUE);
		snprintf(str, 10, "%d", RandomBaseFreq + RandomMaxValue);
		LCD_Print(32+80, 40*4+72, str, WHITE, BLUE);
		

		return true;
	}
	else
	{
		Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
		return false;
	}
}


