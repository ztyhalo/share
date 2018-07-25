#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"


/*硬件平台初始化*/
void BSP_Init(void);
/*创建所有任务*/
void Task_Start(void);

int main(void)
{
	/*禁止CPU中断*/
	CPU_IntDis();

	
	/*UCOS 初始化 */
	OSInit();              /* Initialize "uC/OS-II, The Real-Time Kernel".*/

	/*硬件平台初始化*/
	

//	/*UCOS 初始化 */
//	OSInit();              /* Initialize "uC/OS-II, The Real-Time Kernel".*/

	/*初始ucos的调度节拍*/
//	OS_CPU_SysTickInit();

	Task_Start();

//	OSTimeSet(0);
	/**/

	OSStart();

	

}
