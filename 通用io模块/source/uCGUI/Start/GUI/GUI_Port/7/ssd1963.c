#include "gui.h"
#include "usart.h"
#include "ssd1963.h"
#include "stm32f10x.h"
#include "ascii_8x16_font.h"

/*��ʱ*/
void TimeDelay(u32 Time)
{
  	u32 Daly_Time = 0;
	unsigned char i;
	do
	{
		for(i=0;i<50;i++);
		Daly_Time++;
	}
	while(Daly_Time < Time);
}
void LCD_Configuration(void)
{
	RCC->APB2ENR|=1<<4;//��ʹ������PORTCʱ�� 
 	RCC->APB2ENR|=1<<5;//��ʹ������PORTDʱ��
 	RCC->APB2ENR|=1<<6;//��ʹ������PORTEʱ��

	RCC->APB2ENR|=1<<0;    //��������ʱ��

	//PORTD12~15������� 	
	GPIOC->CRL&=0XF0FFFFFFF;
	GPIOC->CRL|=0X03000000; 
	GPIOC->ODR|=0X0040; 											 
	//PORTD12~15������� 	
	GPIOD->CRH&=0X000FFFFF;
	GPIOD->CRH|=0X33300000; 
	GPIOD->ODR|=0XF000; 	 
	//PORTE ������� 	
	GPIOE->CRH=0X33333333;
	GPIOE->CRL=0X33333333; 	 
	GPIOE->ODR=0XFFFF;
}

/*ssd1963 7������������ʼ��*/
void ssd1963_init(void)
{
	LCD_Configuration();
	TimeDelay(5000);

	/*ssd1963�Ĵ�������*/
	LCD_WriteCom(0x002b);	
	LCD_WriteRAM(0);

	TimeDelay(5); // delay 50 ms 
	LCD_WriteCom(0x00E2);					//PLL multiplier, set PLL clock to 120M
	LCD_WriteRAM(0x0023);					//N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WriteRAM(0x0002);
	LCD_WriteRAM(0x0004);
	
	LCD_WriteCom(0x00E0);					//PLL enable
	LCD_WriteRAM(0x0001);
	TimeDelay(1);
	LCD_WriteCom(0x00E0);
	LCD_WriteRAM(0x0003);
	TimeDelay(5);
	LCD_WriteCom(0x0001);  					//software reset
	TimeDelay(5);
	LCD_WriteCom(0x00E6);					//PLL setting for PCLK, depends on resolution
//	LCD_WriteRAM(0x0001);
//	LCD_WriteRAM(0x0033);
//	LCD_WriteRAM(0x0032);
	LCD_WriteRAM(0x0001);
	LCD_WriteRAM(0x0055);
	LCD_WriteRAM(0x0054);
	TimeDelay(8);

	LCD_WriteCom(0x00B0);					//LCD SPECIFICATION
	LCD_WriteRAM(0x0020);
	LCD_WriteRAM(0x0000);
	LCD_WriteRAM((HDP>>8)&0X00FF);			//Set HDP
	LCD_WriteRAM(HDP&0X00FF);
    LCD_WriteRAM((VDP>>8)&0X00FF);			//Set VDP
	LCD_WriteRAM(VDP&0X00FF);
    LCD_WriteRAM(0x0000);

	LCD_WriteCom(0x00B4);					//HSYNC
	LCD_WriteRAM((HT>>8)&0X00FF); 			//Set HT
	LCD_WriteRAM(HT&0X00FF);
	LCD_WriteRAM((HPS>>8)&0X00FF);			//Set HPS
	LCD_WriteRAM(HPS&0X00FF);
	LCD_WriteRAM(HPW);						//Set HPW
	LCD_WriteRAM((LPS>>8)&0X00FF); 			//Set HPS
	LCD_WriteRAM(LPS&0X00FF);
	LCD_WriteRAM(0x0000);

	LCD_WriteCom(0x00B6);					//VSYNC
	LCD_WriteRAM((VT>>8)&0X00FF);   		//Set VT
	LCD_WriteRAM(VT&0X00FF);
	LCD_WriteRAM((VPS>>8)&0X00FF); 			//Set VPS
	LCD_WriteRAM(VPS&0X00FF);
	LCD_WriteRAM(VPW);						//Set VPW
	LCD_WriteRAM((FPS>>8)&0X00FF);			//Set FPS
	LCD_WriteRAM(FPS&0X00FF);

	//=============================================

	//=============================================
	LCD_WriteCom(0x00F0); //pixel data interface
	LCD_WriteRAM(0x0003); //03:16λ   02:24λ

    LCD_WriteCom(0x0021);	//����ͼ����ɫ��תģʽ

	LCD_WriteCom(0x00BC);//��Ҫ
	LCD_WriteRAM(0x0080);//�Աȶ�
	LCD_WriteRAM(0x0080);//����
	LCD_WriteRAM(0x0080);//���Ͷ�ֵ  //
	LCD_WriteRAM(0x0001);//���������


	LCD_Clear(0xffff);

	LCD_WriteCom(0x0029); //display on
	TimeDelay(50);
}

/*��������������Ļ�����ָ����ɫ*/
void LCD_Clear(uint16_t Color)
{
	uint32_t i;

	LCD_WriteCom(0x002A);	
	LCD_WriteRAM(0);	    
	LCD_WriteRAM(0);
	LCD_WriteRAM(HDP>>8);	    
	LCD_WriteRAM(HDP&0x00ff);
    LCD_WriteCom(0x002b);	
	LCD_WriteRAM(0);	    
	LCD_WriteRAM(0);
	LCD_WriteRAM(VDP>>8);	    
	LCD_WriteRAM(VDP&0x00ff);
	LCD_WriteCom(0x002c);
	
	
	for(i=0;i<384000;i++){  
          	LCD_WriteRAM(Color);
	}				 
}

