/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"





//EventsFunctionList
void OnCtrlRelease(WM_MESSAGE * pMsg);
void OnMMMMRelease(WM_MESSAGE * pMsg);
void OnMMMRelease(WM_MESSAGE * pMsg);
void OnMMRelease(WM_MESSAGE * pMsg);
void OnNRelease(WM_MESSAGE * pMsg);
void OnMRelease(WM_MESSAGE * pMsg);
void OnXRelease(WM_MESSAGE * pMsg);
void OnCRelease(WM_MESSAGE * pMsg);
void OnZRelease(WM_MESSAGE * pMsg);
void OnVRelease(WM_MESSAGE * pMsg);
void OnBRelease(WM_MESSAGE * pMsg);
void OnSPRelease(WM_MESSAGE * pMsg);
void OnLLRelease(WM_MESSAGE * pMsg);
void OnLRelease(WM_MESSAGE * pMsg);
void OnKRelease(WM_MESSAGE * pMsg);
void OnHRelease(WM_MESSAGE * pMsg);
void OnJRelease(WM_MESSAGE * pMsg);
void OnSRelease(WM_MESSAGE * pMsg);
void OnDRelease(WM_MESSAGE * pMsg);
void OnARelease(WM_MESSAGE * pMsg);
void OnFRelease(WM_MESSAGE * pMsg);
void OnGRelease(WM_MESSAGE * pMsg);
void OnENRelease(WM_MESSAGE * pMsg);
void OnPRelease(WM_MESSAGE * pMsg);
void OnORelease(WM_MESSAGE * pMsg);
void OnIRelease(WM_MESSAGE * pMsg);
void OnYRelease(WM_MESSAGE * pMsg);
void OnURelease(WM_MESSAGE * pMsg);
void OnWRelease(WM_MESSAGE * pMsg);
void OnERelease(WM_MESSAGE * pMsg);
void OnQRelease(WM_MESSAGE * pMsg);
void OnRRelease(WM_MESSAGE * pMsg);
void OnTRelease(WM_MESSAGE * pMsg);
void OnBKRelease(WM_MESSAGE * pMsg);
void OnZeroRelease(WM_MESSAGE * pMsg);
void OnNineRelease(WM_MESSAGE * pMsg);
void OnEightRelease(WM_MESSAGE * pMsg);
void OnSixRelease(WM_MESSAGE * pMsg);
void OnSevenRelease(WM_MESSAGE * pMsg);
void OnTwoRelease(WM_MESSAGE * pMsg);
void OnThreeRelease(WM_MESSAGE * pMsg);
void OnOneRelease(WM_MESSAGE * pMsg);
void OnFourRelease(WM_MESSAGE * pMsg);
void OnFiveRelease(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_BUTTON_5   GUI_ID_USER+5
#define GUI_ID_BUTTON_4   GUI_ID_USER+4
#define GUI_ID_BUTTON_1   GUI_ID_USER+1
#define GUI_ID_BUTTON_3   GUI_ID_USER+103
#define GUI_ID_BUTTON_2   GUI_ID_USER+2
#define GUI_ID_BUTTON_7   GUI_ID_USER+7
#define GUI_ID_BUTTON_6   GUI_ID_USER+6
#define GUI_ID_BUTTON_8   GUI_ID_USER+8
#define GUI_ID_BUTTON_9   GUI_ID_USER+9
#define GUI_ID_BUTTON_0   GUI_ID_USER+10
#define GUI_ID_BUTTONBK   GUI_ID_USER+44
#define GUI_ID_BUTTONT   GUI_ID_USER+15
#define GUI_ID_BUTTONR   GUI_ID_USER+14
#define GUI_ID_BUTTONQ   GUI_ID_USER+11
#define GUI_ID_BUTTONE   GUI_ID_USER+13
#define GUI_ID_BUTTONW   GUI_ID_USER+12
#define GUI_ID_BUTTONU   GUI_ID_USER+17
#define GUI_ID_BUTTONY   GUI_ID_USER+16
#define GUI_ID_BUTTONI   GUI_ID_USER+18
#define GUI_ID_BUTTONO   GUI_ID_USER+19
#define GUI_ID_BUTTONP   GUI_ID_USER+20
#define GUI_ID_BUTTONEN   GUI_ID_USER+43
#define GUI_ID_BUTTONG   GUI_ID_USER+25
#define GUI_ID_BUTTONF   GUI_ID_USER+24
#define GUI_ID_BUTTONA   GUI_ID_USER+21
#define GUI_ID_BUTTOND   GUI_ID_USER+23
#define GUI_ID_BUTTONS   GUI_ID_USER+22
#define GUI_ID_BUTTONJ   GUI_ID_USER+27
#define GUI_ID_BUTTONH   GUI_ID_USER+26
#define GUI_ID_BUTTONK   GUI_ID_USER+28
#define GUI_ID_BUTTONL   GUI_ID_USER+29
#define GUI_ID_BUTTONLL   GUI_ID_USER+30
#define GUI_ID_BUTTONSP   GUI_ID_USER+42
#define GUI_ID_BUTTONB   GUI_ID_USER+35
#define GUI_ID_BUTTONV   GUI_ID_USER+34
#define GUI_ID_BUTTONZ   GUI_ID_USER+31
#define GUI_ID_BUTTONC   GUI_ID_USER+33
#define GUI_ID_BUTTONX   GUI_ID_USER+3
#define GUI_ID_BUTTONM   GUI_ID_USER+37
#define GUI_ID_BUTTON   GUI_ID_USER+36
#define GUI_ID_BUTTONUMM   GUI_ID_USER+38
#define GUI_ID_BUTTONMMM   GUI_ID_USER+39
#define GUI_ID_BUTTONMMMM   GUI_ID_USER+40
#define GUI_ID_BUTTONCtrl   GUI_ID_USER+41


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "KeyBoard",          0,                       0,  0,  188,102,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "5",                 GUI_ID_BUTTON_5,         65, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "4",                 GUI_ID_BUTTON_4,         49, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "1",                 GUI_ID_BUTTON_1,         1,  0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "3",                 GUI_ID_BUTTON_3,         33, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "2",                 GUI_ID_BUTTON_2,         17, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "7",                 GUI_ID_BUTTON_7,         97, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "6",                 GUI_ID_BUTTON_6,         81, 0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "8",                 GUI_ID_BUTTON_8,         113,0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "9",                 GUI_ID_BUTTON_9,         129,0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "0",                 GUI_ID_BUTTON_0,         145,0,  14, 19, 0,0},
    { BUTTON_CreateIndirect,    "BK",                GUI_ID_BUTTONBK,         163,0,  20, 19, 0,0},
    { BUTTON_CreateIndirect,    "T",                 GUI_ID_BUTTONT,          65, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "R",                 GUI_ID_BUTTONR,          49, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "Q",                 GUI_ID_BUTTONQ,          1,  22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "E",                 GUI_ID_BUTTONE,          33, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "W",                 GUI_ID_BUTTONW,          17, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "U",                 GUI_ID_BUTTONU,          97, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "Y",                 GUI_ID_BUTTONY,          81, 22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "I",                 GUI_ID_BUTTONI,          113,22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "O",                 GUI_ID_BUTTONO,          129,22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "P",                 GUI_ID_BUTTONP,          145,22, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "EN",                GUI_ID_BUTTONEN,         163,22, 20, 19, 0,0},
    { BUTTON_CreateIndirect,    "G",                 GUI_ID_BUTTONG,          65, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "F",                 GUI_ID_BUTTONF,          49, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "A",                 GUI_ID_BUTTONA,          1,  44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "D",                 GUI_ID_BUTTOND,          33, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "S",                 GUI_ID_BUTTONS,          17, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "J",                 GUI_ID_BUTTONJ,          97, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "H",                 GUI_ID_BUTTONH,          81, 44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "K",                 GUI_ID_BUTTONK,          113,44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "L",                 GUI_ID_BUTTONL,          129,44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "p",                 GUI_ID_BUTTONLL,         145,44, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "SP",                GUI_ID_BUTTONSP,         163,44, 20, 19, 0,0},
    { BUTTON_CreateIndirect,    "B",                 GUI_ID_BUTTONB,          65, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "V",                 GUI_ID_BUTTONV,          49, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "Z",                 GUI_ID_BUTTONZ,          1,  66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "C",                 GUI_ID_BUTTONC,          33, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "X",                 GUI_ID_BUTTONX,          17, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "M",                 GUI_ID_BUTTONM,          97, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "N",                 GUI_ID_BUTTON,           81, 66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    ",",                 GUI_ID_BUTTONUMM,        113,66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    ".",                 GUI_ID_BUTTONMMM,        129,66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "/",                 GUI_ID_BUTTONMMMM,       145,66, 14, 19, 0,0},
    { BUTTON_CreateIndirect,    "Ct",                GUI_ID_BUTTONCtrl,       163,66, 20, 19, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetClientColor(hWin,0x696969);
    FRAMEWIN_SetFont(hWin,&GUI_Font8x13_ASCII);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,12);
    //
    //GUI_ID_BUTTON_5
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_5),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_5),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_4
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_4),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_4),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_1
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_1),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_1),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_3
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_3),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_3),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_2
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_2),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_2),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_7
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_7),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_7),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_6
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_6),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_6),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_8
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_8),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_8),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_9
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_9),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_9),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON_0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_0),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_0),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONBK
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONBK),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONBK),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONT
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONT),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONT),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONR
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONR),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONR),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONQ
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONQ),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONQ),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONE
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONE),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONE),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONW
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONW),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONW),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONU
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONU),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONU),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONY
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONY),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONY),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONI
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONI),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONI),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONO
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONO),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONO),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONP
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONP),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONP),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONEN
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONEN),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONEN),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONG
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONG),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONG),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONF
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONF),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONF),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONA
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONA),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONA),&GUI_Font6x8);
    //
    //GUI_ID_BUTTOND
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTOND),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTOND),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONS
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONS),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONS),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONJ
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONJ),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONJ),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONH
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONH),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONH),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONK
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONK),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONK),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONL
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONL),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONL),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONLL
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONLL),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONLL),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONSP
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONSP),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONSP),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONB
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONB),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONB),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONV
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONV),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONV),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONZ
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONZ),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONZ),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONC
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONC),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONC),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONX
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONX),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONX),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONM
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONM),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONM),&GUI_Font6x8);
    //
    //GUI_ID_BUTTON
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONUMM
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONUMM),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONUMM),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONMMM
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONMMM),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONMMM),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONMMMM
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONMMMM),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONMMMM),&GUI_Font6x8);
    //
    //GUI_ID_BUTTONCtrl
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTONCtrl),BUTTON_CI_UNPRESSED,0xa9a9a9);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTONCtrl),&GUI_Font6x8);

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_BUTTON_5:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnFiveRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_4:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnFourRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnOneRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_3:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnThreeRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnTwoRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_7:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnSevenRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_6:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnSixRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_8:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnEightRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_9:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnNineRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON_0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnZeroRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONBK:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnBKRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONT:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnTRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONR:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnRRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONQ:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnQRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnERelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONW:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnWRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONU:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnURelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONY:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnYRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONI:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnIRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONO:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnORelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONP:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnPRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONEN:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnENRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONG:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnGRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONF:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnFRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONA:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnARelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTOND:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnDRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONS:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnSRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONJ:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnJRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONH:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnHRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONK:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnKRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONL:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnLRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONLL:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnLLRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONSP:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnSPRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONB:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnBRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONV:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnVRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONZ:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnZRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONC:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnCRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONX:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnXRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONM:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnMRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnNRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONUMM:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnMMRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONMMM:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnMMMRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONMMMM:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnMMMMRelease(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BUTTONCtrl:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            OnCtrlRelease(pMsg);
                            break;
                    }
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
unsigned int Main_Task[1024];
void MainTask(void) 
{ 
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	while(1)
	{
		GUI_CURSOR_Show();
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,15);
		WM_Exec();
	}
}

