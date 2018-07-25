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
#include "RTC.H"
#include <math.h>
/***********************************************************/


/*************************************全局变量申明***********************************/
/************************************************************************************/
OS_EVENT*	Sem1;


/************************************led宏定义***************************************/
/************************************************************************************/
#define	LED_1	GPIO_Pin_6	
#define	LED_2	GPIO_Pin_12
#define	LED_3	GPIO_Pin_13

#define	LED1(x)	GPIOD->BSRR=(x<1)?(1<<(16+6)):(1<<6)
#define	LED2(x)	GPIOD->BSRR=(x<1)?(1<<(16+12)):(1<<12)
#define	LED3(x)	GPIOD->BSRR=(x<1)?(1<<(16+13)):(1<<13)

/************************************key宏定义***************************************/
/************************************************************************************/
#define	USER_KEY	(1&(GPIOC->IDR>>13))

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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	//使能端口时钟，重要！！！
	
	GPIO_InitStructure.GPIO_Pin = LED_1 | LED_2 
	| LED_3; 											
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//配置端口速度为2M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
}
			  
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_KEY
** 功能描述: 按键 IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_KEY()
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//配置按键USER挂接到13端口 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//配置成上拉输入
  	GPIO_Init(GPIOC	,&GPIO_InitStructure);				   	//将端口GPIOC进行初始化配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
	| GPIO_Pin_3 | GPIO_Pin_4;								//配置五项摇杆按键挂接到0、1、2、3、4端口 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//配置成上拉输入
  	GPIO_Init(GPIOE,&GPIO_InitStructure);				   	//将端口GPIOE进行初始化配置
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

	/*按键初始化*/
	Init_KEY();

	/*GUI 初始化，这是必须地 */
	GUI_Init();

	/*触屏的Io配置*/
	Touch_Configuration();
}

