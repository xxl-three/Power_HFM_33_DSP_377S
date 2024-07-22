/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Measurement.c

VERSION  : V0.0

DATE     : 2018/5/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/


#include "F28x_Project.h"     // DSP2833x Headerfile Include File
#include "math.h"
#include "Measurement.h"
#include "USR_VAR.H"
#include "Booter.H"
#include "RTC.H"
#include "Eeprom.h"

//----------------------------------------------------------------------------
//adc
struct _AdcMean sADC_Mean;
struct _AdcMeasure sADC_Measure;
struct _AdcSum sAdcSum, sAdcSumBuff;
struct _AdcCal sAdcCal;
struct _sRMS sRMS;


nu32HiLoWord_t su32ADC256Acc;
Uint16 u16InvADCCalCnt, u16InvADCCalFixCnt;
//int16 i16BC2_C;
int16 i16AdcOffset, i16AdcOffsetDelta, i16AdcOffsetLast;

nu32HiLoWord_t su32PfcADC256Acc;
Uint16 u16PfcADCCalCnt, u16PfcADCCalFixCnt;
int16 i16CurrentOffCnt;


//flag
union   UN_ADC_SP_FLAG sAdcOk;

Uint16 u16CD4052OkChannel;
Uint16	u16Adc4052PfcCount1, u16Adc4052PfcCount2, u16Adc4052InvCount1, u16Adc4052InvCount2;

// I/P Volt
Uint16 u16RmsIpRVoltBuff[3][5], u16RmsBpsVoltBuff[3][5];
Uint16 u16RmsIpRVoltBuff_Sum[3], u16RmsBpsVoltBuff_Sum[3];

// O/P Volt
Uint16 u16RmsInv1VoltBuff[3][5], u16RmsOpVoltBuff[3][5];
Uint16 u16RmsInv1VoltBuff_Sum[3], u16RmsOpVoltBuff_Sum[3];

Uint16 u16RmsOpCurrBuff[3][5];
Uint16 u16RmsOpCurrBuff_Sum[3];

// Inverter current
Uint16 u16RmsInv1LCurrBuff[5];
Uint16 u16RmsInv1LCurrBuff_Sum[3];

int32 i32OpWattBuff[3][5];
int32 i32OpWattBuff_Sum[3];

int32 i32OpVaBuff[3][5];
int32 i32OpVaBuff_Sum[3];

// Temperature
Uint16 u16TempInv1Buff[3][5];
Uint16 u16TempInv1Buff_Sum[3];
Uint16 u16TempPfc1Buff[3][5];
Uint16 u16TempPfc1Buff_Sum[3];
Uint16 u16TempEnvBuff[5];
Uint16 u16TempEnvBuff_Sum;

// BUS Volt
Uint16 u16RmsRectBusBuff[3][5], u16RmsD2dBusBuff[3][5];
Uint16 u16RmsRectBusBuff_Sum[3], u16RmsD2dBusBuff_Sum[3];

Uint32	u32OpCurADC_MovingSum;
Uint16	u16OpCurMovingSumIndex;
Uint16	u16OpCurMovingSumArray[128];

Uint16	u16OpCurrIndex, u16TakeOnCnt, u16TakeOffCnt;
Uint16  i16OpCurrDeltaValue;
Uint16  u16OpCurrAdcArray[256];

Uint32	u32InvVoltAdcMovingSum;
Uint16	u16InvVoltMovingSumIndex;
Uint16	u16InvVoltAdcMovingSumArray[128];

static Uint16  u16BusPosHighCnt, u16BusPosLowCnt, u16BusNegHighCnt, u16BusNegLowCnt;    // DC BUS fail Count
//------------------------------------------------------------------------------
// Isr
// time base 58.8us
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrAdcSampling, "ramfuncs");
void IsrAdcSampling(void)
{    
    // temperature
    sADC_Measure.i16TempINV_R = ADC_NTC_I;
    sADC_Measure.i16TempINV_S = ADC_NTC_AMB;
    sADC_Measure.i16TempINV_T = ADC_NTC_B;        

    // AC recifier Volt --- ADC5(Result_2)
    sADC_Measure.i16D2dBus_R = ADC_VBUS_PFC_R;
    sADC_Measure.i16D2dBus_S = ADC_VBUS_PFC_S;
    sADC_Measure.i16D2dBus_T = ADC_VBUS_PFC_T;

    // INV Volt
    sADC_Measure.i16InvVolt_R = ADC_VINV_R - sAdcCal.i1P65Mean;
    sADC_Measure.i16InvVolt_S = ADC_VINV_S - sAdcCal.i1P65Mean;
    sADC_Measure.i16InvVolt_T = ADC_VINV_T - sAdcCal.i1P65Mean;
    
    // Output Current
    sADC_Measure.i16InvCurr_R = -(ADC_IINV_R - sAdcCal.i1P65Mean);
    sADC_Measure.i16InvCurr_S = -(ADC_IINV_S - sAdcCal.i1P65Mean);
    sADC_Measure.i16InvCurr_T = -(ADC_IINV_T - sAdcCal.i1P65Mean);

    
    // Output Voltage
    sADC_Measure.i16OpVolt_R = ADC_VOUT_R - sAdcCal.i1P65Mean;
    sADC_Measure.i16OpVolt_S = ADC_VOUT_S - sAdcCal.i1P65Mean;
    sADC_Measure.i16OpVolt_T = ADC_VOUT_T - sAdcCal.i1P65Mean;
    
    // Output Current
    sADC_Measure.i16OpCurr_R = -(ADC_IOUT_R - sAdcCal.i1P65Mean);
    sADC_Measure.i16OpCurr_S = -(ADC_IOUT_S - sAdcCal.i1P65Mean);
    sADC_Measure.i16OpCurr_T = -(ADC_IOUT_T - sAdcCal.i1P65Mean);

    sADC_Measure.i1P65Ref = ADC_1P65_REF;
}


//----------------------------------------------------------------------------
// check Inverter voltage moving sum
//----------------------------------------------------------------------------
#pragma CODE_SECTION(VoltageInverterMovingSum, "ramfuncs");
void VoltageInverterMovingSum(void)
{
    /*    //subtract the value of the oldest point
        if(u32InvVoltAdcMovingSum > u16InvVoltAdcMovingSumArray[u16InvVoltMovingSumIndex])
        {
            u32InvVoltAdcMovingSum -= u16InvVoltAdcMovingSumArray[u16InvVoltMovingSumIndex];
        }
        else
        {
            u32InvVoltAdcMovingSum = 0;
        }

        //insert the captured value to the latest point
        u16InvVoltAdcMovingSumArray[u16InvVoltMovingSumIndex] = abs(sADC_Measure.i16InvVolt_R);

        //add the latest value to the sum variable
        u32InvVoltAdcMovingSum += u16InvVoltAdcMovingSumArray[u16InvVoltMovingSumIndex];

        u16InvVoltMovingSumIndex++;
        if(u16InvVoltMovingSumIndex >= 128)
        {
            sInverterFlag.sBit.u1MovingSumStart = 1;
            u16InvVoltMovingSumIndex = 0;
        }

        if(sInverterFlag.sBit.u1MovingSumStart)
        {
            sAdcCal.u16InvMovingVol_R  = (Uint16)(((Uint32)u32InvVoltAdcMovingSum * u16Gain_InvMovingSum) >> 16);	// 0.1v
        }
    */
}

