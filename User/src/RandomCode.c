
#include "bsp.h"


bool RandomCode_Check(void)
{
	int i;
	bool ReturnValue = true;
	uint32_t Temp_u32;
	
	if((u16_RandomData[7] > 0) && (u16_RandomData[7] < 1017))	//������Ч��Χ1~1016
	{
		for(i=1; i<u16_RandomData[7]; i++)
		{
			Temp_u32 = u16_RandomData[6] + u16_RandomData[i+7];
			
			if((Temp_u32 < 40000) || (Temp_u32 > 120000))
			{
				ReturnValue = false;
				i = u16_RandomData[0] + 10;
			}
		}
	}
	else
	{	// �������ȼ�����
		ReturnValue = false;
	}
	
	return ReturnValue;
}

/*
α���������
��ʽ��8�ֽڱ�ʶ��+2�ֽ�Ԥ��+2�ֽ�α�������λ��������ռ��4��32λ�ռ䣬�����ﻹʣ���1020���ռ�ɴ��α�������
*/
uint64_t RandomSaveData_Check (uint32_t Address)
{
	union Union_TypeConversion Format_Conversion;
	//uint32_t Random_Info[4];
	uint16_t i, Temp_u16, Number, BaseFreq, MinValue = 0xFFFF, MaxValue = 0x0, *p;
	bool CheckValue = true;
	
	Format_Conversion.TC_u32[0] = u32_FlashBuff[512*Address];
	Format_Conversion.TC_u32[1] = u32_FlashBuff[512*Address+1];
	
	for(i=0; i<6; i++)
	{
		if((Format_Conversion.TC_byte[i]<0x20) || (Format_Conversion.TC_byte[i]>0x7E))
		{	// �ж��ַ��Ƿ�Ϸ�
			CheckValue = false;
			i=100;
		}
	}
	
	if(CheckValue)
	{	// ��������жϺϷ���������жϱ���λ���Ƿ�Ϲ�
		p=(uint16_t *)&u32_FlashBuff[512*Address+3];
		BaseFreq = *p;
		Number = *(p+1);
		if ((Number < 1) && (Number > 1016))
		{
			CheckValue = false;	// �����ı��볤����2��1016
		}
		else
		{
			for(i=0; i<Number; i++)
			{
				Temp_u16 = *(p+2+i);
				if(Temp_u16 > MaxValue) MaxValue = Temp_u16;
				if(Temp_u16 < MinValue) MinValue = Temp_u16;
				if(((Temp_u16 + BaseFreq) < 40000) || ((Temp_u16 + BaseFreq) > 120000))
				{
					CheckValue = false;	// ����Խ��
					i = Number + 10;
				}					
			}
		}
	}
	
	if(CheckValue)	// 
	{
		RandomNumber = Number;
		RandomBaseFreq = BaseFreq;
		RandomMinValue = MinValue;
		RandomMaxValue = MaxValue;
		Format_Conversion.TC_byte[7] = 0x0;
		return Format_Conversion.TC_uint64_t;
	}
	else return 0;
}
