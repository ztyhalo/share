#include "can.h"
#include "stdio.h"
#include "board_cfg.h"
#include "stm32f10x.h"

/*
*******************************************************************************
*变量定义
********************************************************************************
*/
TrsData    CanTrsHead;							           //发送链表首项
TrsData    CanTrsHandle;							       //发送完毕处理链表首项
RxData	   CanRxHead;
static  vu32      CanTrsTimer;
OS_MEM *CanDataBuf;
uint8_t Can_Data[4 * TrsLeng][sizeof(struct can_trs)];   //申请一片内存区域为μc管理
struct can_rec canrtest;

int			Io_State = 0;
int   	Guest_Address = 0x660;

/*********************************************************
** 函数名：	 SetCANSpeed
** 输　入：	 无  
** 输　出：	 无
** 功能描述：//设置CAN通讯速率	单位khz
************************************************************/

void SetCANSpeed(u16 Can1Speed)
{
    RCC_ClocksTypeDef  rcc_clocks; 
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  				
  
	/* CAN1 register init */
	CAN_DeInit(CAN1);

	/* Struct init*/
	CAN_StructInit(&CAN_InitStructure);

	/* CAN1 cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;			//禁止时间触发
	CAN_InitStructure.CAN_ABOM = ENABLE;			//离线自动恢复
	CAN_InitStructure.CAN_AWUM = ENABLE;			//自动唤醒
	CAN_InitStructure.CAN_NART = DISABLE;			//使能自动重传
	CAN_InitStructure.CAN_RFLM = DISABLE;			//接收溢出不锁定
	CAN_InitStructure.CAN_TXFP = ENABLE;			//优先级由发送请求的顺序来决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//;CAN_Mode_LoopBack	
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 

    RCC_GetClocksFreq(&rcc_clocks);   //调用标准库函数，获取系统时钟。

	// PCLK1/((1+BS1+BS2)*Prescaler)
	CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency / 1000 / Can1Speed / 
											(3+CAN_InitStructure.CAN_BS1+CAN_InitStructure.CAN_BS2);
	/*Initializes the CAN1 */
	CAN_Init(CAN1, &CAN_InitStructure);	

	/* CAN1 filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 1;							//过滤器号		  
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (Guest_Address - 0x200) << 5;//0x6420;	
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFE00;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;					
	CAN_FilterInit(&CAN_FilterInitStructure);
  


}
/*******************************************************************************************
**函数名称：CanTrsDummy
**函数作用：发送一个dummy帧，以规避0#邮箱发中断不再产生的可能。
**函数参数：CAN类型的数据
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanTrsDummy(CAN_TypeDef* CANx)
{
	CANx->sTxMailBox[0].TIR = ((u32)1 << 31);						//标准格式、数据帧
    
   	CANx->sTxMailBox[0].TDTR &= (u32)0xFFFFFFF0;					//数据长度为0
   	CANx->sTxMailBox[0].TDLR = 0;
   	CANx->sTxMailBox[0].TDHR = 0;
   	CANx->sTxMailBox[0].TIR |= ((u32)0x00000001);					//请求发送
}
/*********************************************************
** 函数名：	 CAN_Config
** 输　入：
** 输　出：
** 功能描述：CAN配置子程序。

************************************************************/
void CAN_Config(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	  /* Configure CAN1  **********************************************/
	  /* GPIOA, GPIOB and AFIO clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	  //使能PA口，复用功能时钟
	     
	  /* Configure CAN1 RX pin */											          //配置端口功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  /* Configure CAN1 TX pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;				//不使用中断优先级嵌套
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0级用于SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;				//不使用中断优先级嵌套
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0级用于SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SetCANSpeed(250);
 	CanTrsDummy(CAN1);
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);			
}


