
/*****************************************************************************

PROJECT  : SplitPhase_16KVA

COMPILER : Code Composer Studio V8.2.0.00007

MODULE   : WaveLog.h

VERSION  : V0.0

DATE     : 2018/10/05

AUTHOR   : MZER.ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef	WAVELOG_H
#define WAVELOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "F28x_Project.h"   // Device Headerfile and Examples Include File


//----------------------------------------------------------------------------
//控制器参数波形
//----------------------------------------------------------------------------
extern struct _sDebug_Data_In_Ram
{
    int16	wTestData;
    int16 	wDataCnt;
    int16	wDataDiff;
    int16	wLastData;

    int16   i16Data[29][400];

} sDebug_Data_In_Ram;


//-----------------------------------------------------------------------------
//ADC采样波形
//-----------------------------------------------------------------------------
#define ROW_NUM    6
#define COL_NUM    400
extern struct _sWave_Data_In_Ram
{
    //信息区：占据10个word长度
    Uint16 u16IndexCnt;
    Uint16 u16RowNum;           //波形的数量
    Uint16 u16ColNum;           //每个波形的采样点数量
    Uint16 u16TransferPoint;    //故障产生时刻点在数据队列中的位置
    Uint16 u16StopPoint;        //       
    Uint16 u16KeepWave;         //0x55AA:Stop recording waveform
    Uint16 u16YearMonth;        //High byte: Year-2000; Low Byte: Month  1~12  记录故障波形发生的时间
    Uint16 u16DateHour;         //High byte: Data 1~31; Low Byte: Hour   0~23
    Uint16 u16MinuteSecond;     //High byte: Data 1~31; Low Byte: Second 0~59
    Uint16 u16Reserve;

    //数据区：从Address+10开始
    int16   i16WaveData[ROW_NUM][COL_NUM];  //

} sWave_Data_In_Ram;


//----------------------------------------------------------------------------
// Golbal functions
//----------------------------------------------------------------------------

extern void IsrWaveRecord(void);
extern void SetTransferPoint(void);
extern void ClearTransferPoint(void);



#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

