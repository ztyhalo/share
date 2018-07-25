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
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  �ṹ����*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��  */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USB�����ȼ��ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB�����ȼ��ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
}

/************************************LED��������**************************************/
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


/*************************************Task1����****************************************/
/**************************************************************************************/
extern void RCC_Configuration(void);
extern void USB_Disconnect_Config(void);
extern void USB_Init(void);
extern void Joystick_Send(u8 Keys);

unsigned int test1_stk[512];
void test1_Task(char *p_arg)
{
	/*ʹ�ܸ��˿�ʱ�ӣ�������USBʱ��*/
	RCC_Configuration();
	
	/*����USB���ӹܽ�*/
	USB_Disconnect_Config();
	
	/*Ӳ������*/
	//����	

	/*�ж�����*/
	NVIC_Configuration();

	/*usb��ʼ*/
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
/************************************������������**************************************/
/**************************************************************************************/
void Task_Start(void)
{
	/*�������񷵻�*/
	unsigned  char err;


#if	OS_SEM_EN>0
	/*����һ��ӵ��1����Դ���ź���*/
	Sem1 = OSSemCreate(1);
	if(Sem1!=0){
		Printf("Error->Sem1 = OSSemCreate(3);\r\n");
	}
#endif	


	/*��������*/
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
