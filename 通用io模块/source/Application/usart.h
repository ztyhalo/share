#ifndef __USART_H
#define __USART_H

//ͷ�ļ�����
#include "stm32f10x.h"
#include "stdio.h"	

//#define Printf		printf

#define	Printf		/\
 

//��������
void USART1_Send_Byte(uint16_t dat);
uint8_t USART1_Receive_Byte(void);
void Init_Usart(void);
void Usart_Configuration(uint32_t BaudRate); 

#endif