//------------------------------------------------------------------------------
// Isr
//
//------------------------------------------------------------------------------
#pragma CODE_SECTION(DcOffsetCalc, "ramfuncs");
void DcOffsetCalc(void)
{
    // DC Offset Sampling, Call Once At Power Up!
    if((sADC_Mean.i16StartCnt == START_ADC_MEAN_CNT) && (!sADC_Mean.StartMeanOK))
    {
        i16CurrentOffCnt++;

        // INV Volt
        sADC_Mean.i32SumInv1Vol += sADC_Measure.i16InvVolt_R;

        // Output Volt
        sADC_Mean.i32SumOPVolt	+= sADC_Measure.i16OpVolt_R;

        // Output Current
        sADC_Mean.i32SumOPCurr += sADC_Measure.i16OpCurr_R;

        if(i16CurrentOffCnt == 4096)  	// Call Once At Power On
        {
            // INV Volt
            sADC_Mean.i16InvVolt_R = sADC_Mean.i32SumInv1Vol >> 12;

            sADC_Mean.i16OPVolt = sADC_Mean.i32SumOPVolt >> 12;
            // Output Current
            sADC_Mean.i16OpCurr_R = sADC_Mean.i32SumOPCurr >> 12;

            // Output Current
            if(abs(sADC_Mean.i16OpCurr_R) > 400)
            {
                sADC_Mean.i16OpCurr_R = 0;
            }

            sADC_Mean.StartMeanOK = 1;
        }
    }
}


//------------------------------------------------------------------------------
// Isr
// time base 58.8us
//------------------------------------------------------------------------------
#pragma CODE_SECTION(IsrAdcSum, "ramfuncs");
void IsrAdcSum(void)
{
    static unsigned int u16TempCnt = 0;
    static unsigned int u16IpvSumCnt = 0;
    static Uint32 u32IpvSumTimeCnt = 0;

    sAdcSum.u32D2dBus_R += (sADC_Measure.i16D2dBus_R);
    sAdcSum.u32InvVolt_R += (Uint32)((int32)sADC_Measure.i16InvVolt_R * sADC_Measure.i16InvVolt_R);
    sAdcSum.u32OpVolt_R += (Uint32)((int32)sADC_Measure.i16OpVolt_R * sADC_Measure.i16OpVolt_R);
    sAdcSum.u32OpCurr_R += (Uint32)((int32)sADC_Measure.i16OpCurr_R * sADC_Measure.i16OpCurr_R);
    sAdcSum.i32OpWatt_R += ((int32)sADC_Measure.i16OpVolt_R * sADC_Measure.i16OpCurr_R);

    sAdcSum.u32D2dBus_S += (sADC_Measure.i16D2dBus_S);
    sAdcSum.u32InvVolt_S += (Uint32)((int32)sADC_Measure.i16InvVolt_S * sADC_Measure.i16InvVolt_S);
    sAdcSum.u32OpVolt_S += (Uint32)((int32)sADC_Measure.i16OpVolt_S * sADC_Measure.i16OpVolt_S);
    sAdcSum.u32OpCurr_S += (Uint32)((int32)sADC_Measure.i16OpCurr_S * sADC_Measure.i16OpCurr_S);
    sAdcSum.i32OpWatt_S += ((int32)sADC_Measure.i16OpVolt_S * sADC_Measure.i16OpCurr_S);

    sAdcSum.u32D2dBus_T += (sADC_Measure.i16D2dBus_T);
    sAdcSum.u32InvVolt_T += (Uint32)((int32)sADC_Measure.i16InvVolt_T * sADC_Measure.i16InvVolt_T);
    sAdcSum.u32OpVolt_T += (Uint32)((int32)sADC_Measure.i16OpVolt_T * sADC_Measure.i16OpVolt_T);
    sAdcSum.u32OpCurr_T += (Uint32)((int32)sADC_Measure.i16OpCurr_T * sADC_Measure.i16OpCurr_T);
    sAdcSum.i32OpWatt_T += ((int32)sADC_Measure.i16OpVolt_T * sADC_Measure.i16OpCurr_T);
    
    sAdcSum.u32TempINV_R += sADC_Measure.i16TempINV_R;
    sAdcSum.u32TempINV_S += sADC_Measure.i16TempINV_S;
    sAdcSum.u32TempINV_T += sADC_Measure.i16TempINV_T;
    sAdcSum.u321P65 += sADC_Measure.i1P65Ref;
    

    if(u16SineWaveIndex == 0)
    {
        sAdcSumBuff.u32D2dBus_R  = sAdcSum.u32D2dBus_R ;
        sAdcSumBuff.u32InvVolt_R = sAdcSum.u32InvVolt_R;
        sAdcSumBuff.u32OpVolt_R  = sAdcSum.u32OpVolt_R ;
        sAdcSumBuff.u32OpCurr_R  = sAdcSum.u32OpCurr_R ;
        sAdcSumBuff.i32OpWatt_R  = sAdcSum.i32OpWatt_R ;
        sAdcSum.u32D2dBus_R  = 0;
        sAdcSum.u32InvVolt_R = 0;
        sAdcSum.u32OpVolt_R  = 0;
        sAdcSum.u32OpCurr_R  = 0;
        sAdcSum.i32OpWatt_R  = 0;
        sAdcSumBuff.u32D2dBus_S  = sAdcSum.u32D2dBus_S ;
        sAdcSumBuff.u32InvVolt_S = sAdcSum.u32InvVolt_S;
        sAdcSumBuff.u32OpVolt_S  = sAdcSum.u32OpVolt_S ;
        sAdcSumBuff.u32OpCurr_S  = sAdcSum.u32OpCurr_S ;
        sAdcSumBuff.i32OpWatt_S  = sAdcSum.i32OpWatt_S ;
        sAdcSum.u32D2dBus_S  = 0;
        sAdcSum.u32InvVolt_S = 0;
        sAdcSum.u32OpVolt_S  = 0;
        sAdcSum.u32OpCurr_S  = 0;
        sAdcSum.i32OpWatt_S  = 0;
        sAdcSumBuff.u32D2dBus_T  = sAdcSum.u32D2dBus_T ;
        sAdcSumBuff.u32InvVolt_T = sAdcSum.u32InvVolt_T;
        sAdcSumBuff.u32OpVolt_T  = sAdcSum.u32OpVolt_T ;
        sAdcSumBuff.u32OpCurr_T  = sAdcSum.u32OpCurr_T ;
        sAdcSumBuff.i32OpWatt_T  = sAdcSum.i32OpWatt_T ;
        sAdcSum.u32D2dBus_T  = 0;
        sAdcSum.u32InvVolt_T = 0;
        sAdcSum.u32OpVolt_T  = 0;
        sAdcSum.u32OpCurr_T  = 0;
        sAdcSum.i32OpWatt_T  = 0;

        sAdcSumBuff.u32TempINV_R = sAdcSum.u32TempINV_R;
        //sAdcSumBuff.u32TempPFC_R = sAdcSum.u32TempPFC_R;
        sAdcSumBuff.u32RectBus_R = sAdcSum.u32RectBus_R;
        sAdcSum.u32TempINV_R = 0;
        //sAdcSum.u32TempPFC_R = 0;
        sAdcSum.u32RectBus_R = 0;
        sAdcSumBuff.u32TempINV_S = sAdcSum.u32TempINV_S;
        //sAdcSumBuff.u32TempPFC_S = sAdcSum.u32TempPFC_S;
        sAdcSumBuff.u32RectBus_S = sAdcSum.u32RectBus_S;
        sAdcSum.u32TempINV_S = 0;
        //sAdcSum.u32TempPFC_S = 0;
        sAdcSum.u32RectBus_S = 0;
        sAdcSumBuff.u32TempINV_T = sAdcSum.u32TempINV_T;
        //sAdcSumBuff.u32TempPFC_T = sAdcSum.u32TempPFC_T;
        sAdcSumBuff.u32RectBus_T = sAdcSum.u32RectBus_T;
        sAdcSum.u32TempINV_T = 0;
        //sAdcSum.u32TempPFC_T = 0;
        sAdcSum.u32RectBus_T = 0;

        sAdcSumBuff.u32TempEnv = sAdcSum.u32TempEnv;
        sAdcSum.u32TempEnv = 0;
        
        sAdcSumBuff.u321P65 = sAdcSum.u321P65;
        sAdcSum.u321P65 = 0;


        sAdcOk.u16All = 0x00FE;
    }

    u32IpvSumTimeCnt += u32IsrTimeBase;            // +Time base
    if(u32IpvSumTimeCnt > 15000)    //100us
    {
        u32IpvSumTimeCnt -= 15000;
        
        u16IpvSumCnt++;
        sAdcSum.u32IpVolt_R += (Uint32)((int32)sADC_Measure.i16IpVolt_R * sADC_Measure.i16IpVolt_R);
        sAdcSum.u32IpVolt_S += (Uint32)((int32)sADC_Measure.i16IpVolt_S * sADC_Measure.i16IpVolt_S);
        sAdcSum.u32IpVolt_T += (Uint32)((int32)sADC_Measure.i16IpVolt_T * sADC_Measure.i16IpVolt_T);
        
        if(u16IpvSumCnt >= 200)
        {
            u16IpvSumCnt = 0;
            
            sAdcSumBuff.u32IpVolt_R  = sAdcSum.u32IpVolt_R ;
            sAdcSum.u32IpVolt_R  = 0;
            sAdcSumBuff.u32IpVolt_S  = sAdcSum.u32IpVolt_S ;
            sAdcSum.u32IpVolt_S  = 0;
            sAdcSumBuff.u32IpVolt_T  = sAdcSum.u32IpVolt_T ;
            sAdcSum.u32IpVolt_T  = 0;
            
            sAdcOk.sBit.u1CalIpVolt = 1;
        }
    }
}

