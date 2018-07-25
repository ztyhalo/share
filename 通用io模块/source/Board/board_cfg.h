#ifndef	__BSP_H__
#define __BSP_H__


/*
	����ļ��ж�������б�������board_cfg.c���ҵ�
	
*/

/************************************����IO�궨��***************************************/
/************************************************************************************/
#define RCC_APB2_IO	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC

/*����ڶ���*/
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


/*����������ڶ���*/
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
	
/*Ƶ������ڶ���*/
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
#define Delay_Count	   5				//�������ӳ�ȥ������
#define Freq_Count     20               //Ƶ����������� 20*50ms
#define Freq_Shake     3				//Ƶ����������������
#define Out_Freq_Dive  0x167		   //���Ƶ������ʱ����Ƶ��
#define In_Number 	   16              //�������������
#define Out_Number	   12			   //�������������
#define Out_Open       1			   //����Ͽ�
#define Out_Close	   0	   //����պ�


/*Ƶ������ṹ�嶨��*/
typedef struct
{
	uint32_t	SetFreq;	
	uint32_t	Count;
}Freq_Out;

/*����ṹ�嶨��*/

typedef struct
{
	uint8_t     State;		    //״̬
	uint8_t	    Type;		   //����
	uint8_t		Error;		   //����
	uint8_t		Count;		   //����ʱ��
	uint8_t		FreqS;			//Ƶ�ʼ�������
	uint16_t	Value;		   //ֵ
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

/*�ⲿ��������*/
extern OS_EVENT*	Sys_Init_Sem;
extern OS_EVENT*	Sys_Reset_Sem;
extern OS_EVENT*	Can_Count_Sem;
extern Freq_Out 	Freq[];
extern Input		Input_State[];

/*����*/

//void Led_Init(void);                             /*LED��ʼ��*/
void BSP_Init(void);                             /*ƽ̨��ʼ��*/
void Output_Init(void);                          /*�����ʼ��*/
void Out_Write(uint16_t OutPin, uint8_t value);  /*����ڸ�ֵ*/
void Input_Init(uint8_t *Data);		                 /*�����ʼ��*/
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
