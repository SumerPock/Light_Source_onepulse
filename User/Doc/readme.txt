���������������������ʹ��
1. 	EN5		PB0
2.	EN6		PB1

LED0:	PE6
LED1:	PC13(Υ��Datasheet������׼��Ҫ����������)

ARM USART1_TX:	PA9		-->		CPLD: Pin13
ARM USART1_RX:	PA10	<-- 	CPLD: Pin12	

ARM USART3_TX:	PB10	-->		CPLD: Pin65
ARM USART3_RX:	PB11	<-- 	CPLD: Pin66	

�������ź� 	BM1 --> CPLD:Pin57
			BM2 --> CPLD:Pin58
			BM3 --> CPLD:Pin59
�������J4	AN1	-->	CPLD:Pin37
������J9	AN2	-->	CPLD:Pin45
ȡ����ťJ10	AN3 --> CPLD:Pin47

�������ź� 	BM1 --> CPLD:Pin57
			BM2 --> CPLD:Pin58
			BM3 --> CPLD:Pin59
 
ͨѶЭ��
1.����Ӧ�����ݰ�
Byte_1: ID: 0x80
Byte_2: ��Ʒ����״̬��Bit0:0/1 �������ر�/����, Bit1:0/1 ���ر�/����
Byte_3: �������¶ȣ�1LSB=0.5�棬0x0 = -50��
Byte_4: �������

ͨ�����ڴ���itoa�ı�ѡ������sprintf / snprintf����Щ��stdio.h��һ���֡�

α�������洢��ʽ
12Byte���� + 2Byte��Ƶ + 2Byte���� + ����[1016*2Byte] = 2048byte = 2Kbytes

2024.05.06����ָ�����������������
ָ�0x51
�������ݴ�С uint16
�޸�Դ�ļ�Ϊ bsp.c
