#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"
#include "board_cfg.h"
#include "can.h"

/**********************************************************
		ȫ�ֱ���
**********************************************************/


/************************************IO��ѯ����**************************************/
/**************************************************************************************/
unsigned int iodata_stk[128];
void IoData_Task(char *p_arg)
{
	uint8_t err;
	CAN_FRAME SysInitId;						  //�����ϵ��Ͽ�֡
	
	SysInitId.DLC = DLC_BYTES_0;

	SysInitId.StdId.extid_32 = Guest_Address;

	InCanTrsQue(SysInitId);
   	
	OSSemPend(Sys_Init_Sem, 0, &err);     //�ȴ�io��ʼ�����
	
//   Config_Out_Freq(200, 0);
// 	Config_Out_Freq(1200, 1);
	while(1)
	{
		
		OSTimeDly(50);
		
	 	Input_Read(Input_State);
		
	}
	
}

/************************************���ݽ��մ�������**************************************/
/**************************************************************************************/
unsigned int CanRx_stk[128];
void CanRx_Task(void *p_arg) 
{ 
	uint8_t err;

	while(1) 
	{
		if ((NULL == CanRxHead) && (NULL == CanTrsHead))
		{
			OSSemPend(Can_Count_Sem, (6 * OS_TICKS_PER_SEC), &err);				//�ȴ�can�����ź���10s
			if(err == OS_ERR_TIMEOUT)
				OSSemPost(Sys_Reset_Sem);                      //�ȴ���ʱ�����͸�λ�ź�			                                            
		}
		CanRxProc();
		CanTxProc();
		OSTimeDly(1);								            //ִ������10ms
	}
}

/************************************״̬��ѯ����**************************************/
/**************************************************************************************/
unsigned int State_stk[128];
void State_Task(void *p_arg)
{
	OS_SEM_DATA  ResetSem;
	unsigned  char err;

	BSP_Init(); 
	OS_CPU_SysTickInit();
	
	Sys_Init_Sem = OSSemCreate(0);
	Sys_Reset_Sem =	OSSemCreate(0);
	Can_Count_Sem = OSSemCreate(0);

	err = OSTaskCreate((void (*) (void *)) &IoData_Task,
					   (void*)0,
					   (OS_STK *)&iodata_stk[128-1],
					   IODATA_TASK_PRIO);

	err = OSTaskCreate((void (*) (void *)) &CanRx_Task,
				   (void*)0,
				   (OS_STK *)&CanRx_stk[128-1],
				   CANRX_TASK_PRIO);

	while(1)
	{	
		OSSemQuery(Sys_Reset_Sem, &ResetSem);
		if(ResetSem.OSCnt == 0)
			IWDG_ReloadCounter();
		OSTimeDly(50);	
	}
} 
/************************************������������**************************************/
/**************************************************************************************/
void Task_Start(void)
{
//	/*�������񷵻�*/
	unsigned  char err;	

	/*��������*/


	err = OSTaskCreate((void (*) (void *)) &State_Task,
					   (void*)0,
					   (OS_STK *)&State_stk[128-1],
					   MAIN_TASK_PRIO);
#ifdef	 DEBUG
	Printf("err=0x%x\r\n",err);
#endif


	
}
