/*****************************************************************************

PROJECT  : N_PRO_5~10KVA

COMPILER : Code Composer Studio V7.0.0.00042

MODULE   : RTC.h

VERSION  : V0.0

DATE     : 2011/04/06

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef RTC_H
#define RTC_H

#include "F28x_Project.h"   // Device Headerfile and Examples Include File


/******************************************************************************
 RTC ADDRESSRTC 地址定义
******************************************************************************/
#define RTC_SECOND01            (0x00)    // Seconds      00-99   0.1/0.01
#define RTC_SECOND              (0x01)    // Seconds      00-59
#define RTC_MINUTE              (0x02)    // Minutes      00-59
#define RTC_HOUR                (0x03)    // Century/Hour 00-23
#define RTC_DAY                 (0x04)    // Day          01-07
#define RTC_DATE                (0x05)    // Date         01-31
#define RTC_MONTH               (0x06)    // Month        01-12
#define RTC_YEAR                (0x07)    // Year         00-99

#define RTC_RAM                 (0x14)    // User RAM     0x14--0x3F

// BACKUP LCM DATA INTO  RTC RAM备份 LCM 数据到 RTC RAM
#define RTC_Password            (0x15)    // LCM PASSWORD
#define RTC_LcmFlag             (0x16)    // LCM flag

#define RTC_LcmUpsFlag          (0x17)    // LCM UPS     flag
#define RTC_LcmBatFlag          (0x18)    // LCM BATTERY flag

#define RTC_LcmUpsCount         (0x19)    // LCM UPS     COUNT
#define RTC_LcmBatCount         (0x1A)    // LCM BATTERY COUNT

#define RTC_LcmUpsTimer1        (0x1B)    // LCM UPS TIMER, low  word low  byte
#define RTC_LcmUpsTimer2        (0x1C)    // LCM UPS TIMER, low  word high byte
#define RTC_LcmUpsTimer3        (0x1D)    // LCM UPS TIMER, high word low  byte
#define RTC_LcmUpsTimer4        (0x1E)    // LCM UPS TIMER, high word high byte

#define RTC_LcmBatTimer1        (0x1F)    // LCM BATTERY TIMER, low  word low  byte
#define RTC_LcmBatTimer2        (0x20)    // LCM BATTERY TIMER, low  word high byte
#define RTC_LcmBatTimer3        (0x21)    // LCM BATTERY TIMER, high word low  byte
#define RTC_LcmBatTimer4        (0x22)    // LCM BATTERY TIMER, high word high byte

#define RTC_LcmUpsIntervalCnt1  (0x23)    // LCM Alarm 1 Interval Count, low  word low  byte
#define RTC_LcmUpsIntervalCnt2  (0x24)    // LCM Alarm 1 Interval Count, low  word high byte
#define RTC_LcmUpsIntervalCnt3  (0x25)    // LCM Alarm 1 Interval Count, high word low  byte
#define RTC_LcmUpsIntervalCnt4  (0x26)    // LCM Alarm 1 Interval Count, high word high byte

#define RTC_LcmBatIntervalCnt1  (0x27)    // LCM Alarm 2 Interval Count, low  word low  byte
#define RTC_LcmBatIntervalCnt2  (0x28)    // LCM Alarm 2 Interval Count, low  word high byte
#define RTC_LcmBatIntervalCnt3  (0x29)    // LCM Alarm 2 Interval Count, high word low  byte
#define RTC_LcmBatIntervalCnt4  (0x2A)    // LCM Alarm 2 Interval Count, high word high byte

#define BACK_RTC_YEAR           (0x2B)    // Backup RTC YEAR
#define BACK_RTC_MONTH          (0x2C)    // Backup RTC MONTH
#define BACK_RTC_DATE           (0x2D)    // Backup RTC DATE
#define BACK_RTC_HOUR           (0x2E)    // Backup RTC HOUR
#define BACK_RTC_MINUTE         (0x2F)    // Backup RTC MINUTE
#define BACK_RTC_SECOND         (0x30)    // Backup RTC SECOND


//--------------------RTC 标志和时间结构
extern  union   UN_RTCFlag
{
	Uint16  u16All;
    struct  ST_RTCFlag
    {
    	unsigned RTCExist	:1;     // RTC exist
		unsigned HourIsPM	:1;     // 0:AM 1:PM
    }sBit;
}RTCFlag;	// RTC Flag

typedef struct
{
	Uint16  wYear;
	Uint8 bMonth;
	Uint8 bDate;
	Uint8 bHour;
	Uint8 bMin;
	Uint8 bSec;
}RTC_Sys;

extern RTC_Sys sCurrent_Time,sTemp_Time,sEvent_Time;//外部变量存储当前时间、临时时间和事件时间。

extern Uint8 u8RTCWriteData, u8RTCReadData;

extern Uint16  u16RTCData[16];            // RTC Data
//-----------------------------------------外部函数

extern void Get_RTC_Time(void);

extern void Set_RTC_Time(void);

extern void SetSystemRTC(Uint16 u16SetID, Uint16 u16Data);
#endif