//------------------------------------------------------------------------------
// IN: R0        OUT:R0
//------------------------------------------------------------------------------
Uint16 LESS50V(Uint16 i_uint)
{
    if (i_uint < 50)
    {
        i_uint = 0;
    }

    return i_uint;
}


//------------------------------------------------------------------------------
// GET TEMPERATURE
//------------------------------------------------------------------------------
int16 GetTemperature(Uint16 u16TempAdcData, Uint16 u16Type)
{
    Uint16 u16Front, u16Middle, u16End, u16Split;
    int16 i16Temperature;
    
    if (u16Type)                   // 散热片上的温度
    {
        u16Front  = 0;
        u16End = TEMPERATURE_HS_LEN - 1;
        u16Middle = (u16Front + u16End) / 2;

        u16Split = u16TempAdcData;

        while (u16Front < u16End && u16NTC_HS_TABLE[u16Middle] != u16Split)
        {
            //Table递减
            //if (u16NTC_HS_TABLE[u16Middle] > u16Split)
            //{
            //    u16Front = u16Middle + 1;
            //}
            //if (u16NTC_HS_TABLE[u16Middle] < u16Split)
            //{
            //    u16End = u16Middle - 1;
            //}
            
            //Table递增
            if (u16NTC_HS_TABLE[u16Middle] > u16Split)
            {
                u16End = u16Middle - 1;
            }
            if (u16NTC_HS_TABLE[u16Middle] < u16Split)
            {
                u16Front = u16Middle + 1;
            }

            u16Middle = (u16Front + u16End) / 2;
        }
        i16Temperature = (int16)u16Middle - TEMPERATURE_HS_OFFSET;
    }
    else                        // 环境温度
    {
        u16Front  = 0;
        u16End = TEMPERATURE_AMB_LEN - 1;
        u16Middle = (u16Front + u16End) / 2;

        u16Split = u16TempAdcData;

        while (u16Front < u16End && u16NTC_AMB_TABLE[u16Middle] != u16Split)
        {
            if (u16NTC_AMB_TABLE[u16Middle] > u16Split)
            {
                u16Front = u16Middle + 1;
            }
            if (u16NTC_AMB_TABLE[u16Middle] < u16Split)
            {
                u16End = u16Middle - 1;
            }

            u16Middle = (u16Front + u16End) / 2;
        }
        i16Temperature = (int16)u16Middle - TEMPERATURE_AMB_OFFSET;
    }
    
    return (i16Temperature);
}


//------------------------------------------------------------------------------
// Calculate round Value,四舍五入
// timebase: main loop
//------------------------------------------------------------------------------
Uint16 Round(Uint16 u16Data)
{
    Uint16 u16DataRes, u16DataRem;

    u16DataRes = u16Data / 10;
    u16DataRem = u16Data % 10;
    if(u16DataRem > 5)
    {
        u16DataRes++;
    }

    return u16DataRes;
}

