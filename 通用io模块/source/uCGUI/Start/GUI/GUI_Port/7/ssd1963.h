#ifndef __ssd1963_h__
#define __ssd1963_h__


#include "stm32f10x.h"
#include "gui.h"
#include "LCDConf.h"
#include "LCD_Protected.h"
#include "ucgui_api.h"

//#define   HDP	479 //Horizontal Display Period
//#define   HT	1000 //Horizontal Total
//#define   HPS	51  //LLINE Pulse Start Position
//#define   LPS	3   //	Horizontal Display Period Start Position
//#define   HPW	8   //	LLINE Pulse Width
//
//
//#define   VDP	479	//Vertical Display Period
//#define   VT	530	//Vertical Total
//#define   VPS	24	//	LFRAME Pulse Start Position
//#define   FPS	23	//Vertical Display Period Start Positio
//#define   VPW	3	// LFRAME Pulse Width

/////////////////////////////////////////
#define  HDP	479  /*长*/
#define  HT		531
#define  HPS	43
#define  LPS	8
#define  HPW	10

#define  VDP	271	 /*宽*/
#define  VT		288
#define  VPS	12
#define  FPS	23
#define  VPW	10


/*******************************************************************************
* Function Name  : LCD_WriteCom
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
__inline volatile void LCD_WriteCom(u16 LCD_Reg)
{
	GPIOD->BRR  = 0x2000;//1<<13;	  	//RS = 0;
	GPIOC->BRR  = 0x0040;//1<<6;		  //CS = 0;
	GPIOE->ODR  = LCD_Reg;	  // LCD_Reg
	GPIOD->BRR  = 0x4000;//1<<14;	  //WR = 0;
	GPIOD->BSRR = 0x4000;//1<<14;	  //WR = 1;
	GPIOC->BSRR = 0x0040;//1<<6;		  //CS = 1; 
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
__inline volatile void LCD_WriteRAM(u16 RGB_Code)					 
{
  /* Write 16-bit GRAM Reg */
 	/* Write 16-bit Reg */
	GPIOD->BSRR = 0x2000;//1<<13;	  //RS = 1;
	GPIOC->BRR  = 0x0040;//1<<6;		  //CS = 0;
	GPIOE->ODR  = RGB_Code;	  // data
	GPIOD->BRR  = 0x4000;//1<<14;	  //WR = 0;
	GPIOD->BSRR = 0x4000;//1<<14;	  //WR = 1;
	GPIOC->BSRR = 0x0040;//1<<6;		  //CS = 1;
}

////****************************************************************************************************
//// 原  型: unsigned int Get_Dot_LCD(int x,int y)
//// 设计者: tfthome.com
//// 描  述: 返回一个象素的颜色
//// 版本号: 1.0
////****************************************************************************************************
//__inline volatile unsigned int LCD_GetPoint(int x,int y)
//{ 
//  	LCD_WriteCom(0x002A);	
//	LCD_WriteRAM(x>>8);	    
//	LCD_WriteRAM(x&0x00ff);
//	LCD_WriteRAM(HDP>>8);	    
//	LCD_WriteRAM(HDP&0xff);
//    LCD_WriteCom(0x002b);	
//	LCD_WriteRAM(y>>8);	    
//	LCD_WriteRAM(y&0x00ff);
//	LCD_WriteRAM(VDP>>8);	    
//	LCD_WriteRAM(VDP&0xff);
//  return (LCD_ReadRAM());
//}
//
///****************************************************************************
//* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
//* 功    能：在指定座标画点
//* 入口参数：x      行座标
//*           y      列座标
//*           point  点的颜色
//* 出口参数：无
//* 说    明：
//* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
//****************************************************************************/
__inline volatile void LCD_SetPoint(u16 x,u16 y,u16 c)
{
    LCD_WriteCom(0x002A);	
	LCD_WriteRAM(x>>8);	    
	LCD_WriteRAM(x&0x00ff);
	LCD_WriteRAM(HDP>>8);	    
	LCD_WriteRAM(HDP&0xff);
    LCD_WriteCom(0x002b);	
	LCD_WriteRAM(y>>8);	    
	LCD_WriteRAM(y&0x00ff);
	LCD_WriteRAM(VDP>>8);	    
	LCD_WriteRAM(VDP&0xff);
	LCD_WriteCom(0x002c);
  
 	LCD_WriteRAM(c);
}

/*ssd11298 3.2寸屏控制器初始化*/
void ssd1963_init(void);
/*清屏函数：将屏幕清除成指定颜色*/
void LCD_Clear(uint16_t Color);
/*显示字符函数*/
void ssd1963_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
/*设置LCD显示模式*/
void LCD_SetMode(unsigned char i,unsigned char j);


#endif
