
#include "bsp.h"

void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd);
//void attenuation_number(control_number* ap , struct Uart_DataPacket* rp);

union Union_Byte LaserState;	// �����壬������״̬

//extern float LaserTEMP;
extern bool Packet_flag;


//extern uint16_t u16_RandomData[1024];	// α���������������飬��RandomCode.c������
extern uint32_t u32_FlashBuff[2048];	// RandomCode.c������
//extern bool b_Random_EN;	// α�������ʹ��
extern uint16_t RandomMinimum;	// α����������Сֵ(��Ƶ)����RandomCode.c������
extern int32_t RandomCodeTime;	// α������ڣ���RandomCode.c������



/*
*********************************************************************************************************
*	�� �� ��: USART1_Config
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_Config(FunctionalState NewState)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	//GPIO�˿�ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 	
	//���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,	GPIO_PinSource9,	GPIO_AF_USART1); 	//GPIO_A9 ����ΪUSART1
	GPIO_PinAFConfig(GPIOA,	GPIO_PinSource10,	GPIO_AF_USART1); 	//GPIO_A10����ΪUSART1
		
	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�2MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,	&GPIO_InitStructure); 					//��ʼ��PA9��PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 				//��ʼ��PB10��PB11
	
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
		
	//USART NVIC ����
	//������ͬ����ռ���ȼ�����Ӧ���ȼ��������������жϱ��е���λ˳������ȴ�����һ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ�ܴ���
	//USART_Cmd(USART1, NewState);  	// ���ݲ��������Ƿ�򿪴���
	USART_Cmd(USART1, ENABLE);  	//ǿ�ƴ򿪴���
		
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	//��������ж�
	//USART_ITConfig(USART1, USART_IT_RXNE, NewState);  	// ���ݲ��������Ƿ�򿪴���
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  	// ���ݲ��������Ƿ�򿪴���
		
//	// ���ڽ��벿�ֵļĴ�����ʼ��
//	RemoteDataPacket.Start_Flag = 0;
//	RemoteDataPacket.Clarity_Flag = 0;
//	RemoteDataPacket.End_Flag = 0;
//	RemoteDataPacket.RX_BUF[0] = 0;
//	RemoteDataPacket.TX_BUF[0] = 0;	
//	LaserState.All = 0x0;
}


/*
*********************************************************************************************************
*	�� �� ��: USART1_Config
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
����usart1�Ĵ��ڹر�ʱ��Ӱ�쵽usart3��ͨѶ��ԭ��δ֪���ʽ����߷ֿ�д
*********************************************************************************************************
*/
void USART3_Config(FunctionalState NewState)			
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIO�˿�ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 	
	//���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource10,	GPIO_AF_USART3); 	//GPIO_B10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource11,	GPIO_AF_USART3); 	//GPIO_B11����ΪUSART3
	
	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�2MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 					//��ʼ��PA9��PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 				//��ʼ��PB10��PB11
	
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_InitStructure.USART_Mode =  USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	//USART NVIC ����
	//������ͬ����ռ���ȼ�����Ӧ���ȼ��������������жϱ��е���λ˳������ȴ�����һ��
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	//��������ж�
	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}



