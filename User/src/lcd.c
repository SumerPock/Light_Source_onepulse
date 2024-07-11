
#include "lcd_font.h"
#include "bmp.h"
#include "bsp.h"


uint16_t BACK_COLOR = BLUE;   //����ɫ

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;			  
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr;
		if(dat&0x80)
		   LCD_SDIN_Set;
		else 
		   LCD_SDIN_Clr;
		LCD_SCLK_Set;
		dat<<=1;   
	}			
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set;//д����
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_DC_Set;//д����
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr;//д����
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	LCD_WR_REG(0x2a);//�е�ַ����
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);//�е�ַ����
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);//������д
}

/************************************************
�������ƣ� LCD_Init
��    �ܣ� ��ʼ��LCD����SPI�ӿ�
��    ���� ��
�� �� ֵ�� ��
��    �ߣ� gaoyao
*************************************************/
void LCD_PortInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��PORTA��PORTCʱ��

	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
//	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

	//GPIO��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
}

void LCD_Init(void)
{
	LCD_PortInit();
	
	LCD_RES_Clr;
	delay_ms(10);//	delay_ms(200);
	LCD_RES_Set;
	delay_ms(10);//	delay_ms(200);
	LCD_BLK_Set;
	delay_ms(10);//	delay_ms(200);
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0xCF);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xC1);
	LCD_WR_DATA8(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x64);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0X12);
	LCD_WR_DATA8(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x79);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA8(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC0); //Power control
	LCD_WR_DATA8(0x1D); //VRH[5:0]
	LCD_WR_REG(0xC1); //Power control
	LCD_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5); //VCM control
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_REG(0xC7); //VCM control
	LCD_WR_DATA8(0x92);
	LCD_WR_REG(0x3A); // Memory Access Control
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0x36); // Memory Access Control
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xB6); // Display Function Control
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0xA2);

	LCD_WR_REG(0x44);
	LCD_WR_DATA8(0x02);

	LCD_WR_REG(0xF2); // 3Gamma Function Disable
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x26); //Gamma curve selected
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0xB8);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x47);
	LCD_WR_DATA8(0x4B);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(5);//	delay_ms(120);
	LCD_WR_REG(0x29); //Display on
}

