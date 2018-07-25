/*********************************************************
** 函数名：EXTIInit
** 输　入：None
** 输　出：None
** 功能描述：外部中断初始化，用于记频率量
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

	/*配置外部中断使能中断--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x17 ;				    	//EXTI_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI->IMR =EXTI->IMR &0xfffffcff;     //屏蔽中断请求
}

/*********************************************************
** 函数名：TimerInit
** 输　入：None
** 输　出：None
** 功能描述：初始化定时器T2及T3
**           T2用于10ms查询采集输入；T3用于定时采温度脉冲
************************************************************/
void TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 0x4e20;                      //最大计数值20000 产生１0ＭＳ时基　t = Rperiod/(计数器时钟／(分频数加１))       
	TIM_TimeBaseStructure.TIM_Prescaler = 0x23;                     //35分频为分频数加1;      	
	TIM_TimeBaseStructure.TIM_ClockDivision =0;                     // 时钟分割  	//计数器时钟为72MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数方向向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = 0x2710;                      //最大计数值10000 产生500ＭＳ时基　t = Rperiod/(计数器时钟／(分频数加１))       
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0e0f;                   //3599分频为分频数加1;      	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    // 时钟分割  	//计数器时钟为72MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数方向向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/*配置定时器2使能中断--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x1c ;				    	//TIM2_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*配置定时器3使能中断--------------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = 0x1d ;				    	//TIM3_IRQChannel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	                                                 
   	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                           // 清除TIM2溢出中断标志
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                           // 清除TIM3溢出中断标志 	
}