#ifndef __app1_h__
#define	__app1_h__


/*硬件平台初始化*/
void BSP_Init(void);
/*创建所有任务*/
void Task_Start(void);

/*************************************资源表定义**********************************/
/************************************************************************************/
#define	RRAMEWIN_ID		0+200
#define	TEXT_ID1			1+200
#define	LED1_ID1			2+200
#define	LED2_ID2			3+200
#define	LED3_ID3			4+200
#define	TEXT_ID2			5+200
#define	TEXT_ID3			6+200
#define	TEXT_ID4			7+200
#define	TEXT_ID5			8+200
#define	TEXT_ID6			9+200
#define	TEXT_ID7			10+200
#define	TEXT_ID8			11+200
#define EDIT_ID1		  	12+200
#define EDIT_ID2		  	13+200
#define EDIT_ID3		  	14+200
#define EDIT_ID4		  	15+200
#define EDIT_ID5		  	16+200
#define EDIT_ID6		  	17+200
#define EDIT_ID7		  	18+200
#define	BUTTON_ID1			19+200
#define	BUTTON_ID2			20+200
#define	BUTTON_ID3			21+200
#define	BUTTON_ID4			22+200
#define	BUTTON_ID5			23+200
#define	BUTTON_ID6			24+150
#define	BUTTON_ID7			25+200
#define	BUTTON_ID8			26+200
#define	BUTTON_ID9			27+200
#define	BUTTON_ID10			28+200
#define	BUTTON_ID11			29+200
#define	BUTTON_ID12			30+150


#ifndef	GUI_Table
#define	GUI_Table
/*描述符申明*/
WM_HWIN hWin;
WM_HWIN text0,text1,text2,text3,text4,text5,text6,edit0,edit1,edit2,edit3,edit4,edit5,edit6;

/*定义资源表*/
GUI_WIDGET_CREATE_INFO aDialogCreate[] = {
	
	/*框架窗口*/
	{ FRAMEWIN_CreateIndirect, "RTC Demo", RRAMEWIN_ID, 0, 0, 320, 240, /*FRAMEWIN_CF_ACTIVE*/0 ,0 },
	/*文本控件*/
	{ TEXT_CreateIndirect, "Years", TEXT_ID1, 6, 10, 40, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Month", TEXT_ID2, 6, 40, 30, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Day", 	TEXT_ID3, 6, 70, 24, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Hours", TEXT_ID4, 6, 100, 40, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Minute",TEXT_ID5, 6, 130, 48, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Second",TEXT_ID6, 6, 160, 48, 13, 0,TEXT_CF_LEFT  },
	{ TEXT_CreateIndirect, "Week", 	TEXT_ID7, 6, 190, 32, 13, 0,TEXT_CF_LEFT  },

	/*文本编辑*/
	{ EDIT_CreateIndirect, " ", EDIT_ID1, 56, 8, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID2, 56, 38, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID3, 56, 68, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID4, 56, 98, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID5, 56, 128, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID6, 56, 158, 50, 18, 0,TEXT_CF_LEFT  },
	{ EDIT_CreateIndirect, " ", EDIT_ID7, 56, 188, 50, 18, 0,TEXT_CF_LEFT  },

	/*按键*/
	{ BUTTON_CreateIndirect, "+", BUTTON_ID1, 110, 8, 20, 15 ,0,0 },
	{ BUTTON_CreateIndirect, "+", BUTTON_ID2, 110, 38, 20, 15 ,0,0 },
	{ BUTTON_CreateIndirect, "+", BUTTON_ID3, 110, 68, 20, 15 ,0,0 },
	{ BUTTON_CreateIndirect, "+", BUTTON_ID4, 110, 98, 20, 15 ,0,0 },
	{ BUTTON_CreateIndirect, "+", BUTTON_ID5, 110, 128, 20, 15 ,0,0 },
	{ BUTTON_CreateIndirect, "+", BUTTON_ID6, 110, 158, 20, 15 ,0,0 },

//	{ BUTTON_CreateIndirect, "-", BUTTON_ID7, 140, 8, 20, 15 ,0,0 },
//	{ BUTTON_CreateIndirect, "-", BUTTON_ID8, 140, 38, 20, 15 ,0,0 },
//	{ BUTTON_CreateIndirect, "-", BUTTON_ID9, 140, 68, 20, 15 ,0,0 },
//	{ BUTTON_CreateIndirect, "-", BUTTON_ID10, 140, 98, 20, 15 ,0,0 },
//	{ BUTTON_CreateIndirect, "-", BUTTON_ID11, 140, 128, 20, 15 ,0,0 },
//	{ BUTTON_CreateIndirect, "-", BUTTON_ID12, 140, 158, 20, 15 ,0,0 },
};

#endif

/*什么外部资源表*/
#ifdef	GUI_Table
extern 	GUI_WIDGET_CREATE_INFO aDialogCreate[];
#endif

#endif
