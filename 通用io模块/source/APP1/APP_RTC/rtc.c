/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
**  �����ſ�����
**	RTCʱ������
**  �汾��Rev1.0 
**  ���ڣ�2011-6-20
**	��̳��www.openmcu.com
**	��̳��www.openmcu.com	 bbs.openmcu.com
**	�Ա���http://shop36995246.taobao.com/   
**  Copyright(c)@��������	 Dream_etd@163.com	
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
#include "usart.h"
#include "rtc.h"
#include "OS_CPU.H"
#include "includes.h"

struct Data_Time  timer;  //����һ��ʱ��ṹ�����
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_RTC
** ��������: ADC1��ʼ��
				����ΪDMAͨ��1
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t Init_RTC(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		//��ռ���ȼ�1λ,�����ȼ�3λ
//
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1λ,�����ȼ�3λ
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
//	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
																	//ʹ��PWR��BKP����ʱ��   		
	PWR_BackupAccessCmd(ENABLE);									//ʹ��RTC�ͺ󱸼Ĵ������� 
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5555)						//��ָ���ĺ󱸼Ĵ����ж������ݣ��ж��Ƿ�Ϊ��һ������
	{	
      	Printf("ʱ�����á�����\r\n");																
		BKP_DeInit();												//������BKP��ȫ���Ĵ�������Ϊȱʡֵ 	
		RCC_LSEConfig(RCC_LSE_ON);									//ʹ���ⲿ����ʱ�� 32.768KHz
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)			//���ָ����RCC��־λ�������,�ȴ����پ������
  		{
			__nop();__nop();
			__nop();__nop();
			__nop();__nop();
			__nop();__nop();
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);						//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);										//ʹ��RTCʱ��  
		RTC_WaitForSynchro();										//�ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ��
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//ʹ��RTC���ж�
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetPrescaler(32767); 									//����RTCԤ��Ƶ��ֵ  RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		Time_Set();													//ʱ������	
    	BKP_WriteBackupRegister(BKP_DR1, 0x5555);					//��ָ���ĺ󱸼Ĵ�����д���û���������0X5555���жϱ�־										
	}																 	
	else															//���ǵ�һ������ ������ʱ
	{
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)			//���ָ����RCC��־λ�������:POR/PDR��λ
		{
      		Printf("�ϵ縴λ������\r\n");
		}											
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)		//���ָ����RCC��־λ�������:�ܽŸ�λ
		{
      		Printf("�ⲿ��λ������\r\n");
		}
    	printf("����Ҫ���á�����\r\n");
		
		RTC_WaitForSynchro();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//ʹ��RTC���ж�

		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}		    				     
	Time_Get();														//����ʱ�� 
	
	RCC_ClearFlag();												//���RCC�ĸ�λ��־λ
	
	return 0; //ok		
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 			//���������ݱ�	  

const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};	//ƽ����·����ڱ�

