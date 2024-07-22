/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Source.c

VERSION  : V0.0

DATE     : 2018/5/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#include <math.h>
#include "USR_VAR.H"
#include "SOURCE.H"
#include "EEPROM.h"
#include "Buzzer.h"
#include "Measurement.h"

/*****************************************************************************
* System Timebase subroutine
*****************************************************************************/
void TimeBaseSystem(void)
{
    static Uint16 tBuff;

    //WDG_CLK_TRG;

    if(tBuff != u16Mini1ms)
    {
        tBuff = u16Mini1ms;
        Isr_Check_MBus_TimeOut(0);
        Isr_Check_MBus_TimeOut(1);
        Isr_Check_MBus_TimeOut(2);

        Check_Repair_Mode();
    }

    // 10 ms block for Function routine time not for Timebase
    if (sTimesFlag.sBit.u1TimeBase10ms)
    {
        sTimesFlag.sBit.u1TimeBase10ms = 0;

        EepromTask();
        Check_Inverter_FastFail();
    }

    if (u16Mini10ms >= 10)                   // 100ms ?
    {
        u16Mini10ms -= 10;

        TimeBase100ms();                // Base unit 100ms

        //CHECK_FREQUENCY();              // check Frequency

        u16Mini100ms++;
        if (u16Mini100ms >= 10)             // 1S ?
        {
            u16Mini100ms -= 10;

            TimeBase1Sec();              // Base unit 1 sec

            if(u16LcdBacklightOnTime)
            {
                u16LcdBacklightOnTime--;
            }

            u16Second++;
            if (u16Second >= 60)               // 60S ?
            {
                u16Second -= 60;

                // 1 min
                TimeBase1Min();             // Base unit 1 minute

                u16Minute++;
                if (u16Minute >= 60)           // 60min ?
                {
                    u16Minute -= 60;

                    // 1 hour
                    u16Hour++;
                    if (u16Hour >= 24)         // 24 hour ?
                    {
                        u16Hour -= 24;
                    }
                }
            }
        }
    }
}





//------------------------------------------------------------------------------
// 输出频率调整
//------------------------------------------------------------------------------
void Adjust_Frequency(void)
{
    if(sSet.u16OldFrequency == sSet.u16OutputFrequency)
    {
        return;
    }
    
    if(sSet.u16OutputFrequency < LOW_OP_FREQ_LIMIT)
    {
        sSet.u16OutputFrequency = LOW_OP_FREQ_LIMIT;
    }
    if(sSet.u16OutputFrequency > HIGH_OP_FREQ_LIMIT)
    {
        sSet.u16OutputFrequency = HIGH_OP_FREQ_LIMIT;
    }
    sSet.u16OldFrequency = sSet.u16OutputFrequency;

    //                          sSet.u32OpPeriod         u32TimeBase      u32TimeBaseRem           u32TimeBase    u32TimeBaseRem
    //2000000000/5000=400000    =>0x00061A80        /180 2222	(  8AE)   40	            /360   1111	( 457)    40	  
    //2000000000/4000=500000    =>0x0007A120        /180 2777	(  AD9)   140	            /360   1388	( 56C)    320	  
    //2000000000/2000=1000000   =>0x000F4240        /180 5555	( 15B3)   100	            /360   2777	( AD9)    280	
    //2000000000/1000=2000000   =>0x001E8480        /180 11111	( 2B67)   20	            /360   5555	(15B3)    200	 
    //2000000000/500 =4000000   =>0x003D0900        /180 22222	( 56CE)   40	            /360   11111(2B67)	  40	 
    //2000000000/100 =20000000  =>0x01312D00        /180 111111	(1B207)   20	            /360   55555(D903)	  200	 
    
    #ifdef HIGH_FREQ_OUTPUT
    sSet.u32OpPeriod = (Uint32)(((Uint64)(2000000000)) / (sSet.u16OutputFrequency)); 
    #else
    sSet.u32OpPeriod = (Uint32)(((Uint64)(20000000000)) / (sSet.u16OutputFrequency));  //
    #endif
    
    //使用ECap3Regs的APWM模式来产生相位角
    ECap3Regs.CAP3 = sSet.u32OpPeriod;		// shadow registers：period     
    ECap3Regs.CAP4 = sSet.u32OpPeriod>>1;   // shadow registers：compare    
}


/******************************************************************************
 DC BUS VOLTAGE
******************************************************************************/
#define DC_BUS_HIGH         (650)   //265*1.732*1.414
#define DC_BUS_HIGH_RC      (600)
#define DC_BUS_LOW_RC       (450)
#define DC_BUS_LOW          (400)   //160*1.732*1.414
void Check_DCBUS_VOL(void)
{
    Uint16 i_uint;
    static Uint16 u16TimeBuff;
    static Uint16 u16DelayCnt;
    static Uint16 u16High, u16HighRec, u16Low, u16LowRec;

    if(SourceControlFlag.sBit.RepairOn)
    {
        BUS_FLAG.sBit.PLS_HI = 0;
        BUS_FLAG.sBit.PLS_LO = 0;
        BUS_FLAG.sBit.MUS_HI = 0;
        BUS_FLAG.sBit.MUS_LO = 0;
        FAULT1_FLAG.sBit.DCBUS_FAIL = 0;
        return;
    }

    if (u16BusDelayTime > 0)                  // Delay time out ?
    {
        return;                           // skip test
    }


    u16High    = DC_BUS_HIGH   ;
    u16HighRec = DC_BUS_HIGH_RC;
    u16LowRec  = DC_BUS_LOW_RC ;
    u16Low     = DC_BUS_LOW    ;


    // check DCBUS+
    if (sAdcCal.i16D2dBus_R >= u16High             // > DC BUS + high voltage ? 550V
        //|| sAdcCal.i16D2dBus_S >= u16High
        //|| sAdcCal.i16D2dBus_T >= u16High
       )
    {
        BUS_FLAG.sBit.PLS_HI = 1;       // set DC BUS + high fail falg
    }
    else if(sAdcCal.i16D2dBus_R < u16HighRec
            //&& sAdcCal.i16D2dBus_S < u16HighRec
            //&& sAdcCal.i16D2dBus_T < u16HighRec
           )
    {
        BUS_FLAG.sBit.PLS_HI = 0;
    }

    if (sAdcCal.i16D2dBus_R <= u16Low             // < DC BUS + low voltage ?
        //|| sAdcCal.i16D2dBus_S <= u16Low
        //|| sAdcCal.i16D2dBus_T <= u16Low
       )
    {
        BUS_FLAG.sBit.PLS_LO = 1;    // set DC BUS + low  fail falg
    }
    else if (sAdcCal.i16D2dBus_R > u16LowRec
             //&& sAdcCal.i16D2dBus_S > u16LowRec
             //&& sAdcCal.i16D2dBus_T > u16LowRec
            )
    {
        BUS_FLAG.sBit.PLS_LO = 0;
    }

    if (BUS_FLAG.sBit.PLS_HI ||
        BUS_FLAG.sBit.PLS_LO ||
        BUS_FLAG.sBit.MUS_HI ||
        BUS_FLAG.sBit.MUS_LO ||
        BUS_FLAG.sBit.u1BusOvp_R ||
        BUS_FLAG.sBit.u1BusOvp_S ||
        BUS_FLAG.sBit.u1BusOvp_T
       )
    {
        FAULT1_FLAG.sBit.DCBUS_FAIL = 1;

        SetOutputOff();

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }

}

//----------------------------------------------------------------------------
// ASCII 2 BYTE TO HEXDECIMAL BYTE
//
// IN : R0
// OUT: R0L
//----------------------------------------------------------------------------
Uint8 ASC2BYTE(Uint16 i_uint)
{
    Uint8 u16Data;

    u16Data = ((i_uint >> 8) - '0') * 10;
    u16Data += (i_uint & 0x00FF) - '0';

    return u16Data;
}


//----------------------------------------------------------------------------
// Product RUN TIME
// timebase 1sec
//----------------------------------------------------------------------------
void RunTime(void)
{
    if(SourceControlFlag.sBit.OutputOn)
    {
        //total run time
        sPdRunTime.sSecondNone.sByte.u8Second++;
        if (sPdRunTime.sSecondNone.sByte.u8Second >= 60)
        {
            sPdRunTime.sSecondNone.sByte.u8Second -= 60;
        
            sPdRunTime.sHourMinute.sByte.u8Minute++;
            if (sPdRunTime.sHourMinute.sByte.u8Minute >= 60)         // 60 min ?
            {
                sPdRunTime.sHourMinute.sByte.u8Minute -= 60;
        
                // 1 hour
                sPdRunTime.sHourMinute.sByte.u8Hour++;
        
                if (sPdRunTime.sHourMinute.sByte.u8Hour >= 24)         // 24 hour ?
                {
                    sPdRunTime.sHourMinute.sByte.u8Hour -= 24;
        
                    sPdRunTime.u16RunTimeDate++;
                }
            }
        }
        
        //current run time
        sNowRunTime.sSecondNone.sByte.u8Second++;
        if (sNowRunTime.sSecondNone.sByte.u8Second >= 60)
        {
            sNowRunTime.sSecondNone.sByte.u8Second -= 60;
        
            sNowRunTime.sHourMinute.sByte.u8Minute++;
            if (sNowRunTime.sHourMinute.sByte.u8Minute >= 60)         // 60 min ?
            {
                sNowRunTime.sHourMinute.sByte.u8Minute -= 60;
        
                // 1 hour
                sNowRunTime.sHourMinute.sByte.u8Hour++;
        
                if (sNowRunTime.sHourMinute.sByte.u8Hour >= 24)         // 24 hour ?
                {
                    sNowRunTime.sHourMinute.sByte.u8Hour -= 24;
        
                    sNowRunTime.u16RunTimeDate++;
                }
            }
        }
    }
}



void Check_VFSet_Mode_Time(void)
{
    if(SourceControlFlag.sBit.VSET || SourceControlFlag.sBit.FSET)
    {
        u16SetupModeCnt++;
        if (u16SetupModeCnt >= 60)                // >= 1 minutes
        {
            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;

            u16SetupModeCnt = 0;
        }
    }
    else
    {
        u16SetupModeCnt = 0;
    }
}

