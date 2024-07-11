#include "bsp.h"


/*******************************ȫ�ֱ�������****************************************/
unsigned char	Uart_Rx_Buffer1[2048] = {0}; 
static unsigned short SizeUart_Rx_Buffer1 = 2048;
unsigned short Number_Rx_Buffer1 = 0;

unsigned char	Uart_Tx_Buffer1[256] = {0}; 
static unsigned short SizeUart_Tx_Buffer1 = 0;
unsigned short Number_Tx_Buffer1 = 0;

unsigned char	Uart_Rx_Buffer3[256] = {0}; 
static unsigned short SizeUart_Rx_Buffer3 = 256;
unsigned short Number_Rx_Buffer3 = 0;

unsigned char	Uart_Tx_Buffer3[256] = {0}; 
static unsigned short SizeUart_Tx_Buffer3 = 0;
 unsigned short Number_Tx_Buffer3 = 0;

/**
  * @brief  /ͨ�ô������ú���������1�����ںţ�����2�������ʣ�����3/4��ʹ�ùܽź�
  * @param  None
  * @retval None
  */
void Uart_Init(USART_TypeDef* USARTx , \
							 uint32_t bound , \
							 GPIO_TypeDef* GPIOx1 , \
							 uint8_t GPIO_PinSourcex1 , \
							 GPIO_TypeDef* GPIOx2 ,\
							 uint8_t GPIO_PinSourcex2 , \
							 bool IsSbus , \
							 unsigned char prepriority , \
							 unsigned char subpriority)
{
	uint8_t temp_IRQn = 0;
	Myuart_Config(USARTx , bound , IsSbus);
	MyGPIO_Config(USARTx , GPIOx1 , GPIO_PinSourcex1 , GPIOx2 , GPIO_PinSourcex2);
	
	/*���ڽ���DMA����*/
	if(USARTx == USART1)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
								 DMA2_Stream2 , \
								 DMA_Channel_4 , \
								 (uint32_t)&USART1->DR , \
							   /*(uint32_t)UART_RX_BUFFER[0] ,*/ 
								 (uint32_t)Uart_Rx_Buffer1 ,
								 DMA_DIR_PeripheralToMemory , \
							   /*UART_power_detection_rx_SIZE , \*/
								 SizeUart_Rx_Buffer1 ,
								 DMA_Priority_Low);    
		MyNVIC_Config(DMA2_Stream2_IRQn , 9 , 0 , ENABLE);
		temp_IRQn = USART1_IRQn;
	}
	
	//DMA1_Stream5 ��Դ
	if(USARTx == USART2)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1 , \
//							   DMA1_Stream5 , \
//							   DMA_Channel_4 , \
//								 (uint32_t)&USART2->DR , \
//							   (uint32_t)UART_RX_BUFFER1, \
//								 DMA_DIR_PeripheralToMemory , \
//								 GY_UARTRXBuffer_SIZE , \
//								 DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream5_IRQn , 5 , 0 , ENABLE);
//		temp_IRQn = USART2_IRQn;
	}
	
	//DMA1_Stream1 ��о
	if(USARTx == USART3)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA1 , 
								 DMA1_Stream1 , \
								 DMA_Channel_4, \
								 (uint32_t)&USART3->DR , \
						     /*(uint32_t)uart_power_detection_rx , */
								 (uint32_t)Uart_Rx_Buffer3 ,
								 DMA_DIR_PeripheralToMemory , \
								 /*UART_power_detection_rx_SIZE , \*/
								 SizeUart_Rx_Buffer3 ,
								 DMA_Priority_Low);    
		MyNVIC_Config(DMA1_Stream1_IRQn , 12 , 0 , ENABLE);
		temp_IRQn = USART3_IRQn;
	}
	
	if(USARTx == UART4)			//DMA1_Stream2
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream2,DMA_Channel_4,(uint32_t)&UART4->DR,(uint32_t)UART_RX_BUFFER[3],DMA_DIR_PeripheralToMemory,UART_RXBuffer_SIZE,DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream2_IRQn,2,2,ENABLE);
//		temp_IRQn = UART4_IRQn;
	}
	if(USARTx == UART5)			//DMA1_Stream0
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream0,DMA_Channel_4,(uint32_t)&UART5->DR,(uint32_t)UART_RX_BUFFER[4],DMA_DIR_PeripheralToMemory,UART_RXBuffer_SIZE,DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream0_IRQn , 15 , 0 , ENABLE);
//		temp_IRQn = UART5_IRQn;
	}
	
	//DMA2_Stream1 ת̨
	if(USARTx == USART6)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