/******************************************************************************
      ����˵����LCD��������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}
;
/******************************************************************************
      ����˵����LCD��ʾ����
      ������ݣ�x,y   ��ʼ����
                index ���ֵ����
                size  �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t index, uint8_t size, uint16_t color, uint16_t backcolor)	
{  
	uint8_t i, j, size1;
	const uint8_t *temp;
	
	x = x+8;
	if(size==16)
	{
		temp = Hzk16;
	}//ѡ���ֺ�
	if(size==32)
	{
		temp = Hzk32;
	}
  LCD_Address_Set(x , y , x + size - 1 , y + size - 1); //����һ�����ֵ�����
  size1 = size * size / 8;//һ��������ռ���ֽ�
	temp+= index * size1;//д�����ʼλ��
	for(j = 0 ; j < size1 ; j++)
	{
		for(i = 0 ; i < 8 ; i++)
		{
		 	if((*temp&(1<<i)) != 0)//�����ݵĵ�λ��ʼ��
			{
				LCD_WR_DATA(color);//����
			}
			else
			{
				LCD_WR_DATA(backcolor);//������
			}
		}
		temp++;
	 }
}


/******************************************************************************
      ����˵����LCD����
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      ����˵����LCD��һ����ĵ�
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint_big(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Fill(x-1,y-1,x+1,y+1,color);
} 


/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
      ����ֵ��  ��
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	//int di;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}


/******************************************************************************
      ����˵������ʾ�ַ�
      ������ݣ�x,y    �������
                num    Ҫ��ʾ���ַ�
                mode   1���ӷ�ʽ  0�ǵ��ӷ�ʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode,uint16_t color)
{
	uint8_t  temp = 0;
	uint8_t  pos = 0, t = 0;
	uint16_t x0 = x;     
	if(x > LCD_W - 16 || y > LCD_H - 16)
		return;	    		//���ô���		   
	num = num - ' ';	//�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x , y , x + 8 - 1 , y + 16 - 1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos = 0 ; pos < 16 ; pos++)
		{ 
			temp = asc2_1608[(uint16_t)num * 16 + pos];		 //����1608����
			for(t = 0 ; t < 8 ; t++)
		    {                 
		        if(temp & 0x01)
							LCD_WR_DATA(color);
				    else 
							LCD_WR_DATA(BACK_COLOR);
				    temp>>=1; 
				    x++;
		    }
			x = x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos = 0 ; pos < 16 ; pos++)
		{
		    temp = asc2_1608[(uint16_t)num*16+pos];		 //����1608����
				for(t=0;t<8;t++)
				{                 
					if(temp&0x01)
						LCD_DrawPoint(x+t,y+pos,color);//��һ����     
					temp>>=1; 
				}
		}
	}   	   	 	  
}


/******************************************************************************
      ����˵������ʾ16*32����ַ�
      ������ݣ�x,y    �������
                num    Ҫ��ʾ���ַ�
								color/backcolor  ��ɫ/����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowBigChar(uint16_t x, uint16_t y, uint8_t index, uint16_t color, uint16_t backcolor)
{
	uint8_t i, j, size1;
	const uint8_t *temp;
	x = x + 8;
	temp = Half_zk3216;//ѡ���ֺ�
  LCD_Address_Set(x , y , x + 16 - 1 , y + 32 - 1); //����һ�����ֵ�����
  size1 = 64;//һ��������ռ���ֽ�
	index = index - ' ';
	temp += index * size1;//д�����ʼλ��
	for(j =0 ; j < size1 ; j++)
	{
		for(i = 0 ; i < 8 ; i++)
		{
		 	if((*temp&(1<<i))!=0)//�����ݵĵ�λ��ʼ��
			{
				LCD_WR_DATA(color);//����
			}
			else
			{
				LCD_WR_DATA(backcolor);//������
			}
		}
		temp++;
	 }
}

/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y    �������
                *p     �ַ�����ʼ��ַ
								color/backcolor  ��ɫ/����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowBigString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t color, uint16_t backcolor)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=32;}
        if(y>LCD_H-32){y=x=0;}
        LCD_ShowBigChar(x, y, *p, color, backcolor);
        x+=16;
        p++;
    }  
}

// ���������ַ�
uint8_t Search_ch32x32(const uint8_t *p)
{
	uint8_t j=0, Index=255;
	while(Font32x32_Index[j] > 127)
	{
		if(Font32x32_Index[j] == *p)
		{
			if(Font32x32_Index[j+1] == *(p+1))
			{
				Index = j>>1;
				break;
			}
		}
		j+=2;
	}
	return Index;
}


// ���ȫ�Ǻ��ֺͰ���ַ��Ļ���ַ���
void LCD_Print(uint16_t x, uint16_t y, char *p, uint16_t color, uint16_t backcolor)
{
	uint8_t char_Index;
	uint8_t single_ch[3];
	
	while(*p != '\0')
	{
		if(*p > 127)
		{	// ��ȫ�Ǻ���
			single_ch[0] = *p;
			single_ch[1] = *(p+1);
			single_ch[2] = '\0';
			if(x>LCD_W-40){x=0;y+=32;}
      if(y>LCD_H-40){y=x=0;}
			char_Index = Search_ch32x32(single_ch);// ����ȫ�Ǻ������ֿ��е�λ��
			LCD_ShowChinese(x, y, char_Index, 32, color, backcolor);   //
			x += 32;
			p += 2;
		}
		else
		{	// �ǰ���ַ�
			if(x>LCD_W-24){x=0;y+=32;}
      if(y>LCD_H-40){y=x=0;}
			single_ch[0] = *p;
			single_ch[1] = '\0';
			LCD_ShowBigString(x, y, single_ch, color, backcolor);
			x += 16;
			p += 1;
		}
	}
}

/* ������ʾ���ֵ�ĳ��λ
// ��������϶࣬һ��10�������õ�ʱ����Ҫ�ر�ע��
	X_Pos��Y_Pos��	��ʾ������λ��
	ShowNumber��		��Ҫ��ʾ������
	Show_ENOBs��		��ʾ��λ���������ǰ���ÿո��룩
	RadixPointBit��	С������λ�������磺����5000��λ��3������ʾ5.000��
	HighlightBit��	��Ҫ������ʾ�ĵ���λ����0������������1�������λ������ע���޳�С���㣩
	color��backcolor��hcolor��hbcolor������������ɫ������ɫ��������ɫ�������ı���ɫ
*/
void Number_Highlight_Show(uint16_t X_Pos, uint16_t Y_Pos, uint32_t ShowNumber, uint8_t Show_ENOBs, uint8_t RadixPointBit, uint8_t HighlightBit, 
													 uint16_t color, uint16_t backcolor, uint16_t hcolor, uint16_t hbcolor)
{
	char str[10], char_temp[2];
	uint8_t i, str_lenght;
	if(Show_ENOBs > 8) Show_ENOBs = 8;	// ���ݱ�����ʩ
	if(RadixPointBit > Show_ENOBs) RadixPointBit = Show_ENOBs;// ���ݱ�����ʩ
	if(HighlightBit > Show_ENOBs) HighlightBit = 0;// ���ݱ�����ʩ
	switch(Show_ENOBs)
	{	// ������Ҫ�����ĳ��ȣ���ȡĩλ����Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
		case 3:
			ShowNumber = ShowNumber % 1000;
			break;
		case 4:
			ShowNumber = ShowNumber % 10000;
			break;
		case 5:
			ShowNumber = ShowNumber % 100000;
			break;
		case 6:
			ShowNumber = ShowNumber % 1000000;
			break;
		case 7:
			ShowNumber = ShowNumber % 10000000;
			break;
		default:
			ShowNumber = 99999999;
			break;		
	}
	
	snprintf(str, Show_ENOBs+1, "%d", ShowNumber);	// ����ת�ַ���
	str_lenght = (uint8_t)strlen(str);	//��ȡ�ַ������ȣ����ں����ѭ������
	if(str_lenght < Show_ENOBs)	// ������ݳ���С��Ҫ�󳤶ȣ�Ϊ��֤���ݶ���ǰ�油�ո�
	{
		for(i=0; i<str_lenght+1; i++)	//�����ݺ��ƣ�+1�����ŵ���/0Ҳ��Ҫ�ƶ�
		{	//ѭ������
			str[Show_ENOBs-i] = str[str_lenght-i];
		}
		for(i=0; i<(Show_ENOBs-str_lenght); i++)
		{	//ǰ�油�ո�
			str[i] = ' ';//0x20;
		}
	}

	if(RadixPointBit > 0)	// ���С����λ������0������Ƽӵ�
	{
		str_lenght = (uint8_t)strlen(str);	//�ٴθ����ַ�������			
		for(i=0; i<(RadixPointBit+1); i++)
		{	//С���������ַ����κ���һλ
			str[str_lenght-i+1] = str[str_lenght-i];
		}
		str[str_lenght-RadixPointBit] = '.';//0x2E;
		if(str[str_lenght-RadixPointBit-1] == ' ') str[str_lenght-RadixPointBit-1] = '0';	//���С����ǰһλ�ǿո����滻Ϊ0
	}
	
	
	
	//���´����������ַ���������Ҫ����Ӧ�ַ�����
	i=0;
	char_temp[1] = '\0';
	Show_ENOBs++;	//��Ϊ������һ��С���㣬��Ӧ�ܳ��ȱ仯
	if(HighlightBit > RadixPointBit) HighlightBit++;	// ���������λ������С������λ���������λ��+1����ΪС����ռ����1λ
	while(str[i]!='\0')
  {
		char_temp[0] = str[i];
		if(HighlightBit == (Show_ENOBs - i))
			LCD_Print(X_Pos+16*i, Y_Pos, char_temp, hcolor, hbcolor);
		else
			LCD_Print(X_Pos+16*i, Y_Pos, char_temp, color, backcolor);
		i++;
	}
}