/******************************************************************************
 CHECK FREQUENCY
******************************************************************************/
void CHECK_FREQUENCY(void)
{
    if ((sRMS.u16AcFreq >= F_700HZ) ||         // > 70Hz ?
        (sRMS.u16AcFreq < F_400HZ))            // < 40Hz ?
    {
        AC_FREQ_CNT++;
        if (AC_FREQ_CNT >= SEC_1S)        // >= 1S ?
        {
            FREQ_FLAG.sBit.ACFreqOutRange = 1;    // AC Frequency out of range
        }
    }
    else if ((sRMS.u16AcFreq < F_695HZ) &&         // < 69.5Hz ?
             (sRMS.u16AcFreq >= F_405HZ))           // > 40.5Hz ?
    {
        AC_FREQ_CNT = 0;
        FREQ_FLAG.sBit.ACFreqOutRange = 0; // AC Frequency not out of range
    }
}


/******************************************************************************
 CLEAR ALL FLAG & COUNT
******************************************************************************/
void CLR_ALL_FLAG(void)
{
    FAULT1_FLAG.WORD = 0;


    BUS_FLAG.sBit.PLS_HI = 0;             // clear flag
    BUS_FLAG.sBit.PLS_LO = 0;
    BUS_FLAG.sBit.MUS_HI = 0;
    BUS_FLAG.sBit.MUS_LO = 0;


}

/******************************************************************************
 INITIAL VARIABLE & FLAG
******************************************************************************/
void INIT_VAR(void)
{
    AC_FLAG.sBit.AC_NORM = 1;
    AC_FLAG.sBit.AC_LOSS = 1;
    AC_FLAG.sBit.AC_LOW = 1;
    FREQ_FLAG.sBit.ACFreqOutRange = 1;

    sSet.u16OutputOnAngle = 0;
    sSet.u16OutputOnAngleDo = 0;

    u16LcdBacklightOnTime = SEC_5MIN;

}