//								 DMA2_Stream1 , \
//								 DMA_Channel_5 , \
//								 (uint32_t)&USART6->DR , \
//								 (uint32_t)UART_RX_BUFFER2 , \
//								 DMA_DIR_PeripheralToMemory , \
//								 ZT_UARTRXBuffer_SIZE ,  \
//								 DMA_Priority_Low);    
//		MyNVIC_Config(DMA2_Stream1_IRQn , 3 , 0 , ENABLE);
//		temp_IRQn = USART6_IRQn;
	}
	
	
	
	/*DMA�����ж����� DMA2_Stream7 lm1266*/
	if(USARTx == USART1)	
	{
		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
								 DMA2_Stream7 , \
								 DMA_Channel_4 , \
							   (uint32_t)&USART1->DR , \
								 /*(uint32_t)uart_power_detection_tx , \*/
								 (uint32_t)Uart_Tx_Buffer1 ,
								 DMA_DIR_MemoryToPeripheral , \
								 /*UART_power_detection_SIZE , \*/
								 SizeUart_Tx_Buffer1 ,
								 DMA_Priority_Low);
		MyNVIC_Config(DMA2_Stream7_IRQn , 10 , 0 , ENABLE);
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);   //ʹ���ж�
	}
	
	//DMA1_Stream6 ��Դ���ڷ���
	if(USARTx == USART2)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1 , \
//								 DMA1_Stream6 , \
//								 DMA_Channel_4 , \
//								 (uint32_t)&USART2->DR ,\
//								 (uint32_t)dma_send_buffer_gy ,\
//							   DMA_DIR_MemoryToPeripheral , \
//								 GY_UARTTXBuffer_SIZE , \
//								 DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream6_IRQn , 7 , 0 , ENABLE);
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);   //ʹ���ж�
	}
	
	
	//DMA1_Stream3 ��Դ���
	if(USARTx == USART3)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA1 , 
								 DMA1_Stream3 , \
							   DMA_Channel_4 , \
							   (uint32_t)&USART3->DR , \
								 /*(uint32_t)uart_power_detection_tx , \*/
								 (uint32_t)Uart_Tx_Buffer3 ,
								 DMA_DIR_MemoryToPeripheral , \
								 /*UART_power_detection_SIZE , \*/
								 SizeUart_Tx_Buffer3 ,
								 DMA_Priority_Low);
		MyNVIC_Config(DMA1_Stream3_IRQn , 13 , 0 , ENABLE);
		DMA_ITConfig(DMA1_Stream3 , DMA_IT_TC,ENABLE);   //ʹ���ж�
	}
	
	if(USARTx == UART4)			//DMA1_Stream4
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream4,DMA_Channel_4,(uint32_t)&UART4->DR,(uint32_t)UART_TX_BUFFER[3],DMA_DIR_MemoryToPeripheral,UART_TXBuffer_SIZE,DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream4_IRQn,2,2,ENABLE);
//		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);   //ʹ���ж�
	}
	if(USARTx == UART5)			//DMA1_Stream7
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream7,DMA_Channel_4,(uint32_t)&UART5->DR,(uint32_t)UART_TX_BUFFER[4],DMA_DIR_MemoryToPeripheral,UART_TXBuffer_SIZE,DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream7_IRQn , 2 , 2 , ENABLE);
//		DMA_ITConfig(DMA1_Stream7 , DMA_IT_TC , ENABLE);   //ʹ���ж�
//		DMA_Cmd(DMA1_Stream7 , DISABLE);  
	}
	
	
	
	//DMA2_Stream6 ת̨����
	if(USARTx == USART6)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA2 ,\
