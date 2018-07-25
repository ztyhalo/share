/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "sdcard.h"
#include "string.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
extern SD_CardInfo SDCardInfo;
#define BlockSize            512 /* Block Size in Bytes [�����block�����ǡ���������SD����ÿ������Ϊ512�ֽ�]*/
#define BufferWordsSize      (BlockSize >> 2)	//128������ο�MAIN�еĽ��ͣ���128��32λ�Ļ����������512���ֽ�

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive [�ú�������Ҫ��д��]                                                   */
/* ע�⣺�Ӻ����Ĵ�������������ν��drv�������������һ�������������ļ�Ӳ�������ͣ�����switch��俴
   drv = ATA = 0����ʾ��IDEӲ�̵�һ�ֽӿ�
   drv = MMC = 1: ��ʾ����MMC���Ľӿڣ�Ҳ����SD��
   drv = USB = 2: ��ʾ����USB�洢�豸�Ľӿ�
   �����￴��������Ӧ��Ҫѡ�����MMC�ӿ�,�����������Ϊ1*/
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
  SD_Error Status;
  Status = SD_Init();
  if (Status == SD_OK)
  {
    /*----------------- Read CSD/CID MSD registers ------------------*/
    Status = SD_GetCardInfo(&SDCardInfo);
  }

  if (Status == SD_OK)
  {
    /*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  }

  if (Status == SD_OK)
  {
    Status = SD_EnableWideBusOperation(SDIO_BusWide_1b);
  }

  /* Set Device Transfer Mode to DMA [���ô���ģʽΪDMA��ʽ]*/
  if (Status == SD_OK)
  {  
    Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);
  }
  if(Status == SD_OK)return 0;
  else return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status  [�ú�����Ҫ��д]                                                  */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s) [�ú�����Ҫ��д]                                       */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
  //memset(buff2, 0, sizeof(buff2));
	if(count==1)
        {
          SD_ReadBlock(sector << 9 ,(u32 *)(&buff[0]),BlockSize);
          //memcpy(buff,buff2,SECTOR_SIZE);
	}
	else
        {
          SD_ReadMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),BlockSize,count);
          //memcpy(buff,buff2,SECTOR_SIZE * count);
	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s) [�ú�����Ҫ��д]                                                      */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
  //memset(buff2, 0, sizeof(buff2));
	if(count==1)
        {
          //memcpy(buff2,buff,SECTOR_SIZE);
          SD_WriteBlock(sector << 9 ,(u32 *)(&buff[0]),BlockSize);
	}
	else
        {
          //memcpy(buff2,buff,SECTOR_SIZE * count);
          SD_WriteMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),BlockSize,count);
	}
        
  return RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions [�ú�����Ҫ��д]                                              */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