//画大点
void GUI_DrawBigPoint(unsigned short x,unsigned char y)
{
	GUI_DrawPoint(x,y);	
	GUI_DrawPoint(x+1,y);	
	GUI_DrawPoint(x,y+1);	
	GUI_DrawPoint(x+1,y+1);		
}
/*画时钟的指针针*/
unsigned short X[60];  //X 坐标	保存的是时钟里60个点的坐标
unsigned char  Y[60];  //X 坐标	保存的是时钟里60个点的坐标
unsigned short Xmin[60];	//分针
unsigned char  Ymin[60];
unsigned short Xhou[12];	//时针
unsigned char  Yhou[12];
void DrawTime(unsigned char dat/*sec 范围0 - 59,时间*/,unsigned char type/*type : sec min hour*/)
{
	unsigned char sub=0;
	static unsigned short x1,y1;
	static unsigned short x2,y2;
	static unsigned short x3,y3;

	
	if(type == 0){/*更新秒*/
		{/*更新前 清楚上次显示u*/
			GUI_SetColor(0);
			GUI_DrawLine(230,150,x1,y1);
			GUI_SetColor(0xff);
		}
		GUI_DrawLine(230,150,X[dat],Y[dat]);
		x1 = X[dat];
		y1 = Y[dat];
	}
	else if(type == 1){/*更新分*/
		{/*更新前 清楚上次显示u*/
			GUI_SetColor(0);
			GUI_DrawLine(230,150,x2,y2);
			GUI_SetColor(0xff);
		}

		GUI_DrawLine(230,150,Xmin[dat],Ymin[dat]);
		x2 = Xmin[dat];
		y2 = Ymin[dat];
	}	
	else if(type == 2){/*更新时*/
		{/*更新前 清楚上次显示u*/
			GUI_SetColor(0);
			GUI_DrawLine(230,150,x3,y3);
			GUI_SetColor(0xff);
		}

		if(dat>12)dat = dat-12;
		GUI_DrawLine(230,150,Xhou[dat],Yhou[dat]);
		x3 = Xhou[dat];
		y3 = Yhou[dat];
	}
}
/************************************g更新lcd上时间任务********************************/
/**************************************************************************************/
unsigned char TIME_UPDATE_FLAG=0;
unsigned int update_stk[1024];
void Update_LCD(char *p)
{
	static unsigned short i;
	float f;
	GUI_DispStringAt("You need Inpu Data by USART!",80,120);
	/*实时时钟*/
	Init_RTC();
	GUI_Clear();
	GUI_DispStringAt("Note:Ste Timer by Usart!",1,215);
	GUI_SetColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringAt("Years:Month:Day",1,5);
	GUI_DispStringAt("Hour:Minute:Second",1,60);
	GUI_DispStringAt("Week:",30,120);
	GUI_DispStringAt(":",48,35);
	GUI_DispStringAt(":",85,35);
	GUI_DispStringAt(":",48,90);
	GUI_DispStringAt(":",83,90); 
	GUI_SetColor(GUI_RED);
	
	{ /*画时钟*/
//		/*设置笔尖尺寸*/
//		GUI_SetPenSize(18);
		GUI_DrawCircle(230,150,80);
		GUI_DrawCircle(230,150,81);

		/*计算圆周各点的位置（秒针与他公用一个坐标）*/
		for( i = 0; i< 60; i++){
			f = (float)+(i*6*3.141592/180)-(3.141592/2) ;
			X[i] = (int)(230 + (70 * cos(f)));
			Y[i] = (int)(150 + (70 * sin(f)));
		}
		/*分针*/
		for( i = 0; i< 60; i++){
			f = (float)+(i*6*3.141592/180)-(3.141592/2) ;
			Xmin[i] = (int)(230 + (50 * cos(f)));
			Ymin[i] = (int)(150 + (50 * sin(f)));
		}
		/*时针*/
		for( i = 0; i< 12; i++){
			f = (float)+(i*30*3.141592/180)-(3.141592/2) ;
			Xhou[i] = (int)(230 + (30 * cos(f)));
			Yhou[i] = (int)(150 + (30 * sin(f)));
		}

		/*画点：表盘上的点*/
		for(i=0;i<60;i++){
			GUI_DrawBigPoint(X[i],Y[i]);	
		}
		GUI_SetFont(&GUI_Font6x8);
		/*数字：表盘上的数字*/
		GUI_DispDecAt(12,228,90,2);
		GUI_DispDecAt(6,228,210,1);
		GUI_DispDecAt(3,285,148,1);
		GUI_DispDecAt(9,170,148,1);
	}

	GUI_SetFont(&GUI_Font8x16x1x2);	
	while(1)
	{  
		TIME_UPDATE_FLAG = 0;
		if(RTC_GetITStatus(RTC_IT_SEC))
		{ 	/*更新edit里的时间timer.w_year*/
			RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW | RTC_IT_ALR);		//清除溢出，秒钟中断标志		  								 
			RTC_WaitForLastTask();					//等待RTC寄存器操作完成

			Time_Get();	
			RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW | RTC_IT_ALR);		//清除溢出，秒钟中断标志		  								 
			RTC_WaitForLastTask();					//等待RTC寄存器操作完成
			GUI_DispDecAt(timer.w_year,10,30,4);
			GUI_DispDecAt(timer.w_month,60,30,2);
			GUI_DispDecAt(timer.w_date,100,30,2);

			GUI_DispDecAt(timer.hour,20,85,2);
			GUI_DispDecAt(timer.min,60,85,2);
			GUI_DispDecAt(timer.sec,100,85,2);


			GUI_DispDecAt(timer.week,40,145,4);

			/*模拟的时钟更新*/
			DrawTime(timer.sec,0);
			DrawTime(timer.min,1);
			DrawTime(timer.hour,2);

			TIME_UPDATE_FLAG = 1;
		}
		OSTimeDlyHMSM(0,0,0,15);			
	}
}
/**/
unsigned int led_task[125];
void LED_Task(char *p)
{
	while(1)
	{
		if(TIME_UPDATE_FLAG)
			LED1(1);	
		else 
		LED1(0);
		OSTimeDlyHMSM(0,0,0,20);
	}
}

/*按键任务*/
unsigned int key_task[64];
void Key_Task(char *p)
{
	while(1)
	{
		if(USER_KEY == 0){/*按键按下-设置时间*/
			Time_Set();	
		}

		OSTimeDlyHMSM(0,0,0,750); //延时1s这个任务不重要
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
	if(Sem1==0){
		Printf("Error->Sem1 = OSSemCreate(1);\r\n");
	}
#endif	


	/*创建任务*/
	err = OSTaskCreate((void (*) (void *)) &Update_LCD,
					   (void*)0,
					   (OS_STK *)&update_stk[1024-1],
					   3);

   err = OSTaskCreate((void (*) (void *)) &LED_Task,
					   (void*)0,
					   (OS_STK *)&led_task[125-1],
					   9);

	err = OSTaskCreate((void (*) (void *)) &Key_Task,
					   (void*)0,
					   (OS_STK *)&key_task[64-1],
					   10);

	Printf("err=0x%x\r\n",err);
	
}