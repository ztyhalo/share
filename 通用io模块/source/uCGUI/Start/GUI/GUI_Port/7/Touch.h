#ifndef	__Touch_h__
#define	__Touch_h__

#include "stm32f10x.h"

#define	T_DCLK_L	GPIOB->BRR=1<<13
#define T_DCLK_H	GPIOB->BSRR=1<<13

#define T_CS_L		GPIOB->BRR=1
#define	T_CS_H		GPIOB->BSRR=1

#define	T_DOUT_L	GPIOB->BRR=1<<15
#define	T_DOUT_H	GPIOB->BSRR=1<<15

#define	T_DIN		(1&((GPIOB->IDR)>>14))

#define	T_INT		(1&((GPIOC->IDR)>>5))


//SPI写数据
//向7843写入1byte数据   
void ADS_Write_Byte(uint8_t num) ;

//SPI读数据 
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	   
uint16_t ADS_Read_AD(uint8_t CMD)	;

//
void Touch_Configuration(void);

uint8_t Touch_GexX(uint16_t *x) ;
uint8_t Touch_GexY(uint16_t *y) ;


#endif
