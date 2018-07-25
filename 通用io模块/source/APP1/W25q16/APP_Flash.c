#include "flash.h"
#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"

/***********************************************************/
#include "usart.h"
#include "gui.h"
#include "GUIType.h"
#include "stm32f10x.h"
#include "WM.h"
#include "checkbox.h"
#include "button.h"
#include "edit.h"
#include "checkbox.h"
#include "listbox.h"
#include "framewin.h"
#include "PROGBAR.h"
#include "radio.h"
#include "SCROLLBAR.h"
#include "SLIDER.h"
#include "text.h"
#include "touch.h"
#include "usart.h"
#include "gui.h"
#include "GUIType.h"
#include "stm32f10x.h"
#include "WM.h"
#include "checkbox.h"
#include "button.h"
#include "edit.h"
#include "checkbox.h"
#include "listbox.h"
#include "framewin.h"
#include "PROGBAR.h"
#include "radio.h"
#include "SCROLLBAR.h"
#include "SLIDER.h"
#include "text.h"
#include "APP_RTC.H"
/***********************************************************/

/************************************led宏定义***************************************/
/************************************************************************************/
#define	LED_1	GPIO_Pin_6	
#define	LED_2	GPIO_Pin_12
#define	LED_3	GPIO_Pin_13

#define	LED1(x)	GPIOD->BSRR=(x<1)?(1<<(16+6)):(1<<6)
#define	LED2(x)	GPIOD->BSRR=(x<1)?(1<<(16+12)):(1<<12)
#define	LED3(x)	GPIOD->BSRR=(x<1)?(1<<(16+13)):(1<<13)

/*************************************全局变量申明***********************************/
/************************************************************************************/
OS_EVENT*	Sem1;

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::**
** 函数名称: GPIO_Configuration
** 功能描述: IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	//使能各个端口时钟，重要！！！
	
	GPIO_InitStructure.GPIO_Pin = LED_1 | LED_2 
	| LED_3; 											//配置LED端口挂接到6、12、13
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
}

/*************************************硬件平台初始化***********************************/
/**************************************************************************************/
void BSP_Init()
{	
	/*串口初始化*/
	Init_Usart();
	Usart_Configuration(115200);
	Printf("USART Init。。。OK/r/n");
	
	/*led初始化*/
	Led_Init();	

	/*GUI 初始化，这是必须地 */
	GUI_Init();

	/*触屏的Io配置*/
	Touch_Configuration();

}

/*led任务*/
/*****************************************led任务**********************************/
/**************************************************************************************/
void led_delay(unsigned short i)
{
	unsigned short j;
	for(i=i;i>0;i--)
		for(j=0x1f;j>0;j--);
}
unsigned int led_stk[128];
void Led_Task(char *p_arg)
{
	while(1){

			LED1(1);
			LED2(1);
			LED3(1);
			OSTimeDlyHMSM(0,0,0,100);

			LED1(0);
			LED2(0);
			LED3(0);
			OSTimeDlyHMSM(0,0,0,100);
	}
}

/************************************创建所有任务**************************************/
/**************************************************************************************/
void Task_Start(void)
{
	/*保存任务返回*/
	unsigned  char err;


#if	OS_SEM_EN>0
	/*创建一个拥有1个资源的信号量*/
	Sem1 = OSSemCreate(1);
	if(Sem1!=0){
		Printf("Error->Sem1 = OSSemCreate(3);\r\n");
	}
#endif	


	/*创建任务*/
	err = OSTaskCreate((void (*) (void *)) &Led_Task,
					   (void*)0,
					   (OS_STK *)&led_stk[128-1],
					   10);

	Printf("err=0x%x\r\n",err);
	
}


