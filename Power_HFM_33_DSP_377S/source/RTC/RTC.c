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
    sCurrent_Time.bHour  = ((u16RTCData[RTC_HOUR]   >> 4)&0x0003) * 10 + (u16RTCData[RTC_HOUR]   & 0x000F);//小时
    sCurrent_Time.bDate  = ((u16RTCData[RTC_DATE]   >> 4)&0x0003) * 10 + (u16RTCData[RTC_DATE]   & 0x000F);//天
    sCurrent_Time.bMonth = ((u16RTCData[RTC_MONTH]  >> 4)&0x0001) * 10 + (u16RTCData[RTC_MONTH]  & 0x000F);//月
    sCurrent_Time.wYear  = ((u16RTCData[RTC_YEAR]   >> 4)&0x000F) * 10 + (u16RTCData[RTC_YEAR]   & 0x000F) + 2000;//年
}
//1.	首先，定义了一个静态变量u16Address，并将其初始化为0。这个变量用于指示当前要读取的RTC寄存器的地址。
//2.	然后，进入一个循环，循环条件是u16Address小于等于7。这个循环用于依次读取RTC芯片的8个寄存器中的数据。
//3.	在循环中，调用了一个名为I2C_Read的函数，该函数用于从RTC芯片中读取数据，并将读取的数据存储在u16RTCData数组的对应位置上。
//4.	循环结束后，根据读取到的数据计算出当前的秒、分、小时、日期、月份和年份，并将它们存储在sCurrent_Time结构体变量的相应成员中。
//5.    具体来说，通过位运算和逻辑运算，从每个寄存器中的数据中提取出对应的时间信息。
//6.    例如，对于秒钟寄存器，先将高4位右移4位并与0x0007进行按位与运算，得到秒钟的十位数；
// 然后将低4位与0x000F进行按位与运算，得到秒钟的个位数。最后，将十位数和个位数相加，并乘以10，得到最终的秒钟值。
//总结起来，这段程序的原理就是通过读取RTC芯片中的数据，并根据读取到的数据计算出当前的时间。
//在这段代码中，位运算用于从RTC芯片的寄存器数据中提取出时间信息。

//让我们以秒钟寄存器为例来解释位运算的工作原理：
// sCurrent_Time.bSec = ((u16RTCData[RTC_SECOND] >> 4) & 0x0007) * 10 + (u16RTCData[RTC_SECOND] & 0x000F);
// 首先，我们需要从秒钟寄存器中获取秒钟的十位数和个位数。通过右移运算符（ >> ），
// 我们将u16RTCData[RTC_SECOND]中的数据向右移动4位，这样高4位的数据就会移动到低4位的位置上。
// 然后，通过按位与运算符（& ）与0x0007进行按位与运算，我们可以将低4位之外的其他位清零，只保留低4位的数据。
// 接下来，我们将得到的十位数乘以10，再加上个位数，就可以得到最终的秒钟值。

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Set_RTC_Time(void)
{
    u8RTCWriteData = ((unsigned char)(sTemp_Time.wYear  / 10) << 4) + (unsigned char)(sTemp_Time.wYear  % 10);
    I2C_Write(RTC_YEAR, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bMonth / 10) << 4) + (unsigned char)(sTemp_Time.bMonth % 10);
    I2C_Write(RTC_MONTH, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bDate  / 10) << 4) + (unsigned char)(sTemp_Time.bDate  % 10);
    I2C_Write(RTC_DATE, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bHour  / 10) << 4) + (unsigned char)(sTemp_Time.bHour  % 10);
    I2C_Write(RTC_HOUR, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bMin   / 10) << 4) + (unsigned char)(sTemp_Time.bMin   % 10);
    I2C_Write(RTC_MINUTE, u8RTCWriteData);

    u8RTCWriteData = ((unsigned char)(sTemp_Time.bSec   / 10) << 4) + (unsigned char)(sTemp_Time.bSec   % 10);
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