void CalcIpVolt(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp1;

    // Calculate Input RMS Of R,S,T,Bypass (256Points, Q13, 0.1V)
    sAdcCal.i16IpRmsVolt_R = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32IpVolt_R) * sGain_R.wIPV) >> 16);
    sAdcCal.i16IpRmsVolt_S = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32IpVolt_S) * sGain_S.wIPV) >> 16);
    sAdcCal.i16IpRmsVolt_T = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32IpVolt_T) * sGain_T.wIPV) >> 16);

    u16RmsIpRVoltBuff[0][0] = sAdcCal.i16IpRmsVolt_R;
    u16RmsIpRVoltBuff[1][0] = sAdcCal.i16IpRmsVolt_S;
    u16RmsIpRVoltBuff[2][0] = sAdcCal.i16IpRmsVolt_T;

    u16RmsIpRVoltBuff_Sum[0] += u16RmsIpRVoltBuff[0][0] - u16RmsIpRVoltBuff[0][2];
    u16RmsIpRVoltBuff_Sum[1] += u16RmsIpRVoltBuff[1][0] - u16RmsIpRVoltBuff[1][2];
    u16RmsIpRVoltBuff_Sum[2] += u16RmsIpRVoltBuff[2][0] - u16RmsIpRVoltBuff[2][2];

    for(u16TempCnt = 2; u16TempCnt > 0; u16TempCnt--)
    {
        u16RmsIpRVoltBuff[0][u16TempCnt] = u16RmsIpRVoltBuff[0][u16TempCnt - 1];
        u16RmsIpRVoltBuff[1][u16TempCnt] = u16RmsIpRVoltBuff[1][u16TempCnt - 1];
        u16RmsIpRVoltBuff[2][u16TempCnt] = u16RmsIpRVoltBuff[2][u16TempCnt - 1];
    }

    // Ip_R
    u16Temp1 = (u16RmsIpRVoltBuff_Sum[0] >> 1);	// Uint: 1V
    sRMS.u16IpVolt_R = LESS50V(u16Temp1);			// Check < 50V ?
    u16Temp1 = (u16RmsIpRVoltBuff_Sum[1] >> 1);	// Uint: 1V
    sRMS.u16IpVolt_S = LESS50V(u16Temp1);			// Check < 50V ?
    u16Temp1 = (u16RmsIpRVoltBuff_Sum[2] >> 1);	// Uint: 1V
    sRMS.u16IpVolt_T = LESS50V(u16Temp1);			// Check < 50V ?

}

void CalcBusVolt(void)
{
    static Uint16 u16TempCnt;

    // n Points ,0.1V
    sAdcCal.i16D2dBus_R = (int16)((int32)((__divf32(sAdcSumBuff.u32D2dBus_R  , SINEPOT)) * sGain_R.wBUS) >> 16);
    //sAdcCal.i16D2dBus_S = (int16)(((sAdcSumBuff.u32D2dBus_S  / SINEPOT) * sGain_S.wBUS) >> 16);
    //sAdcCal.i16D2dBus_T = (int16)(((sAdcSumBuff.u32D2dBus_T  / SINEPOT) * sGain_T.wBUS) >> 16);
    //工频机：只有一个BUS电压侦测，故ST直接赋值R相的值
    sAdcCal.i16D2dBus_S = sAdcCal.i16D2dBus_R;
	sAdcCal.i16D2dBus_T = sAdcCal.i16D2dBus_R;


    u16RmsD2dBusBuff[0][0] = (Uint16)sAdcCal.i16D2dBus_R ;
    u16RmsD2dBusBuff[1][0] = (Uint16)sAdcCal.i16D2dBus_S ;
    u16RmsD2dBusBuff[2][0] = (Uint16)sAdcCal.i16D2dBus_T ;

    u16RmsD2dBusBuff_Sum[0] += u16RmsD2dBusBuff[0][0] - u16RmsD2dBusBuff[0][4];
    u16RmsD2dBusBuff_Sum[1] += u16RmsD2dBusBuff[1][0] - u16RmsD2dBusBuff[1][4];
    u16RmsD2dBusBuff_Sum[2] += u16RmsD2dBusBuff[2][0] - u16RmsD2dBusBuff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16RmsD2dBusBuff[0][u16TempCnt] = u16RmsD2dBusBuff[0][u16TempCnt - 1];
        u16RmsD2dBusBuff[1][u16TempCnt] = u16RmsD2dBusBuff[1][u16TempCnt - 1];
        u16RmsD2dBusBuff[2][u16TempCnt] = u16RmsD2dBusBuff[2][u16TempCnt - 1];
    }

    sRMS.u16D2dBus_R = (u16RmsD2dBusBuff_Sum[0] >> 2);
    sRMS.u16D2dBus_S = (u16RmsD2dBusBuff_Sum[1] >> 2);
    sRMS.u16D2dBus_T = (u16RmsD2dBusBuff_Sum[2] >> 2);

}

void CalcInvVoltCurr(void)
{
    static Uint16 u16TempCnt;
    static int32 i32Temp1;

    // INV Voltage, 0.1V
    sAdcCal.i16Inv1RMSVol_R = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32InvVolt_R) * sGain_R.wINV) >> 16);
    sAdcCal.i16Inv1RMSVol_S = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32InvVolt_S) * sGain_S.wINV) >> 16);
    sAdcCal.i16Inv1RMSVol_T = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32InvVolt_T) * sGain_T.wINV) >> 16);

    u16RmsInv1VoltBuff[0][0] = sAdcCal.i16Inv1RMSVol_R;
    u16RmsInv1VoltBuff[1][0] = sAdcCal.i16Inv1RMSVol_S;
    u16RmsInv1VoltBuff[2][0] = sAdcCal.i16Inv1RMSVol_T;

    u16RmsInv1VoltBuff_Sum[0] += u16RmsInv1VoltBuff[0][0] - u16RmsInv1VoltBuff[0][4];
    u16RmsInv1VoltBuff_Sum[1] += u16RmsInv1VoltBuff[1][0] - u16RmsInv1VoltBuff[1][4];
    u16RmsInv1VoltBuff_Sum[2] += u16RmsInv1VoltBuff[2][0] - u16RmsInv1VoltBuff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16RmsInv1VoltBuff[0][u16TempCnt] = u16RmsInv1VoltBuff[0][u16TempCnt - 1];
        u16RmsInv1VoltBuff[1][u16TempCnt] = u16RmsInv1VoltBuff[1][u16TempCnt - 1];
        u16RmsInv1VoltBuff[2][u16TempCnt] = u16RmsInv1VoltBuff[2][u16TempCnt - 1];
    }

    sRMS.u16InvVolt_R = u16RmsInv1VoltBuff_Sum[0] >> 2;	    // Uint: 0.1V
    sRMS.u16InvVolt_S = u16RmsInv1VoltBuff_Sum[1] >> 2;
    sRMS.u16InvVolt_T = u16RmsInv1VoltBuff_Sum[2] >> 2;

    ADC_FLAG.sBit.RMS_CONTROL = 1;

}

