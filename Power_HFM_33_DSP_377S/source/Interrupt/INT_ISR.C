/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : INT_ISR.C

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "USR_VAR.H"
#include "INT_ISR.H"
#include "mainUPS.h"
#include "modbus.h"
#include "Measurement.h"

//-----------------------------------------------------------------------
Uint16  u16AdcCountTemp;                // AD count temp register
Uint16  u16BpsAdcAbsBuf;                // Bypass   A/D count temp for Watt
Uint16  u16OpvAdcAbsBuf;                // O/P V    A/D count temp for Watt
Uint16  u16OpiAdcAbsBuf;                // O/P I    A/D count temp for Watt
Uint16  u16IpAdcAbsBufR;                // AC IN R   A/D count temp for Fast
Uint16  u16OpVoltBuff;                  // O/P V A/D count temp for output vol

Uint16  u16Inv1AdcCountBuf;             // INVERTER1 A/D count temp
//Uint16  u16Inv1AbsAdBuf;              // INVERTER1 abs(A/D count) temp

Uint16  u16DelayBypassTime;             // Delay to Bypass time

Uint16  u16PosBusOvpCnt;                // DCBUS + OVP count
Uint16  u16NegBusOvpCnt;                // DCBUS - OVP count
Uint16  u16PosBusOvpDeltaCnt2;          // DCBUS + OVP delta vol count 2
Uint16  u16NegBusOvpDeltaCnt2;          // DCBUS - OVP delta vol count 2

ni32HiLoWord_t si32InvPwmDutyR, si32InvPwmDutyS, si32InvPwmDutyT;
ni32HiLoWord_t si32InvPwmDutyRn, si32InvPwmDutySn, si32InvPwmDutyTn;

//-----------------------------------------------------------------------
// check RS232 time out(100ms)
// Timebase: 1ms
//-----------------------------------------------------------------------
#pragma CODE_SECTION(IsrSciTimeOutDetect, "ramfuncs");
void IsrSciTimeOutDetect(void)
{
    /*
       if (u8RxCount[0] > 0)
       {
           if (u8RxTimeoutCnt[0] > RS232_RX_TIMOUT)
           {
               u8RxCount[0] = 0;
               u8RxTimeoutCnt[0] = 0;
               sSciFlag[0].sBit.RX_HEAD_IN = 0;
           }
           else
           {
               u8RxTimeoutCnt[0]++;
           }
       }

       if (u8RxCount[1] > 0)
       {
           if (u8RxTimeoutCnt[1] > RS232_RX_TIMOUT)
           {
               u8RxCount[1] = 0;
               u8RxTimeoutCnt[1] = 0;
               sSciFlag[1].sBit.RX_HEAD_IN = 0;
           }
           else
           {
               u8RxTimeoutCnt[1]++;
           }
       }

       if (u8RxCount[2] > 0)
       {
           if (u8RxTimeoutCnt[2] > RS232_RX_TIMOUT)
           {
               u8RxCount[2] = 0;
               u8RxTimeoutCnt[2] = 0;
               sSciFlag[2].sBit.RX_HEAD_IN = 0;
           }
           else
           {
               u8RxTimeoutCnt[2]++;
           }
       }*/
}