/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y    �������
                *p     �ַ�����ʼ��ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const char *p, uint16_t color)
{         
	while(*p!='\0')
	{       
		if(x>LCD_W-16){x=0;y+=16;}
		if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
		LCD_ShowChar(x,y,*p,0,color);
		x+=8;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�x,y    �������
                num    Ҫ��ʾ������
                len    Ҫ��ʾ�����ָ���
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t color)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t , y , ' ' , 0 , color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t , y , temp + 48 , 0 , color); 
	}
} 


/******************************************************************************
      ����˵������ʾС��
      ������ݣ�x,y    �������
                num    Ҫ��ʾ��С��
                len    Ҫ��ʾ�����ָ���
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowNum1(uint16_t x , uint16_t y , float num , uint8_t len , uint16_t color)
{         	
	uint8_t t = 0 , temp = 0;
	uint16_t num1 = 0;
	num1 = num * 100;
	for(t = 0 ; t < len ; t++)
	{
		temp = (num1 / mypow(10 , len - t - 1)) % 10;
		if(t == (len - 2))
		{
			LCD_ShowChar(x + 8 * (len - 2) , y , '.' , 0 , color);
			t++;
			len += 1;
		}
	 	LCD_ShowChar(x + 8 * t , y , temp + 48 , 0 , color);
	}
}


/******************************************************************************
      ����˵������ʾ40x40ͼƬ
      ������ݣ�x,y    �������
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	int i;
	  LCD_Address_Set(x1,y1,x2,y2);
		for(i=0;i<1600;i++)
	  { 	
			LCD_WR_DATA8(image[i*2+1]);
			LCD_WR_DATA8(image[i*2]);
	  }			
}



/************************************************
�������ƣ� LCD_Write_Byte
��    �ܣ� ��LCDд��8bit����
��    ���� TxData����LCD��SPI����д�������
�� �� ֵ�� ��SPI���߽��յ�������
��    �ߣ� gaoyao
*************************************************/
uint16_t LCD_Write_Byte(uint16_t TxData)
{
	LCD_RES_Toogle;
	while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);  //�ȴ���������
	SPI_SendData(SPI1, TxData);                              //ͨ������SPIx����һ��byte  ����
	while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET); //�ȴ�������һ��byte
	return SPI_ReceiveData(SPI1);                            //����ͨ��SPIx������յ�����
}

