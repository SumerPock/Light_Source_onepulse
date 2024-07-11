
#include "bsp.h"

static uint32_t ulTimeOut_Time;
static uint8_t I2C_Err = 0;

/************************************************
�������ƣ� IIC_Init
��    �ܣ� ��ʼ��IIC
��    ���� ��
�� �� ֵ�� �ޣ�������PB6��PB9�˿ڣ��޸���I2C.c��ȫ�ֱ���ulTimeOut_Time��ֵ
��    �ߣ� gaoyao
*************************************************/
void IIC_Init(void)
{			
  GPIO_InitTypeDef	GPIO_InitStructure;
	I2C_InitTypeDef 	I2C_InitStructure;
	RCC_ClocksTypeDef	rcc_clocks;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,	ENABLE);	//ʹ��GPIOBʱ��  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,		ENABLE);	//ʹ��I2C1ʱ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,		ENABLE);	//Reset I2Cx IP
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,		DISABLE);	//Release reset signal of I2Cx IP
	
	/*I2C1 configuration		PB6: I2C1_SCL		PB9: I2C1_SDA  */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //ע�⣬�˴����ܺϲ�д��GPIO_PinSource6|GPIO_PinSource9
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//���õڶ�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//2MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;					//��©��ʽ���
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//������Ҳ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);									//��ʼ��
	
	/* I2C Struct Initialize */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);

	/* I2C Initialize */
	I2C_Cmd(I2C1, ENABLE);
	
	/*��ʱ����*/
	RCC_GetClocksFreq(&rcc_clocks);
	ulTimeOut_Time = (rcc_clocks.SYSCLK_Frequency /10000);
	
}

/************************************************
�������ƣ�	I2C_ReadOneByte
��    �ܣ�	��IIC���߶�ȡһ���ֽ�����
��    ����	I2Cx, ʹ�õ�I2C��ţ�
						I2C_addr, ���豸��I2C��ַ
						reg_addr, Ҫ��ȡ�Ĵ��豸�Ĵ�����ַ

�� �� ֵ��	uint8_t, I2C�����϶�ȡ��������
						�޸���I2C.c��ȫ�ֱ���I2C_Err�ı�ǣ���I2C���ݺ���Ҫ���I2C_Err�Ƿ�Ϊ0����Ϊ0����ʾ��ȡI2C����ʱ������ʱ�������������ݿ��ܲ���ȷ��

��    �ߣ�	gaoyao
*************************************************/
uint8_t I2C_ReadOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr)
{
	__IO uint8_t readout;
	__IO uint8_t readout2;
	uint32_t tmr;

	tmr = ulTimeOut_Time;
	while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);	//����I2C��START�źţ��ӿ��Զ��ɴ��豸������豸
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	I2C_Send7bitAddress(I2Cx,I2C_Addr,I2C_Direction_Transmitter);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, Reg_addr);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;
	
//	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
//	tmr = ulTimeOut_Time;
//	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
//	if(tmr==0) I2C_Err = 1;

	//I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	//I2C_GenerateSTOP(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)))); /* EV7 */
	if(tmr==0) I2C_Err = 1;


	//
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	I2C_GenerateSTOP(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)))); /* EV7 */
	if(tmr==0) I2C_Err = 1;


	readout = I2C_ReceiveData(I2Cx);
	readout2 = I2C_ReceiveData(I2Cx);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return readout;
}

/************************************************
�������ƣ�	I2C_WriteOneByte
��    �ܣ�	����豸д��һ���ֽ�����
��    ����	I2Cx, ʹ�õ�I2C��ţ�
						I2C_addr, ���豸��I2C��ַ
						reg_addr, Ҫ��ȡ�Ĵ��豸�Ĵ�����ַ
						value, ��Ҫд�������

�� �� ֵ��	uint8_t, I2C�����϶�ȡ��������
						�޸���I2C.c��ȫ�ֱ���I2C_Err�ı�ǣ���I2C���ݺ���Ҫ���I2C_Err�Ƿ�Ϊ0����Ϊ0����ʾ��ȡI2C����ʱ������ʱ�������������ݿ��ܲ���ȷ��

��    �ߣ�	gaoyao
*************************************************/
void I2C_WriteOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr, uint8_t value)
{
	uint32_t tmr;

	tmr = ulTimeOut_Time;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, Reg_addr);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, value);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTOP(I2Cx, ENABLE);
	//I2C_AcknowledgeConfig(I2Cx, DISABLE);

}


float I2C_LM75_Read(void)
{
	uint8_t TempH,TempL;
	uint16_t data;
	//uint32_t TimeOut;

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C GenerateSTART 0 Error\r\n");
	}
	I2C_Send7bitAddress(I2C2, 0x90,I2C_Direction_Transmitter);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C Send7bitAddress 0 Error\r\n");
	}

	I2C_SendData(I2C2,0x00);

	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C SendData 0 Error\r\n");
	}

	I2C_GenerateSTART(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C GenerateSTART 1 Error\r\n");
	}

	I2C_Send7bitAddress(I2C2, 0x90,I2C_Direction_Receiver);

	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_Send7bitAddress 1 Error\r\n");
	} 

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_ReceiveData 0 Error\r\n");
	}
	TempH = I2C_ReceiveData(I2C2);

	//TimeOut=0x1000;

	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_ReceiveData 1 Error\r\n");
	}
	TempL = I2C_ReceiveData(I2C2);

	data=((uint16_t)TempH << 8) | TempL ;

	return data_convert(data);
}

float data_convert(uint16_t data)
{
	uint8_t is_negative=data&(uint16_t)(0x8000);
	uint8_t vaild_data_part=(((data<<1)&(uint16_t)(0xFF00))>>8);

	return is_negative==1 ? -(((int)vaild_data_part)*0.5):((int)vaild_data_part)*0.5;
}
