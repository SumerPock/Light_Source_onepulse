#include "bsp.h"


/**
 * @brief
 *
 * ��������ṩ���ں����ĸ߼�������������������������
 *
 * @param[in]  ����1  ����1�������������������ͺͺ���
 * @param[out] ����2  ����2�������������������ͺͺ���
 *
 * @return ��������ֵ�������������������ͺͺ��壬������õĻ���
 *
 * @note �κ�����ע��������÷�ʾ����������������ӡ�
 *		V2.01 �����α���ģʽ������
 *		����ģʽ���������ڱ��롣 Դ���룺CycleCode_Renew(CycleCodeTime);
 */
 
void random_mode()
{
		if(b_Random_EN == false) 
		{
			CycleCode_Renew(CycleCodeTime); //����ģʽ
		}
		else
		{
			
		}
		
		//�˳�α���
		if(b_Random_EN == true)	//�����ǰ����α���ģʽ��
		{
			b_Random_EN = false;	//���˳�α���ģʽ
		}
}

