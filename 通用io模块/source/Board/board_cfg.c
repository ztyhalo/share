#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"
#include "board_cfg.h"
#include "can.h"


/*************************************ȫ�ֱ�������***********************************/
/************************************************************************************/
OS_EVENT*	Sys_Init_Sem;			  //ϵͳ��ʼ���ź���
OS_EVENT*	Sys_Reset_Sem;			  //ϵͳ��λ�ź���
OS_EVENT*	Can_Count_Sem;			  //Can�����ݼ�ʱ�ź���
Freq_Out 	Out_Freq[2];		      //���Ƶ����
Freq_In		In_Freq[In_Number][Freq_Shake];
Freq_In	   *Freq_Head[In_Number]; 	
Input		Input_State[16];
volatile uint32_t Fre_Count[16];
uint8_t vtest = 1;




	

/******************************************************************************
** ��������: Input_Init
** ��������: ����ڳ�ʼ������
** ����������Ҫ���õ�����
*******************************************************************************/
void Input_Init(uint8_t *Data)
{
	GPIO_InitTypeDef GPIO_InitStructure;					   //����һ��GPIO�ṹ�����
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint8_t i, j;
	uint32_t ConfigPin;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2_IO,ENABLE);	               //ʹ�ܸ����˿�ʱ�ӣ���Ҫ������

	ConfigPin = 0;
	for(i = 0; i < In_Number * 2 / 8; i++)
	{
		ConfigPin  |= (uint32_t)(*(Data + i)) << (8 * (3 - i));
	}

	for(i = 0; i < In_Number; i++)
	{
		if((ConfigPin << (2 * i)) & 0x80000000)                        //����ʹ��
		{
		 	GPIO_InitStructure.GPIO_Pin   = (uint16_t)0x01 << i; 	   //��������˿�
  			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	   	      //����Ϊ��������
  			GPIO_Init(Input_Port, &GPIO_InitStructure);				  //������˿ڽ��г�ʼ������

			Input_State[i].State = 1;								 //�������ݳ�ʼ��

			if((ConfigPin << (2 * i)) & 0x40000000)		             //����ΪƵ����
			{
				GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, i);
			    EXTI_InitStructure.EXTI_Line = (uint16_t)0x01 << i;
				EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
				EXTI_InitStructure.EXTI_LineCmd = ENABLE;
				EXTI_Init(&EXTI_InitStructure);
			
				NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
				NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

				/*�����ⲿ�ж�ʹ���ж�-------------------------------*/
				if(i >4 && i < 10)
					NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn ;
				else if(i >= 10)
					NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;
				else
					NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn + i ;				    
			    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
			    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);

			  	Input_State[i].Type = Freque_Type;
				Input_State[i].Error = Senor_No_Error;
				Input_State[i].Value = 0;
				Input_State[i].Count = 0;
				Input_State[i].FreqS = 0;

				Freq_Head[i] = In_Freq[i];

				for(j = 0; j < (Freq_Shake - 1); j++)
				{
					In_Freq[i][j].Next = &In_Freq[i][j+1];	
				}
			   In_Freq[i][Freq_Shake - 1].Next = &In_Freq[i][0];
			
				//EXTI->IMR =EXTI->IMR &0xfffffcff;     //�����ж�����
			}
			else										//������������
			{
			  	Input_State[i].Type = Switch_Type;
				Input_State[i].Error = Senor_No_Error;
				Input_State[i].Value = 1;
				Input_State[i].Count = 0;	
			}
		}
		else
		{
			
		 	  GPIO_InitStructure.GPIO_Pin   = (uint16_t)0x01 << i; 	       //��������˿�
  			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	       //����Ϊ��������
  			GPIO_Init(Input_Port, &GPIO_InitStructure);	

			Input_State[i].State = 0;
			Input_State[i].Type = 0;
			Input_State[i].Error = 0;
			Input_State[i].Value = 1;
			Input_State[i].Count = 0;		


		}
	}
	
}


