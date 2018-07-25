#include "can.h"
#include "stdio.h"
#include "board_cfg.h"
#include "stm32f10x.h"

/*
*******************************************************************************
*��������
********************************************************************************
*/
TrsData    CanTrsHead;							           //������������
TrsData    CanTrsHandle;							       //������ϴ�����������
RxData	   CanRxHead;
static  vu32      CanTrsTimer;
OS_MEM *CanDataBuf;
uint8_t Can_Data[4 * TrsLeng][sizeof(struct can_trs)];   //����һƬ�ڴ�����Ϊ��c����
struct can_rec canrtest;

int			Io_State = 0;
int   	Guest_Address = 0x660;

/*********************************************************
** ��������	 SetCANSpeed
** �䡡�룺	 ��  
** �䡡����	 ��
** ����������//����CANͨѶ����	��λkhz
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
	CAN_InitStructure.CAN_TTCM = DISABLE;			//��ֹʱ�䴥��
	CAN_InitStructure.CAN_ABOM = ENABLE;			//�����Զ��ָ�
	CAN_InitStructure.CAN_AWUM = ENABLE;			//�Զ�����
	CAN_InitStructure.CAN_NART = DISABLE;			//ʹ���Զ��ش�
	CAN_InitStructure.CAN_RFLM = DISABLE;			//�������������
	CAN_InitStructure.CAN_TXFP = ENABLE;			//���ȼ��ɷ��������˳��������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//;CAN_Mode_LoopBack	
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 

    RCC_GetClocksFreq(&rcc_clocks);   //���ñ�׼�⺯������ȡϵͳʱ�ӡ�

	// PCLK1/((1+BS1+BS2)*Prescaler)
	CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency / 1000 / Can1Speed / 
											(3+CAN_InitStructure.CAN_BS1+CAN_InitStructure.CAN_BS2);
	/*Initializes the CAN1 */
	CAN_Init(CAN1, &CAN_InitStructure);	

	/* CAN1 filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 1;							//��������		  
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
**�������ƣ�CanTrsDummy
**�������ã�����һ��dummy֡���Թ��0#���䷢�жϲ��ٲ����Ŀ��ܡ�
**����������CAN���͵�����
**�����������
**ע�������
*******************************************************************************************/
void CanTrsDummy(CAN_TypeDef* CANx)
{
	CANx->sTxMailBox[0].TIR = ((u32)1 << 31);						//��׼��ʽ������֡
    
   	CANx->sTxMailBox[0].TDTR &= (u32)0xFFFFFFF0;					//���ݳ���Ϊ0
   	CANx->sTxMailBox[0].TDLR = 0;
   	CANx->sTxMailBox[0].TDHR = 0;
   	CANx->sTxMailBox[0].TIR |= ((u32)0x00000001);					//������
}
/*********************************************************
** ��������	 CAN_Config
** �䡡�룺
** �䡡����
** ����������CAN�����ӳ���

************************************************************/
void CAN_Config(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	  /* Configure CAN1  **********************************************/
	  /* GPIOA, GPIOB and AFIO clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��PA�ڣ����ù���ʱ��
	     
	  /* Configure CAN1 RX pin */											          //���ö˿ڹ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  /* Configure CAN1 TX pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;				//��ʹ���ж����ȼ�Ƕ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0������SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;				//��ʹ���ж����ȼ�Ƕ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0������SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SetCANSpeed(250);
 	CanTrsDummy(CAN1);
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);			
}