void CalcOpVoltCurr(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp1;

    sAdcCal.i16OPRMSVol_R  = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpVolt_R) * sGain_R.wOPV) >> 16);
    sAdcCal.i16OPRMSVol_S  = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpVolt_S) * sGain_S.wOPV) >> 16);
    sAdcCal.i16OPRMSVol_T  = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpVolt_T) * sGain_T.wOPV) >> 16);

    sAdcCal.i16OPRMSCurr_R = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpCurr_R) * sGain_R.wOPI) >> Q_VALUE_CURR);
    sAdcCal.i16OPRMSCurr_S = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpCurr_S) * sGain_S.wOPI) >> Q_VALUE_CURR);
    sAdcCal.i16OPRMSCurr_T = (int16)(((Uint32)__sqrt(sAdcSumBuff.u32OpCurr_T) * sGain_T.wOPI) >> Q_VALUE_CURR);

    u16RmsOpVoltBuff[0][0] = sAdcCal.i16OPRMSVol_R;
    u16RmsOpVoltBuff[1][0] = sAdcCal.i16OPRMSVol_S;
    u16RmsOpVoltBuff[2][0] = sAdcCal.i16OPRMSVol_T;
    u16RmsOpCurrBuff[0][0] = sAdcCal.i16OPRMSCurr_R;
    u16RmsOpCurrBuff[1][0] = sAdcCal.i16OPRMSCurr_S;
    u16RmsOpCurrBuff[2][0] = sAdcCal.i16OPRMSCurr_T;

    u16RmsOpVoltBuff_Sum[0] += u16RmsOpVoltBuff[0][0] - u16RmsOpVoltBuff[0][4];
    u16RmsOpVoltBuff_Sum[1] += u16RmsOpVoltBuff[1][0] - u16RmsOpVoltBuff[1][4];
    u16RmsOpVoltBuff_Sum[2] += u16RmsOpVoltBuff[2][0] - u16RmsOpVoltBuff[2][4];
    u16RmsOpCurrBuff_Sum[0] += u16RmsOpCurrBuff[0][0] - u16RmsOpCurrBuff[0][4];
    u16RmsOpCurrBuff_Sum[1] += u16RmsOpCurrBuff[1][0] - u16RmsOpCurrBuff[1][4];
    u16RmsOpCurrBuff_Sum[2] += u16RmsOpCurrBuff[2][0] - u16RmsOpCurrBuff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16RmsOpVoltBuff[0][u16TempCnt] = u16RmsOpVoltBuff[0][u16TempCnt - 1];
        u16RmsOpVoltBuff[1][u16TempCnt] = u16RmsOpVoltBuff[1][u16TempCnt - 1];
        u16RmsOpVoltBuff[2][u16TempCnt] = u16RmsOpVoltBuff[2][u16TempCnt - 1];
        u16RmsOpCurrBuff[0][u16TempCnt] = u16RmsOpCurrBuff[0][u16TempCnt - 1];
        u16RmsOpCurrBuff[1][u16TempCnt] = u16RmsOpCurrBuff[1][u16TempCnt - 1];
        u16RmsOpCurrBuff[2][u16TempCnt] = u16RmsOpCurrBuff[2][u16TempCnt - 1];
    }

    sRMS.u16OpVolt_R = (u16RmsOpVoltBuff_Sum[0] >> 2);
    if(sRMS.u16OpVolt_R < NO_VOLT_POINT)
    {
        sRMS.u16OpVolt_R = 0;
    }
    sRMS.u16OpVolt_S = (u16RmsOpVoltBuff_Sum[1] >> 2);
    if(sRMS.u16OpVolt_S < NO_VOLT_POINT)
    {
        sRMS.u16OpVolt_S = 0;
    }
    sRMS.u16OpVolt_T = (u16RmsOpVoltBuff_Sum[2] >> 2);
    if(sRMS.u16OpVolt_T < NO_VOLT_POINT)
    {
        sRMS.u16OpVolt_T = 0;
    }


    sRMS.u16OpCurr_R = u16RmsOpCurrBuff_Sum[0] >> 2;		// For LCD Display, 0.1A
    if(sRMS.u16OpCurr_R < NO_CURR_POINT)
    {
        sRMS.u16OpCurr_R = 0;
    }
    sRMS.u16OpCurr_S = u16RmsOpCurrBuff_Sum[1] >> 2;		// For LCD Display, 0.1A
    if(sRMS.u16OpCurr_S < NO_CURR_POINT)
    {
        sRMS.u16OpCurr_S = 0;
    }
    sRMS.u16OpCurr_T = u16RmsOpCurrBuff_Sum[2] >> 2;		// For LCD Display, 0.1A
    if(sRMS.u16OpCurr_T < NO_CURR_POINT)
    {
        sRMS.u16OpCurr_T = 0;
    }
    
    if(sRMS.u16OpCurr_R > sRMS.u16OpCurr_S)
        sRMS.u16OpCurr_Max = sRMS.u16OpCurr_R;
    else
        sRMS.u16OpCurr_Max = sRMS.u16OpCurr_S;
    if(sRMS.u16OpCurr_T > sRMS.u16OpCurr_Max)
        sRMS.u16OpCurr_Max = sRMS.u16OpCurr_T;

    //if(sRMS.u16OpCurr_R >= NO_CURR_POINT && SourceControlFlag.sBit.OutputOn)
    //{
    //    dwOPI_PEAK_SUM *= GAIN_CF;
    //    sRMS.wOPI_CF_R = (dwOPI_PEAK_SUM >> 4) / sRMS.u16OpCurr_R;
    //}
    //else
    //{
    //    sRMS.wOPI_CF_R = 0;
    //}
}

