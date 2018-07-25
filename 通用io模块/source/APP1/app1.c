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

/*************************************��Դ����**********************************/
/************************************************************************************/
#define	RRAMEWIN_ID		0+200
#define	TEXT_ID1			1+200
#define	LED1_ID1			2+200
#define	LED2_ID2			3+200
#define	LED3_ID3			4+200
#define	TEXT_ID2			5+200
#define	TEXT_ID3			6+200
#define	TEXT_ID4			7+200
#define	TEXT_ID5			8+200
#define	SLIDER_ID			9+200
#define BUTTON1_ID			10+200
#define BUTTON2_ID			11+200

/*LED����״̬*/
char LED1_Status;
char LED2_Status;
char LED3_Status;
/*LED����ģʽ:ռ�ձ�ģʽ�����ǿ���ģʽ*/
unsigned char LED_TEST_MODE;
/*ledռ�ձ�*/
unsigned short led_duty;
/*����������*/
WM_HWIN hWin;
WM_HWIN text0,text1,text2,text3,text4,checkbox0,checkbox1,checkbox2,slider0;
/* �����˶Ի�����Դ�б� */
static const GUI_WIDGET_CREATE_INFO aDialogCreate[] = {
	
	/*��ܴ���*/
	{ FRAMEWIN_CreateIndirect, "LED Demo", RRAMEWIN_ID, 0, 0, 320, 240, /*FRAMEWIN_CF_ACTIVE*/0 ,0 },
	/*�ı��ؼ�*/
	{ TEXT_CreateIndirect, "Test 1 - LED On/Off", TEXT_ID1, 40, 10, 152, 13, 0,TEXT_CF_LEFT  },
	/*��ѡ��*/
	{ CHECKBOX_CreateIndirect, "LED1", LED1_ID1, 80, 60, 15, 15, 0 ,0 },
	{ CHECKBOX_CreateIndirect, "LED2", LED2_ID2, 160, 60, 15, 15, 0 ,0 },
	{ CHECKBOX_CreateIndirect, "LED3", LED3_ID3, 240, 60, 15, 15,/* TEXT_CF_HCENTER*/0 ,0 },
	/*�ı��ؼ�*/
	{ TEXT_CreateIndirect, "LED1", TEXT_ID2, 75, 38, 30, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "LED2", TEXT_ID3, 155, 38, 30, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "LED3", TEXT_ID4, 235, 38, 30, 13, 0,TEXT_CF_LEFT  },

	/*�ı��ؼ�*/
	{ TEXT_CreateIndirect, "Test 2 - LED duty cyle", TEXT_ID5, 40, 120, 176, 13, 0,TEXT_CF_LEFT },
	/*������*/
	{ SLIDER_CreateIndirect, 0, SLIDER_ID, 80, 175, 150, 15, TEXT_CF_LEFT ,0 },
	/*button*/
	{ BUTTON_CreateIndirect, "select1", BUTTON1_ID, 250, 10, 60, 15, 0 ,0 },
	{ BUTTON_CreateIndirect, "select2", BUTTON2_ID, 250, 120, 60, 15 ,0,0 },
};

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

/**/
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
/*******************************************GUI����***********************************/
/**************************************************************************************/
/*�Ӵ��ص�����*/
void Window1_CallBack(WM_MESSAGE* pMsg)
{
	/*����Ϣɢת�������Ӧ��Ϣ*/
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
		{
			GUI_SetBkColor(GUI_LIGHTMAGENTA);
			GUI_Clear();
			break;
		}
		default:
		{/*Ĭ����Ϣ����ʽ*/
			WM_DefaultProc(pMsg);
		}
	}
}
/*�Ӵ��ص�����*/
void Window2_CallBack(WM_MESSAGE* pMsg)
{
	/*����Ϣɢת�������Ӧ��Ϣ*/
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
		{
			GUI_SetBkColor(GUI_DARKCYAN);
			GUI_Clear();
			break;
		}
		default:
		{/*Ĭ����Ϣ����ʽ*/
			WM_DefaultProc(pMsg);
		}

	}
}
/*��������*/
static void _IsCLICKED(WM_HWIN hDlg, int Id) {

  if ((Id == LED1_ID1)) {/*���ظ�ѡ��״̬*/						
    LED1_Status = CHECKBOX_IsChecked(checkbox0);						
  }	
  else if ((Id == LED2_ID2)) {				
    LED2_Status = CHECKBOX_IsChecked(checkbox1);						
  }	
  else if ((Id == LED3_ID3)) {				
    LED3_Status = CHECKBOX_IsChecked(checkbox2);						
  }	 
  else if ((Id == BUTTON1_ID)) {/*ѡ��ģʽ1*/
    LED_TEST_MODE = 1;						
  }	 
   else if ((Id == BUTTON2_ID)) {/*ѡ��ģʽ2*/
    LED_TEST_MODE = 2;						
  }	
 
}
/*������ֵ�ı䴦��*/
void _IsValueChanged(hDlg,Id)
{
	if((Id == SLIDER_ID)){/*������õ���������ֵ*/
		led_duty=SLIDER_GetValue(slider0);					 //���slider0��ֵ	
	}		
}
/****************************************************************************
* ��    �ƣ�static void _cbCallback(WM_MESSAGE * pMsg)  
* ��    �ܣ�����ص����� 
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void _cbCallback(WM_MESSAGE * pMsg) {    
  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;	  
  switch (pMsg->MsgId) {     
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /*��ô��岿����ID*/
      NCode = pMsg->Data.v;                 /*�������� */
      switch (NCode) {	          
        case WM_NOTIFICATION_CLICKED: /*���岿����ֵ���ı� */
		{	   
			_IsCLICKED(hDlg, Id);	 
			break;
		}
		case WM_NOTIFICATION_VALUE_CHANGED:
		{
			_IsValueChanged(hDlg,Id);
			break;
		}
		 
        default:
          break;
      }
      break;  
    default:
      WM_DefaultProc(pMsg);
  }
}