/*********************************************************
** ��������CAN_TransFram
** �䡡�룺None
** �䡡����None
** �������������ݷ���
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
//    if (TxMessage->IDE == CAN_Id_Standard)��Ϊ��׼֡
    {
       
      CANx->sTxMailBox[transmit_mailbox].TIR |= (TxMessage->StdId.extid_32 << 21);              //��Ϊ����֡
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
** ��������void CanTrsData(void)
** �䡡�룺None
** �䡡����None
** �������������ݷ��͵Ĵ����߼�
************************************************************/
void CanTrsData(void)
{
	TrsData TrsCan;
	u16 time;
//	uint16_t Count;

	if ((CAN1->TSR & CAN_TSR_TME0) != CAN_TSR_TME0)	   							//0#�������䲻����
		return;		
		
	if (NULL != CanTrsHead)
	{
		TrsCan = CanTrsHead;	                     				//����������Ϣ,�ṹ��ֱ�Ӹ�ֵ
		if(CanTrsHead->State == CAN_ON)										//can����Ϊ����״̬
		{
			CAN_TransFram(CAN1, &(TrsCan->TrsFrame));
			CanTrsHead->State = CAN_WAITE;		
		}
		else															  //���ͳɹ���ȴ�Ӧ���ʱ����
		{
			time = CanTrsHead->Interval;
			if(CanTrsHead->Num != 0xf)											//�������޷���
			{	
				CanTrsHead->Num--;												//���ʹ���-1
			}
			if((time == TIMER_CLOSED) && (CanTrsHead->Num == 0))				//���ͼ��ʱ��ر�
			{
				CanTrsHead = CanTrsHead->Next;
				InTrsHandle(TrsCan, CAN_TRS_SUCCESS);	                         //���ͳɹ������뷢���������
				CAN_ITConfig(CAN1, CAN_IT_TME, (CanTrsHead != NULL) ? ENABLE : DISABLE); //ʧ�ܻ�ʹ�ܷ����ж�	
			}
			else
			{
				CanTrsTimer = time+1;							                 //ȷ�����ͼ��>=i
				CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);					     //ʧ�ܷ����ж��Եȴ�Ӧ���ʱ	
			}
					
		}		
	}

	else														              
	{																		 
		CanTrsDummy(CAN1);													 // ���Ͷ��пշ���һ��dummy֡
		CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);							 //ʧ�ܷ����ж�
	}
	
}
/*********************************************************
** ��������void CanTrsTimer(void)
** �䡡�룺None
** �䡡����None
** ����������can�������ݶ�ʱ������(��ʱ����ϵͳ�δ��й�)
************************************************************/
void DecCanTrsTimer(void)
{
	TrsData p1;
	p1 = CanTrsHead;
 	if ((CanTrsTimer != TIMER_EXPIRED) && (CanTrsTimer != TIMER_CLOSED))			 //�����ط���ʱ����
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
**�������ƣ�USB_HP_CAN1_TX_IRQHandler
**�������ã�CAN1�����ж�
**������������
**�����������
**ע�������
*******************************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
	OSIntEnter();
	CanTrsData(); 
	OSIntExit();
}
/*********************************************************
** �������� void RevAckApplyFrame(RxData RxCan)
** �䡡�룺None
** �䡡����None
** �������������ݽ���Ӧ��֡�Ĵ���
************************************************************/
static void RevAckApplyFrame(RxData RxCan)
{	
	TrsData p0;
	OS_CPU_SR  cpu_sr = 0;
	p0 = CanTrsHead;
	OS_ENTER_CRITICAL();
	if (RxCan->TrsFrame.StdId.extid.func_4 == CanTrsHead->TrsFrame.StdId.extid.func_4)    //ΪӦ��֡
	{
		CanTrsHead = CanTrsHead->Next;
		InTrsHandle(p0, CAN_TRS_SUCCESS);
//		CanTrsHead = (TrsData)DelCanQueueHead(CanTrsHead, CanDataBuf);                  //ɾ��������������
		CanTrsTimer = TIMER_CLOSED;											            //�رշ��ͼ�ʱ
		CAN_ITConfig(CAN1, CAN_IT_TME, (CanTrsHead != NULL) ? ENABLE : DISABLE);
	}
	OS_EXIT_CRITICAL();
}

