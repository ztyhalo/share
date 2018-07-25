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

/*SD 卡需要的头文件*/
#include "sdcard.h"
#include "ff.h"
#include "ffconf.h"

/*关于SD卡的申明*/
SD_CardInfo SDCardInfo;

/*
这两个变量要注意，如果文件比较少，可以适当较小数值
但是要保证这个变量能保证保存你所有的文件名，否则可
能发生覆盖，很有可能将TXT的文件名写到BMP里，更严重
的是覆盖更有意义的数据照成严重的错误
*/
char TXT_Files[100][40];	//最多100个文件,
char BMP_Files[100][40];	//最多100个文件,
DIR dirs;
FILINFO finfo;
FATFS fs;

/**************************在主目录里寻找指定类型的文件*******************************/
/**************************************************************************************/
//type_file 是查找文件的类型：
//			.bmp 或 ..txt
void Find_FileName()
{
	const XCHAR path[]="";
	unsigned short i,j=0,k=0;

/*长文件名支持*/	
#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
#endif


	/*挂载文件系统*/
	f_mount(0, &fs);

	if (f_opendir(&dirs, path) == FR_OK) {/*如果打开主目录成功*/
		
		while (f_readdir(&dirs, &finfo) == FR_OK){/*通过dirs，依次阅读文件名，知道末尾*/
			
			   	
			if(finfo.fattrib & AM_DIR){	 /*是目录就结束本次循环*/
				continue;	
			}
			if (finfo.fattrib & AM_ARC) { /*刚打开的文件属性是存档文件,*/
				
				if(!finfo.fname[0])	 break; /*文件尾，跳出while*/		
				if(finfo.lfname[0]){/*长文件名	*/
					i = (unsigned char )strlen(finfo.lfname);/*求文件名的长度，求出之后得到后缀*/

					if((finfo.lfname[i-3]=='t') && (finfo.lfname[i-2]=='x') && (finfo.lfname[i-1]=='t')){
						strcpy(TXT_Files[j],(const char *)finfo.lfname);
						j++;
					}
					else if((finfo.lfname[i-3]=='b') && (finfo.lfname[i-2]=='m') && (finfo.lfname[i-1]=='p')){
						strcpy(BMP_Files[k],(const char *)finfo.lfname);
						k++;
					}
				}
				else{	/*段文件名*/
					i = (unsigned char )strlen(finfo.fname);/*求文件名的长度，求出之后得到后缀*/

					if((finfo.fname[i-3]=='t') && (finfo.fname[i-2]=='x') && (finfo.fname[i-1]=='t')){
						strcpy(TXT_Files[j],(const char *)finfo.fname);
						j++;
					}
					else if((finfo.fname[i-3]=='b') && (finfo.fname[i-2]=='m') && (finfo.fname[i-1]=='p')){
						strcpy(BMP_Files[k],(const char *)finfo.fname);
						k++;
					}	
				}
			}
		}	
	}
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
extern short hwin,hGUI_ID_DROPDOWN0,hGUI_ID_DROPDOWN1,hGUI_ID_BUTTON0,hGUI_ID_BUTTON1,hGUI_ID_MULTIEDIT0;
extern void Win_Init(void) ;
extern char  GUI_GotoXY(int x, int y);
extern void DROPDOWN_AddString(DROPDOWN_Handle hObj, const char* s);
unsigned int test1_stk[512];
void test1_Task(char *p_arg)
{
	unsigned char i;
	/**/
	NVIC_InitTypeDef NVIC_InitStructure;

	/*SD卡的中断初始化*/	
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/*SD卡初始化*/
	while(SD_Init()!= SD_OK)
	{
		GUI_DispStringAt("SD Card Init Fail!\r\n",50,70);
		OSTimeDlyHMSM(0,0,0,300);
		GUI_DispStringAt("Pls check SD Card!\r\n",50,100);
		OSTimeDlyHMSM(0,0,0,300);
	}

	GUI_DispStringAt("SD_Init Success!!!",50,130);
	GUI_DispStringAt("Find File!",50,160);
	Find_FileName();
	GUI_GotoXY(0,0);
//	/*调试用*/
//	for(i=0;TXT_Files[i][0]!=0;i++)
//	{ /*在屏幕打印TXT文件名*/
//		GUI_DispString(TXT_Files[i]);
//		GUI_DispString("\r\n");
//	}
//
//	for(i=0;BMP_Files[i][0]!=0;i++)
//	{ /*在屏幕打印BMP文件名*/
//		GUI_DispString(BMP_Files[i]);
//		GUI_DispString("\r\n");
//	}
//	while(1);
	
	GUI_Clear();
	
	/*初始窗口框架*/
	Win_Init();

	/*将文件清单加入下拉列表（可在“void InitDialog(WM_MESSAGE * pMsg)”函数里添加）*/	
	for(i=0;TXT_Files[i][0]!=0;i++){
	/*加txt文件到txt的列表清单*/
		DROPDOWN_AddString(hGUI_ID_DROPDOWN0,TXT_Files[i]);/*这两个函数都可以，下一个函数方法更灵活*/
	//	DROPDOWN_InsertString(hGUI_ID_DROPDOWN0,TXT_Files[i],i);
	}
	
	for(i=0;BMP_Files[i][0]!=0;i++){
	/*加bmp文件到txt的列表清单*/
		DROPDOWN_AddString(hGUI_ID_DROPDOWN1,BMP_Files[i]);/*这两个函数都可以，下一个函数方法更灵活*/
	//	DROPDOWN_InsertString(hGUI_ID_DROPDOWN0,TXT_Files[i],i);
	}
	while(1){
	
		GUI_CURSOR_Show();
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,15);
		WM_Exec();	
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


	err = OSTaskCreate((void (*) (void *)) &Key_Task,
					   (void*)0,
					   (OS_STK *)&key_task[64-1],
					   9);


	err = OSTaskCreate((void (*) (void *)) &test1_Task,
					   (void*)0,
					   (OS_STK *)&test1_stk[512-1],
					   4);



	Printf("err=0x%x\r\n",err);
	
}

/*

	开机后SD卡的TXT和BMP文件分别会放到下拉列表中
	点击TXT庞的open按键，将在下面显示打开文件的内容
	(注：点击打开之前应该点击下拉列表选择文件，否则会
	打开默认的文件，默认的文件不是列表框看到的文件——可以设置为第一个文件)

	点击BMP旁边的open会将框架结构最小化 同时显示打开的BMP图片，当然BMP也有默认打开的，
	想将框架结构最大话，只需点击框架结构上的最大化按键就可以看，就是我们电脑操作上的
	最大化最小化和关闭按键（关键按键这里不需要 没有用）
*/