void CalcOpWattVa(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp1, u16Temp2;
    static Uint16 u16VaPercent, u16WattPercent;

    sAdcCal.i32OPWatt_R = (((int64)(__divf32(sAdcSumBuff.i32OpWatt_R , SINEPOT)) * (int32)sGain_R.wWAT) >> Q_VALUE_WATT);
    sAdcCal.i32OPWatt_S = (((int64)(__divf32(sAdcSumBuff.i32OpWatt_S , SINEPOT)) * (int32)sGain_S.wWAT) >> Q_VALUE_WATT);
    sAdcCal.i32OPWatt_T = (((int64)(__divf32(sAdcSumBuff.i32OpWatt_T , SINEPOT)) * (int32)sGain_T.wWAT) >> Q_VALUE_WATT);

    sAdcCal.i32OPVA_R = ((int32)sAdcCal.i16OPRMSVol_R * sAdcCal.i16OPRMSCurr_R * 0.01F);
    sAdcCal.i32OPVA_S = ((int32)sAdcCal.i16OPRMSVol_S * sAdcCal.i16OPRMSCurr_S * 0.01F);
    sAdcCal.i32OPVA_T = ((int32)sAdcCal.i16OPRMSVol_T * sAdcCal.i16OPRMSCurr_T * 0.01F);

    i32OpWattBuff[0][0] = sAdcCal.i32OPWatt_R;
    i32OpWattBuff[1][0] = sAdcCal.i32OPWatt_S;
    i32OpWattBuff[2][0] = sAdcCal.i32OPWatt_T;
    i32OpVaBuff[0][0] = sAdcCal.i32OPVA_R;
    i32OpVaBuff[1][0] = sAdcCal.i32OPVA_S;
    i32OpVaBuff[2][0] = sAdcCal.i32OPVA_T;

    i32OpWattBuff_Sum[0] += i32OpWattBuff[0][0] - i32OpWattBuff[0][4];
    i32OpWattBuff_Sum[1] += i32OpWattBuff[1][0] - i32OpWattBuff[1][4];
    i32OpWattBuff_Sum[2] += i32OpWattBuff[2][0] - i32OpWattBuff[2][4];
    i32OpVaBuff_Sum[0] += i32OpVaBuff[0][0] - i32OpVaBuff[0][4];
    i32OpVaBuff_Sum[1] += i32OpVaBuff[1][0] - i32OpVaBuff[1][4];
    i32OpVaBuff_Sum[2] += i32OpVaBuff[2][0] - i32OpVaBuff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        i32OpWattBuff[0][u16TempCnt] = i32OpWattBuff[0][u16TempCnt - 1];
        i32OpWattBuff[1][u16TempCnt] = i32OpWattBuff[1][u16TempCnt - 1];
        i32OpWattBuff[2][u16TempCnt] = i32OpWattBuff[2][u16TempCnt - 1];
        i32OpVaBuff[0][u16TempCnt] = i32OpVaBuff[0][u16TempCnt - 1];
        i32OpVaBuff[1][u16TempCnt] = i32OpVaBuff[1][u16TempCnt - 1];
        i32OpVaBuff[2][u16TempCnt] = i32OpVaBuff[2][u16TempCnt - 1];
    }

    sRMS.i32OpWatt_R = (i32OpWattBuff_Sum[0] >> 2);
    sRMS.i32OpWatt_S = (i32OpWattBuff_Sum[1] >> 2);
    sRMS.i32OpWatt_T = (i32OpWattBuff_Sum[2] >> 2);
    sRMS.u32OpVa_R = (i32OpVaBuff_Sum[0] >> 2);
    sRMS.u32OpVa_S = (i32OpVaBuff_Sum[1] >> 2);
    sRMS.u32OpVa_T = (i32OpVaBuff_Sum[2] >> 2);        
    
    sRMS.u32OpWatt_R = labs(sRMS.i32OpWatt_R);
    sRMS.u32OpWatt_S = labs(sRMS.i32OpWatt_S);
    sRMS.u32OpWatt_T = labs(sRMS.i32OpWatt_T);


    if(sRMS.u32OpWatt_R < NO_WATT_POINT || sRMS.u32OpVa_R < NO_VA_POINT)
    {
        sRMS.i32OpWatt_R = 0;
        sRMS.u32OpVa_R = 0;
        sRMS.u16OpCurr_R = 0;
    }
    if(sRMS.u32OpWatt_S < NO_WATT_POINT || sRMS.u32OpVa_S < NO_VA_POINT)
    {
        sRMS.i32OpWatt_S = 0;
        sRMS.u32OpVa_S = 0;
        sRMS.u16OpCurr_S = 0;
    }
    if(sRMS.u32OpWatt_T < NO_WATT_POINT || sRMS.u32OpVa_T < NO_VA_POINT)
    {
        sRMS.i32OpWatt_T = 0;
        sRMS.u32OpVa_T = 0;
        sRMS.u16OpCurr_T = 0;
    }

    //Load percent
    u16WattPercent = (Uint16)(__divf32(sRMS.u32OpWatt_R , OP_POWER_WATT) * 0.01F);
    u16VaPercent   = (Uint16)(__divf32(sRMS.u32OpVa_R   , OP_POWER_VA  ) * 0.01F);
    if(u16WattPercent > u16VaPercent)
    {
        sRMS.u16LoadPercent_R = u16WattPercent;
    }
    else
    {
        sRMS.u16LoadPercent_R = u16VaPercent;
    }

    u16WattPercent = (Uint16)(__divf32(sRMS.u32OpWatt_S , OP_POWER_WATT) * 0.01F);
    u16VaPercent   = (Uint16)(__divf32(sRMS.u32OpVa_S   , OP_POWER_VA  ) * 0.01F);
    if(u16WattPercent > u16VaPercent)
    {
        sRMS.u16LoadPercent_S = u16WattPercent;
    }
    else
    {
        sRMS.u16LoadPercent_S = u16VaPercent;
    }

    u16WattPercent = (Uint16)(__divf32(sRMS.i32OpWatt_T , OP_POWER_WATT) * 0.01F);
    u16VaPercent =   (Uint16)(__divf32(sRMS.u32OpVa_T   , OP_POWER_VA  ) * 0.01F);
    if(u16WattPercent > u16VaPercent)
    {
        sRMS.u16LoadPercent_T = u16WattPercent;
    }
    else
    {
        sRMS.u16LoadPercent_T = u16VaPercent;
    }
    
    if(sRMS.u16LoadPercent_R > sRMS.u16LoadPercent_S)
        sRMS.u16LoadPercent_Max = sRMS.u16LoadPercent_R;
    else
        sRMS.u16LoadPercent_Max = sRMS.u16LoadPercent_S;
    if(sRMS.u16LoadPercent_Max < sRMS.u16LoadPercent_T)
        sRMS.u16LoadPercent_Max = sRMS.u16LoadPercent_T;
    

    //PF
    if(sRMS.u32OpWatt_R > 100 && SourceControlFlag.sBit.OutputOn)
    {
        if(sRMS.u32OpWatt_R >= sRMS.u32OpVa_R)
        {
            sRMS.wOPI_PF_R = 100;
        }
        else
        {
            sRMS.wOPI_PF_R = (Uint16)(__divf32(sRMS.u32OpWatt_R , sRMS.u32OpVa_R) * 100);
        }
    }
    else
    {
        sRMS.wOPI_PF_R = 0;
    }

    if(sRMS.u32OpWatt_S > 100 && SourceControlFlag.sBit.OutputOn)
    {
        if(sRMS.u32OpWatt_S >= sRMS.u32OpVa_S)
        {
            sRMS.wOPI_PF_S = 100;
        }
        else
        {
            sRMS.wOPI_PF_S = (Uint16)(__divf32(sRMS.u32OpWatt_S , sRMS.u32OpVa_S) * 100);
        }
    }
    else
    {
        sRMS.wOPI_PF_S = 0;
    }

    if(sRMS.u32OpWatt_T > 100 && SourceControlFlag.sBit.OutputOn)
    {
        if(sRMS.u32OpWatt_T >= sRMS.u32OpVa_T)
        {
            sRMS.wOPI_PF_T = 100;
        }
        else
        {
            sRMS.wOPI_PF_T = (Uint16)(__divf32(sRMS.u32OpWatt_T , sRMS.u32OpVa_T) * 100);
        }
    }
    else
    {
        sRMS.wOPI_PF_T = 0;
    }
}


