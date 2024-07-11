#ifndef __BSP_H
#define __BSP_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_syscfg.h"

#include "exit.h"
#include "exit.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "delay.h"
#include "lcd.h"
#include "ui.h"
#include "delay.h"
#include "flash.h"
#include "fsmc.h"
#include "tim.h"
#include "soft_i2c.h"
#include "soft_i2c_lm75b.h"
#include "ring_buffer.h"
#include "ring_buffer_chapter.h"
#include "misc.h" 
#include "RandomCode.h"

#include "bsp_key.h"
#include "bsp_key_oc.h"
#include "bsp_uart_dma.h"
#include "bsp_uart.h"
#include "bsp_ui_showp_foundation.h"
#include "bsp_mode.h"

#include "SEGGER_RTT.h"

#define BUFFER_SIZE 256

#define Zero_Position		GPIOC,GPIO_Pin_15
#define Motor_EN				GPIOA,GPIO_Pin_8
#define Motor_DIR				GPIOC,GPIO_Pin_9
#define Motor_STEP			GPIOC,GPIO_Pin_8
#define EN5							GPIOB,GPIO_Pin_5
#define EN6							GPIOB,GPIO_Pin_6
#define Motor_STEP			GPIOC,GPIO_Pin_8
#define Dir_Up					GPIO_ResetBits(Motor_DIR)
#define Dir_Down				GPIO_SetBits(Motor_DIR)

#define Motor_Power_On	GPIO_ResetBits(Motor_EN)
#define Motor_Power_Off	GPIO_SetBits(Motor_EN)

typedef struct{
	unsigned short opentimer;
	unsigned short closetimer;
	unsigned char auto_lightMode;
	unsigned char Skill_cooldown;
	unsigned char openovertimer;
}LASER_AUTO_CONTROL;
extern LASER_AUTO_CONTROL LaAutoCon;

typedef struct {
    double x;
    double y;
} DataPoint;
extern DataPoint podata[];
extern unsigned char podatanum;
extern double linearInterpolation(double x, DataPoint* data, size_t dataSize);


enum ATTENUATION
{
	ENERGYGEAR_1 = 30,
	ENERGYGEAR_2 = 50,
	ENERGYGEAR_3 = 100,
	ENERGYGEAR_4 = 150,	
	MAX_Attenuation = 200,   
	MIN_Attenuation = 0,
};

enum LIGHT_TIME
{
	MAXOPEN = 200,
	MAXCLOSE = 200,
};

enum SETBOOL
{
	TRUE = 1,
	FALSE = 0,
};

/*挡位范围*/
//enum GEARSET
//{
//	GEAR_1 = 0,
//	GEAR_2 = 99,
//	GEAR_3 = 199,
//	GEAR_4 = 286,
//	GEAR_5 = 386,
//	GEAR_6 = 484,
//	GEAR_7 = 601,
//};

/*挡位范围*/
enum GEARSET
{
	GEAR_1 = 0,
	GEAR_2 = 30,
	GEAR_3 = 50,
	GEAR_4 = 100,
	GEAR_5 = 150,
	GEAR_6 = 200,
	GEAR_7 = 210,
};



enum DB_SET
{
 DB_0  = 0,
 DB_10 = 1,
 DB_20 = 2,
 DB_30 = 3,
 DB_40 = 4,
 DB_50 = 5,
 DB_60 = 6,
};


//// 定义枚举变量
//enum ATTENUATION Myattenuation;


/*电机控制结构体*/
typedef struct
{
	unsigned int 	 Motor_End_count;
	unsigned short Motor_Current_Positon;
	unsigned short Motor_Next_Positon;
}MOTODATA;
extern MOTODATA Motodata;

/**
 * @brief 结构体描述:用于记录当前激光器的菜单状态
 *
 * 详细描述该结构体的用途、成员和其他相关信息。
 */
typedef struct
{
	unsigned char UIMode;	/**< 成员1的描述 */
}LIGHTSOURCE;

extern LIGHTSOURCE Lightsoutce;



typedef struct 
{
	unsigned char Mode;
	unsigned char temp;
	unsigned char cycle[5];
	unsigned char energy[5];
}RETDATA;
extern RETDATA retdata;

typedef struct
{
	unsigned char  discycle;
	unsigned int discycleshow;
	unsigned char  cbit_point;
	unsigned char  cdecimal_point;

	unsigned char disenergy;
	unsigned char bit_point;
	unsigned char decimal_point;
	unsigned int disenergyshow;
}DISPLAY;

struct Uart_DataPacket
{
    uint8_t Start_Flag;    // 启始符标记
    uint8_t Clarity_Flag;  // 透明符标记
    uint8_t End_Flag;      // 终止符标记
    uint8_t RX_BUF[16];    // 接收缓冲,最大15个字节，第一个空间存放有效数据个数
    uint8_t TX_BUF[16];    // 发送缓冲,最大15个字节，第一个空间存放有效数据个数
};

struct Menu_Index
{
	uint8_t Current_Index;  //当前菜单索引值
	uint8_t Next_Index;   	//下一个预选菜单索引值
	uint8_t Prior_Index;   	//前一个预选菜单索引值
};

extern signed int Number_Pulse;
extern signed int Tempnumber_Pulse;

extern unsigned int pulse_width_formula(unsigned int pw_xus);

extern signed int CycleCodeTime;			
extern signed int Temp_CycleCodeTime;
extern signed short int Temp_dB_Loss;
extern signed short int dB_Loss;

extern uint16_t RandomNumber, RandomBaseFreq, RandomMinValue, RandomMaxValue;

extern bool b_Ext_Sync;

extern unsigned short PauseTime;

extern struct Uart_DataPacket RemoteDataPacket;

extern struct Menu_Index Menu_Indexs;	// 结构体，菜单索引，在ui.c里声明

extern union Union_Byte LaserState;		// 共用体，激光器状态，在usart.c里声明

extern unsigned int u32_FlashBuff[2048];

extern unsigned short u16_RandomData[1024];

/**  **/
extern struct Menu_Index Menu_Indexs;
/**  **/
extern struct Uart_DataPacket RemoteDataPacket;
/**  **/
extern DISPLAY display;

//extern DataPoint data;
//extern unsigned char ucdata;

extern unsigned short PauseTime;

extern bool b_Random_EN;

extern union Union_Byte LaserControl;	// 共用体，激光编码控制位

extern ring_buffer rb_uart1_rx;

extern ring_buffer rb_uart1_tx;

extern ring_buffer rb_uart3_rx;

extern ring_buffer rb_uart3_tx;

extern signed int CycleCodeTime;	// 周期预设50ms

extern void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd);

extern unsigned char UART_RemoteCMD_Process(void);

extern void attenuation_number(control_number* ap , struct Uart_DataPacket* rp);

extern void Motor_Move_To(unsigned short position);
extern void Set_DB(uint16_t DB_Data);
extern uint8_t IO_Read(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin_x);
extern void Motor_Return_Zero(void);

double lagrangeInterpolate(DataPoint* data, int n, double x);
	
float splitNumber(signed short int input);

void Position_In_Zero(void);

signed short int Set_Energy(uint16_t show);

extern unsigned char flag_lightN_F;
#endif

