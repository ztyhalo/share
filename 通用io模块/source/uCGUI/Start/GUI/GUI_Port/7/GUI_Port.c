#include "GUI_Port.h"

#include "GUI.h"
#include "GUIDebug.h"


/**/
void GUI_X_ExecIdle(void)
{
	OSTimeDlyHMSM(0,0,0,15);
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
//	return 0;
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

}
/**/
int  GUI_X_GetTime(void)
{
	return 0;	
}


