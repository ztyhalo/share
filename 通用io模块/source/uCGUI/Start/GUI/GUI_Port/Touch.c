/**************************************************************
** 	启航号开发板
**	功能介绍： 触摸屏驱动代码
**  版本：V1.0  
**	论坛：www.openmcu.com	 bbs.openmcu.com
**	淘宝：http://shop36995246.taobao.com/   
**  Copyright(c)@旺宝电子	 Dream_etd@163.com	
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/* Includes ------------------------------------------------------------------*/

#include "usart.h"
#include "touch.h"
#include "stm32f10x.h"


//SPI写数据
//向7843写入1byte数据   
void ADS_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)T_DOUT_H;  
		else T_DOUT_L;   
		num<<=1;   
		T_DCLK_L;//上升沿有效
		__nop();__nop();
		__nop();__nop();
		T_DCLK_H; 
		__nop();__nop();
		__nop();__nop();
	} 			    
} 		 
//SPI读数据 
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	   
uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t i;
	uint8_t count=0; 	  
	uint16_t Num=0; 
	T_DCLK_L;//先拉低时钟 	 
	T_CS_L; //选中ADS7843	 
	ADS_Write_Byte(CMD);//发送命令字
	for(i=200;i>0;i--);
	T_DCLK_H;//给1个时钟，清除BUSY  
	__nop();__nop();
	__nop();__nop(); 	    
	T_DCLK_L; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 
		T_DCLK_L;//下降沿有效  
		__nop();__nop();
		__nop();__nop();
		T_DCLK_H;
		__nop();__nop();
		if(T_DIN)Num++; 		 
	}  	
	Num>>=4;   //只有高12位有效.
	T_CS_H;//释放ADS7843	 
	return(Num);   
}
//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
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
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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
//带滤波的坐标读取
//最小值不能少于100.
uint8_t Read_ADSX(uint16_t *x)
{
	uint16_t xtemp;			 	 		  
	xtemp=ADS_Read_XY(0xd0);	  												   
	if(xtemp<100)return 0;//读数失败
	*x=xtemp;

	return 1;//读数成功
}

uint8_t Read_ADSY(uint16_t *y)
{
	uint16_t ytemp;			 	 		  
	ytemp=ADS_Read_XY(0x90);	  												   
	if(ytemp<100)return 0;//读数失败
	*y=ytemp;

	return 1;//读数成功
}
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 50 //误差范围 
uint8_t Touch_GexX(uint16_t *x) 
{
	uint16_t x1;
 	uint16_t x2;
 	uint8_t flag;    
    flag=Read_ADSX(&x1);   
    if(flag==0)return(0);
    flag=Read_ADSX(&x2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE)))//前后两次采样在+-50内
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
    if(((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))//前后两次采样在+-50内
    {
        *y=(y1+y2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
}


void Touch_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );  //重要！！

	//下面是SPI相关GPIO初始化  	Configure PB0 pin: TP_CS pin 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//下面是SPI相关GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	//上拉输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//Configure PC4 pin: TP_INT pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//上拉输入
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIOB->ODR=GPIOC->ODR = 0xffff;

	//spi口上其他外设失能
	GPIOA->CRL &= 0xfffff0ff;
	GPIOA->CRL |= 0xfffff3ff; //PA2;

	GPIOB->CRL &= 0xff0fff00;
	GPIOB->CRL |= 0xff3fff33;

	GPIOB->ODR = 0xffff;


}
//PEN中断设置	 
void Pen_Int_Set(uint8_t en)
{
	if(en)EXTI->IMR|=1<<5;   //开启line5上的中断	  	
	else EXTI->IMR&=~(1<<5); //关闭line5上的中断	   
}

