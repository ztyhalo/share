#include "stm32f10x_can.h"
#include "uCOS_II.h"

#define  ADDRESS_MASK	0xfffffff0
#define	 FUNCTION_MASK	0x0000000f
#define	 HOST_ADDRESS	0x200
typedef union can_head{
	struct {
		unsigned int func_4:4;
		unsigned int adre_5:5;
		unsigned int mast_2:2;
		unsigned int no_use_21:21;
	}extid;
	uint32_t extid_32;
}CAN_HEAD;


typedef struct 
{
	CAN_HEAD	StdId;
	uint8_t		DLC;
	uint8_t		Data[8];	
} CAN_FRAME;


//解析上位机发送的不同功能码
#define   START_ACK            0x0
#define   OUTCONTR_ASK          0x1
#define   DATA_ASK              0xF
#define   CHANGE_RESPOND        0x7


//发送帧的标识符ID
enum  
{
	START_ASK_ID         =     0x0650,          //上电认可信息请求
	OUTCONTR_ACK_ID,       	                    //输出控制指令应答
	DATA_ACK_ONE_ID,      				        //数据请求帧应答1
	DATA_ACK_TWO_ID,        	 				//数据请求帧应答2	
	DATA_ACK_THR_ID,      	 				    //数据请求帧应答3
	DATA_ACK_FOU_ID,        					//数据请求帧应答4
	CONFIG_ACK_ID,         					    //上电认可应答及配置的应答
	INCHANGE_ASK_ID              	 		    //输入口变化请求	
};

//发送帧的功能码
enum
{
	START_ASK         =     0x0,           //上电认可信息请求
	OUTCONTR_ACK,       	                //输出控制指令应答
	DATA_ACK_ONE,      				        //数据请求帧应答1
	DATA_ACK_TWO,        	 				//数据请求帧应答2	
	DATA_ACK_THR,      	 				    //数据请求帧应答3
	DATA_ACK_FOU,        					//数据请求帧应答4
	CONFIG_ACK,        					    //上电认及配置的应答
	INCHANGE_ASK          	 		      //输入口变化请求
};

typedef struct can_trs			            //CAN发送数据结构体
{
	CAN_FRAME        TrsFrame;
	u8		          Num;					    //发送次数
	u8		          State;       			  //发送状态
	u8				  Result;           //send of result
	u32		          Interval;				 //发送间隔
	u32				  TrsTime;               //发送时间
	struct can_trs   *Next; 
} *TrsData;

typedef struct 	over_data				  //can发送数据结束处理结构体
{
	CAN_FRAME          TrsFrame;
	u8				   Result;
	struct over_data   *Next;
}  *TrsEnData;

enum									//发送结果定义 
{
CAN_TRS_FAIL=0,
CAN_TRS_SUCCESS
};

typedef struct can_rec
{
	CAN_FRAME		TrsFrame;
	struct can_rec 	*Next;		
}  *RxData;

//帧数据字节数宏定义
#define	DLC_BYTES_0		(u8)0
#define	DLC_BYTES_1		(u8)1
#define	DLC_BYTES_2		(u8)2
#define	DLC_BYTES_3		(u8)3
#define	DLC_BYTES_4		(u8)4
#define	DLC_BYTES_5		(u8)5
#define	DLC_BYTES_6		(u8)6
#define	DLC_BYTES_7		(u8)7
#define	DLC_BYTES_8		(u8)8

//发送时间定时器参数
#define TIMER_CLOSED   0xffff
#define TIMER_EXPIRED  0x0000
#define CAN_ON		   1		//can发送数据
#define CAN_WAITE      0		//can发送成功后等待应答
#define TrsLeng		   8		//发送链表长度


#define CAN_TXMAILBOX_0		((uint8_t)0x00)
#define TMIDxR_TXRQ  ((uint32_t)0x00000001)

/*全局变量声明*/
extern  TrsData    CanTrsHead;				//发送链表首项
extern  TrsData    CanTrsHandle;			//发送完毕处理链表首项
extern  RxData	   CanRxHead;
extern  int      Guest_Address;
/*函数声明*/
u16 InCanTrsQue(CAN_FRAME  TrsCan);
uint8_t CAN_TransFram(CAN_TypeDef* CANx, CAN_FRAME* TxMessage);
void IdSeek(CAN_FRAME TxCan, TrsData TxCanFrame);
uint8_t InTrsHandle(TrsData TxCanHandle, u8 Result);
void *DelCanQueueHead(void *LineHead, OS_MEM *Buf);
u8 DelCanQueueOne(TrsData Line, OS_MEM *Buf);
void CAN_Config(void);
void CanRxProc(void);
void CanTxProc(void);
void DecCanTrsTimer(void);
void CanTrsDummy(CAN_TypeDef* CANx);
