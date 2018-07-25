/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUITouch.Conf.h
Purpose     : Configures touch screen module
----------------------------------------------------------------------
*/


#ifndef GUITOUCH_CONF_H
#define GUITOUCH_CONF_H

								  
//#define GUI_TOUCH_AD_LEFT      	   0x79f
//#define GUI_TOUCH_AD_RIGHT     	   0xa6
//#define GUI_TOUCH_AD_TOP           0x746
//#define GUI_TOUCH_AD_BOTTOM        0x80
#define GUI_TOUCH_AD_LEFT      	   0x72
#define GUI_TOUCH_AD_RIGHT     	   0x7a2
#define GUI_TOUCH_AD_TOP           0x7b
#define GUI_TOUCH_AD_BOTTOM        0x7ac
//#define GUI_TOUCH_AD_LEFT  0x73b   
//#define GUI_TOUCH_AD_RIGHT  0xcc 
//#define GUI_TOUCH_AD_TOP	0x86 
//#define GUI_TOUCH_AD_BOTTOM 0x769
#define GUI_TOUCH_SWAP_XY    1
#define GUI_TOUCH_MIRROR_X   1
#define GUI_TOUCH_MIRROR_Y   0

#endif /* GUITOUCH_CONF_H */
