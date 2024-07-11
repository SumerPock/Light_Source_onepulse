#include "bsp.h"

#define START_SYMBOL    0xEA
#define END_SYMBOL      0xED
#define CLARITY_SYMBOL  0xEB

void HandleEndSymbol(void);
void HandleStartSymbol(void);
void HandleValidData(uint8_t inputCharacter);

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
void UART_RemoteDecode(uint8_t inputCharacter)
{
    if (RemoteDataPacket.Start_Flag == 1)
    {
        if (RemoteDataPacket.Clarity_Flag == 1)
        {
            if (inputCharacter == START_SYMBOL || inputCharacter == CLARITY_SYMBOL || inputCharacter == END_SYMBOL)
            {
                UART_Inbuff(inputCharacter);
            }
            else
            {
                RemoteDataPacket.Start_Flag = 0;
                RemoteDataPacket.Clarity_Flag = 0;
                RemoteDataPacket.RX_BUF[0] = 0;
                // ��Ӵ��������
            }
        }
        else if (inputCharacter == CLARITY_SYMBOL)
        {
            RemoteDataPacket.Clarity_Flag = 1;
        }
        else if (inputCharacter == END_SYMBOL)
        {
            HandleEndSymbol();
        }
        else if (inputCharacter == START_SYMBOL)
        {
            HandleStartSymbol();
        }
        else
        {
            HandleValidData(inputCharacter);
        }
    }
    else if (inputCharacter == START_SYMBOL)
    {
        HandleStartSymbol();
    }
    // ���� Else��δ�����Ҳ�����ʼ�����ж������ݲ�����Ч���ݣ���������ִ���κβ���
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
void HandleStartSymbol()
{
    RemoteDataPacket.Start_Flag = 1;
    RemoteDataPacket.RX_BUF[0] = 0;
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
void HandleEndSymbol()
{
    RemoteDataPacket.Start_Flag = 0;
    RemoteDataPacket.Clarity_Flag = 0;
    RemoteDataPacket.End_Flag = 1;
		UART_RemoteCMD_Process(); 				// ���ݰ�������ϣ��������ݴ������
    RemoteDataPacket.End_Flag = 0;    // ������ϣ�End�������!!!
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
void HandleValidData(uint8_t inputCharacter)
{
    UART_Inbuff(inputCharacter);
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
void HandleClaritySymbol(uint8_t inputCharacter)
{
    RemoteDataPacket.Clarity_Flag = 1;
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
void UART_Inbuff(uint8_t inputCharacter)
{
    if (RemoteDataPacket.RX_BUF[0] > 10)
    {
        RemoteDataPacket.RX_BUF[0] = 0;
        RemoteDataPacket.Start_Flag = 0;
        RemoteDataPacket.Clarity_Flag = 0;
        RemoteDataPacket.End_Flag = 0;
    }
    else
    {
        RemoteDataPacket.RX_BUF[0]++;
        RemoteDataPacket.RX_BUF[RemoteDataPacket.RX_BUF[0]] = inputCharacter;
        RemoteDataPacket.Clarity_Flag = 0;
    }
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
//void SendDataArray(USART_TypeDef* USARTx, unsigned char* dataArray, unsigned int length)
//{
//    for (unsigned int i = 0; i < length; i++)
//    {
//        Dong_Uart_Send(USARTx, dataArray[i]);
//    }
//}

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
//void Dong_Uart_Send(USART_TypeDef* USARTx , unsigned char Data)
//{
//	// �ȴ����ͻ�����Ϊ��
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
//	{
//	}

//	// �����ݷ��͵� USART ���ݼĴ���
//	USART_SendData(USARTx , Data);         		

//	// �ȴ��������
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//	{
//	}
//}

