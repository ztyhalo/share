#ifndef __ssd1298_h__
#define __ssd1298_h__


#include "stm32f10x.h"
#include "gui.h"
#include "LCDConf.h"
#include "LCD_Protected.h"
#include "ucgui_api.h"

/*�ⲿ��������*/
extern uint16_t DeviceID;

/*LCD ��ַ����*/
#define LCD_RAM    *(__IO uint16_t *) ((uint32_t)0x60020000)    	//��ַ�Ĵ���
#define LCD_REG    *(__IO uint16_t *) ((uint32_t)0x60000000)	 	//ָ��Ĵ���


/*�� LCD �Ĵ��������ض����ļĴ���ֵ*/
__inline uint16_t Read_LCDReg(uint16_t LCD_Reg)
{
	LCD_REG= LCD_Reg;
 	__nop();__nop();
	(void)LCD_RAM; 
	__nop();__nop();
  
  	return LCD_RAM;		
}

/*�� LCD�Ĵ���д��ֵ*/
__inline void Write_LCDReg(uint16_t LCD_Reg,uint16_t LCD_Dat)
{
	LCD_REG= LCD_Reg;	 		//��ָ��Ĵ���дָ��

	LCD_RAM= LCD_Dat; 			//�����ݼĴ���д����
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: RGB888ToRGB565
** ��������: 24λ�������16λͼ
** ����������r��g��b ������ɫ
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
__inline uint32_t RGB888ToRGB565(uint8_t r,uint8_t g,uint8_t b)
{
 	return (u32) (r&0xF8)<<8 | (g&0xFC)<<3 | (b&0xF8)>>3; //565
}

/*����LCD����*/
__inline void Set_LCDCorsor(uint16_t x,uint16_t y)
{	
#if	 	LCD_ID_AM == 1
	Write_LCDReg(0x4E, x);							//����x����ʼ����
	Write_LCDReg(0x4F, y);							//����y����ʼ����
#elif  	LCD_ID_AM == 0
	Write_LCDReg(0x4E, 239-y);						//����x����ʼ����
	Write_LCDReg(0x4F, 319-x);						//����y����ʼ����
#endif
}

/*ssd11298 3.2������������ʼ��*/
void ssd1298_init(void);
/*��������������Ļ�����ָ����ɫ*/
void LCD_Clear(uint16_t Color);
/*��ʾ�ַ�����*/
void ssd1298_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
/*����LCD��ʾģʽ*/
void LCD_SetMode(unsigned char i,unsigned char j);


#endif
