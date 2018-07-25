#include "gui.h"
#include "usart.h"
#include "ssd1963.h"
#include "stm32f10x.h"
#include "ascii_8x16_font.h"

/*保存 LCD 的IDCode*/
uint16_t DeviceID;

/*ssd11298 3.2寸屏控制器初始化*/
void ssd1298_init(void)
{
	/*打开FSMC时钟*/
	RCC->AHBENR |= 1<<8;	
	/*打开PD  PE端口时钟，和复用功能时钟*/
	RCC->APB2ENR |= RCC_APB2Periph_GPIOD|\
					RCC_APB2Periph_GPIOE|\
					RCC_APB2Periph_AFIO;
	/*端口D设置*/
	GPIOD->CRL &= 0x0f00ff00;
	GPIOD->CRL |= 0xb0bb00bb;
	GPIOD->CRH &= 0x00ff0000;
	GPIOD->CRH |= 0xbb00bbbb;
	/*端口E设置*/
	GPIOE->CRL &= 0x0fffffff;
	GPIOE->CRL |= 0xb0000000;
	GPIOE->CRH  = 0xbbbbbbbb;
	/*FSMC 设置*/
/**$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$**/
	#define	FSMC_BCR1	(*(volatile unsigned int*)0xA0000000)
	#define	FSMC_BTR1	(*(volatile unsigned int*)0xA0000004)
	#define	FSMC_BWTR1	(*(volatile unsigned int*)0xA0000104)
/**$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$**/
	FSMC_BCR1	= 0;
	FSMC_BCR1 |= (1<<12)|(1<<6)|(1<<4) ;
	FSMC_BTR1 = 0;			  
	FSMC_BTR1 = (1<<8);
//	FSMC_BWTR1 = 1<<8;	   /*不需要*/
	FSMC_BCR1 |= 1;
	/*延时*/
	for(DeviceID=0;DeviceID<0xfff;DeviceID++);
	/*读LCD 的IDCode*/
	DeviceID = Read_LCDReg(0x0000);	  
	Printf("DeviceID=0x%x\r\n",DeviceID);
	if((DeviceID == 0x8999) || ((DeviceID&0x8909) == 0x8909))
	{	 
				//************* Start Initial Sequence **********//
		Write_LCDReg(0x00, 0x0001); // 开启内部时钟
		Write_LCDReg(0x01, 0x3B3F); //输出驱动控制: RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
		Write_LCDReg(0x02, 0x0600); //set 1 line inversion
		//************* Power control setup ************/
		Write_LCDReg(0x0C, 0x0007); // Adjust VCIX2 output voltage
		Write_LCDReg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
		Write_LCDReg(0x0E, 0x3200); // Set alternating amplitude of VCOM
		Write_LCDReg(0x1E, 0x00BB); // Set VcomH voltage
		Write_LCDReg(0x03, 0x6A64); // Step-up factor/cycle setting
		//************ RAM position control **********/
		Write_LCDReg(0x0F, 0x0000); // Gate scan position start at G0.
		Write_LCDReg(0x44, 0xEF00); // Horizontal RAM address position
		Write_LCDReg(0x45, 0x0000); // Vertical RAM address start position
		Write_LCDReg(0x46, 0x013F); // Vertical RAM address end position
		// ----------- Adjust the Gamma Curve ----------//
		Write_LCDReg(0x30, 0x0000);
		Write_LCDReg(0x31, 0x0706);
		Write_LCDReg(0x32, 0x0206);
		Write_LCDReg(0x33, 0x0300);
		Write_LCDReg(0x34, 0x0002);
		Write_LCDReg(0x35, 0x0000);
		Write_LCDReg(0x36, 0x0707);
		Write_LCDReg(0x37, 0x0200);
		Write_LCDReg(0x3A, 0x0908);
		Write_LCDReg(0x3B, 0x0F0D);
		//************* Special command **************/
		Write_LCDReg(0x28, 0x0006); // Enable test command
		Write_LCDReg(0x2F, 0x12EB); // RAM speed tuning
		Write_LCDReg(0x26, 0x7000); // Internal Bandgap strength
		Write_LCDReg(0x20, 0xB0E3); // Internal Vcom strength
		Write_LCDReg(0x27, 0x0044); // Internal Vcomh/VcomL timing
		Write_LCDReg(0x2E, 0x7E45); // VCOM charge sharing time  
		//************* Turn On display ******************/
		Write_LCDReg(0x10, 0x0000); // Sleep mode off.
		Write_LCDReg(0x11, 0x6878); //262K色彩  Entry mode setup. 262K type B, take care on the data bus with 16it 
		Write_LCDReg(0x07, 0x0033); // Display ON	*/
	}
	/*设置LCD显示模式，也就是设置VM 等*/
	LCD_SetMode(0,1);

	/*将屏幕清除成红色*/
	LCD_Clear(0xf900);
}

/*清屏函数：将屏幕清除成指定颜色*/
void LCD_Clear(uint16_t Color)
{
	uint32_t i;
	/*count 用来保存需要向屏幕写点的次数，也就是保存整个屏幕共有多少点*/
	uint32_t count;

	count = LCD_YSIZE*LCD_XSIZE;
	Set_LCDCorsor(0,0);								//设置坐标起始地址
	LCD_REG = 0x22;									//准备写RAM
	for(i=0;i<count;i++)				//240*320=76800像素
	{
		LCD_RAM = Color;							//向数据寄存器写数据
	}					 
}

/*显示字符函数*/
void ssd1298_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          LCD_L0_SetPixelIndex(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          LCD_L0_SetPixelIndex(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}

/*设置LCD显示模式*/
void LCD_SetMode(unsigned char i,unsigned char j)
{
	if(i==0)
		Write_LCDReg(0x11, 0x6840);
	
	else if(i==1)
		Write_LCDReg(0x11, 0x6850);
	
	else if(i==2)
		Write_LCDReg(0x11, 0x6860);
	
	else if(i==3)
		Write_LCDReg(0x11, 0x6870);
	
	else if(i==4)
		Write_LCDReg(0x11, 0x6848); 
	
	else if(i==5)
		Write_LCDReg(0x11, 0x6858); 
	
	else if(i==6)
		Write_LCDReg(0x11, 0x6868); 
	
	else if(i==7)
		Write_LCDReg(0x11, 0x6878);

	if(j==0)
		Write_LCDReg(0x01, 0x3B3F);
	if(j==1)
		Write_LCDReg(0x01, 0x7B3F);
}