/******************************************************************************
** ��������: Freq_Output_Init() 
** ��������: �����Ƶ������ʼ������
** ����������Ҫ���õ�����
*******************************************************************************/
void Freq_Output_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2_IO,ENABLE);
		            
	GPIO_InitStructure.GPIO_Pin = FreOutput1 | FreOutput2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FreOutput, &GPIO_InitStructure);


	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);   //��ʱ��1������ӳ��
		
}
/******************************************************************************
** ��������: Timer_Init() 
** ��������: �����Ƶ������ʼ������
** ����������Ҫ���õ�����
*******************************************************************************/
void Timer_Init(void)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
#ifdef OLDBOARD
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);             // ʹ�ܶ�ʱ��3ʱ��

	TIM_TimeBaseStructure.TIM_Period = 0xffff;                      //������ֵ��ѭ������         
	TIM_TimeBaseStructure.TIM_Prescaler = Out_Freq_Dive;             //359��ƵΪ��Ƶ����1      	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         // ʱ�ӷָ� 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //�����������ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	 	               

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				    	//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 500;								  //��ʼ�Ƶ��200hz
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);			          //ʧ��Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
#else

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);             // ʹ�ܶ�ʱ��1ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff;                      //������ֵ��ѭ������         
	TIM_TimeBaseStructure.TIM_Prescaler = Out_Freq_Dive;             //359��ƵΪ��Ƶ����1      	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         // ʱ�ӷָ� 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //�����������ϼ���
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	 	               

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;				 //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;    //TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 500;								                //��ʼ�Ƶ��200hz
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);			          //ʧ��Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
#endif

}
/******************************************************************************
** ��������: Output_Init() 
** ��������: ����ڳ�ʼ������
** ����������Ҫ���õ�����
*******************************************************************************/
void Output_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����
	uint8_t i;

	RCC_APB2PeriphClockCmd(RCC_APB2_IO,ENABLE);	            //ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
	GPIO_InitStructure.GPIO_Pin   = OutputAll; 	             //��������˿�
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Output_Port, &GPIO_InitStructure);

	for(i = 0; i < 12; i++)
	  Out_Write(i, 1);

	Freq_Output_Init();
	Timer_Init();
}
/******************************************************************************
** ��������: Config_Out_Freq() 
** ��������: ��������ڵ�Ƶ��
** ����������Ҫ���õ�����	ȡֵΪ 0,1
*******************************************************************************/
void Config_Out_Freq(uint16_t Frequence, uint8_t Pin)
{
#ifdef OLDBOARD
	Out_Freq[Pin].SetFreq = 72000000 / Out_Freq_Dive / (2 * Frequence);
	TIM3->CCER |= (uint16_t)(TIM_OutputState_Enable << (4 * Pin));      //ʹ��ָ���Ķ˿����
	TIM_ITConfig(TIM3, (TIM_IT_CC1 << Pin),  ENABLE);
	TIM_Cmd(TIM3, ENABLE);
#else
// 	if(Frequence == 0)
// 	{
// 		TIM1->CCER |= (uint16_t)(TIM_OutputState_Disable << (4 * Pin));      //ʹ��ָ���Ķ˿����
// 	}
// 	else
	{		
	Out_Freq[Pin].SetFreq = 72000000 / Out_Freq_Dive / (2 * Frequence);
	TIM1->CCER |= (uint16_t)(TIM_OutputState_Enable << (4 * Pin));      //ʹ��ָ���Ķ˿����
	TIM_ClearFlag(TIM1, TIM_FLAG_CC1);	   //2013-12-1��� ���ж�	
	TIM_ITConfig(TIM1, (TIM_IT_CC1 << Pin),  ENABLE); 
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//2013-12-1��� ͨ�����ʹ��	
	}
#endif
}
/******************************************************************************
** ��������: void TIM3_IRQHandler(void)
** ��������: ��ʱ������Ƚ��жϴ���
** ����������Ҫ���õ�����
*******************************************************************************/

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1 )!= RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		Out_Freq[0].Count = TIM_GetCapture1(TIM3) + Out_Freq[0].SetFreq;
		if(Out_Freq[0].Count > 65535)
		{
			Out_Freq[0].Count = Out_Freq[0].Count - 65536;
		}
		else
		{
		}
		TIM_SetCompare1(TIM3, (uint16_t)(Out_Freq[0].Count));	
	}	

	if(TIM_GetITStatus(TIM3,TIM_IT_CC2 )!= RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		Out_Freq[1].Count = TIM_GetCapture2(TIM3) + Out_Freq[1].SetFreq;
		if(Out_Freq[1].Count > 65535)
		{
			Out_Freq[1].Count = Out_Freq[1].Count - 65536;
		}
		else
		{
		}
		TIM_SetCompare2(TIM3, (uint16_t)(Out_Freq[1].Count));	
	}
}
/******************************************************************************
** ��������: void TIM1_CC_IRQHandler(void)
** ��������: ��ʱ������Ƚ��жϴ���
** ����������Ҫ���õ�����
*******************************************************************************/

