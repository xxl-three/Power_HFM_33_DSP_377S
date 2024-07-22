/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : SplitPhase_16KVA

COMPILER : Code Composer Studio 9.2.0.00013

MODULE   : WaveLog.c

VERSION  : V0.0

DATE     : 2019/12/16

AUTHOR   : MZER.ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/
#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "WaveLog.h"
#include "USR_VAR.H"
#include "Measurement.h"
#include "INT_ISR.H"

//----------------------------------------------------------------------------
//控制器参数波形
//----------------------------------------------------------------------------
#pragma DATA_SECTION(sDebug_Data_In_Ram, "DEBUG_RAM");
struct _sDebug_Data_In_Ram sDebug_Data_In_Ram;


//-----------------------------------------------------------------------------
//ADC采样波形
//-----------------------------------------------------------------------------
#pragma DATA_SECTION(sWave_Data_In_Ram, "WAVE_RAM");
struct _sWave_Data_In_Ram sWave_Data_In_Ram;

//----------------------------------------------------------------------------
//Timebase 100us
//Function:连续循环记录400个点的ADC采样波形(循环记录)
//         UPS shutdown时，记忆shutdown的位置于u16TransferPoint中      
//         并设置记录停止的位置u16StopPoint=(u16TransferPoint+200)%400
//         即再连续记录200个点后，停止记录波形
//----------------------------------------------------------------------------
#pragma CODE_SECTION(IsrWaveRecord, "ramfuncs");
void IsrWaveRecord(void)
{
    if(sWave_Data_In_Ram.u16KeepWave != 0x55AA)
    {
        sWave_Data_In_Ram.u16IndexCnt++;
        if(sWave_Data_In_Ram.u16IndexCnt >= COL_NUM)
            sWave_Data_In_Ram.u16IndexCnt = 0;
        //sWave_Data_In_Ram.i16WaveData[0][sWave_Data_In_Ram.u16IndexCnt] = sADC_Measure.i16InvVolt_R;
        //sWave_Data_In_Ram.i16WaveData[1][sWave_Data_In_Ram.u16IndexCnt] = sADC_Measure.i16InvVolt_S;
        //sWave_Data_In_Ram.i16WaveData[2][sWave_Data_In_Ram.u16IndexCnt] = sADC_Measure.i16InvVolt_T;
        //sWave_Data_In_Ram.i16WaveData[3][sWave_Data_In_Ram.u16IndexCnt] = sADC_Measure.i16InvCurr_R;
        
        sWave_Data_In_Ram.i16WaveData[0][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutyR.sWord.i16HiWord;
        sWave_Data_In_Ram.i16WaveData[1][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutyS.sWord.i16HiWord;
        sWave_Data_In_Ram.i16WaveData[2][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutyT.sWord.i16HiWord;
        sWave_Data_In_Ram.i16WaveData[3][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutyRn.sWord.i16HiWord;
        sWave_Data_In_Ram.i16WaveData[4][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutySn.sWord.i16HiWord;
        sWave_Data_In_Ram.i16WaveData[5][sWave_Data_In_Ram.u16IndexCnt] = si32InvPwmDutyTn.sWord.i16HiWord;
        
        if(sWave_Data_In_Ram.u16IndexCnt == sWave_Data_In_Ram.u16StopPoint)
            sWave_Data_In_Ram.u16KeepWave = 0x55AA;
    }
}


//----------------------------------------------------------------------------
//called by ShutdownUPS function
//----------------------------------------------------------------------------
void SetTransferPoint(void)
{
    sWave_Data_In_Ram.u16RowNum = ROW_NUM;
    sWave_Data_In_Ram.u16ColNum = COL_NUM;
    
    sWave_Data_In_Ram.u16TransferPoint = sWave_Data_In_Ram.u16IndexCnt;
    
    sWave_Data_In_Ram.u16StopPoint = sWave_Data_In_Ram.u16TransferPoint+200;
    if(sWave_Data_In_Ram.u16StopPoint >= COL_NUM)
        sWave_Data_In_Ram.u16StopPoint -= COL_NUM;
}

//----------------------------------------------------------------------------
//called by UpsBoot function
//----------------------------------------------------------------------------
void ClearTransferPoint(void)
{
    sWave_Data_In_Ram.u16TransferPoint = 0xFFFF;
    sWave_Data_In_Ram.u16StopPoint = 0xFFFF;
    sWave_Data_In_Ram.u16KeepWave = 0;
}