#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

/*SD ����Ҫ��ͷ�ļ�*/
#include "sdcard.h"
#include "ff.h"
#include "ffconf.h"


#include "ssd1298.h"

/*λͼ�ṹ*/
struct BMP_HEAD
{
	uint8_t  pic_head[2];				//1
	uint16_t pic_size_l;			    //2
	uint16_t pic_size_h;			    //3
	uint16_t pic_nc1;				    //4
	uint16_t pic_nc2;				    //5
	uint16_t pic_data_address_l;	    //6
	uint16_t pic_data_address_h;		//7	
	uint16_t pic_message_head_len_l;	//8
	uint16_t pic_message_head_len_h;	//9
	uint16_t pic_w_l;					//10
	uint16_t pic_w_h;				    //11
	uint16_t pic_h_l;				    //12
	uint16_t pic_h_h;				    //13	
	uint16_t pic_bit;				    //14
	uint16_t pic_dip;				    //15
	uint16_t pic_zip_l;			    	//16
	uint16_t pic_zip_h;			    	//17
	uint16_t pic_data_size_l;		    //18
	uint16_t pic_data_size_h;		    //19
	uint16_t pic_dipx_l;			    //20
	uint16_t pic_dipx_h;			    //21	
	uint16_t pic_dipy_l;			    //22
	uint16_t pic_dipy_h;			    //23
	uint16_t pic_color_index_l;	    	//24
	uint16_t pic_color_index_h;	    	//25
	uint16_t pic_other_l;			    //26
	uint16_t pic_other_h;			    //27
	uint16_t pic_color_p01;		    	//28
	uint16_t pic_color_p02;		    	//29
	uint16_t pic_color_p03;		    	//30
	uint16_t pic_color_p04;		    	//31
	uint16_t pic_color_p05;		    	//32
	uint16_t pic_color_p06;		    	//33
	uint16_t pic_color_p07;		    	//34
	uint16_t pic_color_p08;				//35			
};

struct BMP_POINT
{
	uint16_t x;
	uint16_t y;
	uint8_t  r;
	uint8_t  g;
	uint8_t  b;		
};
char buffer[1440]; // file copy buffer;

/*������ͼ�ṹ*/
struct BMP_HEAD bmp1;
struct BMP_POINT point;

/*�ⲿ����*/
extern short hwin,hGUI_ID_DROPDOWN0,hGUI_ID_DROPDOWN1,hGUI_ID_BUTTON0,hGUI_ID_BUTTON1,hGUI_ID_MULTIEDIT0;

extern DIR dirs;
extern FILINFO finfo;
extern FATFS fs;

extern char TXT_Files[20][40];	//���20���ļ�,
extern char BMP_Files[20][40];	//���20���ļ�,

/*��������*/
char TXT_SelectFileName[40];  //ռʱδ��
char BMP_SelectFileName[40];
unsigned DROPDOWN1_Index=3,DROPDOWN2_Index=3;


//void OnDropDownSelChanged(WM_MESSAGE * pMsg)
//{
//}

void OnDropDown1SelChanged(WM_MESSAGE * pMsg)
{
	/*���ǲ��û�ȡ�����Ϊ֮ǰ�����Ѿ�֪����Ӧ�û�ȡ��*/
	DROPDOWN1_Index = DROPDOWN_GetSel(hGUI_ID_DROPDOWN0);
		
}

void OnDropDown2SelChanged(WM_MESSAGE * pMsg)
{
	/*�õ�ѡ������ݣ��Է������ȷ����ʾ�Ƿ�ͼ*/
	DROPDOWN2_Index = DROPDOWN_GetSel(hGUI_ID_DROPDOWN1);	
}