void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1 )!= RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		Out_Freq[0].Count = TIM_GetCapture1(TIM1) + Out_Freq[0].SetFreq;
		if(Out_Freq[0].Count > 65535)
		{
			Out_Freq[0].Count = Out_Freq[0].Count - 65536;
		}
		else
		{
		}
		TIM_SetCompare1(TIM1, (uint16_t)(Out_Freq[0].Count));	
	}	

	if(TIM_GetITStatus(TIM1,TIM_IT_CC2 )!= RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		Out_Freq[1].Count = TIM_GetCapture2(TIM1) + Out_Freq[1].SetFreq;
		if(Out_Freq[1].Count > 65535)
		{
			Out_Freq[1].Count = Out_Freq[1].Count - 65536;
		}
		else
		{
		}
		TIM_SetCompare2(TIM1, (uint16_t)(Out_Freq[1].Count));	
	}
}
/******************************************************************************
** ��������: EXTI0_IRQHandler()
** ��������: �ⲿ�ж���·1
** ����������
*******************************************************************************/
void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line0);
	    Fre_Count[0]++;
	}
}
/******************************************************************************
** ��������: EXTI1_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line1);
	    Fre_Count[1]++;
	}
}
/******************************************************************************
** ��������: EXTI2_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line2);
	    Fre_Count[2]++;
	}
}
/******************************************************************************
** ��������: EXTI2_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void EXTI3_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line3)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line3);
	    Fre_Count[3]++;
	}
}
/******************************************************************************
** ��������: EXTI4_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void EXTI4_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line4)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line4);
	    Fre_Count[4]++;
	}
}
/******************************************************************************
** ��������: EXTI4_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void EXTI9_5_IRQHandler()
{
	uint16_t i;
	for(i = 5; i < 10; i++)
    if(EXTI_GetITStatus(EXTI_Line0 << i)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line0 << i);
		Fre_Count[i]++;
	}
}
/******************************************************************************
** ��������: EXTI4_IRQHandler() 
** ��������:�ⲿ�ж���· 
** ����������
*******************************************************************************/
void  EXTI15_10_IRQHandler()
{
	uint16_t i;
	for(i = 10; i < 16; i++)
    if(EXTI_GetITStatus(EXTI_Line0 << i)!= RESET)				   //Ƶ����������
	{
	    EXTI_ClearFlag(EXTI_Line0 << i);
		Fre_Count[i]++;
	}
}
/******************************************************************************
** ��������: Input_Read() 
** ��������: ����ֵ
** ����������Ҫ���õ�����
*******************************************************************************/
void Input_Read(Input Data[])
{
	uint16_t i, j;
	CAN_FRAME Change_Data;
	OS_CPU_SR  cpu_sr = 0;

	for(i = 0; i < In_Number; i++)
	{
		if(Data[i].State == ENABLE)
		{
			if(Data[i].Type == Freque_Type)				  //Ƶ����������
			{
				Data[i].Count++;
				if(Data[i].Count == Freq_Count) 
				
				{
					OS_ENTER_CRITICAL();	            //�ر�ȫ���ж�
					Data[i].FreqS++;
					if(Data[i].FreqS <= Freq_Shake)
					{
						In_Freq[i][Data[i].FreqS - 1].Freq = Fre_Count[i];
						
					}
					else
					{
						Freq_Head[i]->Freq = Fre_Count[i];
						Freq_Head[i] = Freq_Head[i]->Next;
						Data[i].FreqS--;
					}
					Data[i].Value = 0;
					for(j = 0; j < Data[i].FreqS; j++) 
							Data[i].Value += In_Freq[i][j].Freq;
					Data[i].Value = Data[i].Value / Data[i].FreqS;

					if(Data[i].Value < Freq_Low)
						Data[i].Error = Senor_Low;
					else if(Data[i].Value > Freq_High)
						Data[i].Error = Senor_High;
					else
						Data[i].Error = Senor_No_Error;	
					Fre_Count[i] = 0;                        //���ֿ�ʼ����
					Data[i].Count = 0;
					OS_EXIT_CRITICAL();
				}
			}
			else							              //������
			{ 
				vtest = GPIO_ReadInputDataBit(Input_Port, (uint16_t)0x01 << i);
				if(Data[i].Value^vtest)//GPIO_ReadInputDataBit(Input_Port, (uint16_t)0x01 << i))
				{
					Data[i].Count++;
					if(Data[i].Count >= Delay_Count)          //�ﵽ�ӳ�ȥ������  �������仯
					{
						Data[i].Value = !Data[i].Value;
						Change_Data.StdId.extid_32 = Guest_Address | INCHANGE_ASK;			       
						Change_Data.Data[0] = i;
						Change_Data.Data[1] = Data[i].Value;
						Change_Data.DLC = DLC_BYTES_2;							 
						InCanTrsQue(Change_Data);			     //���������仯���͵���λ��
						
					}
					
				}
				else
				{
					Data[i].Count = 0;
				}	
			}

		}
	}	
}

