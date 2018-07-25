#ifndef	__BSP_H__
#define __BSP_H__


/*
	这个文件中定义的所有变量可在board_cfg.c中找到
	
*/

/************************************输入IO宏定义***************************************/
/************************************************************************************/
#define RCC_APB2_IO	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC

/*输入口定义*/
#define Input_Port 	GPIOC 
#define Input0 		GPIO_Pin_0              
#define Input1		GPIO_Pin_1                 
#define Input2		GPIO_Pin_2              
#define Input3		GPIO_Pin_3                
#define Input4		GPIO_Pin_4            
#define Input5		GPIO_Pin_5              
#define Input6		GPIO_Pin_6               
#define Input7		GPIO_Pin_7              
#define Input8		GPIO_Pin_8                 
#define Input9		GPIO_Pin_9                
#define Input10		GPIO_Pin_10                
#define Input11		GPIO_Pin_11                
#define Input12		GPIO_Pin_12                
#define Input13		GPIO_Pin_13                
#define Input14		GPIO_Pin_14                
#define Input15		GPIO_Pin_15               
#define InputAll	GPIO_Pin_All               

//#define ReadInput(x)  GPIO_ReadInputDataBit(GPIOC, x)


/*开关量输出口定义*/
#define Output_Port  GPIOB
#define Output0 	GPIO_Pin_0              
#define Output1		GPIO_Pin_5                 
#define Output2		GPIO_Pin_6              
#define Output3		GPIO_Pin_7                
#define Output4		GPIO_Pin_8            
#define Output5		GPIO_Pin_9              
#define Output6		GPIO_Pin_10               
#define Output7		GPIO_Pin_11             
#define Output8		GPIO_Pin_12                
#define Output9		GPIO_Pin_13                
#define Output10	GPIO_Pin_14                
#define Output11	GPIO_Pin_15 
#define OutputAll	(GPIO_Pin_All ^ (GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ))

//#define WriteOut(x)  GPIO_ReadInputDataBit(GPIOC, x)               
	
/*频率输出口定义*/
#define FreOutput	GPIOA
#ifdef OLDBOARD
#define FreOutput1	GPIO_Pin_6
#define FreOutput2	GPIO_Pin_7
#else
#define FreOutput1	GPIO_Pin_8
#define FreOutput2	GPIO_Pin_9
#endif

#define Switch_Type    0x00
#define Freque_Type    0x01
#define Delay_Count	   5				//开关量延迟去抖次数
#define Freq_Count     20               //频率量计数间隔 20*50ms
#define Freq_Shake     3				//频率量抖动计数次数
#define Out_Freq_Dive  0x167		   //输出频率量定时器分频数
#define In_Number 	   16              //定义输入口数量
#define Out_Number	   12			   //定义输出口数量
#define Out_Open       1			   //输出断开
#define Out_Close	   0	   //输出闭合


/*频率输出结构体定义*/
typedef struct
{
	uint32_t	SetFreq;	
	uint32_t	Count;
}Freq_Out;

/*输入结构体定义*/

typedef struct
{
	uint8_t     State;		    //状态
	uint8_t	    Type;		   //类型
	uint8_t		Error;		   //故障
	uint8_t		Count;		   //计数时间
	uint8_t		FreqS;			//频率计数次数
	uint16_t	Value;		   //值
} Input;

typedef struct	freq_shake
{
	uint16_t 	       Freq;
	struct freq_shake *Next;
}Freq_In;

enum
{
	Senor_Bug = 0,
	Senor_Kill,
	Senor_Short,
	Senor_Low,
	Senor_High,
	Senor_No_Error = 7
};

#define  Freq_Low 	200
#define  Freq_High	1000

/*外部变量申明*/
extern OS_EVENT*	Sys_Init_Sem;
extern OS_EVENT*	Sys_Reset_Sem;
extern OS_EVENT*	Can_Count_Sem;
extern Freq_Out 	Freq[];
extern Input		Input_State[];

/*函数*/

//void Led_Init(void);                             /*LED初始化*/
void BSP_Init(void);                             /*平台初始化*/
void Output_Init(void);                          /*输出初始化*/
void Out_Write(uint16_t OutPin, uint8_t value);  /*输出口赋值*/
void Input_Init(uint8_t *Data);		                 /*输入初始化*/
void Freq_Output_Init(void);
void Timer_Init(void);
void Config_Out_Freq(uint16_t Frequence, uint8_t Pin);
void Out_Init(uint8_t Out[]);
void TIM3_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void IWDGInit(void);
void Input_Read(Input Data[]);
void InitCan(void);

#endif
