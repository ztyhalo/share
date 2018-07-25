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

/************************************led�궨��***************************************/
/************************************************************************************/
#define	LED_1	GPIO_Pin_6	
#define	LED_2	GPIO_Pin_12
#define	LED_3	GPIO_Pin_13

#define	LED1(x)	GPIOD->BSRR=(x<1)?(1<<(16+6)):(1<<6)
#define	LED2(x)	GPIOD->BSRR=(x<1)?(1<<(16+12)):(1<<12)
#define	LED3(x)	GPIOD->BSRR=(x<1)?(1<<(16+13)):(1<<13)

/*************************************ȫ�ֱ�������***********************************/
/************************************************************************************/
OS_EVENT*	Sem1;

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::**
** ��������: GPIO_Configuration
** ��������: IO��������
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
	GPIO_InitStructure.GPIO_Pin = LED_1 | LED_2 
	| LED_3; 											//����LED�˿ڹҽӵ�6��12��13
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������
}

/*************************************Ӳ��ƽ̨��ʼ��***********************************/
/**************************************************************************************/
void BSP_Init()
{	
	/*���ڳ�ʼ��*/
	Init_Usart();
	Usart_Configuration(115200);
	Printf("USART Init������OK/r/n");
	
	/*led��ʼ��*/
	Led_Init();	

	/*GUI ��ʼ�������Ǳ���� */
	GUI_Init();

	/*������Io����*/
	Touch_Configuration();

}

/*led����*/
/*****************************************led����**********************************/
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

	Printf("err=0x%x\r\n",err);
	
}