void Program_Start_Data_init(void)
{
    sSet.wBeforePgSetOutputVoltage_R = sSet.u16OutputVoltage_R;
    sSet.wBeforePgSetOutputVoltage_S = sSet.u16OutputVoltage_S;
    sSet.wBeforePgSetOutputVoltage_T = sSet.u16OutputVoltage_T;
    sSet.wBeforePgSetOutputFrequency = sSet.u16OutputFrequency;

    if(SourceControlFlag.sBit.OutputOn && CHK_INVERTER_ENABLE)
    {
        sSet.wBeforePgStatus.sByte.u8LowByte = 1;   //记忆编程开始之前的状态
    }
    else
    {
        sSet.wBeforePgStatus.sByte.u8LowByte = 0;
    }

    sProgRun.u16CycleCnt = 1;
    sProgRun.u32TimeCnt = 0;
    sProgRun.u16Index = sProgramming.u16Start - 1;
    sProgRun.dwTimePeriod = ((Uint32)sProgramming.u16Step[sProgRun.u16Index].u16TimeHigh << 16) + (Uint32)sProgramming.u16Step[sProgRun.u16Index].u16TimeLow;

    //输出频率调整
    sSet.u16OutputFrequency = sProgramming.u16Step[sProgRun.u16Index].wFreq_Start;
    if(sProgramming.u16Step[sProgRun.u16Index].wFreq_Start > sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
    {
        sProgRun.wF_Delta = sProgramming.u16Step[sProgRun.u16Index].wFreq_Start - sProgramming.u16Step[sProgRun.u16Index].wFreq_End;
        sProgRun.bF_UpDown = 2;	//递减
    }
    else if(sProgramming.u16Step[sProgRun.u16Index].wFreq_Start < sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
    {
        sProgRun.wF_Delta = sProgramming.u16Step[sProgRun.u16Index].wFreq_End - sProgramming.u16Step[sProgRun.u16Index].wFreq_Start;
        sProgRun.bF_UpDown = 1;	//递增
    }
    else
    {
        sProgRun.wF_Delta = 0;
        sProgRun.bF_UpDown = 0;
    }

    if(sProgRun.bF_UpDown == 0)
    {
        sProgRun.dwF_StepTime = sProgRun.dwTimePeriod;
    }
    else if(sProgRun.dwTimePeriod >= sProgRun.wF_Step)	//如果时间比频率大，则固定频率跳变单位为0.01Hz，时间step以0.01Hz做等分
    {
        sProgRun.dwF_StepTime = sProgRun.dwTimePeriod / sProgRun.wF_Delta;
        sProgRun.wF_Step = 1;		//unit:0.01Hz
    }
    else							//如果时间比频率小，则时间单位固定为1ms，频率step以1ms做等分
    {
        sProgRun.dwF_StepTime = 1;
        sProgRun.wF_Step = (unsigned long)sProgRun.wF_Step / sProgRun.dwTimePeriod;
    }
    sProgRun.dwF_TotalStepTime = sProgRun.dwF_StepTime;

    //R相电压参考值调整
    sSet.u16OutputVoltage_R = sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt;
    if(sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt)
    {
        sProgRun.wR_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt;
        sProgRun.bR_UpDown = 2;	//递减
    }
    else if(sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt)
    {
        sProgRun.wR_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt;
        sProgRun.bR_UpDown = 1;	//递增
    }
    else
    {
        sProgRun.wR_DeltaVolt = 0;
        sProgRun.bR_UpDown = 0;
    }

    if(sProgRun.wR_DeltaVolt <= 10)
    {
        sProgRun.dwR_StepTime = sProgRun.dwTimePeriod;
        sProgRun.wR_StepVolt = sProgRun.wR_DeltaVolt;
    }
    else if(sProgRun.dwTimePeriod >= sProgRun.wR_DeltaVolt)
    {
        sProgRun.dwR_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wR_DeltaVolt;
        sProgRun.wR_StepVolt = 10;	//0.1V
    }
    else
    {
        sProgRun.dwR_StepTime = 10;
        sProgRun.wR_StepVolt = (unsigned long)sProgRun.wR_DeltaVolt * 10 / sProgRun.dwTimePeriod;
    }
    sProgRun.dwR_TotalStepTime = sProgRun.dwR_StepTime;

    //S相电压参考值调整
    sSet.u16OutputVoltage_S = sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt;
    if(sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt)
    {
        sProgRun.wS_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt;
        sProgRun.bS_UpDown = 2;	//递减
    }
    else if(sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt)
    {
        sProgRun.wS_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt;
        sProgRun.bS_UpDown = 1;	//递增
    }
    else
    {
        sProgRun.wS_DeltaVolt = 0;
        sProgRun.bS_UpDown = 0;
    }

    if(sProgRun.wS_DeltaVolt <= 10)
    {
        sProgRun.dwS_StepTime = sProgRun.dwTimePeriod;
        sProgRun.wS_StepVolt = sProgRun.wS_DeltaVolt;
    }
    else if(sProgRun.dwTimePeriod >= sProgRun.wS_DeltaVolt)
    {
        sProgRun.dwS_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wS_DeltaVolt;
        sProgRun.wS_StepVolt = 10;	//0.1V
    }
    else
    {
        sProgRun.dwS_StepTime = 10;
        sProgRun.wS_StepVolt = (unsigned long)sProgRun.wS_DeltaVolt * 10 / sProgRun.dwTimePeriod;
    }
    sProgRun.dwS_TotalStepTime = sProgRun.dwS_StepTime;

    //T相电压参考值调整
    sSet.u16OutputVoltage_T = sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt;
    if(sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt)
    {
        sProgRun.wT_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt;
        sProgRun.bT_UpDown = 2;	//递减
    }
    else if(sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt)
    {
        sProgRun.wT_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt;
        sProgRun.bT_UpDown = 1;	//递增
    }
    else
    {
        sProgRun.wT_DeltaVolt = 0;
        sProgRun.bT_UpDown = 0;
    }

    if(sProgRun.wT_DeltaVolt <= 10)
    {
        sProgRun.dwT_StepTime = sProgRun.dwTimePeriod;
        sProgRun.wT_StepVolt = sProgRun.wT_DeltaVolt;
    }
    else if(sProgRun.dwTimePeriod >= sProgRun.wT_DeltaVolt)
    {
        sProgRun.dwT_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wT_DeltaVolt;
        sProgRun.wT_StepVolt = 10;	//0.1V
    }
    else
    {
        sProgRun.dwT_StepTime = 10;
        sProgRun.wT_StepVolt = (unsigned long)sProgRun.wT_DeltaVolt * 10 / sProgRun.dwTimePeriod;
    }
    sProgRun.dwT_TotalStepTime = sProgRun.dwT_StepTime;

    if(CONFIG_FLAG.sBit.ProgramSoftstartEnable && sSet.wBeforePgStatus.sByte.u8LowByte == 0)
    {
        DINT;
        i32Inv1RMSAmpVref_R = 0;
        i32Inv1RMSAmpVref_S = 0;
        i32Inv1RMSAmpVref_T = 0;
        i32Inv1AmpVref_R = 0;    //softstart
        i32Inv1AmpVref_S = 0;
        i32Inv1AmpVref_T = 0;
        EINT;
    }
    else
    {
        Adjust_Pwm_Gain(0x07);
    }

    if(sProgRun.bR_UpDown || sProgRun.bS_UpDown || sProgRun.bT_UpDown || sProgRun.bF_UpDown)
    {
        DryContactFlag.sBit.ProgVoltFreqUpDown = 1;
    }

    sProgRun.u16StartPoint = u16SineWaveIndex;
    sProgRun.u16StepChangeAdjustDelay = 2;
}


void Program_Stop(void)
{
    if(SourceControlFlag.sBit.Program_Running)
    {
        SourceControlFlag.sBit.Program_Running = 0;
        //LCD_FLAG.sBit.ShowProgramRunStatus = 0;
        sSet.u16OutputVoltage_R = 0;
        #ifdef HIGH_FREQ_OUTPUT
        sSet.u16OutputFrequency = NORMAL_OP_FREQ_LIMIT;
        #else
        sSet.u16OutputFrequency = NORMAL_OP_FREQ_LIMIT;
        #endif
    }
}

void Program_Start(void)
{
    u16ProgramErrorCode = Check_Programming_Data();
    if(!CHK_INVERTER_ENABLE_R &&
       u16ProgramErrorCode == 0 && //所有的输出电压设定没有超过设定的高低档位限制
       (CHK_INPUT_RELAY_ON || SourceControlFlag.sBit.RepairOn) &&
       FAULT1_FLAG.WORD == 0)
    {
        Program_Start_Data_init();
        Clear_Set_Flag();

        sProgRun.u16InitWaitCnt = 0;
        SourceControlFlag.sBit.Program_Init_Wait = 1;
    }
    else
    {
        FAULT1_FLAG.sBit.ProgramParaError = 1;
    }
}


void Program_Running(void)
{
    static unsigned int	wVoltDiff;
    static unsigned int	LastIndex, NextIndex;

    if(SourceControlFlag.sBit.Program_Init_Wait)
    {
        //if(sProgRun.u16InitWaitCnt >= 5)
        {
            SourceControlFlag.sBit.Program_Init_Wait = 0;

            SourceControlFlag.sBit.Program_Running = 1;
            //LCD_FLAG.sBit.ShowProgramRunStatus = 1;
            SourceControlFlag.sBit.OutputOn = 1;
        }
    }
    else if(SourceControlFlag.sBit.Program_Running)
    {
        if(sProgRun.u32TimeCnt >= sProgRun.dwTimePeriod)
        {
            sProgRun.u16Index++;
            if(sProgRun.u16Index > (sProgramming.u16End - 1))
            {
                sProgRun.u16Index = sProgramming.u16Start - 1;
                sProgRun.u16CycleCnt++;
                if(sProgRun.u16CycleCnt > sProgramming.u16Cycles)
                {
                    if(sSet.wBeforePgStatus.sByte.u8LowByte == 0)   //编程运行完成后，关闭输出
                    {
                        SetOutputOff();
                    }
                    Program_Stop();			//编程循环运行完成
                    if(sSet.wBeforePgStatus.sByte.u8LowByte == 1)	//恢复编程运行前的设定值继续运行
                    {
                        sSet.u16OutputVoltage_R = sSet.wBeforePgSetOutputVoltage_R;
                        sSet.u16OutputVoltage_S = sSet.wBeforePgSetOutputVoltage_S;
                        sSet.u16OutputVoltage_T = sSet.wBeforePgSetOutputVoltage_T;
                        sSet.u16OutputFrequency = sSet.wBeforePgSetOutputFrequency;
                        Adjust_Pwm_Gain(0x07);
                        sSet.wBeforePgStatus.sByte.u8LowByte = 0;
                    }
                    return;
                }

                LastIndex = sProgramming.u16End - 1;
                NextIndex = sProgRun.u16Index;
            }
            else
            {
                LastIndex = sProgRun.u16Index - 1;
                NextIndex = sProgRun.u16Index;
            }

            sProgRun.u32TimeCnt = 0;
            sProgRun.dwTimePeriod = ((Uint32)sProgramming.u16Step[sProgRun.u16Index].u16TimeHigh << 16) + (Uint32)sProgramming.u16Step[sProgRun.u16Index].u16TimeLow;

            //输出频率调整
            sSet.u16OutputFrequency = sProgramming.u16Step[sProgRun.u16Index].wFreq_Start;
            if(sProgramming.u16Step[sProgRun.u16Index].wFreq_Start > sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
            {
                sProgRun.wF_Delta = sProgramming.u16Step[sProgRun.u16Index].wFreq_Start - sProgramming.u16Step[sProgRun.u16Index].wFreq_End;
                sProgRun.bF_UpDown = 2;	//递减
            }
            else if(sProgramming.u16Step[sProgRun.u16Index].wFreq_Start < sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
            {
                sProgRun.wF_Delta = sProgramming.u16Step[sProgRun.u16Index].wFreq_End - sProgramming.u16Step[sProgRun.u16Index].wFreq_Start;
                sProgRun.bF_UpDown = 1;	//递增
            }
            else
            {
                sProgRun.wF_Delta = 0;
                sProgRun.bF_UpDown = 0;
            }

            if(sProgRun.bF_UpDown == 0)
            {
                sProgRun.dwF_StepTime = sProgRun.dwTimePeriod;
            }
            else if(sProgRun.dwTimePeriod >= sProgRun.wF_Step)	//如果时间比频率大，则固定频率跳变单位为0.01Hz，时间step以0.01Hz做等分
            {
                sProgRun.dwF_StepTime = sProgRun.dwTimePeriod / sProgRun.wF_Delta;
                sProgRun.wF_StepTimeDiv = ((sProgRun.dwTimePeriod * 10) / sProgRun.wF_Delta) % 10;
                sProgRun.wF_TotalTimeDiv = 0;
                sProgRun.wF_Step = 1;		//unit:0.01Hz
            }
            else							//如果时间比频率小，则时间单位固定为1ms，频率step以1ms做等分
            {
                sProgRun.dwF_StepTime = 1;
                sProgRun.wF_Step = (unsigned long)sProgRun.wF_Step / sProgRun.dwTimePeriod;
            }
            sProgRun.dwF_TotalStepTime = sProgRun.dwF_StepTime;

            //R相电压参考值调整
            sSet.u16OutputVoltage_R = sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt;
            if(sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt)
            {
                sProgRun.wR_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt;
                sProgRun.bR_UpDown = 2;	//递减
            }
            else if(sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt)
            {
                sProgRun.wR_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wR_StartVolt;
                sProgRun.bR_UpDown = 1;	//递增
            }
            else
            {
                sProgRun.wR_DeltaVolt = 0;
                sProgRun.bR_UpDown = 0;
            }

            if(sProgRun.wR_DeltaVolt <= 10)	//如果跳变小于等于1V，step之内不做连续变更，只变化一次
            {
                sProgRun.dwR_StepTime = sProgRun.dwTimePeriod;
                sProgRun.wR_StepVolt = sProgRun.wR_DeltaVolt;
            }
            else if(sProgRun.dwTimePeriod >= sProgRun.wR_DeltaVolt) //如果时间比电压大，则固定电压跳变单位为1V，时间step以1V电压做等分
            {
                sProgRun.dwR_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wR_DeltaVolt;
                sProgRun.wR_StepVolt = 10;	//unit:0.1V
            }
            else							//如果时间比电压小，则时间单位固定为10ms，电压step以10ms做等分
            {
                sProgRun.dwR_StepTime = 10;
                sProgRun.wR_StepVolt = (unsigned long)sProgRun.wR_DeltaVolt * 10 / sProgRun.dwTimePeriod;
            }
            sProgRun.dwR_TotalStepTime = sProgRun.dwR_StepTime;

            //S相电压参考值调整
            sSet.u16OutputVoltage_S = sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt;
            if(sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt)
            {
                sProgRun.wS_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt;
                sProgRun.bS_UpDown = 2;	//递减
            }
            else if(sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt)
            {
                sProgRun.wS_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wS_StartVolt;
                sProgRun.bS_UpDown = 1;	//递增
            }
            else
            {
                sProgRun.wS_DeltaVolt = 0;
                sProgRun.bS_UpDown = 0;
            }

            if(sProgRun.wS_DeltaVolt <= 10)
            {
                sProgRun.dwS_StepTime = sProgRun.dwTimePeriod;
                sProgRun.wS_StepVolt = sProgRun.wS_DeltaVolt;
            }
            else if(sProgRun.dwTimePeriod >= sProgRun.wS_DeltaVolt)
            {
                sProgRun.dwS_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wS_DeltaVolt;
                sProgRun.wS_StepVolt = 10;	//unit:0.1V
            }
            else
            {
                sProgRun.dwS_StepTime = 10;
                sProgRun.wS_StepVolt = (unsigned long)sProgRun.wS_DeltaVolt * 10 / sProgRun.dwTimePeriod;
            }
            sProgRun.dwS_TotalStepTime = sProgRun.dwS_StepTime;

            //T相电压参考值调整
            sSet.u16OutputVoltage_T = sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt;
            if(sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt > sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt)
            {
                sProgRun.wT_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt - sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt;
                sProgRun.bT_UpDown = 2;	//递减
            }
            else if(sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt < sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt)
            {
                sProgRun.wT_DeltaVolt = sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt - sProgramming.u16Step[sProgRun.u16Index].wT_StartVolt;
                sProgRun.bT_UpDown = 1;	//递增
            }
            else
            {
                sProgRun.wT_DeltaVolt = 0;
                sProgRun.bT_UpDown = 0;
            }

            if(sProgRun.wT_DeltaVolt <= 10)
            {
                sProgRun.dwT_StepTime = sProgRun.dwTimePeriod;
                sProgRun.wT_StepVolt = sProgRun.wT_DeltaVolt;
            }
            else if(sProgRun.dwTimePeriod >= sProgRun.wT_DeltaVolt)
            {
                sProgRun.dwT_StepTime = sProgRun.dwTimePeriod * 10 / sProgRun.wT_DeltaVolt;
                sProgRun.wT_StepVolt = 10;	//unit:0.1V
            }
            else
            {
                sProgRun.dwT_StepTime = 10;
                sProgRun.wT_StepVolt = (unsigned long)sProgRun.wT_DeltaVolt * 10 / sProgRun.dwTimePeriod;
            }
            sProgRun.dwT_TotalStepTime = sProgRun.dwT_StepTime;


            if(sProgramming.u16Step[NextIndex].wR_StartVolt != sProgramming.u16Step[LastIndex].wR_EndVolt)
            {
                Adjust_Pwm_Gain(0x01);
            }
            if(sProgramming.u16Step[NextIndex].wS_StartVolt != sProgramming.u16Step[LastIndex].wS_EndVolt)
            {
                Adjust_Pwm_Gain(0x02);
            }
            if(sProgramming.u16Step[NextIndex].wT_StartVolt != sProgramming.u16Step[LastIndex].wT_EndVolt)
            {
                Adjust_Pwm_Gain(0x04);
            }

            if((sProgRun.bR_UpDown || sProgRun.bS_UpDown || sProgRun.bT_UpDown || sProgRun.bF_UpDown) ||
               (sProgramming.u16Step[NextIndex].wFreq_Start  != sProgramming.u16Step[LastIndex].wFreq_End  ||
                sProgramming.u16Step[NextIndex].wR_StartVolt != sProgramming.u16Step[LastIndex].wR_EndVolt ||
                sProgramming.u16Step[NextIndex].wS_StartVolt != sProgramming.u16Step[LastIndex].wS_EndVolt ||
                sProgramming.u16Step[NextIndex].wT_StartVolt != sProgramming.u16Step[LastIndex].wT_EndVolt))
            {
                DryContactFlag.sBit.ProgVoltFreqUpDown = 1;
            }

            sProgRun.u16StartPoint = u16SineWaveIndex;
            sProgRun.u16StepChangeAdjustDelay = 2;
        }
        else
        {
            if(sProgRun.bF_UpDown > 0)
            {
                if(sProgRun.u32TimeCnt >= (sProgRun.dwTimePeriod - 5))	//为了确保最终执行完后的频率是用户设定的终止频率
                {
                    sSet.u16OutputFrequency = sProgramming.u16Step[sProgRun.u16Index].wFreq_End;
                }
                else if(sProgRun.u32TimeCnt >= (sProgRun.dwF_TotalStepTime))
                {
                    sProgRun.dwF_TotalStepTime += sProgRun.dwF_StepTime;
                    sProgRun.wF_TotalTimeDiv += sProgRun.wF_StepTimeDiv;
                    if(sProgRun.wF_TotalTimeDiv >= 10)
                    {
                        sProgRun.dwF_TotalStepTime++;
                        sProgRun.wF_TotalTimeDiv -= 10;
                    }
                    if(sProgRun.bF_UpDown == 1)
                    {
                        if((sSet.u16OutputFrequency + sProgRun.wF_Step) < sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
                        {
                            sSet.u16OutputFrequency += sProgRun.wF_Step;
                        }
                        else
                        {
                            sSet.u16OutputFrequency = sProgramming.u16Step[sProgRun.u16Index].wFreq_End;
                        }
                    }
                    else
                    {
                        if((sSet.u16OutputFrequency - sProgRun.wF_Step) > sProgramming.u16Step[sProgRun.u16Index].wFreq_End)
                        {
                            sSet.u16OutputFrequency -= sProgRun.wF_Step;
                        }
                        else
                        {
                            sSet.u16OutputFrequency = sProgramming.u16Step[sProgRun.u16Index].wFreq_End;
                        }
                    }
                }
            }

            if(sProgRun.bR_UpDown > 0)
            {
                if(sProgRun.u32TimeCnt >= (sProgRun.dwTimePeriod - 2))	//为了确保最终执行完后的电压是用户设定的终止电压
                {
                    sSet.u16OutputVoltage_R = sProgramming.u16Step[sProgRun.u16Index].wR_EndVolt;
                    if(sSet.u16OutputVoltage_R > sRMS.u16InvVolt_R)
                    {
                        wVoltDiff = sSet.u16OutputVoltage_R - sRMS.u16InvVolt_R;
                    }
                    else
                    {
                        wVoltDiff = sRMS.u16InvVolt_R - sSet.u16OutputVoltage_R;
                    }
                    if(wVoltDiff > 50)
                    {
                        Adjust_Pwm_Gain(0x01);
                    }
                }
                else if(sProgRun.u32TimeCnt >= (sProgRun.dwR_TotalStepTime))
                {
                    sProgRun.dwR_TotalStepTime += sProgRun.dwR_StepTime;
                    if(sProgRun.bR_UpDown == 1)
                    {
                        sSet.u16OutputVoltage_R += sProgRun.wR_StepVolt;
                    }
                    else
                    {
                        if(sSet.u16OutputVoltage_R > sProgRun.wR_StepVolt)
                        {
                            sSet.u16OutputVoltage_R -= sProgRun.wR_StepVolt;
                        }
                        else
                        {
                            sSet.u16OutputVoltage_R = 0;
                        }
                    }
                    if(sProgRun.wR_StepVolt >= 50)
                    {
                        Adjust_Pwm_Gain(0x01);    // 每一个step的爬升或下降小于5V时，只修改输出电压参考，不对PWM的gain值进行干预
                    }
                }
            }

            if(sProgRun.bS_UpDown > 0)
            {
                if(sProgRun.u32TimeCnt >= (sProgRun.dwTimePeriod - 2))	//为了确保最终执行完后的电压是用户设定的终止电压
                {
                    sSet.u16OutputVoltage_S = sProgramming.u16Step[sProgRun.u16Index].wS_EndVolt;
                    if(sSet.u16OutputVoltage_S > sRMS.u16InvVolt_S)
                    {
                        wVoltDiff = sSet.u16OutputVoltage_S - sRMS.u16InvVolt_S;
                    }
                    else
                    {
                        wVoltDiff = sRMS.u16InvVolt_S - sSet.u16OutputVoltage_S;
                    }
                    if(wVoltDiff > 50)
                    {
                        Adjust_Pwm_Gain(0x02);
                    }
                }
                else if(sProgRun.u32TimeCnt >= (sProgRun.dwS_TotalStepTime))
                {
                    sProgRun.dwS_TotalStepTime += sProgRun.dwS_StepTime;
                    if(sProgRun.bS_UpDown == 1)
                    {
                        sSet.u16OutputVoltage_S += sProgRun.wS_StepVolt;
                    }
                    else
                    {
                        if(sSet.u16OutputVoltage_S > sProgRun.wS_StepVolt)
                        {
                            sSet.u16OutputVoltage_S -= sProgRun.wS_StepVolt;
                        }
                        else
                        {
                            sSet.u16OutputVoltage_S = 0;
                        }
                    }
                    if(sProgRun.wS_StepVolt >= 50)
                    {
                        Adjust_Pwm_Gain(0x02);    // 每一个step的爬升或下降小于5V时，只修改输出电压参考，不对PWM的gain值进行干预
                    }
                }
            }

            if(sProgRun.bT_UpDown > 0)
            {
                if(sProgRun.u32TimeCnt >= (sProgRun.dwTimePeriod - 2))	//为了确保最终执行完后的电压是用户设定的终止电压
                {
                    sSet.u16OutputVoltage_T = sProgramming.u16Step[sProgRun.u16Index].wT_EndVolt;
                    if(sSet.u16OutputVoltage_T > sRMS.u16InvVolt_T)
                    {
                        wVoltDiff = sSet.u16OutputVoltage_T - sRMS.u16InvVolt_T;
                    }
                    else
                    {
                        wVoltDiff = sRMS.u16InvVolt_T - sSet.u16OutputVoltage_T;
                    }
                    if(wVoltDiff > 50)
                    {
                        Adjust_Pwm_Gain(0x04);
                    }
                }
                else if(sProgRun.u32TimeCnt >= (sProgRun.dwT_TotalStepTime))
                {
                    sProgRun.dwT_TotalStepTime += sProgRun.dwT_StepTime;
                    if(sProgRun.bT_UpDown == 1)
                    {
                        sSet.u16OutputVoltage_T += sProgRun.wT_StepVolt;
                    }
                    else
                    {
                        if(sSet.u16OutputVoltage_T > sProgRun.wT_StepVolt)
                        {
                            sSet.u16OutputVoltage_T -= sProgRun.wT_StepVolt;
                        }
                        else
                        {
                            sSet.u16OutputVoltage_T = 0;
                        }
                    }
                    if(sProgRun.wT_StepVolt >= 50)
                    {
                        Adjust_Pwm_Gain(0x04);    // 每一个step的爬升或下降小于5V时，只修改输出电压参考，不对PWM的gain值进行干预
                    }
                }
            }
        }
    }
}



unsigned char Check_Programming_Data(void)
{
    unsigned char i;
    unsigned int vol_high_limit;
    Uint32 u32Data;

    if(sProgramming.u16Start > sProgramming.u16End)
    {
        return 255;
    }

    if(sProgramming.u16Cycles == 0)
    {
        return 254;
    }

    vol_high_limit = HIGH_OP_VOLT_PROG_LIMIT;

    for(i = sProgramming.u16Start - 1; i <= sProgramming.u16End - 1; i++)
    {
        u32Data = ((Uint32)sProgramming.u16Step[i].u16TimeHigh << 16) + (Uint32)sProgramming.u16Step[i].u16TimeLow;

        if( sProgramming.u16Step[i].wR_StartVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wR_StartVolt > vol_high_limit      ||
            sProgramming.u16Step[i].wR_EndVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wR_EndVolt > vol_high_limit ||
            sProgramming.u16Step[i].wS_StartVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wS_StartVolt > vol_high_limit      ||
            sProgramming.u16Step[i].wS_EndVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wS_EndVolt > vol_high_limit ||
            sProgramming.u16Step[i].wT_StartVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wT_StartVolt > vol_high_limit      ||
            sProgramming.u16Step[i].wT_EndVolt < SMALL_OP_VOLT_LIMIT ||
            sProgramming.u16Step[i].wT_EndVolt > vol_high_limit
          )
        {
            return ((i + 1) * 3);
        }
        else if (sProgramming.u16Step[i].wFreq_Start < LOW_OP_FREQ_LIMIT  ||
                 sProgramming.u16Step[i].wFreq_Start > HIGH_OP_FREQ_LIMIT ||
                 sProgramming.u16Step[i].wFreq_End   < LOW_OP_FREQ_LIMIT  ||
                 sProgramming.u16Step[i].wFreq_End   > HIGH_OP_FREQ_LIMIT)
        {
            return ((i + 1) * 3 + 1);
        }
        else if(u32Data < 8 || u32Data > 999999999L)    //最长约277.78小时，11.57天
        {
            return ((i + 1) * 3 + 2);
        }
    }

    return 0;
}


//------------------------------------------------------------------------------------------
//Timebase: main loop
//------------------------------------------------------------------------------------------
void Check_Over_Load(void)
{
    static int32 i32OpPowerLimit, i32OpVaLimit;

    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }
    
    #ifndef OP_PROTECT_SLOW
    
    if(sRMS.u32OpWatt_R >= OP_POWER_WATT_102 ||
       sRMS.u32OpWatt_S >= OP_POWER_WATT_102 ||
       sRMS.u32OpWatt_T >= OP_POWER_WATT_102)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverWatt102 = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    else if(sRMS.u32OpVa_R >= OP_POWER_WATT_102 ||
            sRMS.u32OpVa_S >= OP_POWER_WATT_102 ||
            sRMS.u32OpVa_T >= OP_POWER_WATT_102)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverVa102 = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    
    #endif
}

//------------------------------------------------------------------------------------------
//Timebase: 100ms
//------------------------------------------------------------------------------------------
#define MAX_OVER_LOAD_CNT   36000
void Check_Over_Load_Time(void)
{
    static Uint32 u32OverloadCnt;

    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }
        
    if(!SourceControlFlag.sBit.OutputOn)
    {
        u32OverloadCnt = 0;
        return;
    }
    
    #ifdef POWER_RATING_90K_S01
    //100%-120% 持续60分钟后报警
    //120%-150% 持续60秒后报警
    //150%-200% 15秒后报警

    if(sRMS.u16LoadPercent_Max > 200)
    {
        u32OverloadCnt = MAX_OVER_LOAD_CNT;
    }
    else if(sRMS.u16LoadPercent_Max > 150)
    {
        u32OverloadCnt += 240;
    }
    else if(sRMS.u16LoadPercent_Max > 120)
    {
        u32OverloadCnt += 60;
    }
    else if(sRMS.u16LoadPercent_Max > 100)
    {
        u32OverloadCnt += 1;
    }
    else
    {
        if(u32OverloadCnt > 10)
        {
            u32OverloadCnt -= 10;
        }
    }

    if(u32OverloadCnt >= MAX_OVER_LOAD_CNT)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverWatt102 = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    
    #elif defined POWER_RATING_30K_S01
    //100%-200% 持续30秒后报警
    //200%-300% 持续10秒后报警
    //>300%     立即报警
    
    if(sRMS.u16LoadPercent_Max > 300)
    {
        u32OverloadCnt = MAX_OVER_LOAD_CNT;
    }
    else if(sRMS.u16LoadPercent_Max > 200)
    {
        u32OverloadCnt += 360;
    }
    else if(sRMS.u16LoadPercent_Max > 100)
    {
        u32OverloadCnt += 120;
    }
    else
    {
        if(u32OverloadCnt > 10)
        {
            u32OverloadCnt -= 10;
        }
    }

    if(u32OverloadCnt >= MAX_OVER_LOAD_CNT)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverWatt102 = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    
    #endif
}


//------------------------------------------------------------------------------------------
//Timebase: main loop
//------------------------------------------------------------------------------------------
void Check_Over_Current(void)
{
    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if (SourceControlFlag.sBit.OutputOn)
    {
        if((sSet.u16OutputCurrentLimit > 0 && sRMS.u16OpCurr_Max > sSet.u16OutputCurrentLimit) //自定义的过流保护，不收允许超载限制
           #ifndef OP_PROTECT_SLOW
           ||
           (sSet.u16OutputRange == 2 && sRMS.u16OpCurr_Max > HIGH_OP_CURRENT_LIMIT) ||
           (sRMS.u16OpCurr_Max > LOW_OP_CURRENT_LIMIT)
           #endif
           )
        {
            SetOutputOff();
            FAULT1_FLAG.sBit.OverCurrent = 1;

            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;
        }
    }

    if (FAULT1_FLAG.sBit.InvIPeakProtect ||
        FAULT1_FLAG.sBit.bInv316ScProtect  //||
        //FAULT1_FLAG.sBit.InvDcOffsetProtect
       )
    {
        SetOutputOff();

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }

}

//------------------------------------------------------------------------------------------
//Timebase: 100ms
//------------------------------------------------------------------------------------------
void Check_Over_Current_Time(void)
{
    static Uint16 u16OverCurrTimeCnt;
    
    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }
        
    if(!SourceControlFlag.sBit.OutputOn)
    {
        u16OverCurrTimeCnt = 0;
        return;
    }
    
    #ifdef POWER_RATING_90K_S01
    //100%-120% 持续60分钟后报警
    //120%-150% 持续60秒后报警
    //150%-200% 15秒后报警
    //>200 立即报警
    if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*2.05))
    {
        u16OverCurrTimeCnt = MAX_OVER_LOAD_CNT;
    }
    else if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*1.55))
    {
        u16OverCurrTimeCnt += 240;
    }
    else if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*1.25))
    {
        u16OverCurrTimeCnt += 60;
    }
    else if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*1.02))
    {
        u16OverCurrTimeCnt += 1;
    }
    else
    {
        if(u16OverCurrTimeCnt > 10)
        {
            u16OverCurrTimeCnt -= 10;
        }
    }

    if(u16OverCurrTimeCnt >= MAX_OVER_LOAD_CNT)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverCurrent = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    
    #elif defined POWER_RATING_30K_S01
    //100%-200% 持续30秒后报警
    //200%-300% 持续10秒后报警
    //>300%     立即报警
    
    if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*3))
    {
        u16OverCurrTimeCnt = MAX_OVER_LOAD_CNT;
    }
    else if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT*2))
    {
        u16OverCurrTimeCnt += 360;
    }
    else if(sRMS.u16OpCurr_Max > (HIGH_OP_CURRENT_LIMIT))
    {
        u16OverCurrTimeCnt += 120;
    }
    else
    {
        if(u16OverCurrTimeCnt > 10)
        {
            u16OverCurrTimeCnt -= 10;
        }
    }

    if(u16OverCurrTimeCnt >= MAX_OVER_LOAD_CNT)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverCurrent = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    
    #endif
}