/*
*********************************************************************************************************
*	�� �� ��: UART_SEND
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART_SEND(USART_TypeDef* USARTx , unsigned short Data)
{
	USART_SendData(USARTx , Data);         		
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
	{
	}
}


/*
*********************************************************************************************************
*	�� �� ��: UART_SendInbuff
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UART_SendInbuff(uint8_t TX_Buff[], uint8_t InChar)
{
	if(TX_Buff[0] > 10)
	{
		TX_Buff[0] = 0;
	}
	else
	{
		TX_Buff[0]++;
		TX_Buff[TX_Buff[0]] = InChar;
	}
}


#define MAX_UART_SEND_DATA_SIZE 128 // �����С������Ҫ����

uint8_t UART_Send_Buffer[MAX_UART_SEND_DATA_SIZE]; 		// ���ڴ洢����������
uint8_t UART_Send_Buffer_Index = 0; 									// ���ڼ�¼��������
void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[])
{
	unsigned char i = 0, CheckSum = 0, OutChar = 0;
	
	if((TX_Buff[0] > 0) && (TX_Buff[0] < 9))
	{
		// ��շ��ͻ��������������
		memset(UART_Send_Buffer, 0, sizeof(UART_Send_Buffer));
		UART_Send_Buffer_Index = 0;
		//UART_SEND(USARTx, Start_Symbol);	
		UART_Send_Buffer[0] = Start_Symbol;
		CheckSum = 0;
		UART_Send_Buffer_Index = 1;
		
		for(i = 0; i < TX_Buff[0]; i++)
		{
			OutChar = TX_Buff[i + 1];
			CheckSum = CheckSum + OutChar;
			
			if (OutChar == Start_Symbol || OutChar == Clarity_Symbol || OutChar == End_Symbol)
			{
				//UART_SEND(USARTx, Clarity_Symbol);
				UART_Send_Buffer[UART_Send_Buffer_Index++] = Clarity_Symbol; // ���ַ���������
			}
			
			//UART_SEND(USARTx, OutChar);
			UART_Send_Buffer[UART_Send_Buffer_Index++] = OutChar; // ���ַ���������
		}
		
		if (CheckSum == Start_Symbol || CheckSum == Clarity_Symbol || CheckSum == End_Symbol)
		{	
			//UART_SEND(USARTx, Clarity_Symbol);
			UART_Send_Buffer[UART_Send_Buffer_Index++] = Clarity_Symbol; // ���ַ���������
		}	
		
		//UART_SEND(USARTx, CheckSum);
		UART_Send_Buffer[UART_Send_Buffer_Index++] = CheckSum; // ���ַ���������
		
		//UART_SEND(USARTx, End_Symbol);
		UART_Send_Buffer[UART_Send_Buffer_Index++] = End_Symbol; // ���ַ���������
		
		// �ں�������󣬱������鲢��ӡ����		
		if(USARTx == USART1)
		{
			memset(Uart_Tx_Buffer1, 0, sizeof(Uart_Tx_Buffer1));
			memcpy(Uart_Tx_Buffer1, UART_Send_Buffer, UART_Send_Buffer_Index + 2);
			DMA_Tx(USART1 , UART_Send_Buffer_Index + 2);
		}else if(USART3)
		{
			memset(Uart_Tx_Buffer3, 0, sizeof(Uart_Tx_Buffer3));
			memcpy(Uart_Tx_Buffer3, UART_Send_Buffer, UART_Send_Buffer_Index + 2);
			DMA_Tx(USART3 , UART_Send_Buffer_Index);
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: UART_RemoteEncode
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
* �Ҳ��붯����
*********************************************************************************************************
DMA_Tx(USART1 , 5);
Uart_Tx_Buffer1*/
//void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[])
//{
//	unsigned char i = 0, CheckSum = 0, OutChar = 0;
//	if((TX_Buff[0] > 0) && (TX_Buff[0] < 9))
//	{
//		UART_SEND(USARTx, Start_Symbol);	
//		CheckSum = 0;
//		for(i = 0; i < TX_Buff[0]; i++)
//		{
//			OutChar = TX_Buff[i + 1];
//			CheckSum = CheckSum + OutChar;
//			if (OutChar == Start_Symbol || OutChar == Clarity_Symbol || OutChar == End_Symbol )//�����͵��ַ��������ؼ�����ͬ
//			{
//				UART_SEND(USARTx, Clarity_Symbol);				
//			}
//			UART_SEND(USARTx, OutChar);
//		}
//		if (CheckSum==Start_Symbol || CheckSum==Clarity_Symbol || CheckSum==End_Symbol )//�����͵��ַ��������ؼ�����ͬ
//		{	
//			//�ж�У����Ƿ���ؼ�����ͬ 
//			UART_SEND(USARTx, Clarity_Symbol);	
//		}	
//		UART_SEND(USARTx, CheckSum);
//		UART_SEND(USARTx, End_Symbol);		
//	}
//}




// ���ڵ��ֽ�ָ��Ӧ��
void UART_Answer_NoData(USART_TypeDef* USARTx)
{	
	static unsigned char ucdata = 0;
	static unsigned char g_ansondata = 0;
	
//	float TEMP;
//	TEMP = 100 + 2 * LaserTEMP;
//	if(TEMP < 0) TEMP = 0; else if(TEMP > 255) TEMP = 255;	// ����ȡֵ��Χ���
	//TEMP = 0x99;
	//LaserState.All = 0x50;//�޸Ĺ���״̬
	
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	/*���ض�Ӧ�İ����*/
	UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[1]);	
	/*�豸����ID*/
	UART_SendInbuff(RemoteDataPacket.TX_BUF , g_ansondata);
	if(g_ansondata == 0x80)
	{
		UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, ucdata);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	}
	else
	{
		UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, ucdata);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	}
	UART_RemoteEncode(USART1 , RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}

 //����α���λ��װ��Ӧ��
void UART_Answer_RandomNumber(USART_TypeDef* USARTx, int16_t data)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// ���ض�Ӧ�İ����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xA0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);						// �������
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	UART_RemoteEncode(USART1, RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}

 //����α�������װ��Ӧ��
void UART_Answer_RandomData(USART_TypeDef* USARTx, int16_t data_16, uint32_t data_32)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// ���ض�Ӧ�İ����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xA1);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);						// �������
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	UART_RemoteEncode(USART1, RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}



 //����ͨѶ����Ӧ��
void UART_AnswerError(USART_TypeDef* USARTx, uint8_t ErrorID)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// ���ض�Ӧ�İ����
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xAE);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, ErrorID);						// �������
	UART_RemoteEncode(USARTx, RemoteDataPacket.TX_BUF);
}