//								 DMA2_Stream6 ,\
//								 DMA_Channel_5 ,\
//								 (uint32_t)&USART6->DR , \
//								 (uint32_t)dma_send_buffer_zt, \
//								 DMA_DIR_MemoryToPeripheral , \
//								 15 , \
//								 DMA_Priority_Low);
//		MyNVIC_Config(DMA2_Stream6_IRQn , 0 , 7 , ENABLE);
//		DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);   //ʹ���ж�
	}
	
	/*����DMA��ʽ����*/
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);	
	USART_ITConfig(USARTx,USART_IT_IDLE,ENABLE);  	
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);	  
	MyNVIC_Config(temp_IRQn , prepriority , subpriority , ENABLE);//�򿪴����ж�   
	
  USART_Cmd(USARTx, ENABLE);    //�������� 

	SizeUart_Tx_Buffer1 = 32;	
	SizeUart_Tx_Buffer3 = 32;
}


/**
  * @brief  USART1_IRQHandler
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) 				
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		DMA_Rx(USART1);
	}		
}

/**
  * @brief  ����1�����жϺ���
  * @param  None
  * @retval None
  */
void DMA2_Stream7_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);						//���DMA2_Steam7������ɱ�־
		USART_ClearFlag(USART1,USART_FLAG_TC);								//�������2�ı�־λ
		DMA_Cmd(DMA2_Stream7, DISABLE);                      	//�ر�DMA���� 
	}	
}

/**
  * @brief  USART2_IRQHandler
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void) 
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET){
		DMA_Rx(USART2);
	}	
}

/**
  * @brief  DMA1_Stream6_IRQHandler ����2�����жϺ���
  * @param  None
  * @retval None
  */
void DMA1_Stream6_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);				//���DMA2_Steam7������ɱ�־
		USART_ClearFlag(USART2,USART_FLAG_TC);						//�������2�ı�־λ
		DMA_Cmd(DMA1_Stream6, DISABLE);                   //�ر�DMA���� 
	}	
}

/**
  * @brief  USART3_IRQHandler
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET){
		DMA_Rx(USART3);
	}	
}


/**
  * @brief  USART3_IRQHandler ����3�����жϺ���
  * @param  None
  * @retval None
  */
void DMA1_Stream3_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);							//���DMA2_Steam7������ɱ�־
		DMA_Cmd(DMA1_Stream3, DISABLE);                      		//�ر�DMA���� 
	}	
}


/**
  * @brief  UART4_IRQHandler
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) 	
{
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET){
		DMA_Rx(UART4);
	}	
}


/**
  * @brief  DMA1_Stream4_IRQHandler  ����4�����жϺ���
  * @param  None
  * @retval None
  */
void DMA1_Stream4_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);							//���DMA2_Steam7������ɱ�־
		DMA_Cmd(DMA1_Stream4, DISABLE);                      		//�ر�DMA���� 
	}	
}

/**
  * @brief  UART5_IRQHandler 
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void) 			
{
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET){
		DMA_Rx(UART5);
	}	
}


/**
  * @brief  DMA1_Stream7_IRQHandler  ����5�����жϺ���
  * @param  None
  * @retval None
  */
void DMA1_Stream7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
		DMA_Cmd(DMA1_Stream7, DISABLE);                      //�ر�DMA���� 
	}	
}



void USART6_IRQHandler(void) 			
{
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET){
		DMA_Rx(USART6);
	}	
}



void DMA2_Stream6_IRQHandler(void)	//����6�����жϺ���
{
	if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
	{ 
		DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־
		DMA_Cmd(DMA2_Stream6, DISABLE);                      //�ر�DMA���� 
	}	
}


void processData(unsigned char *dataPtr) 
{
    // �����ﴦ�����ݣ�ʹ�� *dataPtr ��������
   //printf("Received value in File1: %d\n", *dataPtr);
	
	*dataPtr = Number_Rx_Buffer1;
}