void CalcEnvTemperature(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp1;
    
    sAdcCal.u16Temp_Env  = (Uint16)(__divf32(sAdcSumBuff.u32TempEnv , SINEPOT));
    sAdcCal.i1P65Mean    = (Uint16)(__divf32(sAdcSumBuff.u321P65 , SINEPOT));
    
    if(sAdcCal.i1P65Mean < 1948)
        sAdcCal.i1P65Mean = 1948;
    else if(sAdcCal.i1P65Mean > 2148)
        sAdcCal.i1P65Mean = 2148;
    
    u16TempEnvBuff[0] = sAdcCal.u16Temp_Env;

    u16TempEnvBuff_Sum = (u16TempEnvBuff_Sum + u16TempEnvBuff[0]) - u16TempEnvBuff[4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16TempEnvBuff[u16TempCnt] = u16TempEnvBuff[u16TempCnt - 1];
    }
    u16Temp1 = u16TempEnvBuff_Sum >> 2;

    // Read Temperature Form NTC_Table
    sRMS.i16Temperature_AMB = GetTemperature(u16Temp1, 0);
        
}

void CalcTemperature1(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp[3];
    
    sAdcCal.u16Temp_PFC1_R = (Uint16)(__divf32(sAdcSumBuff.u32TempPFC_R , SINEPOT));  
    sAdcCal.u16Temp_PFC1_S = (Uint16)(__divf32(sAdcSumBuff.u32TempPFC_S , SINEPOT));  
    sAdcCal.u16Temp_PFC1_T = (Uint16)(__divf32(sAdcSumBuff.u32TempPFC_T , SINEPOT));  
    
    u16TempPfc1Buff[0][0] = sAdcCal.u16Temp_PFC1_R;
    u16TempPfc1Buff[1][0] = sAdcCal.u16Temp_PFC1_S;
    u16TempPfc1Buff[2][0] = sAdcCal.u16Temp_PFC1_T;

    u16TempPfc1Buff_Sum[0] += u16TempPfc1Buff[0][0] - u16TempPfc1Buff[0][4];
    u16TempPfc1Buff_Sum[1] += u16TempPfc1Buff[1][0] - u16TempPfc1Buff[1][4];
    u16TempPfc1Buff_Sum[2] += u16TempPfc1Buff[2][0] - u16TempPfc1Buff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16TempPfc1Buff[0][u16TempCnt] = u16TempPfc1Buff[0][u16TempCnt - 1];
        u16TempPfc1Buff[1][u16TempCnt] = u16TempPfc1Buff[1][u16TempCnt - 1];
        u16TempPfc1Buff[2][u16TempCnt] = u16TempPfc1Buff[2][u16TempCnt - 1];
    }
    u16Temp[0] = u16TempPfc1Buff_Sum[0] >> 2;
    u16Temp[1] = u16TempPfc1Buff_Sum[1] >> 2; 
    u16Temp[2] = u16TempPfc1Buff_Sum[2] >> 2;

    // Read Temperature Form NTC_Table
    sRMS.i16Temperature_PFC_R = GetTemperature(u16Temp[0], 1);
    sRMS.i16Temperature_PFC_S = GetTemperature(u16Temp[1], 1);
    sRMS.i16Temperature_PFC_T = GetTemperature(u16Temp[2], 1);
    
    sRMS.i16Temperature_PFC_S = GetTemperature(u16Temp[1], 1);
        
}

void CalcTemperature2(void)
{
    static Uint16 u16TempCnt;
    static Uint16 u16Temp[3];
    
    sAdcCal.u16Temp_INV1_R = (Uint16)(__divf32(sAdcSumBuff.u32TempINV_R , SINEPOT));
    sAdcCal.u16Temp_INV1_S = (Uint16)(__divf32(sAdcSumBuff.u32TempINV_S , SINEPOT));
    sAdcCal.u16Temp_INV1_T = (Uint16)(__divf32(sAdcSumBuff.u32TempINV_T , SINEPOT));

    u16TempInv1Buff[0][0] = sAdcCal.u16Temp_INV1_R;
    u16TempInv1Buff[1][0] = sAdcCal.u16Temp_INV1_S;
    u16TempInv1Buff[2][0] = sAdcCal.u16Temp_INV1_T;

    u16TempInv1Buff_Sum[0] += u16TempInv1Buff[0][0] - u16TempInv1Buff[0][4];
    u16TempInv1Buff_Sum[1] += u16TempInv1Buff[1][0] - u16TempInv1Buff[1][4];
    u16TempInv1Buff_Sum[2] += u16TempInv1Buff[2][0] - u16TempInv1Buff[2][4];

    for(u16TempCnt = 4; u16TempCnt > 0; u16TempCnt--)
    {
        u16TempInv1Buff[0][u16TempCnt] = u16TempInv1Buff[0][u16TempCnt - 1];
        u16TempInv1Buff[1][u16TempCnt] = u16TempInv1Buff[1][u16TempCnt - 1];
        u16TempInv1Buff[2][u16TempCnt] = u16TempInv1Buff[2][u16TempCnt - 1];
    }
    u16Temp[0] = u16TempInv1Buff_Sum[0] >> 2;
    u16Temp[1] = u16TempInv1Buff_Sum[1] >> 2;
    u16Temp[2] = u16TempInv1Buff_Sum[2] >> 2;

    // Read Temperature Form NTC_Table
    sRMS.i16Temperature_INV_R = GetTemperature(u16Temp[0], 1);
    sRMS.i16Temperature_INV_S = GetTemperature(u16Temp[1], 1);
    sRMS.i16Temperature_INV_T = GetTemperature(u16Temp[2], 1);
    
    if(sRMS.i16Temperature_INV_R <= ((int16)10-TEMPERATURE_HS_OFFSET))
    {
        sRMS.i16Temperature_INV_R = 150;
    }
    if(sRMS.i16Temperature_INV_S <= ((int16)10-TEMPERATURE_HS_OFFSET))
    {
        sRMS.i16Temperature_INV_S = 150;
    }
    if(sRMS.i16Temperature_INV_T <= ((int16)10-TEMPERATURE_HS_OFFSET))
    {
        sRMS.i16Temperature_INV_T = 150;
    }
     
}


//------------------------------------------------------------------------------
// Calculate RMS Value
// timebase: main loop
//------------------------------------------------------------------------------
//#pragma CODE_SECTION(ADC_Square, "ramfuncs");
void ADC_Square(void)
{
    if(sAdcOk.sBit.u1CalIpVolt)
    {
        sAdcOk.sBit.u1CalIpVolt = 0;

        CalcIpVolt();
    }
    else if(sAdcOk.sBit.u1CalBusVolt)
    {
        sAdcOk.sBit.u1CalBusVolt = 0;

        CalcBusVolt();
    }
    else if(sAdcOk.sBit.u1CalInvVolt)
    {
        sAdcOk.sBit.u1CalInvVolt = 0;

        CalcInvVoltCurr();
    }
    else if(sAdcOk.sBit.u1CalOpVolt)
    {
        sAdcOk.sBit.u1CalOpVolt = 0;

        CalcOpVoltCurr();
    }
    else if(sAdcOk.sBit.u1CalOPWatt)
    {
        sAdcOk.sBit.u1CalOPWatt = 0;

        CalcOpWattVa();
    }
    else if(sAdcOk.sBit.u1CalTempEnv)
    {
        sAdcOk.sBit.u1CalTempEnv = 0;

        CalcEnvTemperature();
    }
    /*else if(sAdcOk.sBit.u1CalTempPfc)
    {
        sAdcOk.sBit.u1CalTempPfc = 0;

        CalcTemperature1();
    }*/
    else if(sAdcOk.sBit.u1CalTempInv)
    {
        sAdcOk.sBit.u1CalTempInv = 0;

        CalcTemperature2();
    }
}

