/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIDEMO_Intro.c
Purpose     : Introduction for 礐/GUI generic demo
              (This is also a good file to demo and explain basic
              uC/GUI features by setting breakpoints)
----------------------------------------------------------------------
*/

//#include "GUI.h"
//#include "GUIDEMO.h"
#include "GUI.h"
#include "GUIDEMO.h"
#include <string.h>

/*********************************************************************
*
*       GUIDEMO_Intro
*
**********************************************************************
*/

#if GUIDEMO_LARGE
#define u16 unsigned short
//extern const GUI_FONT GUI_FontHZ_hwhb_32;
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
void GUIDEMO_Intro(void)
{
 int xCenter = LCD_GET_XSIZE() / 2;
 int y;
 char acText[50] = "Version of 礐/GUI: ";
  
  strcat(acText, GUI_GetVersionString());
  
  GUI_SetBkColor(GUI_BLUE);
  GUI_SetColor(GUI_LIGHTRED);
  GUI_Clear();
  GUI_SetFont(&GUI_Font24B_1);
  GUI_DispStringHCenterAt("礐/GUI", xCenter, y= 15);

  GUI_SetFont(&GUI_Font13HB_1);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringHCenterAt(acText, xCenter, y += 26);
  
  GUI_DrawBitmap(&bmMicriumLogo, (LCD_GET_XSIZE() - bmMicriumLogo.XSize) / 2, y += 16);

  GUI_Line(0, y+45, LCD_XSIZE-1, y+45, GUI_RED);
  GUI_Line(0, y+46, LCD_XSIZE-1, y+46, GUI_RED);

//  GUI_SetFont(&GUI_FontHZ_hwhb_32);
  GUI_SetColor(GUI_RED);
  GUI_DispStringHCenterAt("旺宝电子", LCD_GET_XSIZE() / 2, y += 56);
//  GUI_DispStringHCenterAt("国内一流开发工具供应商", LCD_GET_XSIZE() / 2, y += 56);	  
  GUI_DispStringHCenterAt("www.openmcu.com", LCD_GET_XSIZE() / 2, y += 56);
  GUIDEMO_Delay(5000);
}

#else

void GUIDEMO_Intro(void) {
  int xCenter = LCD_GET_XSIZE() / 2;
  int y;
  char acText[50] = "Version of 礐/GUI: ";
  strcat(acText, GUI_GetVersionString());
  GUI_SetBkColor(GUI_BLUE);
  GUI_SetColor(GUI_YELLOW);
  GUI_Clear();
  GUI_SetFont(&GUI_Font13B_1);
  GUI_DispStringHCenterAt("礐/GUI", xCenter, y= 10);
  GUI_SetFont(&GUI_Font10_ASCII);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringHCenterAt("Universal graphic software"
                          "\nfor embedded applications"
                          , xCenter, y += 20);
  GUI_SetFont(&GUI_Font10S_ASCII);
  GUI_DispStringHCenterAt("Compiled " __DATE__ " "__TIME__, xCenter, y += 25);
  GUI_DispStringHCenterAt(acText, xCenter, y += 16);
  GUIDEMO_Delay(5000);

  GUI_Clear();
  GUI_DrawBitmap(&bmMicriumLogo, (LCD_GET_XSIZE() - bmMicriumLogo.XSize) / 2, 6);
  GUI_SetFont(&GUI_Font13B_1);
  GUI_DispStringHCenterAt("www.PowerAVR.com", LCD_GET_XSIZE() / 2, LCD_GET_YSIZE() - 50);
  GUIDEMO_Delay(5000);


}

#endif
