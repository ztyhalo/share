#include "includes.h"
#include "usart.h"
#include "stm32f10x.h"


/*Ӳ��ƽ̨��ʼ��*/
void BSP_Init(void);
/*������������*/
void Task_Start(void);

int main(void)
{
	/*��ֹCPU�ж�*/
	CPU_IntDis();

	
	/*UCOS ��ʼ�� */
	OSInit();              /* Initialize "uC/OS-II, The Real-Time Kernel".*/

	/*Ӳ��ƽ̨��ʼ��*/
	

//	/*UCOS ��ʼ�� */
//	OSInit();              /* Initialize "uC/OS-II, The Real-Time Kernel".*/

	/*��ʼucos�ĵ��Ƚ���*/
//	OS_CPU_SysTickInit();

	Task_Start();

//	OSTimeSet(0);
	/**/

	OSStart();

	

}
