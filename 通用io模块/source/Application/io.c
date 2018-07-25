/*********************************************************
** ��������EXTIInit
** �䡡�룺None
** �䡡����None
** �����������ⲿ�жϳ�ʼ�������ڼ�Ƶ����
************************************************************/
void EXTIInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(((uint8_t)0x02), GPIO_PinSource8);
	GPIO_EXTILineConfig(((uint8_t)0x02), GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/*�����ⲿ�ж�ʹ���ж�--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x17 ;				    	//EXTI_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI->IMR =EXTI->IMR &0xfffffcff;     //�����ж�����
}

/*********************************************************
** ��������TimerInit
** �䡡�룺None
** �䡡����None
** ������������ʼ����ʱ��T2��T3
**           T2����10ms��ѯ�ɼ����룻T3���ڶ�ʱ���¶�����
************************************************************/
void TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 0x4e20;                      //������ֵ20000 ������0�ͣ�ʱ����t = Rperiod/(������ʱ�ӣ�(��Ƶ���ӣ�))       
	TIM_TimeBaseStructure.TIM_Prescaler = 0x23;                     //35��ƵΪ��Ƶ����1;      	
	TIM_TimeBaseStructure.TIM_ClockDivision =0;                     // ʱ�ӷָ�  	//������ʱ��Ϊ72MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //�����������ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = 0x2710;                      //������ֵ10000 ����500�ͣ�ʱ����t = Rperiod/(������ʱ�ӣ�(��Ƶ���ӣ�))       
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0e0f;                   //3599��ƵΪ��Ƶ����1;      	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    // ʱ�ӷָ�  	//������ʱ��Ϊ72MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //�����������ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/*���ö�ʱ��2ʹ���ж�--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x1c ;				    	//TIM2_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*���ö�ʱ��3ʹ���ж�--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x1d ;				    	//TIM3_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	                                                 
   	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                           // ���TIM2����жϱ�־
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                           // ���TIM3����жϱ�־ 	
}