void OnButton1Released(WM_MESSAGE * pMsg)
{	
	char buf[512];
	UINT br;
	FRESULT res;
	FIL fsrc;


	Printf("TXT_SelectFileName=%s\r\n\r\nDROPDOWN1_Index=%d\r\n",TXT_Files[DROPDOWN1_Index],DROPDOWN1_Index);
	/*���ļ�*/ /*TXT_Files[DROPDOWN1_Index] �ﱣ����ļ���*/
	res = f_open(&fsrc, TXT_Files[DROPDOWN1_Index], FA_OPEN_EXISTING | FA_READ); 
	if(0 != res){
	
		Printf("ERROR->OPEN FILE -%s!\r\n\r\nreturn value is res=%d\r\nDROPDOWN1_Index=%d\r\nREBOOT THE BOARD,PLEASE\r\n",TXT_Files[DROPDOWN1_Index],res,DROPDOWN1_Index);
		MULTIEDIT_SetText(hGUI_ID_MULTIEDIT0,"ERROR->OPEN FILE \r\nREBOOT THE BOARD,PLEASE");
		return ;
	}

	while(1) {

		/*���ļ������壨��*/
		res = f_read(&fsrc, buf, 512, &br);
		

		if(res == FR_OK)
			/*���������ļ���ʾ�����б༭�ؼ���*/
			MULTIEDIT_SetText(hGUI_ID_MULTIEDIT0,buf);
		else break;

		if(br < 512){/*���ļ�ĩβ��*/
			
			Printf("br=0x%x,�ļ�β��\r\n",br);
			break;
		}

//		Printf("br=0x%x\r\n",br);
		
	}	

	/*ж���ļ����򵥵Ķ���*/
	f_close(&fsrc);

	
}
unsigned char BMP_EXIT_FLAG;
void OnButton2Release(WM_MESSAGE * pMsg)
{
	
	UINT br;
	FRESULT res;
	FIL fsrc;
	unsigned short temp;

	uint32_t tx,ty,r_data,g_data,b_data;//��ʱ�õ�������

//	FRAMEWIN_Minimize(hwin);
//	FRAMEWIN_Delete(hwin);

	BMP_EXIT_FLAG = 1;

	/*BMP_Files[DROPDOWN2_Index] �ﱣ����ļ���*/
	res = f_open(&fsrc, BMP_Files[DROPDOWN2_Index], FA_OPEN_EXISTING | FA_READ);

	/*��ȡ�ļ�ͷ����ʼ�ṹ*/
	res = f_read(&fsrc, &bmp1, sizeof(bmp1), &br);
	if(0 != res){ /*�򿪳���*/
	
		Printf("ERROR->OPEN FILE -%s!\r\n\r\nreturn value is res=%d\r\nDROPDOWN2_Index=%d\r\nREBOOT THE BOARD,PLEASE\r\n",BMP_Files[DROPDOWN2_Index],res,DROPDOWN2_Index);
//		MULTIEDIT_SetText(hGUI_ID_MULTIEDIT0,"ERROR->OPEN FILE \r\nREBOOT THE BOARD,PLEASE");
		GUI_DispStringAt("ERROR->OPEN FILE \r\nREBOOT THE BOARD,PLEASE",50,80);
		return ;
	}

	/*��λ��ָ��λ�ã�����λͼ�ṹȷ��*/
	res = f_lseek(&fsrc, ((bmp1.pic_data_address_h<<16)|bmp1.pic_data_address_l));
	if(0 != res){ /*����*/
	
		Printf("ERROR->LSEEK FILE -%s!\r\nres=%d\r\nREBOOT THE BOARD,PLEASE\r\n",BMP_Files[DROPDOWN2_Index],res);
//		MULTIEDIT_SetText(hGUI_ID_MULTIEDIT0,"ERROR->OPEN FILE \r\nREBOOT THE BOARD,PLEASE");
		GUI_DispStringAt("ERROR->LSEEK FILE \r\nREBOOT THE BOARD,PLEASE",50,80);
		return ;
	}


	temp = 	(bmp1.pic_w_l)*3;
	for(tx=0;tx<bmp1.pic_h_l;tx++){	/*���ｫ�����͵���Ļ���õ��˶����������û���õ�GUI*/

		res = f_read(&fsrc, buffer,temp , &br);

		for(ty=0;ty<bmp1.pic_w_l;ty++)
		{
			r_data = *(ty*3 +2+buffer);
			g_data = *(ty*3 +1+buffer);
			b_data = *(ty*3 +0+buffer);	
						
			if(tx<bmp1.pic_h_l)
			{
				point.x = tx;
				point.y = ty;
				point.r = r_data;
				point.g = g_data;
				point.b = b_data;
				if(bmp1.pic_h_l>240)
				{
					Write_LCDReg(0x4E, 239-point.y);					//����x����ʼ����
					Write_LCDReg(0x4F, 319-point.x);					//����y����ʼ����
				}	
				else
				{
					Write_LCDReg(0x4E, point.x);							//����x����ʼ����
					Write_LCDReg(0x4F, point.y);							//����y����ʼ����
				}
				Write_LCDReg(0X22,RGB888ToRGB565(point.r,point.g,point.b));
			}		
		}				
	}

	f_close(&fsrc);

	while(BMP_EXIT_FLAG){

		OSTimeDlyHMSM(0,0,0,50);
	}

	FRAMEWIN_Minimize(hwin);
	FRAMEWIN_Maximize(hwin);
		
}

//extern void Win_Init(void) ;
//void OnButton3Release(WM_MESSAGE * pMsg)
//{
////	FRAMEWIN_Minimize(hwin);
//	FRAMEWIN_Maximize(hwin);
//	BUTTON_Delete(hGUI_ID_BUTTON2);
//	BMP_EXIT_FLAG = 0;
//}

void OnMultiEditSelChanged(WM_MESSAGE * pMsg)
{
}




