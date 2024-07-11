

#include "bsp.h"

uint32_t GetSector(uint32_t Address);
uint32_t STMFLASH_ReadWord(uint32_t Address);

	/************************************************
	�������� �� STMFLASH_ReadWord
	��    �� �� ��Ŀ���ַ��ȡһ��32λ��
	��    �� �� Address
	�� �� ֵ �� 
	��	  ע �� 
	*************************************************/
uint32_t STMFLASH_ReadWord(uint32_t Address)
{
	return *(__IO uint32_t*)Address;
} 

	/************************************************
	�������� �� Read_Memory
	��    �� �� ����RAM��Flash�е�����
	��    �� �� Start_Address ��ʼ��ַ
							size 					��ȡ�����ݸ�����32bit��
							*p 						Ŀ������ָ��
	�� �� ֵ �� ��
	��	  ע �� 
	*************************************************/
void Read_Memory(uint32_t Start_Address, uint16_t size, uint32_t *p)
{
	uint16_t i;
  for (i=0; i<size; i++)
  {
    *(p+i) = *(__IO uint32_t*)(Start_Address+i*4);   //��FLASH�е����ݣ�ֱ�Ӹ�����ַ�����ˡ������ڴ��ж�����һ����
  }
}


/************************************************
�������� �� STMFLASH_Write
��    �� �� 
��    �� �� WriteAddr:	��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
						pBuffer:		����ָ��
						size:	����(����Ҫд���32λ���ݵĸ���.)
�� �� ֵ �� ��
��	  ע �� 
*************************************************/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t size)
{
	FLASH_Status status = FLASH_COMPLETE;
	u32 addrx = 0;
	u32 endaddr = 0;
	int SectorStart,SectorEnd;
	//int SectorStart = 0;
	/*�Ƿ���ַ*/
	if(WriteAddr<ADDR_FLASH_SECTOR_0||WriteAddr%4)
	{
		return;	
	}
	/*FLASH����*/
	FLASH_Unlock();		
	/* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
	/* �������FLASH�ı�־λ */  	
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| 
                  FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	/*FLASH�����ڼ�,�����ֹ���ݻ���*/
	FLASH_DataCacheCmd(DISABLE);
	/*
	д�����ʼ��ַ
	*/
	addrx = WriteAddr;		
	/*
	д��Ľ�����ַ
	*/
	endaddr = WriteAddr + size;	
//	endaddr=WriteAddr+NumToWrite*4;		
//	SectorStart = STMFLASH_GetFlashSector(addrx);
//	SectorEnd = STMFLASH_GetFlashSector(endaddr);
	
	SectorStart =GetSector(addrx);
	SectorEnd = GetSector(endaddr);
	
	/*ֻ�����洢��,����Ҫִ�в�������!!*/
	if(addrx<0X1FFF0000)			
	{
		status=FLASH_EraseSector(SectorStart, VoltageRange_3);/*VCC=2.7~3.6V֮��!!*/
		/*ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)*/
//		while(SectorStart <= SectorEnd)
//		{
//			/*�з�0XFFFFFFFF�ĵط�,Ҫ�����������*/
//			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
//			{   
//				
//				status=FLASH_EraseSector(SectorStart, VoltageRange_3);/*VCC=2.7~3.6V֮��!!*/
//				if(status!=FLASH_COMPLETE)
//				{
//					/*��������*/
//					break;	
//				}
//			}else 
//			{
//				SectorStart += 8;
//			}
//		} 
	}
	if(status==FLASH_COMPLETE)
	{
		/*д����*/
		while(WriteAddr<endaddr)
		{
			/*д������*/
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//
			{ 
				/*д���쳣*/
				break;	
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	/*FLASH��������,�������ݻ���*/
	FLASH_DataCacheCmd(ENABLE);	
	/*FLASH����*/
	FLASH_Lock();
}
	/************************************************
	�������� �� GetSector
	��    �� �� 
	��    �� �� Address
	�� �� ֵ �� sector
	��	  ע �� 
	*************************************************/
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}