//------------------------------------------------------------------------------------------
//Timebase:10ms
//------------------------------------------------------------------------------------------
void Check_Inverter_FastFail(void)
{
    static unsigned char bInvFastErrorDelayCnt = 0;
    static unsigned char bInvFastErrorCnt = 0;
    
    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if(SourceControlFlag.sBit.OutputOn)
    {
        if(bInvFastErrorDelayCnt < 10)	//100ms*30=1s
        {
            bInvFastErrorDelayCnt++;
        }
        else
        {
            if((sSet.u16OutputVoltage_R > 200 && sRMS.u16InvVolt_R < 100) || 
               (sSet.u16OutputVoltage_S > 200 && sRMS.u16InvVolt_S < 100) ||  
               (sSet.u16OutputVoltage_T > 200 && sRMS.u16InvVolt_T < 100))
            {
                if(bInvFastErrorCnt < 20)
                    bInvFastErrorCnt++;
                else
                {
                    SetOutputOff();
                    FAULT1_FLAG.sBit.InverterOverVoltage = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }  
            }
            else
            {
                bInvFastErrorCnt = 0;
            }
        }
    }
    else
    {
        bInvFastErrorDelayCnt = 0;
        bInvFastErrorCnt = 0;
    }
    
}

//------------------------------------------------------------------------------------------
//Timebase:100ms
//------------------------------------------------------------------------------------------
#define INVERTER_VOLT_ERROR_RANGE   200
void Check_Over_Voltage(void)
{
    static unsigned char bInvErrorDelayCnt = 0;
    static unsigned char bInvErrorCnt_R, bInvErrorCnt_S, bInvErrorCnt_T;
    static unsigned char bOverVoltageCnt_R, bOverVoltageCnt_S, bOverVoltageCnt_T;

    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if(SourceControlFlag.sBit.OutputOn)
    {
        if(bInvErrorDelayCnt < 30)	//100ms*30=3s
        {
            bInvErrorDelayCnt++;
        }
        else
        {
            //----------------------------------------------------------------------
            //检测Inverter电压是否异常
            if(sRMS.u16InvVolt_R > sSet.u16OutputVoltage_R)
            {
                wInvOpVolError = sRMS.u16InvVolt_R - sSet.u16OutputVoltage_R;
            }
            else
            {
                wInvOpVolError = sSet.u16OutputVoltage_R - sRMS.u16InvVolt_R;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bOverVoltageCnt_R++;

                if(bOverVoltageCnt_R >= 20)	//100ms*10=2s
                {
                    bOverVoltageCnt_R = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InverterOverVoltage = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bOverVoltageCnt_R = 0;
            }

            if(sRMS.u16InvVolt_S > sSet.u16OutputVoltage_S)
            {
                wInvOpVolError = sRMS.u16InvVolt_S - sSet.u16OutputVoltage_S;
            }
            else
            {
                wInvOpVolError = sSet.u16OutputVoltage_S - sRMS.u16InvVolt_S;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bOverVoltageCnt_S++;

                if(bOverVoltageCnt_S >= 20)	//100ms*10=2s
                {
                    bOverVoltageCnt_S = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InverterOverVoltage = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bOverVoltageCnt_S = 0;
            }

            if(sRMS.u16InvVolt_T > sSet.u16OutputVoltage_T)
            {
                wInvOpVolError = sRMS.u16InvVolt_T - sSet.u16OutputVoltage_T;
            }
            else
            {
                wInvOpVolError = sSet.u16OutputVoltage_T - sRMS.u16InvVolt_T;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bOverVoltageCnt_T++;

                if(bOverVoltageCnt_T >= 20)	//100ms*10=2s
                {
                    bOverVoltageCnt_T = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InverterOverVoltage = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bOverVoltageCnt_T = 0;
            }



            //----------------------------------------------------------------------
            //检测输出电压是否异常
            if(sRMS.u16InvVolt_R > sRMS.u16OpVolt_R)
            {
                wInvOpVolError = sRMS.u16InvVolt_R - sRMS.u16OpVolt_R;
            }
            else
            {
                wInvOpVolError = sRMS.u16OpVolt_R - sRMS.u16InvVolt_R;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bInvErrorCnt_R++;
                if(bInvErrorCnt_R >= 20)	//100ms*10=2s
                {
                    bInvErrorCnt_R = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InvOpDifferent = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bInvErrorCnt_R = 0;
            }

            if(sRMS.u16InvVolt_S > sRMS.u16OpVolt_S)
            {
                wInvOpVolError = sRMS.u16InvVolt_S - sRMS.u16OpVolt_S;
            }
            else
            {
                wInvOpVolError = sRMS.u16OpVolt_S - sRMS.u16InvVolt_S;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bInvErrorCnt_S++;
                if(bInvErrorCnt_S >= 20)	//100ms*10=2s
                {
                    bInvErrorCnt_S = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InvOpDifferent = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bInvErrorCnt_S = 0;
            }

            if(sRMS.u16InvVolt_T > sRMS.u16OpVolt_T)
            {
                wInvOpVolError = sRMS.u16InvVolt_T - sRMS.u16OpVolt_T;
            }
            else
            {
                wInvOpVolError = sRMS.u16OpVolt_T - sRMS.u16InvVolt_T;
            }

            if(wInvOpVolError >= INVERTER_VOLT_ERROR_RANGE)
            {
                bInvErrorCnt_T++;
                if(bInvErrorCnt_T >= 20)	//100ms*10=2s
                {
                    bInvErrorCnt_T = 20;

                    SetOutputOff();
                    FAULT1_FLAG.sBit.InvOpDifferent = 1;

                    SourceControlFlag.sBit.VSET = 0;
                    SourceControlFlag.sBit.FSET = 0;
                }
            }
            else
            {
                bInvErrorCnt_T = 0;
            }
        }
    }
    else
    {
        bOverVoltageCnt_R = 0;
        bInvErrorCnt_R = 0;
        bOverVoltageCnt_S = 0;
        bInvErrorCnt_S = 0;
        bOverVoltageCnt_S = 0;
        bInvErrorCnt_S = 0;
        bInvErrorDelayCnt = 0;
    }
}

//------------------------------------------------------------------------------------------
//Timebase: main loop
//------------------------------------------------------------------------------------------
void Check_Over_Temperature(void)
{
    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if(sRMS.i16Temperature_INV_R > HSINK_PFC_OTP ||   //OTP:70度
       sRMS.i16Temperature_INV_S > HSINK_INV_OTP) //||   //OTP:70度
        //sRMS.i16Temperature_AMB > AMB_OTP)
    {
        SetOutputOff();
        FAULT1_FLAG.sBit.OverTemperature = 1;

        SourceControlFlag.sBit.VSET = 0;
        SourceControlFlag.sBit.FSET = 0;
    }
    else if(sRMS.i16Temperature_INV_R < (HSINK_PFC_OTP - 20) &&
            sRMS.i16Temperature_INV_S < (HSINK_INV_OTP - 20))
    {
        FAULT1_FLAG.sBit.OverTemperature = 0;
    }
    /*
        if(sRMS.wIpTrTemperature < 512)
        {
            SetOutputOff();
            FAULT1_FLAG.sBit.IpTrOverTemperature = 1;

            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;
        }

        if(sRMS.wOpTrTemperature < 512)
        {
            SetOutputOff();
            FAULT1_FLAG.sBit.OpTrOverTemperature = 1;

            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;
        }
    */
}
//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------
void Check_Transfermer_OTP(void)
{
    static Uint16 u16TrsOTPFailCnt = 0;

    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if(!CHK_OP_TRS_TEMP)		//NORMAL CLOSE: 低电平fail
    {
        if(u16TrsOTPFailCnt < 5)
        {
            u16TrsOTPFailCnt++;
        }
        else
        {
            SourceControlFlag.sBit.OutputOn = 0;
            FAULT1_FLAG.sBit.OpTrOverTemperature = 1;

            Program_Stop();

            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;
        }
    }
    else
    {
        u16TrsOTPFailCnt = 0;
    }
}
//------------------------------------------------------------------------------------------
//Timebase:100ms
//------------------------------------------------------------------------------------------
void Check_EPO(void)
{
    static Uint16 u16EpoFailCnt = 0;

    if(SourceControlFlag.sBit.RepairOn)
    {
        return;
    }

    if(!CHK_EPO)
    {
        if(u16EpoFailCnt < 5)
        {
            u16EpoFailCnt++;
        }
        else
        {
            SourceControlFlag.sBit.OutputOn = 0;
            FAULT1_FLAG.sBit.u1EPO = 1;

            Program_Stop();

            SourceControlFlag.sBit.VSET = 0;
            SourceControlFlag.sBit.FSET = 0;
        }
    }
    else
    {
        u16EpoFailCnt = 0;
    }

}

//------------------------------------------------------------------------------------------
//Timebase:1s
//------------------------------------------------------------------------------------------
void Check_Updata_Eeprom(void)
{
    if(LastSourceControlFlag.sBit.OutputOn != SourceControlFlag.sBit.OutputOn)
    {
        if(!SourceControlFlag.sBit.OutputOn)
        {
            Updata_Power_Energy();

            ExtEepromQueueIn(EEP_ADDR_RUN_TIME_LIMIT, sSet.u16RemainingRunTime);
        }
        LastSourceControlFlag.sBit.OutputOn = SourceControlFlag.sBit.OutputOn;
    }
}

//------------------------------------------------------------------------------
// CHECK REPAIR PIN
// Run it every 0.1s
//------------------------------------------------------------------------------
void Check_Repair_Mode(void)
{
    #ifdef DEBUG_OPEN_LOOP
    SourceControlFlag.sBit.RepairOn = 1;
    #else
    if(!CHK_REPAIR_MODE)
    {
        if(u16RepairModeCnt < 10)
        {
            u16RepairModeCnt++;
        }
        else
        {
            SourceControlFlag.sBit.RepairOn = 1;
        }
    }
    else
    {
        u16RepairModeCnt = 0;
        SourceControlFlag.sBit.RepairOn = 0;
    }
    #endif
}

void Adjust_Angle_Offset(void)
{
    sSet.u16OutputOnAngleDo = sSet.u16OutputOnAngle + 180 + sGain_R.wAngle;
    if(sSet.u16OutputOnAngleDo >= 360)
    {
        sSet.u16OutputOnAngleDo -= 360;
    }
}

//------------------------------------------------------------------
//Time base:1sec
//------------------------------------------------------------------
void Input_Relay_Control(void)
{
    static Uint16 u16SoftStartCtl = 0;

    #ifdef DEBUG_NO_PROTECT

    if(u16RelayDelayCnt < 0xFF)
    {
        u16RelayDelayCnt++;
    }

    if(u16RelayDelayCnt > 5 && sAdcCal.i16D2dBus_R > (DC_BUS_LOW - 20))
    {
        INPUT_RELAY_ON;
        //SOFTSTART_RELAY_OFF;
        //PFC_ON;
    }
    else
    {
        INPUT_RELAY_OFF;
        //SOFTSTART_RELAY_ON;
    }

    if(u16RelayDelayCnt > 6)
    {
        //DC2DC_ON;
    }

    OUTPUT_RELAY_ON;

    #else

    //if(sRMS.u16RectBus_R > 250 && sRMS.u16RectBus_R < 382 //1V      //170*1.414=240
    //||
    //(SourceControlFlag.sBit.RepairOn && u16SoftstartCnt > 20)
    //)
    {
        if(u16RelayDelayCnt < 0xFF)
        {
            u16RelayDelayCnt++;
        }

        if(u16RelayDelayCnt > 5 && sAdcCal.i16D2dBus_R > (DC_BUS_LOW - 20))
        {
            INPUT_RELAY_ON;
            //SOFTSTART_RELAY_OFF;
            //PFC_ON;
        }
        else
        {
            INPUT_RELAY_OFF;
            //SOFTSTART_RELAY_ON;
        }

        if(u16RelayDelayCnt > 6)
        {
            //DC2DC_ON;
        }

        OUTPUT_RELAY_ON;
    }


    /*
    if(sRMS.u16IpVolt_R > 180 && sRMS.u16IpVolt_R < 245)
    {
        if(u16SoftStartCtl == 0)
            SOFTSTART_RELAY_ON;

        if(u16RelayDelayCnt < 0xFF)
            u16RelayDelayCnt++;

        if(u16RelayDelayCnt > 6 && (sRMS.u16D2dBus_R > sRMS.u16IpVolt_R*6/50))
        {
            INPUT_RELAY_ON;
            SOFTSTART_RELAY_OFF;
            u16SoftStartCtl = 1;
        }

        if(u16RelayDelayCnt > 10)
        {
            DC2DC_ON;
        }
    }
    else if((sRMS.u16IpVolt_R < 170 || sRMS.u16IpVolt_R > 250)&& !SourceControlFlag.sBit.RepairOn)
    {
        INVERTER_DISABLE_R;
        INVERTER_DISABLE_S;
        INVERTER_DISABLE_T;
        DC2DC_OFF;
        SOFTSTART_RELAY_OFF;
        INPUT_RELAY_OFF;

        //if(u16SoftstartCnt < 0xFF)
        //    u16SoftstartCnt++;

        SetOutputOff();

        u16RelayDelayCnt = 0;
        u16SoftStartCtl = 0;
    }*/


    #endif


}

//------------------------------------------------------------------
//Time base:100ms
//------------------------------------------------------------------
void HighLowRange_Relay_Control(void)
{

}

#define FAN_SPEED_LOAD1    20
#define FAN_SPEED_LOAD2    40
#define FAN_SPEED_LOAD3    60
#define FAN_SPEED_LOAD4    80
void FanSpeedControl(void)
{
    static unsigned int u16LoadLevel;
    static unsigned int u16FanPwm;

    /*
    if(sRMS.u16LoadPercent_R > 80)
        PWMDTY3 = FAN_PWM_PERIOD*0.7;
    else if(sRMS.u16LoadPercent_R > 60)
        PWMDTY3 = FAN_PWM_PERIOD*0.6;
    else if(sRMS.u16LoadPercent_R > 40)
        PWMDTY3 = FAN_PWM_PERIOD*0.5;
    else if(sRMS.u16LoadPercent_R > 20)
        PWMDTY3 = FAN_PWM_PERIOD*0.4;
    else
        PWMDTY3 = FAN_PWM_PERIOD*0.3;
    */
    if(sRMS.u16LoadPercent_Max > FAN_SPEED_LOAD4)
    {
        u16LoadLevel |= 0x0010;
    }
    else if(sRMS.u16LoadPercent_Max <= (FAN_SPEED_LOAD4 - 5))
    {
        u16LoadLevel &= ~0x0010;
    }

    if(sRMS.u16LoadPercent_Max > FAN_SPEED_LOAD3)
    {
        u16LoadLevel |= 0x0008;
    }
    else if(sRMS.u16LoadPercent_Max <= (FAN_SPEED_LOAD3 - 5))
    {
        u16LoadLevel &= ~0x0008;
    }

    if(sRMS.u16LoadPercent_Max > FAN_SPEED_LOAD2)
    {
        u16LoadLevel |= 0x0004;
    }
    else if(sRMS.u16LoadPercent_Max <= (FAN_SPEED_LOAD2 - 5))
    {
        u16LoadLevel &= ~0x0004;
    }

    if(sRMS.u16LoadPercent_Max > FAN_SPEED_LOAD1)
    {
        u16LoadLevel |= 0x0002;
    }
    else if(sRMS.u16LoadPercent_Max <= (FAN_SPEED_LOAD1 - 5))
    {
        u16LoadLevel &= ~0x0002;
    }

    u16LoadLevel |= 0x0001;


    if(u16LoadLevel >= 0x0010)
    {
        u16FanPwm = FAN_PWM_PERIOD * 1.00;
    }
    else if(u16LoadLevel >= 0x0008)
    {
        u16FanPwm = FAN_PWM_PERIOD * 0.95;
    }
    else if(u16LoadLevel >= 0x0004)
    {
        u16FanPwm = FAN_PWM_PERIOD * 0.80;
    }
    else if(u16LoadLevel >= 0x0002)
    {
        u16FanPwm = FAN_PWM_PERIOD * 0.85;
    }
    else
    {
        u16FanPwm = FAN_PWM_PERIOD * 0.80;
    }



    #ifdef DEBUG_OPEN_LOOP
    u16FanPwm = FAN_PWM_PERIOD * 0.65;
    #endif

    if(EPwm7Regs.CMPA.bit.CMPA > u16FanPwm)
    {
        EPwm7Regs.CMPA.bit.CMPA -= 10;
    }
    else
    {
        EPwm7Regs.CMPA.bit.CMPA = u16FanPwm;
    }
}

//--------------------------------------------------------------------------------
//For inverter Ipeak control
//--------------------------------------------------------------------------------
void IpeakControl(void)
{
    static unsigned int u16LastProtectPoint = 0;
    static unsigned int u16IpeakProtectTemp;

    u16IpeakProtectTemp = u16IpeakProtect;

    if(u16IpeakProtectTemp != u16LastProtectPoint)
    {
        u16LastProtectPoint = u16IpeakProtectTemp;

        if(u16IpeakProtectTemp >= 100)
        {
            EPwm11Regs.CMPA.bit.CMPA = IPEAK_PWM_PERIOD;
        }
        else
        {
            EPwm11Regs.CMPA.bit.CMPA = (Uint16)((Uint32)IPEAK_PWM_PERIOD * u16IpeakProtectTemp / 100);
        }
    }

}



void Clear_Set_Flag(void)
{
    //sKEY_SCAN2.sBit.DISPLAY = 0;
    //sKEY_SCAN1.WORD = 0;
    //sKEY_SCAN2.WORD = 0;
    //sKeyBoardIn.u16Edit = 0;
    //
    //u16LcdDisplayMode = 0;
    //u16Page = 0;
    //sLcdItem.u16Level1 = 0;
    //sLcdItem.u16Level2 = 0;
    //sLcdItem.u16Level3 = 0;

    FAULT1_FLAG.sBit.ProgramParaError = 0;
    u16ProgramErrorCode = 0;


    //sKeyBoardIn.u16Integral = 0;
    //sKeyBoardIn.u16Decimal = 0;
    //sKeyBoardIn.u16IntLen = 0;
    //sKeyBoardIn.u16DecLen = 0;
    //sKeyBoardIn.u16Dot = 0;
    //sKeyBoardIn.u16NumPress = 0;
}

void HarmDataInit(void)
{
    HarmSuperData.u16HarmConfig1 = 0x0000;

    HarmSuperData.i16Harmrms[ 0] = 0;   //	2
    HarmSuperData.i16Harmrms[ 1] = 0;   //	3
    HarmSuperData.i16Harmrms[ 2] = 0;   //	4
    HarmSuperData.i16Harmrms[ 3] = 0;   //	5
    HarmSuperData.i16Harmrms[ 4] = 0;   //	6
    HarmSuperData.i16Harmrms[ 5] = 0;   //	7
    HarmSuperData.i16Harmrms[ 6] = 0;   //	8
    HarmSuperData.i16Harmrms[ 7] = 0;   //	9
    HarmSuperData.i16Harmrms[ 8] = 0;   //	10
    HarmSuperData.i16Harmrms[ 9] = 0;   //	11
    HarmSuperData.i16Harmrms[10] = 0;   //	12
    HarmSuperData.i16Harmrms[11] = 0;   //	13
    HarmSuperData.i16Harmrms[12] = 0;   //	14
    HarmSuperData.i16Harmrms[13] = 0;   //	15
    HarmSuperData.i16Harmrms[14] = 0;   //	16
    HarmSuperData.i16Harmrms[15] = 0;   //	17
    HarmSuperData.i16Harmrms[16] = 0;   //	18
    HarmSuperData.i16Harmrms[17] = 0;   //	19
    HarmSuperData.i16Harmrms[18] = 0;   //	20
    HarmSuperData.i16Harmrms[19] = 0;   //	21
    HarmSuperData.i16Harmrms[20] = 0;   //	22
    HarmSuperData.i16Harmrms[21] = 0;   //	23
    HarmSuperData.i16Harmrms[22] = 0;   //	24
    HarmSuperData.i16Harmrms[23] = 0;   //	25
    HarmSuperData.i16Harmrms[24] = 0;   //	26
    HarmSuperData.i16Harmrms[25] = 0;   //	27
    HarmSuperData.i16Harmrms[26] = 0;   //	28
    HarmSuperData.i16Harmrms[27] = 0;   //	29
    HarmSuperData.i16Harmrms[28] = 0;   //	30
    HarmSuperData.i16Harmrms[29] = 0;   //	31
    HarmSuperData.i16Harmrms[30] = 0;   //	32
    HarmSuperData.i16Harmrms[31] = 0;   //	33
    HarmSuperData.i16Harmrms[32] = 0;   //	34
    HarmSuperData.i16Harmrms[33] = 0;   //	35
    HarmSuperData.i16Harmrms[34] = 0;   //	36
    HarmSuperData.i16Harmrms[35] = 0;   //	37
    HarmSuperData.i16Harmrms[36] = 0;   //	38
    HarmSuperData.i16Harmrms[37] = 0;   //	39
    HarmSuperData.i16Harmrms[38] = 0;   //	40
    HarmSuperData.i16Harmrms[39] = 0;   //	41
    HarmSuperData.i16Harmrms[40] = 0;   //	42
    HarmSuperData.i16Harmrms[41] = 0;   //	43
    HarmSuperData.i16Harmrms[42] = 0;   //	44
    HarmSuperData.i16Harmrms[43] = 0;   //	45
    HarmSuperData.i16Harmrms[44] = 0;   //	46
    HarmSuperData.i16Harmrms[45] = 0;   //	47
    HarmSuperData.i16Harmrms[46] = 0;   //	48
    HarmSuperData.i16Harmrms[47] = 0;   //	49
    HarmSuperData.i16Harmrms[48] = 0;   //	50

    HarmSuperData.u16Command = 0x0001;  //计算

}


//圆周率系数
#define COEF_2PI	    6.283185307F    // 2π
#define COEF_2PI_PHS	0.017453292F	// 2π / 360
void HarmonicCalc(void)
{
    static int32 i, j;
    static float32 f32PhaseData;
    static float32 f32HarmonicTotal;

    if(!SourceControlFlag.sBit.OutputOn)
    {
        if(HarmSuperData.u16HarmConfig1 & 0x0001)   //谐波使能
        {
            //n次谐波叠加计算
            if(HarmSuperData.u16Command == 0x0001)
            {
                u32TotalHarmValue = 0;
                for(j = 2; j <= HARM_TOTAL_NUM; j++)
                {
                    u32TotalHarmValue += HarmSuperData.i16Harmrms[j - 2];
                }
                if(u32TotalHarmValue > 1000)    //总谐波含量超过100%
                {
                    return;
                }

                for(i = 0; i < 360; i++)
                {
                    f32HarmonicTotal = 0;
                    for(j = 2; j <= HARM_TOTAL_NUM; j++)    //计算高次谐波总和
                    {
                        //谐波无相移
                        f32PhaseData = (COEF_2PI_PHS * i * j);
                        //谐波增加相移
                        //f32PhaseData += COEF_2PI*(float32)HarmSuperData.i16Harmphase[j-2];
                        f32HarmonicTotal += __sin(f32PhaseData) * (float32)HarmSuperData.i16Harmrms[j - 2] * 0.001F;
                    }
                    f32SinewaveTable[i] = (__sin(COEF_2PI_PHS * i) + f32HarmonicTotal);	    //基波加所有的高次谐波
                    i16SinewaveTable[i] = (int16)(f32SinewaveTable[i] * 4096);
                }

                HarmSuperData.u16Command = HarmSuperData.u16Command << 8;
            }
        }
        else    //谐波关闭
        {
            if(HarmSuperData.u16Command == 0x0001)
            {
                for(i = 0; i < 360; i++)
                {
                    f32SinewaveTable[i] = __sin(COEF_2PI_PHS * i);	    //基波
                    i16SinewaveTable[i] = (int16)(f32SinewaveTable[i] * 4096);
                }
            }

            HarmSuperData.u16Command = HarmSuperData.u16Command << 8;
        }
    }
}



//------------------------------------------------------------------------------
// Function：用于输出电压跳变
//------------------------------------------------------------------------------
void Adjust_Pwm_Gain(unsigned char RST)
{
    static int32 i32PwmGain_R, i32PwmGain_S, i32PwmGain_T;

    #ifdef DEBUG_NO_PROTECT
    if(SourceControlFlag.sBit.RepairOn)
    {
        sRMS.u16RectBus_R = 400;
    }
    #endif
    if(sAdcCal.i16D2dBus_R >= DC_BUS_LOW &&
       sAdcCal.i16D2dBus_S >= DC_BUS_LOW &&
       sAdcCal.i16D2dBus_T >= DC_BUS_LOW &&
       sSet.u16OutputVoltage_R >= SMALL_OP_VOLT_LIMIT
      )
    {
        if(RST & 0x01)
        {
            //i32PwmGain_R = (Uint32)(((float32)sSet.u16OutputVoltage_R * 0.1214 / sAdcCal.i16D2dBus_R) * (INV_SW_HALF_PERIOD / 2));	//2021/08/04 跳变电压有点偏高，故将1.414修改为1.214
            //i32PwmGain_R = i32PwmGain_R << 16;
            
            //用户可以自己调整PWM GAIN（65536*0.1214）=7955
            i32PwmGain_R = (Uint32)(((float32)sSet.u16OutputVoltage_R * sGain_R.wPWM / sAdcCal.i16D2dBus_R) * (INV_SW_HALF_PERIOD ));
        }

        if(RST & 0x02)
        {
            //i32PwmGain_S = (Uint32)(((float32)sSet.u16OutputVoltage_S * 0.1214 / sAdcCal.i16D2dBus_S) * (INV_SW_HALF_PERIOD / 2));
            //i32PwmGain_S = i32PwmGain_S << 16;
            
            //用户可以自己调整PWM GAIN
            i32PwmGain_S = (Uint32)(((float32)sSet.u16OutputVoltage_S * sGain_S.wPWM / sAdcCal.i16D2dBus_S) * (INV_SW_HALF_PERIOD ));
        }

        if(RST & 0x04)
        {
            //i32PwmGain_T = (Uint32)(((float32)sSet.u16OutputVoltage_T * 0.1214 / sAdcCal.i16D2dBus_T) * (INV_SW_HALF_PERIOD / 2));
            //i32PwmGain_T = i32PwmGain_T << 16;
            
            //用户可以自己调整PWM GAIN
            i32PwmGain_T = (Uint32)(((float32)sSet.u16OutputVoltage_T * sGain_T.wPWM / sAdcCal.i16D2dBus_T) * (INV_SW_HALF_PERIOD ));
        }
        
        u16RmsErrorDelayCnt = 0;
        DINT;
        i32Inv1RMSAmpVref_R = i32PwmGain_R;
        i32Inv1RMSAmpVref_S = i32PwmGain_S;
        i32Inv1RMSAmpVref_T = i32PwmGain_T;
        i32Inv1AmpVref_R = i32Inv1RMSAmpVref_R >> 12;
        i32Inv1AmpVref_S = i32Inv1RMSAmpVref_S >> 12;
        i32Inv1AmpVref_T = i32Inv1RMSAmpVref_T >> 12;
        EINT;
    }
    else
    {

    }
}


//------------------------------------------------------------------------------
// Function：输出电压RMS控制器
//------------------------------------------------------------------------------
#define VAMP_HI_LIMIT           (int32)(((INV_SW_HALF_PERIOD/2.)*98/100))
#define VAMP_LOW_LIMIT          (int32)(0)
#define RMS_VOLTS_ALPHA			((int)2688 )
#define RMS_VOLTS_BETA			((int)-2249)
void InvRmsVolController(void)
{
    static int16 i16InvRmsErr_R, i16InvRmsLastErr_R;
    static int16 i16InvRmsErr_S, i16InvRmsLastErr_S;
    static int16 i16InvRmsErr_T, i16InvRmsLastErr_T;

    if(SourceControlFlag.sBit.OutputOn)
    {
        if(ADC_FLAG.sBit.RMS_CONTROL)   // Run Once, One cycle
        {
            if(u16RmsErrorDelayCnt <= 2)
            {
                u16RmsErrorDelayCnt++;
            }

            //---------------------------------------------------------------------------------------------
            // R Phase
            //---------------------------------------------------------------------------------------------
            // VRMS PI controler
            if(u16RmsErrorDelayCnt <= 2)
            {
                i16InvRmsErr_R = 0;
                i16InvRmsLastErr_R = 0;
            }
            else
            {
                i16InvRmsErr_R = (int16)sSet.u16OutputVoltage_R - sAdcCal.i16Inv1RMSVol_R;
            }

            if(i16InvRmsErr_R > 300)
            {
                i16InvRmsErr_R = 300;
            }
            else if(i16InvRmsErr_R < -300)
            {
                i16InvRmsErr_R = -300;
            }

            i32Inv1RMSAmpVref_R += (((int32)i16InvRmsErr_R * RMS_VOLTS_ALPHA + (int32)i16InvRmsErr_R * RMS_VOLTS_BETA) << 4);
            i16InvRmsLastErr_R = i16InvRmsErr_R;

            // limit
            // INV1...
            if((int16)(i32Inv1RMSAmpVref_R >> 16) > VAMP_HI_LIMIT)
            {
                i32Inv1RMSAmpVref_R = (int32)VAMP_HI_LIMIT << 16;
            }
            else if((int16)(i32Inv1RMSAmpVref_R >> 16) < VAMP_LOW_LIMIT)
            {
                i32Inv1RMSAmpVref_R = (int32)VAMP_LOW_LIMIT << 16;
            }

            //---------------------------------------------------------------------------------------------
            // S Phase
            //---------------------------------------------------------------------------------------------
            // VRMS PI controler
            if(u16RmsErrorDelayCnt <= 2)
            {
                i16InvRmsErr_S = 0;
                i16InvRmsLastErr_S = 0;
            }
            else
            {
                i16InvRmsErr_S = (int16)sSet.u16OutputVoltage_S - sAdcCal.i16Inv1RMSVol_S;
            }

            if(i16InvRmsErr_S > 300)
            {
                i16InvRmsErr_S = 300;
            }
            else if(i16InvRmsErr_S < -300)
            {
                i16InvRmsErr_S = -300;
            }

            i32Inv1RMSAmpVref_S += (((int32)i16InvRmsErr_S * RMS_VOLTS_ALPHA + (int32)i16InvRmsErr_S * RMS_VOLTS_BETA) << 4);
            i16InvRmsLastErr_S = i16InvRmsErr_S;

            // limit
            // INV1...
            if((int16)(i32Inv1RMSAmpVref_S >> 16) > VAMP_HI_LIMIT)
            {
                i32Inv1RMSAmpVref_S = (int32)VAMP_HI_LIMIT << 16;
            }
            else if((int16)(i32Inv1RMSAmpVref_S >> 16) < VAMP_LOW_LIMIT)
            {
                i32Inv1RMSAmpVref_S = (int32)VAMP_LOW_LIMIT << 16;
            }

            //---------------------------------------------------------------------------------------------
            // T Phase
            //---------------------------------------------------------------------------------------------
            // VRMS PI controler
            if(u16RmsErrorDelayCnt <= 2)
            {
                i16InvRmsErr_T = 0;
                i16InvRmsLastErr_T = 0;
            }
            else
            {
                i16InvRmsErr_T = (int16)sSet.u16OutputVoltage_T - sAdcCal.i16Inv1RMSVol_T;
            }

            if(i16InvRmsErr_T > 300)
            {
                i16InvRmsErr_T = 300;
            }
            else if(i16InvRmsErr_T < -300)
            {
                i16InvRmsErr_T = -300;
            }

            i32Inv1RMSAmpVref_T += (((int32)i16InvRmsErr_T * RMS_VOLTS_ALPHA + (int32)i16InvRmsErr_T * RMS_VOLTS_BETA) << 4);
            i16InvRmsLastErr_T = i16InvRmsErr_T;

            // limit
            // INV1...
            if((int16)(i32Inv1RMSAmpVref_T >> 16) > VAMP_HI_LIMIT)
            {
                i32Inv1RMSAmpVref_T = (int32)VAMP_HI_LIMIT << 16;
            }
            else if((int16)(i32Inv1RMSAmpVref_T >> 16) < VAMP_LOW_LIMIT)
            {
                i32Inv1RMSAmpVref_T = (int32)VAMP_LOW_LIMIT << 16;
            }


            //for interrupt use
            DINT;
            i32Inv1AmpVref_R = i32Inv1RMSAmpVref_R >> 12;
            i32Inv1AmpVref_S = i32Inv1RMSAmpVref_S >> 12;
            i32Inv1AmpVref_T = i32Inv1RMSAmpVref_T >> 12;
            EINT;

            ADC_FLAG.sBit.RMS_CONTROL = 0;
        }
    }
    else
    {
        if(!CHK_INVERTER_ENABLE)    //如果逆变器有延迟关闭时，等关闭后再清除控制的的值
        {
            u16RmsErrorDelayCnt = 0;

            i16InvRmsErr_R = 0;
            i16InvRmsErr_S = 0;
            i16InvRmsErr_T = 0;
            i16InvRmsLastErr_R = 0;
            i16InvRmsLastErr_S = 0;
            i16InvRmsLastErr_T = 0;

            if( !SourceControlFlag.sBit.Program_Init_Wait &&
                !SourceControlFlag.sBit.Program_Running)
            {
                if(CONFIG_FLAG.sBit.InverterSoftstartEnable)
                {
                    DINT;
                    i32Inv1RMSAmpVref_R = 0;
                    i32Inv1RMSAmpVref_S = 0;
                    i32Inv1RMSAmpVref_T = 0;
                    i32Inv1AmpVref_R = 0;    //softstart
                    i32Inv1AmpVref_S = 0;
                    i32Inv1AmpVref_T = 0;
                    EINT;
                }
                else
                {
                    Adjust_Pwm_Gain(0x07);
                }
            }
        }
    }
}


void TaskDryContact(void)
{
    static Uint16 u16RestartCnt = 0;

    //设置回灌使能干接点
    if(CHK_INPUT_RELAY_ON &&
       //u16DelayOn == 0 &&
       sSystemFlag.sBit.u1Ready &&
       FAULT1_FLAG.WORD == 0)
    {
        if(u16PowerRecycle == 1)
        {
            if(FAULT1_FLAG.WORD == 0)
            {
                DRY_OUTPUT1_1;
            }
            else
            {
                DRY_OUTPUT1_0;
            }
        }
        else
        {
            DRY_OUTPUT1_0;
        }
    }
    else
    {
        DRY_OUTPUT1_0;

        u16RestartCnt = 0;
    }


    //设定输出电流
    if(SourceControlFlag.sBit.OutputOn)
    {
        switch(u16PowerRecycleLevel)
        {
            case 3:
                DRY_OUTPUT3_1;
                DRY_OUTPUT2_1;
                break;

            case 2:
                DRY_OUTPUT3_1;
                DRY_OUTPUT2_0;
                break;

            case 1:
                DRY_OUTPUT3_0;
                DRY_OUTPUT2_1;
                break;

            case 0:
                DRY_OUTPUT3_0;
                DRY_OUTPUT2_0;
                break;

            default:
                DRY_OUTPUT3_0;
                DRY_OUTPUT2_0;
                break;
        }
    }
    else
    {
        DRY_OUTPUT3_0;
        DRY_OUTPUT2_0;
    }
}


void SetOutputOff(void)
{
    if(SourceControlFlag.sBit.OutputOn)
    {
        u16DelayOn = 20;
    }
    SourceControlFlag.sBit.OutputOn = 0;

    Program_Stop();
    
    Updata_PD_RunTime();
    Updata_Power_Energy();
}

void SourceStatusDisplay(void)
{
    if(SourceControlFlag.sBit.OutputOn)
    {
        LCD_DATA1_PIN_1;
    }
    else
    {
        LCD_DATA1_PIN_0;
    }

    //故障LED显示
    if(FAULT1_FLAG.WORD)
    {
        LCD_DATA0_PIN_1;
    }
    else
    {
        LCD_DATA0_PIN_0;
    }
}

/******************************************************************************
 BASE TIME UNIT 100MS
******************************************************************************/
void TimeBase100ms(void)
{
    u16DisplayTime++;

    // check DCBUS delay time
    if (u16BusDelayTime)
    {
        u16BusDelayTime--;
    }

    if(u16DelayOn)
    {
        u16DelayOn--;
    }

    if(SourceControlFlag.sBit.Program_Init_Wait)
    {
        sProgRun.u16InitWaitCnt++;
    }

    TaskBuzzerControl();

    Check_Over_Voltage();

    SourceStatusDisplay();

    Check_Transfermer_OTP();
    Check_EPO();
    
    #ifdef OP_PROTECT_SLOW
    Check_Over_Current_Time();
    Check_Over_Load_Time();
    #endif
    
    HighLowRange_Relay_Control();
}

//----------------------------------------------------------------------------
// BASE TIME UNIT 1 SEC
//----------------------------------------------------------------------------
void TimeBase1Sec(void)
{
    if(u16LcdBacklightOnTime)
    {
        u16LcdBacklightOnTime--;
    }

    Check_VFSet_Mode_Time();

    #ifdef RTC_ENABLE
    Get_RTC_Time();
    #endif

    CalculateKWH();

    Input_Relay_Control();

    Check_Updata_Eeprom();

    FanSpeedControl();
    IpeakControl();
    
    RunTime();
}

/******************************************************************************
 BASE TIME UNIT 1 MIN
******************************************************************************/
void TimeBase1Min(void)
{
    //Run_Time_Limit_Countdown();
}

