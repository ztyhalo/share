#include "GUI_Port.h"

#include "GUI.h"
#include "GUIDebug.h"
#include "ucos_ii.h"


/**/
extern unsigned char  OSTimeDlyHMSM (unsigned char   hours,
                      unsigned char   minutes,
                      unsigned char   seconds,
                      unsigned short  ms);
void GUI_X_ExecIdle(void)
{
//	GUI_TOUCH_Exec();
//	OSTimeDlyHMSM(0,0,0,1);
}
/**/
void GUI_X_Unlock(void)
{
}
/**/
void GUI_X_Lock(void)
{
}
/**/
U32  GUI_X_GetTaskId(void)
{
	return ((U32)(OSTCBCur->OSTCBPrio));
}
/**/
void GUI_X_InitOS(void)
{
}
/**/
void GUI_X_Warn(const char *s){

}
/*GUI ERR out*/
void GUI_X_ErrorOut(const char *s)
{
	printf(s);
}

/*GUI_X_LOG do nothing*/
void GUI_X_Log(const char *s)
{
	printf(s);
}
/**/
void GUI_X_Init(void)
{
	
}
/**/
void GUI_X_Delay(int Period)
{
	OSTimeDlyHMSM(0,0,0,Period);		
}
/**/
//extern unsigned int  OSTimeGet (void);
int  GUI_X_GetTime(void)
{
	return ((int)OSTimeGet()); //返回USB当前倒计数值	
}


