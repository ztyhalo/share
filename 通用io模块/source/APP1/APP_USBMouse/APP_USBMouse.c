#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"
#include "BSP.H"

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
#include "DROPDOWN.h"
//#include "APP_RTC.H"
/***********************************************************/
/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  结构声明*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数  */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USB低优先级中断请求
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//抢占优先级 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB高优先级中断请求
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//抢占优先级 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
}

/************************************LED闪速任务**************************************/
/**************************************************************************************/
unsigned int led_stk[128];
void Led_Task(char *p_arg)
{
	while(1){

			LED1(1);
			LED2(1);
			LED3(1);
		OSTimeDlyHMSM(0,0,0,350);


			LED1(0);
			LED2(0);
			LED3(0);
		OSTimeDlyHMSM(0,0,0,350);

	}
}


/*************************************Task1任务****************************************/
/**************************************************************************************/
extern void RCC_Configuration(void);
extern void USB_Disconnect_Config(void);
extern void USB_Init(void);
extern void Joystick_Send(u8 Keys);

unsigned int test1_stk[512];
void test1_Task(char *p_arg)
{
	/*使能各端口时钟，并配置USB时钟*/
	RCC_Configuration();
	
	/*配置USB连接管脚*/
	USB_Disconnect_Config();
	
	/*硬件配置*/
	//……	

	/*中断配置*/
	NVIC_Configuration();

	/*usb初始*/
	USB_Init();

	while(1)
	{
		if(!(1&(GPIOC->IDR>>13)))
			Joystick_Send(0);
	//	OSTimeDlyHMSM(0,0,0,350);
	}

}

unsigned int app_kb[128];
static  void  AppTaskKbd (void *p_arg)
{
	GUI_CURSOR_Show();

	while(1)
	{
		GUI_TOUCH_Exec();
		WM_Exec();
		OSTimeDlyHMSM(0,0,0,15);
		
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

	err = OSTaskCreate((void (*) (void *)) &AppTaskKbd,
					   (void*)0,
					   (OS_STK *)&app_kb[128-1],
					   9);

//
//	err = OSTaskCreate((void (*) (void *)) &test1_Task,
//					   (void*)0,
//					   (OS_STK *)&test1_stk[512-1],
//					   4);



	Printf("err=0x%x\r\n",err);
	
}
