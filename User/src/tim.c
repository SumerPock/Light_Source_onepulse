#include "tim.h"

//static unsigned int Sys_Count = 0; //Sys_Count
unsigned int Sys_Count = 0; //Sys_Count
extern MOTODATA Motodata;


void TIM6_Int_Init(unsigned short arr , unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6 , &TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM6 , TIM_IT_Update , ENABLE); 						
	TIM_Cmd(TIM6 , DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;			         //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void splitDigits(unsigned short num, int result[3]) {
    // 获取百位数
    result[2] = num / 100;
    // 获取十位数
    result[1] = (num / 10) % 10;
    // 获取个位数
    result[0] = num % 10;
}




//3.整形转字符串
unsigned char ConvertNum_Character(int data) {
    unsigned char num;
    switch(data) {
        case 0:
            num = ' 0';
            break;
        case 1:
            num = ' 1';
            break;
        case 2:
            num = ' 2';
            break;
        case 3:
            num = ' 3';
            break;
        case 4:
            num = ' 4';
            break;
        case 5:
            num = ' 5';
            break;
        case 6:
            num = ' 6';
            break;
        case 7:
            num = ' 7';
            break;
        case 8:
            num = ' 8';
            break;
        case 9:
            num = ' 9';
            break;
        default:
            num = '\0'; // 处理不在0-9范围内的情况，你可以根据需要修改
    }
    return num;
}


void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6 , TIM_IT_Update)==SET) //溢出中断
	{		
		TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);
	}	
}


void TIM3_Int_Init(unsigned short arr , unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						
	TIM_Cmd(TIM3 , DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;			         //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

extern unsigned char findzero_bigin;


/*上电寻零动作*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	}
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 						//允许定时器3更新中断
	TIM_Cmd(TIM2,DISABLE); //不使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;			         //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*暂停功能*/
void TIM2_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		Sys_Count++;
		if(Sys_Count == 1)						//中断计时时间到 Sys_Count
		{
			Pulse_StateTurn(ENABLE);		//中断1s短时脉冲跳低
			Sys_Count = 0;							//计时器清零 Sys_Count
			TIM_Cmd(TIM2,DISABLE); 			//关闭计时器，等待下次由串口命令时再开启
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}


void TIM4_Int_Init(unsigned short arr , unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM4 , TIM_IT_Update , ENABLE); 						
	TIM_Cmd(TIM4 , ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;			           	//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 		//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;          	//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint32_t MOTO_COUNT = 0;
void TIM4_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET){
		MOTO_COUNT++;		
		#if (VERSION_SELECTION == Ver_2_3_0)
		GPIO_ToggleBits(Motor_STEP);
		LED0_Toogle;
		if((Motodata.Motor_End_count) && (MOTO_COUNT >= Motodata.Motor_End_count)){
			Motor_Power_Off;
			Motodata.Motor_End_count = 0;
			#ifdef BUGMODE
				LCD_ShowString(0, 0 , "_stop", WHITE);
			#endif			
			TIM_Cmd(TIM4 , DISABLE); 
		}
		#endif
		TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
	}
}