//-----------------------------------------------------------------------------
//SCI-A TX/RX service subroutine
//
//RS485
//-----------------------------------------------------------------------------
//#pragma CODE_SECTION(Scia_ISR, "ramfuncs");
void Scia_ISR(void)
{
    static Uint8 u8Data;

    // ***** Receive data *****
    if (SciaRegs.SCIFFRX.bit.RXFFST != 0)       // Receive FIFO empty ?
    {
        u8Data = (unsigned char)(SciaRegs.SCIRXBUF.bit.SAR & 0x00FF);

        Isr_MBus_HandleMBusRx(0, u8Data);
    }

    // ***** Check Error *****
    if (SciaRegs.SCIRXST.bit.RXERROR)           // Receive Error ?
    {
        SciaRegs.SCICTL1.bit.SWRESET = 0;       // Initialize the SCI state machines and flags
        SciaRegs.SCICTL1.bit.SWRESET = 1;       // Re-enable the SCI
    }

    // ***** Transmit data *****
    if (SciaRegs.SCIFFTX.bit.TXFFST == 0)       // Transmit FIFO empty ?
    {
        if (sModbusRxTx[0].u8fState == MBUS_STATE_TX)
        {
            if (sModbusRxTx[0].i16IndexTx < sModbusRxTx[0].i16LenTx)
            {
                SciaRegs.SCITXBUF.bit.TXDT = sModbusRxTx[0].u8BufTx[sModbusRxTx[0].i16IndexTx];
                sModbusRxTx[0].i16IndexTx++;
            }
            else
            {
                sModbusRxTx[0].i16LenRx = 0;
                sModbusRxTx[0].u16RxTimer = 0;
                sModbusRxTx[0].i16IndexTx = 0;
                sModbusRxTx[0].i16LenTx = 0;
                sModbusRxTx[0].u8fState = MBUS_STATE_RX;
                sModbusRxTx[0].u8fRxOK = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------
//SCI-B TX/RX service subroutine
//
//For LCD communication
//-----------------------------------------------------------------------------
//#pragma CODE_SECTION(Scib_ISR, "ramfuncs");
void Scic_ISR(void)
{
    static Uint8 u8Data;

    // ***** Receive data *****
    if (ScicRegs.SCIFFRX.bit.RXFFST != 0) // Receive FIFO empty ?
    {
        u8Data = (unsigned char)(ScicRegs.SCIRXBUF.bit.SAR & 0x00FF);

        Isr_MBus_HandleMBusRx(2, u8Data);
    }

    // ***** Check Error *****
    if (ScicRegs.SCIRXST.bit.RXERROR) // Receive Error ?
    {
        ScicRegs.SCICTL1.bit.SWRESET = 0; // Initialize the SCI state machines and flags
        ScicRegs.SCICTL1.bit.SWRESET = 1; // Re-enable the SCI
    }

    // ***** Transmit data *****
    if (ScicRegs.SCIFFTX.bit.TXFFST == 0) // Transmit FIFO empty ?
    {
        if (sModbusRxTx[2].u8fState == MBUS_STATE_TX)
        {
            if (sModbusRxTx[2].i16IndexTx < sModbusRxTx[2].i16LenTx)
            {
                ScicRegs.SCITXBUF.bit.TXDT = sModbusRxTx[2].u8BufTx[sModbusRxTx[2].i16IndexTx];
                sModbusRxTx[2].i16IndexTx++;
            }
            else
            {
                sModbusRxTx[2].i16LenRx = 0;
                sModbusRxTx[2].u16RxTimer = 0;
                sModbusRxTx[2].i16IndexTx = 0;
                sModbusRxTx[2].i16LenTx = 0;
                sModbusRxTx[2].u8fState = MBUS_STATE_RX;
                sModbusRxTx[2].u8fRxOK = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------
//SCI-C TX/RX service subroutine
//
//RS232
//-----------------------------------------------------------------------------
//#pragma CODE_SECTION(Scic_ISR, "ramfuncs");
/*void Scic_ISR(void)
{
    static Uint8 u8Data;

    // ***** Receive data *****
    if (ScicRegs.SCIRXST.bit.RXRDY != 0) // Receive FIFO empty ?
    {
        u8Data = (unsigned char)(ScicRegs.SCIRXBUF.bit.SAR & 0x00FF);

        Isr_MBus_HandleMBusRx(1, u8Data);
    }

    // ***** Check Error *****
    if (ScicRegs.SCIRXST.bit.RXERROR) // Receive Error ?
    {
        ScicRegs.SCICTL1.bit.SWRESET = 0; // Initialize the SCI state machines and flags
        ScicRegs.SCICTL1.bit.SWRESET = 1; // Re-enable the SCI
    }

    // ***** Transmit data *****
    if (ScicRegs.SCICTL2.bit.TXEMPTY) // Transmit FIFO empty ?
    {
        if (sModbusRxTx[1].u8fState == MBUS_STATE_TX)
        {
            if (sModbusRxTx[1].i16IndexTx < sModbusRxTx[1].i16LenTx)
            {
                ScicRegs.SCITXBUF.bit.TXDT = sModbusRxTx[1].u8BufTx[sModbusRxTx[1].i16IndexTx];
                sModbusRxTx[1].i16IndexTx++;
            }
            else
            {
                sModbusRxTx[1].i16LenRx = 0;
                sModbusRxTx[1].u16RxTimer = 0;
                sModbusRxTx[1].i16IndexTx = 0;
                sModbusRxTx[1].i16LenTx = 0;
                sModbusRxTx[1].u8fState = MBUS_STATE_RX;
                sModbusRxTx[1].u8fRxOK = 0;
            }
        }
    }
}*/

//-----------------------------------------------------------------------------
//SCI-D TX/RX service subroutine
//
//RS232
//-----------------------------------------------------------------------------
void Scid_ISR(void)
{
    static Uint8 u8Data;

    // ***** Receive data *****
    if (ScidRegs.SCIFFRX.bit.RXFFST != 0) // Receive FIFO empty ?
    {
        u8Data = (unsigned char)(ScidRegs.SCIRXBUF.bit.SAR & 0x00FF);

        Isr_MBus_HandleMBusRx(1, u8Data);
    }

    // ***** Check Error *****
    if (ScidRegs.SCIRXST.bit.RXERROR) // Receive Error ?
    {
        ScidRegs.SCICTL1.bit.SWRESET = 0; // Initialize the SCI state machines and flags
        ScidRegs.SCICTL1.bit.SWRESET = 1; // Re-enable the SCI
    }

    // ***** Transmit data *****
    if (ScidRegs.SCIFFTX.bit.TXFFST == 0) // Transmit FIFO empty ?
    {
        if (sModbusRxTx[1].u8fState == MBUS_STATE_TX)
        {
            if (sModbusRxTx[1].i16IndexTx < sModbusRxTx[1].i16LenTx)
            {
                ScidRegs.SCITXBUF.bit.TXDT = sModbusRxTx[1].u8BufTx[sModbusRxTx[1].i16IndexTx];
                sModbusRxTx[1].i16IndexTx++;
            }
            else
            {
                sModbusRxTx[1].i16LenRx = 0;
                sModbusRxTx[1].u16RxTimer = 0;
                sModbusRxTx[1].i16IndexTx = 0;
                sModbusRxTx[1].i16LenTx = 0;
                sModbusRxTx[1].u8fState = MBUS_STATE_RX;
                sModbusRxTx[1].u8fRxOK = 0;
            }
        }
    }
}

#define DC_BUS_OVP  3803        //500V
#pragma CODE_SECTION(IsrBusOvpDetect, "ramfuncs");
void IsrBusOvpDetect(void)
{
    static Uint16 u16BusOvtCnt_R, u16BusOvtCnt_S, u16BusOvtCnt_T;

    if(sADC_Measure.i16D2dBus_R > DC_BUS_OVP)
    {
        if(u16BusOvtCnt_R < 10)
        {
            u16BusOvtCnt_R++;
        }
        else
        {
            BUS_FLAG.sBit.u1BusOvp_R = 1;
        }
    }
    else
    {
        if(u16BusOvtCnt_R > 0)
        {
            u16BusOvtCnt_R--;
        }
        else
        {
            BUS_FLAG.sBit.u1BusOvp_R = 0;
        }
    }

    /*if(sADC_Measure.i16D2dBus_S > DC_BUS_OVP)
    {
        if(u16BusOvtCnt_S < 10)
            u16BusOvtCnt_S++;
        else
            BUS_FLAG.sBit.u1BusOvp_S = 1;
    }
    else
    {
        if(u16BusOvtCnt_S > 0)
            u16BusOvtCnt_S--;
        else
            BUS_FLAG.sBit.u1BusOvp_S = 0;
    }

    if(sADC_Measure.i16D2dBus_T > DC_BUS_OVP)
    {
        if(u16BusOvtCnt_T < 10)
            u16BusOvtCnt_T++;
        else
            BUS_FLAG.sBit.u1BusOvp_T = 1;
    }
    else
    {
        if(u16BusOvtCnt_T > 0)
            u16BusOvtCnt_T--;
        else
            BUS_FLAG.sBit.u1BusOvp_T = 0;
    }*/
}

#pragma CODE_SECTION(ErrorLimit, "ramfuncs");
void ErrorLimit(int16 *pi16Data, int16 i16HighLimit, int16 i16LowLimit)
{
    if (*pi16Data > i16HighLimit)
    {
        *pi16Data = i16HighLimit;
    }
    else if (*pi16Data < i16LowLimit)
    {
        *pi16Data = i16LowLimit;
    }
}


//------------------------------------------------------------------------------
// System interrupt service subroutine
// Timebase: 1ms
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrCheckDryInput, "ramfuncs");
void IsrCheckDryInput(void)
{
    static Uint16 u16Dry1Cnt = 0;
    static Uint16 u16Dry2Cnt = 0;
    static Uint16 u16Dry3Cnt = 0;

    if(CHK_DRY_INPUT1)
    {
        if(u16Dry1Cnt < 500)
        {
            u16Dry1Cnt++;
        }
        else
        {
            DryContactFlag.sBit.u1Input1 = 1;
        }
    }
    else
    {
        if(u16Dry1Cnt > 5)
        {
            u16Dry1Cnt -= 5;
        }
        else
        {
            u16Dry1Cnt = 0;
            DryContactFlag.sBit.u1Input1 = 0;
        }
    }

    if(CHK_DRY_INPUT2)
    {
        if(u16Dry2Cnt < 500)
        {
            u16Dry2Cnt++;
        }
        else
        {
            DryContactFlag.sBit.u1Input2 = 1;
        }
    }
    else
    {
        if(u16Dry2Cnt > 5)
        {
            u16Dry2Cnt -= 5;
        }
        else
        {
            u16Dry2Cnt = 0;
            DryContactFlag.sBit.u1Input2 = 0;
        }
    }

    if(CHK_DRY_INPUT3)
    {
        if(u16Dry3Cnt < 500)
        {
            u16Dry3Cnt++;
        }
        else
        {
            DryContactFlag.sBit.u1Input3 = 1;
        }
    }
    else
    {
        if(u16Dry3Cnt > 5)
        {
            u16Dry3Cnt -= 5;
        }
        else
        {
            u16Dry3Cnt = 0;
            DryContactFlag.sBit.u1Input3 = 0;
        }
    }
}
//------------------------------------------------------------------------------
//响应时间：从DSP设定到同步触发信号发生延时 100us，同步脉冲信号高电平维持 5ms，
//频率在一个工频周期内响应，电压在1ms内响应。 
// Timebase: 100us
//------------------------------------------------------------------------------
void IsrSyncPlusSignal(void)
{
    static Uint16 ut6SyncSignalCnt = 0;
    if(DryContactFlag.sBit.ProgVoltFreqUpDown || DryContactFlag.sBit.ManualVoltFreqUpDown)
    {
        DryContactFlag.sBit.ProgVoltFreqUpDown = 0;
        DryContactFlag.sBit.ManualVoltFreqUpDown = 0;

        ut6SyncSignalCnt = 50;

        INVERTER_STEP_SINGEL_1;
    }
    else
    {
        if(ut6SyncSignalCnt > 0)
        {
            ut6SyncSignalCnt--;
        }
        else
        {
            INVERTER_STEP_SINGEL_0;
        }
    }
}

//圆周率系数
#define COEF_2PI	        6.283185307F    // 2π
#define COEF_2PI_120DEG	    2.094395102F	// 2π / 3
#define COEF_2PI_PHS	    0.017453292F	// 2π / 360
//------------------------------------------------------------------------------
// PWMA
// Timebase:
//
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrInverterPWM_A, "ramfuncs");
void IsrInverterPWM_A(void)
{
    static float32 f32PhaseData;

    //f32PhaseData = __divf32((CpuTimer1.RegsAddr->PRD.all - CpuTimer1.RegsAddr->TIM.all), CpuTimer1.RegsAddr->PRD.all) * COEF_2PI;
    f32PhaseData = __divf32(ECap3Regs.TSCTR, ECap3Regs.CAP1) * COEF_2PI;

    si32InvPwmDutyR.i32DbWord = (float32)i32Inv1AmpVref_R * (4096.0 * __sin(f32PhaseData));
    si32InvPwmDutyS.i32DbWord = (float32)i32Inv1AmpVref_S * (4096.0 * __sin(f32PhaseData + COEF_2PI_120DEG * 2.0));
    si32InvPwmDutyT.i32DbWord = (float32)i32Inv1AmpVref_T * (4096.0 * __sin(f32PhaseData + COEF_2PI_120DEG));
    
    //增加0.5%二次谐波注入，以抵消实际产生的二次谐波
    //si32InvPwmDutyR.i32DbWord = i32Inv1AmpVref_R * ((__sin(f32PhaseData) + __sin(f32PhaseData*5)*0.008)*4096.0);
    //si32InvPwmDutyS.i32DbWord = i32Inv1AmpVref_S * ((__sin(f32PhaseData + COEF_2PI_120DEG * 2) + __sin((f32PhaseData + COEF_2PI_120DEG * 2)*5)*0.008)*4096.0);
    //si32InvPwmDutyT.i32DbWord = i32Inv1AmpVref_T * ((__sin(f32PhaseData + COEF_2PI_120DEG) + __sin((f32PhaseData + COEF_2PI_120DEG)*5)*0.008)*4096.0);

    ErrorLimit(&si32InvPwmDutyR.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);
    ErrorLimit(&si32InvPwmDutyS.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);
    ErrorLimit(&si32InvPwmDutyT.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);

    //updata the PWM duty
    EPwm1Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutyR.sWord.i16HiWord;
    EPwm3Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutyS.sWord.i16HiWord;
    EPwm5Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutyT.sWord.i16HiWord;
}

//------------------------------------------------------------------------------
// PWMA
// Timebase:
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrInverterPWM_B, "ramfuncs");
void IsrInverterPWM_B(void)
{
    static float32 f32PhaseData;

    //f32PhaseData = __divf32((CpuTimer1.RegsAddr->PRD.all - CpuTimer1.RegsAddr->TIM.all), CpuTimer1.RegsAddr->PRD.all) * COEF_2PI;
    f32PhaseData = __divf32(ECap3Regs.TSCTR, ECap3Regs.CAP1) * COEF_2PI;

    si32InvPwmDutyRn.i32DbWord = (float32)i32Inv1AmpVref_R * (4096.0 * __sin(f32PhaseData));                         
    si32InvPwmDutySn.i32DbWord = (float32)i32Inv1AmpVref_S * (4096.0 * __sin(f32PhaseData + COEF_2PI_120DEG * 2.0));   
    si32InvPwmDutyTn.i32DbWord = (float32)i32Inv1AmpVref_T * (4096.0 * __sin(f32PhaseData + COEF_2PI_120DEG));       

    ErrorLimit(&si32InvPwmDutyRn.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);
    ErrorLimit(&si32InvPwmDutySn.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);
    ErrorLimit(&si32InvPwmDutyTn.sWord.i16HiWord, INV_DUTY_HIGH_LIMIT, INV_DUTY_LOW_LIMIT);

    //updata the PWM duty
    EPwm2Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutyRn.sWord.i16HiWord;
    EPwm4Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutySn.sWord.i16HiWord;
    EPwm6Regs.CMPA.bit.CMPA = (INV_SW_HALF_PERIOD / 2) + si32InvPwmDutyTn.sWord.i16HiWord;

}

//------------------------------------------------------------------------------
// System interrupt service subroutine
// Timebase: 
//低频(45~65Hz)：每个AC周期均分为360个点,进行采样（中断周期50Hz~=55.56us）
//中频(350~450Hz)：每个AC周期均分为180个点,进行采样（中断周期450Hz~=12.35us）
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrInverterController, "ramfuncs");
void IsrInverterController(void)
{
    static Uint32 u32SystemControlCnt = 0;

    WDG_CLK_TRG;

    Uint8 u16Data;
    Uint16 i_uint, u16Data1;
    Uint32 u32Data;
    Uint16 freq_t;

    u16SineWaveIndex++;
    if(u16SineWaveIndex >= SINEPOT)
    {
        u16SineWaveIndex = 0;
    }

    u32TimeBase = (ECap3Regs.CAP1 / SINEPOT);
    u32TimeBaseRem = (ECap3Regs.CAP1 % SINEPOT);
    
    if(u16SineWaveIndex < u32TimeBaseRem)
    {
        u32IsrTimeBase = u32TimeBase + 1;
    }
    else
    {
        u32IsrTimeBase = u32TimeBase;
    }
    CpuTimer0.RegsAddr->PRD.all = u32IsrTimeBase;

    if(SourceControlFlag.sBit.OutputOn)
    {
        if(sSet.u16OutputOnAngleDo == u16SineWaveIndex && !CHK_INVERTER_ENABLE)
        {
            INVERTER_ENABLE_R;
            INVERTER_ENABLE_S;
            INVERTER_ENABLE_T;
        }
    }
    else
    {
        if(u16SineWaveIndex == 22 ||        //增加输出过0点时关闭
           u16SineWaveIndex == (SEMI_SIN + 22) ||
           FAULT1_FLAG.WORD)    //增加故障时立即关闭
        {
            INVERTER_DISABLE_R;
            INVERTER_DISABLE_S;
            INVERTER_DISABLE_T;
        }
    }

}


//------------------------------------------------------------------------------
// System interrupt service subroutine
// Timebase: 50us
//------------------------------------------------------------------------------
#pragma CODE_SECTION(SystemControl_ISR, "ramfuncs");
void SystemControl_ISR(void)
{
    static Uint32 u32SystemControlCnt = 0;

    WDG_CLK_TRG;

    Uint8 u16Data;
    Uint16 i_uint, u16Data1;
    Uint32 u32Data;
    Uint16 freq_t;

    if(SourceControlFlag.sBit.Program_Running)
    {
        if(sProgRun.u16StepChangeAdjustDelay > 0)
        {
            if(sProgRun.u16StartPoint == u16SineWaveIndex)
            {
                sProgRun.u16StepChangeAdjustDelay--;
            }
        }
    }


    // ---------------------------------------------
    // 100us time base
    // ---------------------------------------------
    u32SystemControlCnt += u32IsrTimeBase;            // +Time base
    if (u32SystemControlCnt >= 20000)            // 100us ?
    {
        u32SystemControlCnt -= 20000;

        u16MicroSec++;
        if (u16MicroSec >= 10)                   // = 1ms ? 100us * 10 = 1ms
        {
            u16MicroSec = 0;

            sTimesFlag.sBit.u1TimeBase1ms = 1;

            // Timer
            u16Mini1ms++;                                   // u16Mini1ms+1
            if (u16Mini1ms >= 10)              		// >=10 ms ?
            {
                u16Mini1ms -= 10;
                u16Mini10ms++;
                sTimesFlag.sBit.u1TimeBase10ms = 1;
            }

            //IsrCheckDryInput();

            if(SourceControlFlag.sBit.Program_Running)
            {
                sProgRun.u32TimeCnt++;		//1ms time base
            }

            //#ifndef DEBUG_NO_PROTECT
            if (CHK_INVERTER_ENABLE)
            {
                if(!CHK_INV_I_PEAK_PROTECT)
                {
                    if(u16InvIPeakProtectCnt > 10)
                    {
                        FAULT1_FLAG.sBit.InvIPeakProtect = 1;
                    }
                    else
                    {
                        u16InvIPeakProtectCnt++;
                    }
                }
                else
                {
                    u16InvIPeakProtectCnt = 0;
                }

                if(CHK_316_SC_R || CHK_316_SC_S || CHK_316_SC_T)
                {
                    if(u16Inv316ScProtectCnt >= 10)
                    {
                        FAULT1_FLAG.sBit.bInv316ScProtect = 1;
                    }
                    else
                    {
                        u16Inv316ScProtectCnt++;
                    }
                }
                else
                {
                    u16Inv316ScProtectCnt = 0;
                }
            }
            else
            {
                u16InvIPeakProtectCnt = 0;
                u16Inv316ScProtectCnt = 0;
            }
            //#endif

        }

        IsrBusOvpDetect();
        IsrSyncPlusSignal();

    }


}



