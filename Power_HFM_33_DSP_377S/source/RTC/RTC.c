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

RTC_Sys	sCurrent_Time, sTemp_Time, sEvent_Time;//�洢��ǰʱ�䡢��ʱʱ����¼�ʱ�䡣

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

    sCurrent_Time.bSec   = ((u16RTCData[RTC_SECOND] >> 4)&0x0007) * 10 + (u16RTCData[RTC_SECOND] & 0x000F);//��
    sCurrent_Time.bMin   = ((u16RTCData[RTC_MINUTE] >> 4)&0x0007) * 10 + (u16RTCData[RTC_MINUTE] & 0x000F);//��
    sCurrent_Time.bHour  = ((u16RTCData[RTC_HOUR]   >> 4)&0x0003) * 10 + (u16RTCData[RTC_HOUR]   & 0x000F);//Сʱ
    sCurrent_Time.bDate  = ((u16RTCData[RTC_DATE]   >> 4)&0x0003) * 10 + (u16RTCData[RTC_DATE]   & 0x000F);//��
    sCurrent_Time.bMonth = ((u16RTCData[RTC_MONTH]  >> 4)&0x0001) * 10 + (u16RTCData[RTC_MONTH]  & 0x000F);//��
    sCurrent_Time.wYear  = ((u16RTCData[RTC_YEAR]   >> 4)&0x000F) * 10 + (u16RTCData[RTC_YEAR]   & 0x000F) + 2000;//��
}
//1.	���ȣ�������һ����̬����u16Address���������ʼ��Ϊ0�������������ָʾ��ǰҪ��ȡ��RTC�Ĵ����ĵ�ַ��
//2.	Ȼ�󣬽���һ��ѭ����ѭ��������u16AddressС�ڵ���7�����ѭ���������ζ�ȡRTCоƬ��8���Ĵ����е����ݡ�
//3.	��ѭ���У�������һ����ΪI2C_Read�ĺ������ú������ڴ�RTCоƬ�ж�ȡ���ݣ�������ȡ�����ݴ洢��u16RTCData����Ķ�Ӧλ���ϡ�
//4.	ѭ�������󣬸��ݶ�ȡ�������ݼ������ǰ���롢�֡�Сʱ�����ڡ��·ݺ���ݣ��������Ǵ洢��sCurrent_Time�ṹ���������Ӧ��Ա�С�
//5.    ������˵��ͨ��λ������߼����㣬��ÿ���Ĵ����е���������ȡ����Ӧ��ʱ����Ϣ��
//6.    ���磬�������ӼĴ������Ƚ���4λ����4λ����0x0007���а�λ�����㣬�õ����ӵ�ʮλ����
// Ȼ�󽫵�4λ��0x000F���а�λ�����㣬�õ����ӵĸ�λ������󣬽�ʮλ���͸�λ����ӣ�������10���õ����յ�����ֵ��
//�ܽ���������γ����ԭ�����ͨ����ȡRTCоƬ�е����ݣ������ݶ�ȡ�������ݼ������ǰ��ʱ�䡣
//����δ����У�λ�������ڴ�RTCоƬ�ļĴ�����������ȡ��ʱ����Ϣ��

//�����������ӼĴ���Ϊ��������λ����Ĺ���ԭ��
// sCurrent_Time.bSec = ((u16RTCData[RTC_SECOND] >> 4) & 0x0007) * 10 + (u16RTCData[RTC_SECOND] & 0x000F);
// ���ȣ�������Ҫ�����ӼĴ����л�ȡ���ӵ�ʮλ���͸�λ����ͨ������������� >> ����
// ���ǽ�u16RTCData[RTC_SECOND]�е����������ƶ�4λ��������4λ�����ݾͻ��ƶ�����4λ��λ���ϡ�
// Ȼ��ͨ����λ���������& ����0x0007���а�λ�����㣬���ǿ��Խ���4λ֮�������λ���㣬ֻ������4λ�����ݡ�
// �����������ǽ��õ���ʮλ������10���ټ��ϸ�λ�����Ϳ��Եõ����յ�����ֵ��

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