/*********************************************************
** ��������InsertCanTrsQue(CAN_FRAME  TrsCan)
** �䡡�룺None
** �䡡����None
** �����������������ȼ�������֡���뷢�Ͷ���
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
		
	IdSeek(TrsCan, InCanTrs);               //����id֡��䷢�����ݽṹ

	p1 = CanTrsHead;
	p0 = InCanTrs;
	OS_ENTER_CRITICAL();	               //�ر�ȫ���ж�
	if(NULL != CanTrsHead)                 //�����ж�ͷָ���Ƿ�Ϊ��
	{
		while((TrsCan.StdId.extid_32 >= p1->TrsFrame.StdId.extid_32) && (NULL != p1->Next))	//�������ȼ����в���
		{
			p2 = p1;
			p1 = p1->Next;
		}

		if(TrsCan.StdId.extid_32 < p1->TrsFrame.StdId.extid_32)                   
		{
			if(CanTrsHead == p1)                                        //����ͷ ���ȼ����
			{
				IdSeek(CanTrsHead->TrsFrame, CanTrsHead);			    //����ͷ����
				CanTrsHead = p0;
				p0->Next =p1;
				CanTrsTimer = TIMER_CLOSED;					           //�رշ��Ͷ�ʱ��						                                          
			}		
			else
			{	
				p2->Next = p0;
				p0->Next =p1;
			}		
		}
		else					                                      //��������β��
		{
			p1->Next = p0;
			p0->Next = NULL;
		}                                						
	}
	else								                              //������
	{
		CanTrsHead = p0;
		p0->Next = NULL;
	}

	if (CanTrsTimer == TIMER_CLOSED || CanTrsTimer == TIMER_EXPIRED)	 //���ͼ�ʱ����
	{
		CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);	                     //ʹ�ܷ����ж�
	}
	
	RetVal = 1;						                       
	

	OS_EXIT_CRITICAL();	  	                                         //��ȫ���ж�
	return(RetVal);
}
/*********************************************************
** ��������InitCan(void)
** �䡡�룺None
** �䡡����None
** ����������can���ͳ�ʼ��
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
** ��������IdSeek(CAN_FRAME TxCan, TrsData TxCanFrame)
** �䡡�룺None
** �䡡����None
** ��������������ID��䷢�ͽṹ��
************************************************************/
void IdSeek(CAN_FRAME TxCan, TrsData TxCanFrame)
{	
	TxCanFrame->TrsFrame = TxCan;
	TxCanFrame->State = CAN_ON;
	switch (TxCan.StdId.extid_32 & FUNCTION_MASK)
	{
		case START_ASK:			                               //�ϵ��Ͽ���Ϣ����֡
			TxCanFrame->Num = 0xf;
			TxCanFrame->Interval = 2 * OS_TICKS_PER_SEC;	      //ʱ�������ϵͳ�δ��й�
			TxCanFrame->TrsTime = 0xffff;
			
		break;

		case OUTCONTR_ACK:        	       	                  //�������ָ��Ӧ��
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff; 
			TxCanFrame->TrsTime = 20 * OS_TICKS_PER_SEC / 1000; //ʱ�������ϵͳ�δ��й�
		break;

		case DATA_ACK_ONE:      				                 //��������֡Ӧ��1
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //ʱ�������ϵͳ�δ��й�
			TxCanFrame->TrsTime = 50 * OS_TICKS_PER_SEC / 1000 ;
		break;
		case DATA_ACK_TWO:        	 				         //��������֡Ӧ��2	
	  case DATA_ACK_THR:      	 				             //��������֡Ӧ��3
	  case DATA_ACK_FOU:        					         //��������֡Ӧ��4
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //ʱ�������ϵͳ�δ��й�
			TxCanFrame->TrsTime = 0xffff;
		break;

	    case CONFIG_ACK:         					         //�ϵ��Ͽ�Ӧ�����õ�Ӧ��
			TxCanFrame->Num = 0x1;
			TxCanFrame->Interval = 0xffff;	                     //ʱ�������ϵͳ�δ��й�
			TxCanFrame->TrsTime = 0xffff;
		break;

	    case INCHANGE_ASK:
			TxCanFrame->Num = 0x5;
			TxCanFrame->Interval = 50 * OS_TICKS_PER_SEC / 1000; //ʱ�������ϵͳ�δ��й�
			TxCanFrame->TrsTime = 0xffff;
		break;
		
		default:
		break;	      

	}
}
/*********************************************************
** ��������InTrsHandle(CAN_FRAME TxCan, TrsData TxCanFrame)
** �䡡�룺None
** �䡡����None
** �������������뷢����ϴ������
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
		CanTrsHandle = TxCanHandle;                         //��ͷ
	} 
	OS_EXIT_CRITICAL();
	return(1);		
}
/*******************************************************************************************
**�������ƣ�void InRxQue(RxData InRxData)
** �䡡�룺None
** �䡡����None
** ����������������ն���
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
** ��������u8 DelCanQueueHead��void *LineHead��
** �䡡�룺None
** �䡡����None
** ����������ɾ����������     delete send line of head
************************************************************/
void *DelCanQueueHead(void *LineHead, OS_MEM *Buf)
{
	
	TrsData  p0;
	TrsData  p1;
	OS_CPU_SR  cpu_sr = 0;
	p1 = (TrsData)LineHead;
	OS_ENTER_CRITICAL();	               //�ر�ȫ���ж�
	if (p1 != NULL)
	{
		p0 = p1;
		p1 = p1->Next;
		OSMemPut(Buf, p0);                 //�ͷŷ���
		

	}
	else 
		p1 = NULL;
	OS_EXIT_CRITICAL();
	return(p1);
}
/*********************************************************
** ��������DelCanQueueOne(void *Line, void *Buf, void *LineHead)
** �䡡�룺None
** �䡡����None
** ����������ɾ����������ĳһ��	 /���ж���ʹ��
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
		if(NULL == p0->Next)		                //ɾ�����ָ��������
		{
			err = 144;
			
		}
		else
		{

			if(p0 == CanTrsHead)			
				CanTrsHead = p0->Next;			
			else
				p1->Next = p0->Next;
			err = OSMemPut(Buf, p0);                 //�ͷŷ���
		}
		 
	}
	return(err);		
}

/*******************************************************************************************
**�������ƣ�void USB_LP_CAN1_RX0_IRQHandler(void) 
**�䡡�룺None
** �䡡����None
** ����������CAN�����ж�
*******************************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)   
{
	u16 i;
	u8 err;
	OS_SEM_DATA pdata;
	CanRxMsg  CanRxMsg1;
	RxData 	  CanRxData;

	if((CanRxData = (RxData)OSMemGet(CanDataBuf, &err)) == NULL)		//�����������ݴ洢�ռ� 
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
	if(pdata.OSEventGrp != 0x00)					 //	�������ڵȴ��ź���
	{
//		OS_ENTER_CRITICAL();                        //����ȫ���жϱ�־,�����ж�
		OSIntNesting++;	                            //OSSemPost(NMEA_MBOX);
//		OS_EXIT_CRITICAL();	                       //�ָ�ȫ���жϱ�־
		OSSemPost(Can_Count_Sem);					 //�����ź��� ֹͣcan���м�ʱ
		OSIntExit();

	}
	
}
/*******************************************************************************************
**�������ƣ�void Out_Control(uint8_t Out_Data[])
**�䡡�룺None
** �䡡����None
** ����������Can�������ݴ���
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
		Out_Write(Out_Data[0], Out_Data[1]);   //outdata[0]������� [1]�����
	}			
}
/*******************************************************************************************
**�������ƣ�void Data_Ask_Proc(TrsData Ack_Data)
**�䡡�룺None
** �䡡����None
** ��������������������
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
**�������ƣ�CanRxProc
**�䡡�룺None
** �䡡����None
** ����������Can�������ݴ���
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
				Out_Init(CanRxHead->TrsFrame.Data);				   //��������
				Input_Init(&CanRxHead->TrsFrame.Data[3]);
				CanRxHead->TrsFrame.StdId.extid_32 = Guest_Address | CONFIG_ACK;
				InCanTrsQue(CanRxHead->TrsFrame);					//����Ӧ��֡
				OSSemPost(Sys_Init_Sem);						 //���ͳ�ʼ������ź���
				Io_State = 1;
			break;			
	
			case OUTCONTR_ASK:
				if(Io_State)
				{
					Out_Control(CanRxHead->TrsFrame.Data);		       //����ڿ���
					CanRxHead->TrsFrame.StdId.extid_32 = Guest_Address | OUTCONTR_ACK;
					InCanTrsQue(CanRxHead->TrsFrame);
				}
			break;
	
			case DATA_ASK:
				if(Io_State)
					Data_Ask_Proc(CanRxHead);                           //��������
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
**�������ƣ�CanTxProc
**�䡡�룺None
** �䡡����None
** ����������Can�������ݴ���
*******************************************************************************************/
void CanTxProc(void)
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();	               //�ر�ȫ���ж�   
	while(CanTrsHandle != NULL)
	{
		switch(CanTrsHandle->TrsFrame.StdId.extid_32 & 0x000f)
		{
			case OUTCONTR_ACK:       	                //�������ָ��Ӧ��
			case DATA_ACK_ONE:      				        //��������֡Ӧ��1
			case DATA_ACK_TWO:        	 				//��������֡Ӧ��2	
			case DATA_ACK_THR:      	 				    //��������֡Ӧ��3
			case DATA_ACK_FOU:        					//��������֡Ӧ��4
			case INCHANGE_ASK:
			{				//����ڱ仯����
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