uint8_t Time_Update(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)											//��������ݵ��������
		{
		if(Is_Leap_Year(t))seccount+=31622400;						//�����������
		else seccount+=31536000;			  						//ƽ���������
		}
	smon-=1;
	for(t=0;t<smon;t++)	   											//��ǰ���·ݵ����������
		{
		seccount+=(uint32_t)mon_table[t]*86400;						//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;				//����2�·�����һ���������	   
		}
	seccount+=(uint32_t)(sday-1)*86400;								//��ǰ�����ڵ���������� 
	seccount+=(uint32_t)hour*3600;									//Сʱ������
	seccount+=(uint32_t)min*60;	 									//����������
	seccount+=sec;													//�������Ӽ���ȥ
											    
	RTC_WaitForLastTask();											//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_SetCounter(seccount);										//����RTC��������ֵ
	RTC_WaitForLastTask();											//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;		
}
//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
uint8_t Time_Get(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;	  
	 
	timecount = RTC_GetCounter();									//��� RTC ������ֵ(������)			 
	temp=timecount/86400;   //�õ�����(��������Ӧ��)=24Сʱ
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		timer.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		timer.w_month=temp1+1;//�õ��·�
		timer.w_date=temp+1;  //�õ����� 
	}
	temp=timecount%86400;     //�õ�������   	   
	timer.hour=temp/3600;     //Сʱ
	timer.min=(temp%3600)/60; //����	
	timer.sec=(temp%3600)%60; //����
	timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����   
	return 0;
}
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 	 
	if (yearH>19)yearL+=100;// ���Ϊ21����,�������100 
  
	temp2=yearL+yearL/4;  	// ����������ֻ��1900��֮���
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
} 
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t Is_Leap_Year(uint16_t year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}
///*****************************************************************************
//** ��������: RTC_IRQHandler
//** ��������: RTC�жϷ�����
//				ÿ�봥��һ��  
//** ��  ����: Dream
//** �ա�  ��: 2011��6��20��
//*****************************************************************************/
// unsigned char TIME_UPDATE_FLAG;
// typedef  unsigned  long  CPU_SR; 
//void RTC_IRQHandler(void)
//{							    
//	if(RTC_GetITStatus(RTC_IT_SEC))			//�����ж�
//	{							
//		Time_Get();							//����ʱ�� 
//		TIME_UPDATE_FLAG = 1;				//����lcd��Ļʱ�����	 
//	}
//	if(RTC_GetITStatus(RTC_IT_ALR))			//�����ж�
//	{
//		RTC_ClearITPendingBit(RTC_IT_ALR);	//�������ж�		  								 				
//	}
//	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW | RTC_IT_ALR);		//�������������жϱ�־		  								 
//	RTC_WaitForLastTask();					//�ȴ�RTC�Ĵ����������
//}
/*****************************************************************************
** ��������: Time_Set
** ��������: ͨ����������ʱ��
					����������ʱ����
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
*****************************************************************************/
void Time_Set(void)
{
	uint16_t Data[6];	
Loop:
	printf("������������ʱ���룬���磺20110708103029\r\n");
	Data[0]= Usart_Scanf(2099,4);printf("%d��\r\n",Data[0]);	
	Data[1]= Usart_Scanf(12,2);  printf("%d��\r\n",Data[1]);
	Data[2]= Usart_Scanf(31,2);  printf("%d��\r\n",Data[2]);
	Data[3]= Usart_Scanf(24,2);	 printf("%d��\r\n",Data[3]);
	Data[4]= Usart_Scanf(60,2);	 printf("%d��\r\n",Data[4]);
	Data[5]= Usart_Scanf(60,2);	 printf("%d��\r\n",Data[5]);

	Printf("%d��%d��%d��%d��%d��%d��\r\n",Data[0],Data[1],Data[2],Data[3],Data[4],Data[5]);
	if((Data[0]<1970) || (Data[0]>2099) || (Data[1]>12) || (Data[2]>31) || (Data[3]>24) || (Data[4]>60) || (Data[5]>60)){
		printf("���벻�Ϸ�\r\n");
		goto Loop;
	}

  	Time_Update(Data[0],Data[1],Data[2],Data[3],Data[4],Data[5]);
}
/*****************************************************************************
** ��������: Usart_Scanf
** ��������: �������뺯��
					����������ʱ��
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
*****************************************************************************/
uint16_t Usart_Scanf(uint32_t value,uint8_t count)
{
  	uint32_t index = 0;
  	uint32_t tmp[4] = {0, 0, 0, 0};

DATA_TIME :

  	while (index < count)
  	{
    	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    	{}
    	tmp[index++] = (USART_ReceiveData(USART1));
    	if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    	{
      		Printf("Please enter valid number between 0 and 9\n\r");
      		index--;
    	}
  	}
	if(count<3){index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);}	
	else {index = (tmp[3] - 0x30) + ((tmp[2] - 0x30) * 10) + ((tmp[1] - 0x30) * 100) + ((tmp[0] - 0x30) * 1000);}
	
  	if (index > value)
  	{
    	Printf("Please enter valid number between 0 and %d\n\r", value);
		index = 0;
		goto  DATA_TIME;
  	}
  	return index;
}
