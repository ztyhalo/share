/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
	加入Includes.h的好处： 在c文件头不必添加每个需要的文件包含
	坏处：includes中任意文件改变后，只要包含了includes.h文件的文件都会重新编译，增加了编译时间
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>



#include  "stm32f10x.h"
#include  <stdio.h>
#include  <stdlib.h>

#include  "app_cfg.h"
#include  "os_cfg.h"

#include  "os_cpu.h"
#include  "ucos_ii.h"

//#include  "can.h"



#endif


