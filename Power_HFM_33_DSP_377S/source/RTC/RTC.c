/*****************************************************************************

PROJECT  : RT_5~10KVA

COMPILER : Code Composer Studio V7.0.0.00042

MODULE   : RTC.c

VERSION  : V0.0

DATE     : 2018/4/13

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/
#include "RTC.h"
#include "I2cDriver.h"

//-----------------------------------------------------------------------------
union   UN_RTCFlag RTCFlag;             // RTC Flag

RTC_Sys	sCurrent_Time, sTemp_Time, sEvent_Time;//存储当前时间、临时时间和事件时间。

Uint8 u8RTCWriteData, u8RTCReadData;

Uint16 u16RTCData[16];                    // RTC Data
/******************************************************************************
 READ DATA FROM RTC
******************************************************************************/


//-----------------------------------------------------------------------------
void Get_RTC_Time(void)
{
    static Uint16 u16Address;

    u16Address = 0;

    while (u16Address <= 7)
    {
        I2C_Read(u16Address,&u16RTCData[u16Address]);

        u16Address++;
    }

    sCurrent_Time.bSec   = ((u16RTCData[RTC_SECOND] >> 4)&0x0007) * 10 + (u16RTCData[RTC_SECOND] & 0x000F);//秒
    sCurrent_Time.bMin   = ((u16RTCData[RTC_MINUTE] >> 4)&0x0007) * 10 + (u16RTCData[RTC_MINUTE] & 0x000F);//分
    sCurrent_Time.bHour  = ((u16RTCData[RTC_HOUR] >> 4)&0x0003) * 10 + (u16RTCData[RTC_HOUR] & 0x000F);//小时
    sCurrent_Time.bDate  = ((u16RTCData[RTC_DATE] >> 4)&0x0003) * 10 + (u16RTCData[RTC_DATE] & 0x000F);//天
    sCurrent_Time.bMonth = ((u16RTCData[RTC_MONTH] >> 4)&0x0001) * 10 + (u16RTCData[RTC_MONTH] & 0x000F);//月
    sCurrent_Time.wYear  = ((u16RTCData[RTC_YEAR] >> 4)&0x000F) * 10 + (u16RTCData[RTC_YEAR] & 0x000F) + 2000;//年
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Set_RTC_Time(void)
{
    u8RTCWriteData = ((unsigned char)(sTemp_Time.wYear / 10) << 4) + (unsigned char)(sTemp_Time.wYear % 10);
    I2C_Write(RTC_YEAR, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bMonth / 10) << 4) + (unsigned char)(sTemp_Time.bMonth % 10);
    I2C_Write(RTC_MONTH, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bDate / 10) << 4) + (unsigned char)(sTemp_Time.bDate % 10);
    I2C_Write(RTC_DATE, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bHour / 10) << 4) + (unsigned char)(sTemp_Time.bHour % 10);
    I2C_Write(RTC_HOUR, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bMin / 10) << 4) + (unsigned char)(sTemp_Time.bMin % 10);
    I2C_Write(RTC_MINUTE, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bSec / 10) << 4) + (unsigned char)(sTemp_Time.bSec % 10);
    I2C_Write(RTC_SECOND, u8RTCWriteData);

}

void SetSystemRTC(Uint16 u16SetID, Uint16 u16Data)
{
    switch(u16SetID)
    {
        case RTC_YEAR:
            sTemp_Time.wYear = u16Data;
            I2C_Write(RTC_YEAR, sTemp_Time.wYear);
            break;
        case RTC_MONTH:
            sTemp_Time.bMonth = u16Data;
            I2C_Write(RTC_MONTH, sTemp_Time.bMonth);
            break;
        case RTC_DATE:
            sTemp_Time.bDate = u16Data;
            I2C_Write(RTC_DATE, sTemp_Time.bDate);
            break;
        case RTC_HOUR:
            sTemp_Time.bHour = u16Data;
            I2C_Write(RTC_HOUR, sTemp_Time.bHour);
            break;
        case RTC_MINUTE:
            sTemp_Time.bMin = u16Data;
            I2C_Write(RTC_MINUTE, sTemp_Time.bMin);
            break;
        case RTC_SECOND:
            sTemp_Time.bSec = u16Data;
            I2C_Write(RTC_SECOND, sTemp_Time.bSec);
            break;
        default:
            break;
    }
}