/*************************************************************????????????????????????????????????*/
unsigned int test_stk[4048];
void test1_Task(char *p)
{	
	/*�����Ļ*/
	GUI_Clear();

	/*������ܺ���*/  
  	hWin = GUI_CreateDialogBox(aDialogCreate, GUI_COUNTOF(aDialogCreate), _cbCallback, 0, 0, 0);

	/* ���TEXT �����ľ�� */
  	text0 = WM_GetDialogItem(hWin, TEXT_ID1);
	text1 = WM_GetDialogItem(hWin, TEXT_ID2);
	text2 = WM_GetDialogItem(hWin, TEXT_ID3);
	text3 = WM_GetDialogItem(hWin, TEXT_ID4);
	text4 = WM_GetDialogItem(hWin, TEXT_ID5);

	/*��ø�ѡ��������*/
	checkbox0 = WM_GetDialogItem(hWin, LED1_ID1);
	checkbox1 = WM_GetDialogItem(hWin, LED2_ID2);
	checkbox2 = WM_GetDialogItem(hWin, LED3_ID3);
										
	/*�������ؼ�*/
	slider0 = WM_GetDialogItem(hWin, SLIDER_ID);

	/* �����ı����� */
	TEXT_SetFont(text0, &GUI_Font8x13_ASCII);
	TEXT_SetFont(text1, &GUI_Font6x9);
	TEXT_SetFont(text2, &GUI_Font6x9);
	TEXT_SetFont(text3, &GUI_Font6x9);
	TEXT_SetFont(text4, &GUI_Font8x13_ASCII);

	/* ����TEXT������������ɫ */        	
	TEXT_SetTextColor(text0,0XFf);	
	TEXT_SetTextColor(text1,0XFF);		 //��
	TEXT_SetTextColor(text2,0XFF);
	TEXT_SetTextColor(text3,0XFF);
	TEXT_SetTextColor(text4,0XFf);
//
//	TEXT_SetText(text0,"TesDFSFDSDt 1");
//	TEXT_SetText(text1,"LEDFSADFSAD1");
//	TEXT_SetText(text2,"LEFDFSDFD2");
//	TEXT_SetText(text3,"LEASDFDSAD3");
//	TEXT_SetText(text4,"TeDAFADFst 2");
	
	/*������������ֵ��Χ*/
	SLIDER_SetRange(slider0,1,400);

	
	while(1)
	{
		GUI_CURSOR_Show();
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,15);
		WM_Exec();
	}
}
/*******************************************��������***********************************/
/**************************************************************************************/
unsigned int usart_stk[128];
void Usart_Task(char *p_arg)
{
	INT8U	err;

	while(1){
		/*�����ź���*/
		OSSemPend(Sem1,0,&err);

		Printf(p_arg);
		Printf("err=0x%x\r\n",err);
		/**/
		OSTimeDlyHMSM(0,0,0,500);


		/*�ͷ��ź���*/
		OSSemPost(Sem1);		
	}
	
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
		if(LED_TEST_MODE == 1)
		{
			if(LED1_Status == 1){
				LED1(1);	
			}
			else{
				LED1(0);	
			}
	
			if(LED2_Status == 1){
				LED2(1);	
			}
			else{
				LED2(0);	
			}
	
			if(LED3_Status == 1){
				LED3(1);	
			}
			else{
				LED3(0);	
			}
	
			OSTimeDlyHMSM(0,0,0,10);
		}
		else if(LED_TEST_MODE == 2)
		{
			LED1(1);
			LED2(1);
			LED3(1);
			led_delay(led_duty);
			
			LED1(0);
			LED2(0);
			LED3(0);
			led_delay(400-led_duty);
		}
		OSTimeDlyHMSM(0,0,0,10);
	}
}

/**/
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
					   2);
//
//	Printf("err=0x%x\r\n",err);
//
//	err = OSTaskCreate((void (*) (void *)) &Usart_Task,
//					   (void*)"hello",
//					   (OS_STK *)&usart_stk[128-1],
//					   3);
//
//	Printf("err=0x%x\r\n",err);

	err = OSTaskCreate((void (*) (void *)) &test1_Task,
					   (void*)0,
					   (OS_STK *)&test_stk[2048-1],
					   4);

	Printf("err=0x%x\r\n",err);
	
}