//------------------------------------------------------------------------------
// Calculate RMS Value
// timebase: main loop
//------------------------------------------------------------------------------
void TaskRmsCalc(void)
{
    ADC_Square();
}
//----------------------------------------------------------------------------
// CHECK FREQUENCY
// 1> Check it every 100ms;
// 2> sRMS.u16AcFreq: AC I/P R_Phase Freq;
// 3> Check "u1AcFreqOutofRange" For AC&DC Transfer
//----------------------------------------------------------------------------
#define FREQ_MAX            (900)           // 90.0 Hz 
#define FREQ_MIN            (200)           // 20.0 Hz 
void FrequencyAcInputCheck(void)
{
    /*static Uint16 u16Time100msBuf;
    static Uint16  u16AcFreqCnt = 0;
    static Uint16  u16AcLossFreqCnt = 0;

    if(u16Time100msBuf == u16Mini100ms)    //100ms timebase
    {
        return;
    }
    u16Time100msBuf = u16Mini100ms;

    if ((sRMS.u16ACFreq > FREQ_HIGH) ||                 // > 70.0Hz(Hz) ?
        (sRMS.u16ACFreq < FREQ_LOW))                    // < 40.0Hz ?
    {
        u16AcFreqCnt++;
        if (u16AcFreqCnt >= SEC_1S)
        {
            if(sRMS.u16ACFreq > FREQ_HIGH)
            {
                sQualityFlag.sInput.sBit.u1AcFreqHigh = 1;
                sQualityFlag.sInput.sBit.u1AcFreqLow = 0;
            }
            else
            {
                sQualityFlag.sInput.sBit.u1AcFreqHigh = 0;
                sQualityFlag.sInput.sBit.u1AcFreqLow = 1;
            }
            sQualityFlag.sInput.sBit.u1AcFreqOutofRange = 1;
        }
    }
    else if ((sRMS.u16ACFreq < FREQ_HIGH_RET) &&        // < 69.5Hz ?
             (sRMS.u16ACFreq > FREQ_LOW_RET))           // > 40.5Hz ?
    {
        u16AcFreqCnt = 0;
        sQualityFlag.sInput.sBit.u1AcFreqHigh = 0;
        sQualityFlag.sInput.sBit.u1AcFreqLow = 0;
        sQualityFlag.sInput.sBit.u1AcFreqOutofRange = 0;
    }

    if(sRMS.u16ACFreq > FREQ_MIN)
    {
        if(u16AcLossFreqCnt < SEC_1S)
            u16AcLossFreqCnt++;
        else
            sQualityFlag.sInput.sBit.u1AcFreqLoss = 0;
    }
    else
    {
        u16AcLossFreqCnt = 0;
        sQualityFlag.sInput.sBit.u1AcFreqLoss = 1;
    }*/
}




//----------------------------------------------------------------------------
// IN: R0
// OUT: R0 XXX=XX.X Hz
//----------------------------------------------------------------------------
Uint16 FreqComm(Uint16 i_uint)
{
    Uint16 u16Mod;

    if (i_uint != 0)                            // cycle count=0 ?
    {
        i_uint = ((Uint32)125000000 / i_uint);  // ((Uint32)1000000000 / i_uint) / 8(0.8us) = 125000000/i_uint
    }

    u16Mod = (i_uint+5/10);   //四舍五入

    return u16Mod;   
}


//----------------------------------------------------------------------------
// CALCULATE FREQUENCY
// AC INPUT FREQUENCY & BYPASS FREQUENCY
// FREQUENCY   XXX =XX.X Hz
// Run it every 0.1s(in SysTimeBase())
// timebase:10ms
//----------------------------------------------------------------------------
void GetFrequency(void)
{
    Uint16 i_ACFreq, i_BPFreq, i_OPFREQ;

    static Uint16 u16MesAcFreqCnt = 0;              // ACIN   FREQ, RS-232 & LCD count
    static Uint16 u16MesBpsFreqCnt = 0;             // BYPASS FREQ, RS-232 & LCD count
    /*
        // Calculate AC INPUT frequency
        i_ACFreq = FreqComm(u16HwSyncPrdIpR);

        if (i_ACFreq >= 1000)                   // >= 100.0Hz ?
        {
            i_ACFreq = 999;                     // set to 99.9Hz ?
        }

        if (sRMS.u16IPVol_R == 0)
        {
            sRMS.u16ACFreq = 0;
        }
        else
        {
            sRMS.u16ACFreq = i_ACFreq;          // get AC INPUT frequency
        }


        // Calculate output frequency
        sRMS.u16InverterFreq = FreqComm(u16InvPrd);
        if (sRMS.u16OpVolt_R)                         // O/P Have Volt!!
        {
            i_OPFREQ = sRMS.u16InverterFreq;    // get OUTPUT frequency
        }
        else
        {
            i_OPFREQ = 0;
        }
        sRMS.u16OpFreq = i_OPFREQ;
    */
}

//------------------------------------------------------------------------------
// KWH:unit 0.01Kwh
// timebase : 1sec
//------------------------------------------------------------------------------
void CalculateKWH(void)
{
    if(SourceControlFlag.sBit.OutputOn)
    {
        u32TempLoadEnergy_R += sRMS.u32OpWatt_R;
        if(u32TempLoadEnergy_R >= 36000)    //0.01Kwh
        {
            sRMS.u32TotalKWH_R += (__divf32(u32TempLoadEnergy_R, 36000));
            u32TempLoadEnergy_R = u32TempLoadEnergy_R % 36000;
        }
        
        u32TempLoadEnergy_S += sRMS.u32OpWatt_S;
        if(u32TempLoadEnergy_S >= 36000)    //0.01Kwh
        {
            sRMS.u32TotalKWH_S += (__divf32(u32TempLoadEnergy_S, 36000));
            u32TempLoadEnergy_S = u32TempLoadEnergy_S % 36000;
        }
        
        u32TempLoadEnergy_T += sRMS.u32OpWatt_T;
        if(u32TempLoadEnergy_T >= 36000)    //0.01Kwh
        {
            sRMS.u32TotalKWH_T += (__divf32(u32TempLoadEnergy_T, 36000));
            u32TempLoadEnergy_T = u32TempLoadEnergy_T % 36000;
        }        
    }
    sRMS.u32TotalKWH = (sRMS.u32TotalKWH_R + sRMS.u32TotalKWH_S + sRMS.u32TotalKWH_T)/10;
}
