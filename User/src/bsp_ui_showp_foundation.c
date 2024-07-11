#include "bsp.h"
#include "main.h"

/**
 * @brief ���˵�����
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
void UI_Init(void)
{
	Menu_Indexs.Current_Index = 0;
	Menu_Indexs.Next_Index = 1;
	Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
	UI_Level_0();
	Menu_Refresh();
}


/**
 * @brief 0���˵���Ҳ����1-6��ģʽѡ��
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
void UI_Level_0(void)
{
	LCD_Clear(BLUE);					//��ָ����ɫ����
	Menu_Title();
	Menu_0_1(true);
	Menu_0_2(true);
	Menu_0_3(true);
	Menu_0_4(true);
	Menu_0_5(true);
	Menu_0_6(true);
	Menu_Infobar();
	
	if(b_Random_EN)	//�����ǰ����α���ģʽ�����˳�α���ģʽ
	{
		b_Random_EN = false;	
	}
	
	Lightsoutce.UIMode = 0;
}

/**
 * @brief 1���˵�����ȷƵ����
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
void UI_Level_1(void)
{
//char str[12];
//snprintf(str, 10, "%d", 50.013);
	LCD_Clear(BLUE);	//��ָ����ɫ����
	Menu_Title();			//����				
	Menu_0_1(true);
	Menu_1();
	LCD_Print(32+16*7, 36*2+32+16, "ms", WHITE, BLUE);
	Menu_Infobar();
	Lightsoutce.UIMode = 1;
}

/**
 * @brief 2���˵���α���
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
void UI_Level_2(void)
{
	LCD_Clear(BLUE);		//��ָ����ɫ����
	Menu_Title();				//����		
	Menu_0_2(true);
	Menu_2_x(true, 1);
	Menu_2_x(true, 2);
	Menu_2_x(true, 3);
	Menu_2_x(true, 4);
	Menu_Infobar();
	Lightsoutce.UIMode = 2;
}

/**
 * @brief ����3��������
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
void UI_Level_3(void)
{
	LCD_Clear(BLUE);					//��ָ����ɫ����
	Menu_Title();
	Menu_0_3(true);
	Menu_Infobar();
	Lightsoutce.UIMode = 3;
}

/**
 * @brief ����4����������趨
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
void UI_Level_4(void)
{	
	LCD_Clear(BLUE);
	Menu_Title();
	Menu_0_4(true);
	LCD_Print(16, 36*2+32+16, 	"˥��:    dB", WHITE, BLUE);
	Menu_4();
	Menu_Infobar();
	Lightsoutce.UIMode = 4;
}

/**
 * @brief ����ģʽ
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
void UI_Level_5(void)
{
	char str[12];
	snprintf(str, 10, "%d", 123456);	
	LCD_Clear(BLUE);					//��ָ����ɫ����
	Menu_Title();
	Menu_0_5(true);
	LCD_Print(48, 36*2+32+16, 	" 40 ms", WHITE, BLUE);
	LCD_Print(48, 36*3+32+16, 	" 60 ms", WHITE, BLUE);
	LCD_Print(48, 36*4+32+16, 	" 80 ms", WHITE, BLUE);
	LCD_Print(48, 36*5+32+16, 	"100 ms", WHITE, BLUE);
	Menu_Infobar();
	Lightsoutce.UIMode = 5;
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
void Program_control(control_number* cp)
{
	cp->Program_control_mode_1 = 0xFF;
	cp->Program_control_mode_2 = 0xFF;
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
void UI_Level_6(void)
{
	LCD_Clear(BLUE);					//��ָ����ɫ����
	Menu_Title();
	Menu_0_6(true);
	LCD_Print(0, 36*2+32+16, "������λ������", WHITE, BLUE);
	//LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 , 	"˥��:    dB" , WHITE , BLUE);
	LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 , 				    "˥��:       " , WHITE , BLUE);
	LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 + 32 + 16 , 	"����:       " , WHITE , BLUE);
	/*�˴����� FLAG ��֤���û������˳̿�ģʽ*/
	Program_control(&cn);
	Menu_Infobar();
	//USART1_Config(ENABLE);
	Lightsoutce.UIMode = 6; 	
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
void Menu_Title(void)
{
	LCD_Print(0, 16, 			"����Ŀ��ģ����", WHITE, BLUE);
}


/**
 * @brief �ͺż�����汾��
 *
 * ��������ṩ���ں����ĸ߼�������������������������
 *
 * @param[in]  ����1  ����1�������������������ͺͺ���
 * @param[out] ����2  ����2�������������������ͺͺ���
 *
 * @return ��������ֵ�������������������ͺͺ��壬������õĻ���
 *
 * @note �κ�����ע��������÷�ʾ����������������ӡ�
 * 2.11 ����̶�˥��20DB
 */
void Menu_Infobar(void)
{
	#if (VERSION_SELECTION == Ver_2_1_0)
		LCD_ShowString(100 , 300 , "Ver2.10 CL202-B" , WHITE);
	#elif (VERSION_SELECTION == Ver_2_1_1)
		LCD_ShowString(100 , 300 , "Ver2.11 CL202-B" , WHITE); 
	#elif (VERSION_SELECTION == Ver_2_2_0)
		LCD_ShowString(100 , 300 , "Ver2.20 CL202-B" , WHITE); 
	#elif (VERSION_SELECTION == Ver_2_3_0)
		LCD_ShowString(100 , 300 , "Ver1.00 CL1535" , WHITE); 
	#endif	
}