//ʹ��DMA�������ݽ���ʱ���øú���,��Ҫ�ڴ��ڽ����ж��е��ã���Ϊʹ��DMA���գ����ջ����ַ����ǰԤ�ã����ұ�־λҲ�̶�����˲�֧����������ͱ�־λ����
void DMA_Rx(USART_TypeDef* USARTx)	
{
	uint8_t num;
	DMA_Stream_TypeDef* DMAy_Streamx;
	uint32_t Flag;
	
	if(USARTx == USART1)
	{
		DMAy_Streamx = DMA2_Stream2;
		num = 0;
		Flag = DMA_FLAG_TCIF2;
		Number_Rx_Buffer1 = SizeUart_Rx_Buffer1 - DMA_GetCurrDataCounter(DMA2_Stream2);/*���ݳ��ȼ���*/
		if(RB_Get_FreeSize(&rb_uart1_rx) != 0)	/*�������*/
		{
			for(int i = 0 ; i < Number_Rx_Buffer1 ; i++)
			{
				RB_Write_Byte(&rb_uart1_rx, Uart_Rx_Buffer1[i]);
			}
		}	
	}
	
	if(USARTx == USART2)
	{
		DMAy_Streamx = DMA1_Stream5;
		num = 1;
		Flag = DMA_FLAG_TCIF5;
	}
	
	if(USARTx == USART3)
	{
		DMAy_Streamx = DMA1_Stream1;
		num = 2;
		Flag = DMA_FLAG_TCIF1;
		Number_Rx_Buffer3 = SizeUart_Rx_Buffer3 - DMA_GetCurrDataCounter(DMA1_Stream1);	/*���ݳ��ȼ���*/
		if(RB_Get_FreeSize(&rb_uart3_rx) != 0)	/*�������*/
		{
			for(int i = 0 ; i < Number_Rx_Buffer3 ; i++)
			{
				RB_Write_Byte(&rb_uart3_rx, Uart_Rx_Buffer3[i]);
			}
		}
	}
	
	if(USARTx == UART4)
	{
		DMAy_Streamx = DMA1_Stream2;
		num = 3;
		Flag = DMA_FLAG_TCIF2;
	}
	
	if(USARTx == UART5)
	{
		DMAy_Streamx = DMA1_Stream0;
		num = 4;
		Flag = DMA_FLAG_TCIF0;
	}
	
	if(USARTx == USART6)
	{
		DMAy_Streamx = DMA2_Stream1;
		num = 5;
		Flag = DMA_FLAG_TCIF1;
	}
	
	USARTx->SR;
  USARTx->DR; 																																		//��USART_IT_IDLE��־
  DMA_Cmd(DMAy_Streamx,DISABLE);																									//�ر�DMA
	DMA_ClearFlag(DMAy_Streamx,Flag);																								//�����־λ
	//Uart_RData_Len[num] = UART_RXBuffer_SIZE - DMA_GetCurrDataCounter(DMAy_Streamx);	//��ý���֡֡��
  DMA_Cmd(DMAy_Streamx,ENABLE);																										//��DMA
	//UART_DATA_FULL[num] = true;
}

//ʹ��DMA�����ݷ���ʱ���øú����������ݸ�ֵ����Ӧ����������øú����������ⲿ���ã���Ϊʹ��DMA���ͣ����͵�ַ����ǰ���úã���˲�֧���������鷢��
void DMA_Tx(USART_TypeDef* USARTx , uint16_t Len)		
{	
	DMA_Stream_TypeDef* DMAy_Streamx;
	
	if(USARTx == USART1){
		DMAy_Streamx = DMA2_Stream7;
	}	
	if(USARTx == USART2){
		DMAy_Streamx = DMA1_Stream6;
	}	
	if(USARTx == USART3){
		DMAy_Streamx = DMA1_Stream3;
	}	
	if(USARTx == UART4){
		DMAy_Streamx = DMA1_Stream4;
	}		
	if(USARTx == UART5){
		DMAy_Streamx = DMA1_Stream7;
	}		
	if(USARTx == USART6){
		DMAy_Streamx = DMA2_Stream6;
	}	
//  if(USARTx == USART_485) IO_ON(DIR_485);
	DMA_Cmd(DMAy_Streamx , DISABLE);                      //�ر�DMA���� 
	DMA_SetCurrDataCounter(DMAy_Streamx , Len);	
	DMA_Cmd(DMAy_Streamx , ENABLE);   
//  if(USARTx == USART_485)
//  {
//    delay_us(4200*Len);
//    IO_OFF(DIR_485);
//  }
}


