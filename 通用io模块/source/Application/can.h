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


//������λ�����͵Ĳ�ͬ������
#define   START_ACK            0x0
#define   OUTCONTR_ASK          0x1
#define   DATA_ASK              0xF
#define   CHANGE_RESPOND        0x7


//����֡�ı�ʶ��ID
enum  
{
	START_ASK_ID         =     0x0650,          //�ϵ��Ͽ���Ϣ����
	OUTCONTR_ACK_ID,       	                    //�������ָ��Ӧ��
	DATA_ACK_ONE_ID,      				        //��������֡Ӧ��1
	DATA_ACK_TWO_ID,        	 				//��������֡Ӧ��2	
	DATA_ACK_THR_ID,      	 				    //��������֡Ӧ��3
	DATA_ACK_FOU_ID,        					//��������֡Ӧ��4
	CONFIG_ACK_ID,         					    //�ϵ��Ͽ�Ӧ�����õ�Ӧ��
	INCHANGE_ASK_ID              	 		    //����ڱ仯����	
};

//����֡�Ĺ�����
enum
{
	START_ASK         =     0x0,           //�ϵ��Ͽ���Ϣ����
	OUTCONTR_ACK,       	                //�������ָ��Ӧ��
	DATA_ACK_ONE,      				        //��������֡Ӧ��1
	DATA_ACK_TWO,        	 				//��������֡Ӧ��2	
	DATA_ACK_THR,      	 				    //��������֡Ӧ��3
	DATA_ACK_FOU,        					//��������֡Ӧ��4
	CONFIG_ACK,        					    //�ϵ��ϼ����õ�Ӧ��
	INCHANGE_ASK          	 		      //����ڱ仯����
};

typedef struct can_trs			            //CAN�������ݽṹ��
{
	CAN_FRAME        TrsFrame;
	u8		          Num;					    //���ʹ���
	u8		          State;       			  //����״̬
	u8				  Result;           //send of result
	u32		          Interval;				 //���ͼ��
	u32				  TrsTime;               //����ʱ��
	struct can_trs   *Next; 
} *TrsData;

typedef struct 	over_data				  //can�������ݽ�������ṹ��
{
	CAN_FRAME          TrsFrame;
	u8				   Result;
	struct over_data   *Next;
}  *TrsEnData;

enum									//���ͽ������ 
{
CAN_TRS_FAIL=0,
CAN_TRS_SUCCESS
};

typedef struct can_rec
{
	CAN_FRAME		TrsFrame;
	struct can_rec 	*Next;		
}  *RxData;

//֡�����ֽ����궨��
#define	DLC_BYTES_0		(u8)0
#define	DLC_BYTES_1		(u8)1
#define	DLC_BYTES_2		(u8)2
#define	DLC_BYTES_3		(u8)3
#define	DLC_BYTES_4		(u8)4
#define	DLC_BYTES_5		(u8)5
#define	DLC_BYTES_6		(u8)6
#define	DLC_BYTES_7		(u8)7
#define	DLC_BYTES_8		(u8)8

//����ʱ�䶨ʱ������
#define TIMER_CLOSED   0xffff
#define TIMER_EXPIRED  0x0000
#define CAN_ON		   1		//can��������
#define CAN_WAITE      0		//can���ͳɹ���ȴ�Ӧ��
#define TrsLeng		   8		//����������


#define CAN_TXMAILBOX_0		((uint8_t)0x00)
#define TMIDxR_TXRQ  ((uint32_t)0x00000001)

/*ȫ�ֱ�������*/
extern  TrsData    CanTrsHead;				//������������
extern  TrsData    CanTrsHandle;			//������ϴ�����������
extern  RxData	   CanRxHead;
extern  int      Guest_Address;
/*��������*/
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
