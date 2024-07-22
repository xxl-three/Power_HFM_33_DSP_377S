
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
//��������������
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
//ADC��������
//-----------------------------------------------------------------------------
#define ROW_NUM    6
#define COL_NUM    400
extern struct _sWave_Data_In_Ram
{
    //��Ϣ����ռ��10��word����
    Uint16 u16IndexCnt;
    Uint16 u16RowNum;           //���ε�����
    Uint16 u16ColNum;           //ÿ�����εĲ���������
    Uint16 u16TransferPoint;    //���ϲ���ʱ�̵������ݶ����е�λ��
    Uint16 u16StopPoint;        //       
    Uint16 u16KeepWave;         //0x55AA:Stop recording waveform
    Uint16 u16YearMonth;        //High byte: Year-2000; Low Byte: Month  1~12  ��¼���ϲ��η�����ʱ��
    Uint16 u16DateHour;         //High byte: Data 1~31; Low Byte: Hour   0~23
    Uint16 u16MinuteSecond;     //High byte: Data 1~31; Low Byte: Second 0~59
    Uint16 u16Reserve;

    //����������Address+10��ʼ
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