/*********************************************************
** 函数名：CAN_TransFram
** 输　入：None
** 输　出：None
** 功能描述：数据发送
************************************************************/
uint8_t CAN_TransFram(CAN_TypeDef* CANx, CAN_FRAME* TxMessage)
{
  uint8_t transmit_mailbox = 0;

  /* Select one empty transmit mailbox */
  if ((CANx->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
  {
    transmit_mailbox = 0;
  }
  else if ((CANx->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
  {
    transmit_mailbox = 1;
  }
  else if ((CANx->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
  {
    transmit_mailbox = 2;
  }
  else
  {
    transmit_mailbox = CAN_TxStatus_NoMailBox;
  }

  if (transmit_mailbox != CAN_TxStatus_NoMailBox)
  {
    /* Set up the Id */
    CANx->sTxMailBox[transmit_mailbox].TIR &= TMIDxR_TXRQ;
//    if (TxMessage->IDE == CAN_Id_Standard)都为标准帧
    {
       
      CANx->sTxMailBox[transmit_mailbox].TIR |= (TxMessage->StdId.extid_32 << 21);              //都为数据帧
    }
   
    
    /* Set up the DLC */
    TxMessage->DLC &= (uint8_t)0x0000000F;
    CANx->sTxMailBox[transmit_mailbox].TDTR &= (uint32_t)0xFFFFFFF0;
    CANx->sTxMailBox[transmit_mailbox].TDTR |= TxMessage->DLC;

    /* Set up the data field */
    CANx->sTxMailBox[transmit_mailbox].TDLR = (((uint32_t)TxMessage->Data[3] << 24) | 
                                             ((uint32_t)TxMessage->Data[2] << 16) |
                                             ((uint32_t)TxMessage->Data[1] << 8) | 
                                             ((uint32_t)TxMessage->Data[0]));
    CANx->sTxMailBox[transmit_mailbox].TDHR = (((uint32_t)TxMessage->Data[7] << 24) | 
                                             ((uint32_t)TxMessage->Data[6] << 16) |
                                             ((uint32_t)TxMessage->Data[5] << 8) |
                                             ((uint32_t)TxMessage->Data[4]));
    /* Request transmission */
    CANx->sTxMailBox[transmit_mailbox].TIR |= TMIDxR_TXRQ;
  }
  return transmit_mailbox;
}




/*********************************************************
** 函数名：void CanTrsData(void)
** 输　入：None
** 输　出：None
** 功能描述：数据发送的处理逻辑
************************************************************/
void CanTrsData(void)
{
	TrsData TrsCan;
	u16 time;
//	uint16_t Count;

	if ((CAN1->TSR & CAN_TSR_TME0) != CAN_TSR_TME0)	   							//0#发送邮箱不可用
		return;		
		
	if (NULL != CanTrsHead)
	{
		TrsCan = CanTrsHead;	                     				//保留待发信息,结构体直接赋值
		if(CanTrsHead->State == CAN_ON)										//can数据为发送状态
		{
			CAN_TransFram(CAN1, &(TrsCan->TrsFrame));
			CanTrsHead->State = CAN_WAITE;		
		}
		else															  //发送成功后等待应答或超时处理
		{
			time = CanTrsHead->Interval;
			if(CanTrsHead->Num != 0xf)											//不是无限发送
			{	
				CanTrsHead->Num--;												//发送次数-1
			}
			if((time == TIMER_CLOSED) && (CanTrsHead->Num == 0))				//发送间隔时间关闭
			{
				CanTrsHead = CanTrsHead->Next;
				InTrsHandle(TrsCan, CAN_TRS_SUCCESS);	                         //发送成功，加入发送完成链表
				CAN_ITConfig(CAN1, CAN_IT_TME, (CanTrsHead != NULL) ? ENABLE : DISABLE); //失能或使能发送中断	
			}
			else
			{
				CanTrsTimer = time+1;							                 //确保发送间隔>=i
				CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);					     //失能发送中断以等待应答或超时	
			}
					
		}		
	}

	else														              
	{																		 
		CanTrsDummy(CAN1);													 // 发送队列空发送一个dummy帧
		CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);							 //失能发送中断
	}
	
}
/*********************************************************
** 函数名：void CanTrsTimer(void)
** 输　入：None
** 输　出：None
** 功能描述：can发送数据定时器处理(定时数与系统滴答有关)
************************************************************/
void DecCanTrsTimer(void)
{
	TrsData p1;
	p1 = CanTrsHead;
 	if ((CanTrsTimer != TIMER_EXPIRED) && (CanTrsTimer != TIMER_CLOSED))			 //进行重发计时处理
		{
			CanTrsTimer--;
			if (CanTrsTimer == TIMER_EXPIRED)
			{
				if (CanTrsHead->Num == 0)
				{
					CanTrsHead = CanTrsHead->Next;
					InTrsHandle(p1, CAN_TRS_FAIL);	
	

				}
				if(CanTrsHead != NULL)
					CanTrsHead->State = CAN_ON;
				CAN_ITConfig(CAN1, CAN_IT_TME, (CanTrsHead != NULL) ? ENABLE : DISABLE);
			}
		}


}
/*******************************************************************************************
**函数名称：USB_HP_CAN1_TX_IRQHandler
**函数作用：CAN1发送中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
	OSIntEnter();
	CanTrsData(); 
	OSIntExit();
}
/*********************************************************
** 函数名： void RevAckApplyFrame(RxData RxCan)
** 输　入：None
** 输　出：None
** 功能描述：数据接收应答帧的处理
************************************************************/
static void RevAckApplyFrame(RxData RxCan)
{	
	TrsData p0;
	OS_CPU_SR  cpu_sr = 0;
	p0 = CanTrsHead;
	OS_ENTER_CRITICAL();
	if (RxCan->TrsFrame.StdId.extid.func_4 == CanTrsHead->TrsFrame.StdId.extid.func_4)    //为应答帧
	{
		CanTrsHead = CanTrsHead->Next;
		InTrsHandle(p0, CAN_TRS_SUCCESS);
//		CanTrsHead = (TrsData)DelCanQueueHead(CanTrsHead, CanDataBuf);                  //删除发送链表首项
		CanTrsTimer = TIMER_CLOSED;											            //关闭发送计时
		CAN_ITConfig(CAN1, CAN_IT_TME, (CanTrsHead != NULL) ? ENABLE : DISABLE);
	}
	OS_EXIT_CRITICAL();
}

/*********************************************************
** 函数名：InsertCanTrsQue(CAN_FRAME  TrsCan)
** 输　入：None
** 输　出：None
** 功能描述：根据优先级将发送帧插入发送队列
************************************************************/
u16 InCanTrsQue(CAN_FRAME  TrsCan)
{
	uint8_t err;
	u16 RetVal; 
	TrsData  InCanTrs;
	TrsData  p0 = NULL;
	TrsData  p1 = NULL;
	TrsData  p2 = NULL;
	OS_CPU_SR  cpu_sr = 0;
	RetVal = 0;
	if((InCanTrs = (TrsData)OSMemGet(CanDataBuf, &err)) == NULL)
	{
		if(err == OS_ERR_MEM_NO_FREE_BLKS)
		{
			return (RetVal);
		}
	}
		
	IdSeek(TrsCan, InCanTrs);               //根据id帧填充发送数据结构

	p1 = CanTrsHead;
	p0 = InCanTrs;
	OS_ENTER_CRITICAL();	               //关闭全局中断
	if(NULL != CanTrsHead)                 //首先判断头指针是否为空
	{
		while((TrsCan.StdId.extid_32 >= p1->TrsFrame.StdId.extid_32) && (NULL != p1->Next))	//按照优先级进行插入
		{
			p2 = p1;
			p1 = p1->Next;
		}

		if(TrsCan.StdId.extid_32 < p1->TrsFrame.StdId.extid_32)                   
		{
			if(CanTrsHead == p1)                                        //链表头 优先级最高
			{
				IdSeek(CanTrsHead->TrsFrame, CanTrsHead);			    //链表头处理
				CanTrsHead = p0;
				p0->Next =p1;
				CanTrsTimer = TIMER_CLOSED;					           //关闭发送定时器						                                          
			}		
			else
			{	
				p2->Next = p0;
				p0->Next =p1;
			}		
		}
		else					                                      //插入链表尾部
		{
			p1->Next = p0;
			p0->Next = NULL;
		}                                						
	}
	else								                              //空链表
	{
		CanTrsHead = p0;
		p0->Next = NULL;
	}

	if (CanTrsTimer == TIMER_CLOSED || CanTrsTimer == TIMER_EXPIRED)	 //发送计时结束
	{
		CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);	                     //使能发送中断
	}
	
	RetVal = 1;						                       
	

	OS_EXIT_CRITICAL();	  	                                         //开全局中断
	return(RetVal);
}
/*********************************************************
** 函数名：InitCan(void)
** 输　入：None
** 输　出：None
** 功能描述：can发送初始化
************************************************************/
void InitCan(void)
{
	
	uint8_t err;
	CanDataBuf = OSMemCreate(Can_Data, 4 * TrsLeng, sizeof(struct can_trs), &err);
	if(err == OS_ERR_NONE)
	{
		CanTrsTimer = TIMER_CLOSED;
		CanTrsHead = NULL;
		CanTrsHandle = NULL;
		CanRxHead = NULL;
	}

													   
}
/*********************************************************
** 函数名：IdSeek(CAN_FRAME TxCan, TrsData TxCanFrame)
** 输　入：None
** 输　出：None
** 功能描述：根据ID填充发送结构体
************************************************************/
void IdSeek(CAN_FRAME TxCan, TrsData TxCanFrame)
{	
	TxCanFrame->TrsFrame = TxCan;
	TxCanFrame->State = CAN_ON;
	switch (TxCan.StdId.extid_32 & FUNCTION_MASK)
	{
		case START_ASK:			                               //上电认可信息请求帧
			TxCanFrame->Num = 0xf;
			TxCanFrame->Interval = 2 * OS_TICKS_PER_SEC;	      //时间计数与系统滴答有关
			TxCanFrame->TrsTime = 0xffff;
			
		break;

		case OUTCONTR_ACK:        	       	                  //输出控制指令应答
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff; 
			TxCanFrame->TrsTime = 20 * OS_TICKS_PER_SEC / 1000; //时间计数与系统滴答有关
		break;

		case DATA_ACK_ONE:      				                 //数据请求帧应答1
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //时间计数与系统滴答有关
			TxCanFrame->TrsTime = 50 * OS_TICKS_PER_SEC / 1000 ;
		break;
		case DATA_ACK_TWO:        	 				         //数据请求帧应答2	
	  case DATA_ACK_THR:      	 				             //数据请求帧应答3
	  case DATA_ACK_FOU:        					         //数据请求帧应答4
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //时间计数与系统滴答有关
			TxCanFrame->TrsTime = 0xffff;
		break;

	    case CONFIG_ACK:         					         //上电认可应答及配置的应答
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //时间计数与系统滴答有关
			TxCanFrame->TrsTime = 0xffff;
		break;

	    case INCHANGE_ASK:
			TxCanFrame->Num = 0x5;
			TxCanFrame->Interval = 50 * OS_TICKS_PER_SEC / 1000; //时间计数与系统滴答有关
			TxCanFrame->TrsTime = 0xffff;
		break;
		
		default:
		break;	      

	}
}
/*********************************************************
** 函数名：InTrsHandle(CAN_FRAME TxCan, TrsData TxCanFrame)
** 输　入：None
** 输　出：None
** 功能描述：插入发送完毕处理队列
************************************************************/
uint8_t InTrsHandle(TrsData TxCanHandle, u8 Res)
{
//	uint8_t err; 
	TrsData  p0;
	OS_CPU_SR  cpu_sr = 0;
	
	OS_ENTER_CRITICAL();	
	p0 = CanTrsHandle;
	
	TxCanHandle->Result = Res;
	TxCanHandle->Next   = NULL;

	if(NULL != CanTrsHandle)
	{
		while(p0->Next != NULL)
		{
			p0 = p0->Next;	
		}
		p0->Next = TxCanHandle;
	}
	else
	{
		CanTrsHandle = TxCanHandle;                         //表头
	} 
	OS_EXIT_CRITICAL();
	return(1);		
}
/*******************************************************************************************
**函数名称：void InRxQue(RxData InRxData)
** 输　入：None
** 输　出：None
** 功能描述：插入接收队列
*******************************************************************************************/
void InRxQue(RxData InRxData)
{
	RxData p0;
	OS_CPU_SR  cpu_sr = 0;
	
	OS_ENTER_CRITICAL();	
	p0 = CanRxHead;
	if(NULL != CanRxHead)
	{
		while(NULL != p0->Next)
		{
			p0 = p0->Next;
		}
		p0->Next = InRxData; 
	} 
	else
	{
		CanRxHead = InRxData;
	}	
	OS_EXIT_CRITICAL();
}
/*********************************************************
** 函数名：u8 DelCanQueueHead（void *LineHead）
** 输　入：None
** 输　出：None
** 功能描述：删除链表首项     delete send line of head
************************************************************/
void *DelCanQueueHead(void *LineHead, OS_MEM *Buf)
{
	
	TrsData  p0;
	TrsData  p1;
	OS_CPU_SR  cpu_sr = 0;
	p1 = (TrsData)LineHead;
	OS_ENTER_CRITICAL();	               //关闭全局中断
	if (p1 != NULL)
	{
		p0 = p1;
		p1 = p1->Next;
		OSMemPut(Buf, p0);                 //释放分区
		

	}
	else 
		p1 = NULL;
	OS_EXIT_CRITICAL();
	return(p1);
}
/*********************************************************
** 函数名：DelCanQueueOne(void *Line, void *Buf, void *LineHead)
** 输　入：None
** 输　出：None
** 功能描述：删除发送链表某一项	 /在中断中使用
************************************************************/
u8 DelCanQueueOne(TrsData Line, OS_MEM *Buf)
{
	TrsData p0;
	TrsData p1;
	u8 err = 0;
	p0 = CanTrsHead;
	p1 = CanTrsHead;
	if(CanTrsHead != NULL)
	{
		while((Line != p0) && (p0->Next != NULL))
		{
			p1 = p0;
			p0 = p0->Next;
		}
		if(NULL == p0->Next)		                //删除项不在指定链表中
		{
			err = 144;
			
		}
		else
		{

			if(p0 == CanTrsHead)			
				CanTrsHead = p0->Next;			
			else
				p1->Next = p0->Next;
			err = OSMemPut(Buf, p0);                 //释放分区
		}
		 
	}
	return(err);		
}

/*******************************************************************************************
**函数名称：void USB_LP_CAN1_RX0_IRQHandler(void) 
**输　入：None
** 输　出：None
** 功能描述：CAN接收中断
*******************************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)   
{
	u16 i;
	u8 err;
	OS_SEM_DATA pdata;
	CanRxMsg  CanRxMsg1;
	RxData 	  CanRxData;

	if((CanRxData = (RxData)OSMemGet(CanDataBuf, &err)) == NULL)		//创建处理数据存储空间 
	{
		if(err == OS_ERR_MEM_NO_FREE_BLKS)
		{
			return;
		}
	}
	CAN_Receive(CAN1, CAN_FIFO0, &CanRxMsg1);
	CanRxData->TrsFrame.StdId.extid_32 = CanRxMsg1.StdId;
	CanRxData->TrsFrame.DLC = CanRxMsg1.DLC;
	for(i = 0; i < CanRxMsg1.DLC; i++)
	{
		CanRxData->TrsFrame.Data[i] = CanRxMsg1.Data[i];
	} 
	CanRxData->Next = NULL;
	InRxQue(CanRxData);
	
	OSSemQuery(Can_Count_Sem, &pdata);
	if(pdata.OSEventGrp != 0x00)					 //	有任务在等待信号量
	{
//		OS_ENTER_CRITICAL();                        //保存全局中断标志,关总中断
		OSIntNesting++;	                            //OSSemPost(NMEA_MBOX);
//		OS_EXIT_CRITICAL();	                       //恢复全局中断标志
		OSSemPost(Can_Count_Sem);					 //发送信号量 停止can空闲计时
		OSIntExit();

	}
	
}
/*******************************************************************************************
**函数名称：void Out_Control(uint8_t Out_Data[])
**输　入：None
** 输　出：None
** 功能描述：Can接收数据处理
*******************************************************************************************/
void Out_Control(uint8_t Out_Data[])
{
	uint16_t Freq_Set;
	if(Out_Data[0] >= Out_Number)
	{
		Freq_Set = (uint16_t)Out_Data[1] >> 4;
		Freq_Set =	(Freq_Set << 8) | Out_Data[2]; 
		Config_Out_Freq(Freq_Set, (Out_Data[0] - Out_Number));			
	}
	else
	{
		Out_Write(Out_Data[0], Out_Data[1]);   //outdata[0]输出引脚 [1]输出数
	}			
}
/*******************************************************************************************
**函数名称：void Data_Ask_Proc(TrsData Ack_Data)
**输　入：None
** 输　出：None
** 功能描述：数据请求处理
*******************************************************************************************/
void Data_Ask_Proc(RxData Ack_Data)
{
	uint8_t i,j;

	Ack_Data->TrsFrame.DLC = DLC_BYTES_8;
	Ack_Data->TrsFrame.StdId.extid_32 = Guest_Address | DATA_ACK_ONE;
	for(j = 0; j< 4; j++)

	{
		 for(i = 0; i < 4; i++)
			{
				Ack_Data->TrsFrame.Data[2 * i] = (Input_State[i + 4 * j].Type << 7) | (Input_State[i + 4 * j].Error << 4) |
												(Input_State[i + 4 * j].Value >> 8) ;
				Ack_Data->TrsFrame.Data[2 * i + 1] =  (uint8_t)Input_State[i + 4 * j].Value;
							
			}
		Ack_Data->TrsFrame.StdId.extid_32 = (Guest_Address | DATA_ACK_ONE) + j;
		InCanTrsQue(Ack_Data->TrsFrame);
	}	
}
/*******************************************************************************************
**函数名称：CanRxProc
**输　入：None
** 输　出：None
** 功能描述：Can接收数据处理
*******************************************************************************************/
void CanRxProc(void)
{
	RxData p0;
	p0 = CanRxHead;
	if(CanRxHead != NULL)
	{
		switch(CanRxHead->TrsFrame.StdId.extid_32 & 0x000f)
		{
			case START_ACK:
				RevAckApplyFrame(CanRxHead);
				canrtest = *CanRxHead;
				Output_Init();
				Out_Init(CanRxHead->TrsFrame.Data);				   //进行配置
				Input_Init(&CanRxHead->TrsFrame.Data[3]);
				CanRxHead->TrsFrame.StdId.extid_32 = Guest_Address | CONFIG_ACK;
				InCanTrsQue(CanRxHead->TrsFrame);					//发送应答帧
				OSSemPost(Sys_Init_Sem);						 //发送初始化完成信号量
				Io_State = 1;
			break;			
	
			case OUTCONTR_ASK:
				if(Io_State)
				{
					Out_Control(CanRxHead->TrsFrame.Data);		       //输出口控制
					CanRxHead->TrsFrame.StdId.extid_32 = Guest_Address | OUTCONTR_ACK;
					InCanTrsQue(CanRxHead->TrsFrame);
				}
			break;
	
			case DATA_ASK:
				if(Io_State)
					Data_Ask_Proc(CanRxHead);                           //数据请求
			break; 
	
			case CHANGE_RESPOND:
				if(Io_State)
					RevAckApplyFrame(CanRxHead);
			break;
	
			default:
			break;
				
		}
		
		CanRxHead = CanRxHead->Next;
		OSMemPut(CanDataBuf, (TrsData)p0); 
	}	
}
/*******************************************************************************************
**函数名称：CanTxProc
**输　入：None
** 输　出：None
** 功能描述：Can发送数据处理
*******************************************************************************************/
void CanTxProc(void)
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();	               //关闭全局中断   
	while(CanTrsHandle != NULL)
	{
		switch(CanTrsHandle->TrsFrame.StdId.extid_32 & 0x000f)
		{
			case OUTCONTR_ACK:       	                //输出控制指令应答
			case DATA_ACK_ONE:      				        //数据请求帧应答1
			case DATA_ACK_TWO:        	 				//数据请求帧应答2	
			case DATA_ACK_THR:      	 				    //数据请求帧应答3
			case DATA_ACK_FOU:        					//数据请求帧应答4
			case INCHANGE_ASK:
			{				//输入口变化请求
				if(CanTrsHandle->Result == CAN_TRS_FAIL)
				{
					OSSemPost(Sys_Reset_Sem);
			
				}
			}
			break;
	
			default:
			break;	 		
		}

		CanTrsHandle = DelCanQueueHead(CanTrsHandle, CanDataBuf);

	}		
	OS_EXIT_CRITICAL();
}




/*******************************************************************************************/

