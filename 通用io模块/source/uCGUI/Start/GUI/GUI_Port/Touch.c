/**************************************************************
** 	�����ſ�����
**	���ܽ��ܣ� ��������������
**  �汾��V1.0  
**	��̳��www.openmcu.com	 bbs.openmcu.com
**	�Ա���http://shop36995246.taobao.com/   
**  Copyright(c)@��������	 Dream_etd@163.com	
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/* Includes ------------------------------------------------------------------*/

#include "usart.h"
#include "touch.h"
#include "stm32f10x.h"


//SPIд����
//��7843д��1byte����   
void ADS_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)T_DOUT_H;  
		else T_DOUT_L;   
		num<<=1;   
		T_DCLK_L;//��������Ч
		__nop();__nop();
		__nop();__nop();
		T_DCLK_H; 
		__nop();__nop();
		__nop();__nop();
	} 			    
} 		 
//SPI������ 
//��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ	   
uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t i;
	uint8_t count=0; 	  
	uint16_t Num=0; 
	T_DCLK_L;//������ʱ�� 	 
	T_CS_L; //ѡ��ADS7843	 
	ADS_Write_Byte(CMD);//����������
	for(i=200;i>0;i--);
	T_DCLK_H;//��1��ʱ�ӣ����BUSY  
	__nop();__nop();
	__nop();__nop(); 	    
	T_DCLK_L; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 
		T_DCLK_L;//�½�����Ч  
		__nop();__nop();
		__nop();__nop();
		T_DCLK_H;
		__nop();__nop();
		if(T_DIN)Num++; 		 
	}  	
	Num>>=4;   //ֻ�и�12λ��Ч.
	T_CS_H;//�ͷ�ADS7843	 
	return(Num);   
}
//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//���˲��������ȡ
//��Сֵ��������100.
uint8_t Read_ADSX(uint16_t *x)
{
	uint16_t xtemp;			 	 		  
	xtemp=ADS_Read_XY(0xd0);	  												   
	if(xtemp<100)return 0;//����ʧ��
	*x=xtemp;

	return 1;//�����ɹ�
}

uint8_t Read_ADSY(uint16_t *y)
{
	uint16_t ytemp;			 	 		  
	ytemp=ADS_Read_XY(0x90);	  												   
	if(ytemp<100)return 0;//����ʧ��
	*y=ytemp;

	return 1;//�����ɹ�
}
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 50 //��Χ 
uint8_t Touch_GexX(uint16_t *x) 
{
	uint16_t x1;
 	uint16_t x2;
 	uint8_t flag;    
    flag=Read_ADSX(&x1);   
    if(flag==0)return(0);
    flag=Read_ADSX(&x2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE)))//ǰ�����β�����+-50��
    {
        *x=(x1+x2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
} 

uint8_t Touch_GexY(uint16_t *y) 
{
	uint16_t y1;
 	uint16_t y2;
 	uint8_t flag;    
    flag=Read_ADSY(&y1);   
    if(flag==0)return(0);
    flag=Read_ADSY(&y2);	   
    if(flag==0)return(0);   
    if(((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))//ǰ�����β�����+-50��
    {
        *y=(y1+y2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
}


void Touch_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );  //��Ҫ����

	//������SPI���GPIO��ʼ��  	Configure PB0 pin: TP_CS pin 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//������SPI���GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//Configure PC4 pin: TP_INT pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//��������
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIOB->ODR=GPIOC->ODR = 0xffff;

	//spi������������ʧ��
	GPIOA->CRL &= 0xfffff0ff;
	GPIOA->CRL |= 0xfffff3ff; //PA2;

	GPIOB->CRL &= 0xff0fff00;
	GPIOB->CRL |= 0xff3fff33;

	GPIOB->ODR = 0xffff;


}
//PEN�ж�����	 
void Pen_Int_Set(uint8_t en)
{
	if(en)EXTI->IMR|=1<<5;   //����line5�ϵ��ж�	  	
	else EXTI->IMR&=~(1<<5); //�ر�line5�ϵ��ж�	   
}

