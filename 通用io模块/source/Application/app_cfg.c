#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"
#include "board_cfg.h"
#include "can.h"

/**********************************************************
		全局变量
**********************************************************/


/************************************IO查询任务**************************************/
/**************************************************************************************/
unsigned int iodata_stk[128];
void IoData_Task(char *p_arg)
{
	uint8_t err;
	CAN_FRAME SysInitId;						  //发送上电认可帧
	
	SysInitId.DLC = DLC_BYTES_0;

	SysInitId.StdId.extid_32 = Guest_Address;

	InCanTrsQue(SysInitId);
   	
	OSSemPend(Sys_Init_Sem, 0, &err);     //等待io初始化完成
	
//   Config_Out_Freq(200, 0);
// 	Config_Out_Freq(1200, 1);
	while(1)
	{
		
		OSTimeDly(50);
		
	 	Input_Read(Input_State);
		
	}
	
}

/************************************数据接收处理任务**************************************/
/**************************************************************************************/
unsigned int CanRx_stk[128];
void CanRx_Task(void *p_arg) 
{ 
	uint8_t err;

	while(1) 
	{
		if ((NULL == CanRxHead) && (NULL == CanTrsHead))
		{
			OSSemPend(Can_Count_Sem, (6 * OS_TICKS_PER_SEC), &err);				//等待can接收信号量10s
			if(err == OS_ERR_TIMEOUT)
				OSSemPost(Sys_Reset_Sem);                      //等待超时，发送复位信号			                                            
		}
		CanRxProc();
		CanTxProc();
		OSTimeDly(1);								            //执行周期10ms
	}
}

/************************************状态查询任务**************************************/
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
/************************************创建所有任务**************************************/
/**************************************************************************************/
void Task_Start(void)
{
//	/*保存任务返回*/
	unsigned  char err;	

	/*创建任务*/


	err = OSTaskCreate((void (*) (void *)) &State_Task,
					   (void*)0,
					   (OS_STK *)&State_stk[128-1],
					   MAIN_TASK_PRIO);
#ifdef	 DEBUG
	Printf("err=0x%x\r\n",err);
#endif


	
}
