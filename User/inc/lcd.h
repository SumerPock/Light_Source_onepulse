#ifndef __LCD_H
#define __LCD_H	

#include "stm32f4xx.h" 

#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif

/**************************************
// �ⲿ��˵���Ĺܽ�ӳ��Ͷ��ܷ����ĳ���ͬ���������⣡����
SCL --- LCD.3 -- PA5 -- SPI_SCK
SDA --- LCD.4 -- PA7 -- SPI_MOSI
RES --- LCD.5 -- PC4 -- LCD_RESET
DC  --- LCD.6 -- PA6 -- LCD_DC
CS  --- LCD.7 -- PA4 -- SPI_NSS
BLK --- LCD.8 -- PC5 -- LCD_BLK
**************************************/

/**
// ���Ƕ��ܷ����Ĺܽ�ӳ��
SCL---OLED.3--PA5--SPI_SCK
SDA---OLED.4--PA7--SPI_MOSI
RES---OLED.5--PC4--OLED_RES
DC----OLED.6--PC5--OLED_DC
CS----OLED.2--PA15--SPI_NSS
BLK---OLED 	--PE6-
**/

#define LCD_Port			SPI1

//CLK-PA5
#define LCD_SCLK_Clr			GPIO_ResetBits(GPIOA,	GPIO_Pin_5)
#define LCD_SCLK_Set			GPIO_SetBits(GPIOA,		GPIO_Pin_5)
#define LCD_SCLK_Toogle		GPIO_ToggleBits(GPIOA,GPIO_Pin_5)

//DIN-PA7
#define LCD_SDIN_Clr			GPIO_ResetBits(GPIOA,	GPIO_Pin_7)
#define LCD_SDIN_Set			GPIO_SetBits(GPIOA,		GPIO_Pin_7)
#define LCD_SDIN_Toogle		GPIO_ToggleBits(GPIOA,GPIO_Pin_7)

//RES-PC4
#define LCD_RES_Clr			GPIO_ResetBits(GPIOC,	GPIO_Pin_4)
#define LCD_RES_Set			GPIO_SetBits(GPIOC,		GPIO_Pin_4)
#define LCD_RES_Toogle	GPIO_ToggleBits(GPIOC,GPIO_Pin_4)

//DC-PA6
#define LCD_DC_Clr			GPIO_ResetBits(GPIOA,	GPIO_Pin_6)
#define LCD_DC_Set			GPIO_SetBits(GPIOA,		GPIO_Pin_6)
#define LCD_DC_Toogle		GPIO_ToggleBits(GPIOA,GPIO_Pin_6)

//BLK-PC5
#define LCD_BLK_Clr			GPIO_ResetBits(GPIOC,	GPIO_Pin_5)
#define LCD_BLK_Set			GPIO_SetBits(GPIOC,		GPIO_Pin_5)
#define LCD_BLK_Toogle	GPIO_ToggleBits(GPIOC,GPIO_Pin_5)

#define LCD_CMD  0	//д����
#define LCD_DATA 1	//д����

extern  uint16_t BACK_COLOR;   //����ɫ

void LCD_PortInit(void);
void LCD_Init(void);
uint16_t LCD_Write_Byte(uint16_t TxData);

void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_Clear(uint16_t Color);
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t index, uint8_t size, uint16_t color, uint16_t backcolor);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawPoint_big(uint16_t x,uint16_t y,uint16_t colory);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode,uint16_t color);
void LCD_ShowString(uint16_t x,uint16_t y,const char *p,uint16_t color);
uint32_t mypow(uint8_t m,uint8_t n);
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t color);
void LCD_ShowNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t color);
void LCD_ShowPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void LCD_ShowBigChar(uint16_t x, uint16_t y, uint8_t index, uint16_t color, uint16_t backcolor);
void LCD_ShowBigString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t color, uint16_t backcolor);
void LCD_Print(uint16_t x, uint16_t y, char *p, uint16_t color, uint16_t backcolor);
uint8_t Search_ch32x32(const uint8_t *p);
void Number_Highlight_Show(uint16_t X_Pos, uint16_t Y_Pos, uint32_t ShowNumber, uint8_t Show_ENOBs, uint8_t RadixPointBit, uint8_t HighlightBit, 
													 uint16_t color, uint16_t backcolor, uint16_t hcolor, uint16_t hbcolor);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
					  		 
#endif   		     
