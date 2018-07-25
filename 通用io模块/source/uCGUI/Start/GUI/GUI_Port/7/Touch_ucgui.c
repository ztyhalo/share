#include "touch.h"
#include "usart.h"
#include "stm32f10x.h"
void GUI_TOUCH_X_ActivateX(void){

}
void GUI_TOUCH_X_ActivateY(void){

}
void GUI_TOUCH_X_Disable(void){

}

static unsigned short x,y;
int  GUI_TOUCH_X_MeasureX(void){

	Touch_GexX(&x);
	return 	x ;
}
int  GUI_TOUCH_X_MeasureY(void){

	Touch_GexY(&y);
	return 	y ;
}
