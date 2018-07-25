#ifndef __ssd1298_h__
#define __ssd1298_h__


#include "stm32f10x.h"
#include "gui.h"
#include "LCDConf.h"
#include "LCD_Protected.h"
#include "ucgui_api.h"

/*外部变量申明*/
extern uint16_t DeviceID;

/*LCD 地址定义*/
#define LCD_RAM    *(__IO uint16_t *) ((uint32_t)0x60020000)    	//地址寄存器
#define LCD_REG    *(__IO uint16_t *) ((uint32_t)0x60000000)	 	//指令寄存器


/*读 LCD 寄存器，返回读到的寄存器值*/
__inline uint16_t Read_LCDReg(uint16_t LCD_Reg)
{
	LCD_REG= LCD_Reg;
 	__nop();__nop();
	(void)LCD_RAM; 
	__nop();__nop();
  
  	return LCD_RAM;		
}

/*向 LCD寄存器写入值*/
__inline void Write_LCDReg(uint16_t LCD_Reg,uint16_t LCD_Dat)
{
	LCD_REG= LCD_Reg;	 		//往指令寄存器写指令

	LCD_RAM= LCD_Dat; 			//往数据寄存器写数据
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: RGB888ToRGB565
** 功能描述: 24位。。变成16位图
** 参数描述：r、g、b 三种颜色
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
__inline uint32_t RGB888ToRGB565(uint8_t r,uint8_t g,uint8_t b)
{
 	return (u32) (r&0xF8)<<8 | (g&0xFC)<<3 | (b&0xF8)>>3; //565
}

/*设置LCD坐标*/
__inline void Set_LCDCorsor(uint16_t x,uint16_t y)
{	
#if	 	LCD_ID_AM == 1
	Write_LCDReg(0x4E, x);							//定义x的起始坐标
	Write_LCDReg(0x4F, y);							//定义y的起始坐标
#elif  	LCD_ID_AM == 0
	Write_LCDReg(0x4E, 239-y);						//定义x的起始坐标
	Write_LCDReg(0x4F, 319-x);						//定义y的起始坐标
#endif
}

/*ssd11298 3.2寸屏控制器初始化*/
void ssd1298_init(void);
/*清屏函数：将屏幕清除成指定颜色*/
void LCD_Clear(uint16_t Color);
/*显示字符函数*/
void ssd1298_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
/*设置LCD显示模式*/
void LCD_SetMode(unsigned char i,unsigned char j);


#endif
