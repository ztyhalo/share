

/************************************IO宏定义***************************************/
/************************************************************************************/


#define RCC_APB2_IO	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC

/*输入口定义*/
#define Input 		GPIOC 
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

#define ReadInput(x)  GPIO_ReadInputDataBit(GPIOC, x)


/*开关量输入口定义*/
#define Output 		GPIOB
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

//#define WriteOut(x)  GPIO_ReadInputDataBit(GPIOC, x)               
	
/*开关量输入口定义*/
#define FreOutput	GPIOA
#define FreOutput1	GPIO_Pin_6
#define FreOutput2	GPIO_Pin_7