//���ڵ�GPIO�����Ӻ���������1Ϊ�����õĴ��ںţ�����2/4Ϊ������������GPIOD������3/5Ϊ����ܽźţ�����GPIO_PinSource8
void MyGPIO_Config(USART_TypeDef* USARTx , GPIO_TypeDef* GPIOx1 , uint8_t GPIO_PinSourcex1 , GPIO_TypeDef* GPIOx2 , uint8_t GPIO_PinSourcex2)
{
	uint8_t AF_Flag;

 	GPIO_InitTypeDef GPIO_InitStructure;
	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE); 
	  
	if((USARTx == USART1) || (USARTx == USART2) || (USARTx == USART3))	AF_Flag = 0x07;
	else	AF_Flag = 0x08;

	GPIO_PinAFConfig(GPIOx1,GPIO_PinSourcex1,AF_Flag);  
	GPIO_PinAFConfig(GPIOx2,GPIO_PinSourcex2,AF_Flag);
  
	GPIO_InitStructure.GPIO_Pin = 		(0x0001 << GPIO_PinSourcex1);//GPIO_Pin_8;//�ܽ�ָ��
  GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_AF;	//�ܽ�ģʽ:�����
  GPIO_InitStructure.GPIO_OType = 	GPIO_OType_PP;	//����:����ģʽ
  GPIO_InitStructure.GPIO_PuPd = 		GPIO_PuPd_UP;	//������������
	GPIO_InitStructure.GPIO_Speed = 	GPIO_Speed_2MHz;//IO���ٶ�
  GPIO_Init(GPIOx1, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = 		(0x0001 << GPIO_PinSourcex2);//GPIO_Pin_8;//�ܽ�ָ��
  GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_AF;	//�ܽ�ģʽ:�����
  GPIO_InitStructure.GPIO_OType = 	GPIO_OType_PP;	//����:����ģʽ
  GPIO_InitStructure.GPIO_PuPd = 		GPIO_PuPd_UP;	//������������
	GPIO_InitStructure.GPIO_Speed = 	GPIO_Speed_2MHz;//IO���ٶ�
  GPIO_Init(GPIOx2, &GPIO_InitStructure);//��ʼ��
}


//���ڵ�USART���������Ӻ���������1Ϊ�����õĴ��ںţ�����2Ϊ������
void Myuart_Config(USART_TypeDef* USARTx , uint32_t bound , bool IsSbus)
{
	USART_InitTypeDef USART_InitStructure;//���崮�ڽṹ��  
	
	if(USARTx == USART1)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	if(USARTx == USART2)	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	if(USARTx == USART3)	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	if(USARTx == UART4)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	if(USARTx == UART5)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	
	if(USARTx == USART6)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
  if(IsSbus)
  {
		USART_InitStructure.USART_StopBits = USART_StopBits_2;  
		USART_InitStructure.USART_Parity = USART_Parity_Even;  
    USART_InitStructure.USART_BaudRate = 100000; 
  }
  else
  {
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_BaudRate = bound; 
  }
  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
  
  USART_Init(USARTx,&USART_InitStructure);  //��ʼ������ 
}


//DMA���ú���������1��ʹ����·DMA������2��ʹ�õ���������ţ�����3��ʹ�õ�ͨ����ţ�����5�������ַ������6���ڴ��ַ������7�����䷽�򣻲���8����������С������9�����ȼ�
void MyDMA_Config(uint32_t DMAx,DMA_Stream_TypeDef* DMAy_Streamx,uint32_t DMA_Channel_x,uint32_t cpar,uint32_t cmar,uint32_t dma_dir,uint16_t bufsize,uint32_t priority)
{
	DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��
	
	RCC_AHB1PeriphClockCmd(DMAx, ENABLE);																			//����DMAʱ��
	DMA_DeInit(DMAy_Streamx);																									//DMAͨ������
	DMA_InitStructure.DMA_Channel = DMA_Channel_x;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;													//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = cmar;															//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = dma_dir;																			//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = bufsize;																//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;															//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = priority;																//����DMA�����ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    								//ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 							//ָ����Burstת�������ڴ洫��       
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 			//ָ����Burstת��������Χת��      
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);  
	DMA_Cmd(DMAy_Streamx,ENABLE);																							//ʹ��ͨ��
}

//ͨ���ж����ú���
void MyNVIC_Config(uint8_t NVIC_IRQChannel,uint8_t MainPriority,uint8_t SubPriority,FunctionalState NewState)		
{
	NVIC_InitTypeDef NVIC_InitStructure ;//�����жϽṹ��
	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MainPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
	NVIC_Init(&NVIC_InitStructure);
}