/******************************************************************************
** ��������: Out_Write() 
** ��������: ���ֵ
** ����������
*******************************************************************************/
void Out_Write(uint16_t OutPin, uint8_t value)
{
	if(Output0 << OutPin == Output0)
    	GPIO_WriteBit(Output_Port, Output0,   value ? Bit_SET : Bit_RESET);
	else
		GPIO_WriteBit(Output_Port, Output0 << (OutPin + 4),   value ? Bit_SET : Bit_RESET);	
		
}
/******************************************************************************
** ��������: Out_Config() 
** ��������: ���ֵ
** ����������
*******************************************************************************/
void Out_Init(uint8_t Out[])
{
	uint8_t i,j;
	for(i = 0; i < Out_Number * 2 / 8; i++)
	{
		for(j = 0; j < 4 ;j++)
		{		
			if((Out[i] << (2 * j)) & (uint8_t)0x80)
			{
			                                        //�������
			} 
			   //��j+4*i·����պ�
			Out_Write((j + 4 * i), (Out[i] << (2 * j)) & (uint8_t)0x40 ? Out_Close : Out_Open);
		}			
	}
}
/*********************************************************
** ��������IWDGInit
** �䡡�룺None
** �䡡����None
** �������������Ź���ʼ��
************************************************************/
void IWDGInit(void)
{
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        RCC_ClearFlag();
    }
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);          /*ʹ��дIWDG_PR and IWDG_RLR*/    
    IWDG_SetPrescaler(IWDG_Prescaler_32);                  /* ���Ź�ʱ��: 40KHz(LSI) / 128 = 0.625 KHz */
    IWDG_SetReload(450);                                    /*��ʱ*/
    IWDG_ReloadCounter();                   			   //���Ź��幷	60ms��ʱ
    IWDG_Enable();                                         /*ʹ�ܿ��Ź�(the LSI oscillator will be enabled by hardware) */
}
/******************************************************************************
** ��������: BSP_Init()
** ��������: ��ʼ��
** ������������
******************************************************************************/
void BSP_Init()
{	
	/*���ڳ�ʼ��*/
//	Init_Usart();
//	Usart_Configuration(115200);
	IWDGInit();
	CAN_Config();
	InitCan();             //can���ݳ�ʼ��
	
}

