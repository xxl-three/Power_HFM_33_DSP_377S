#ifndef Measurement_H
#define Measurement_H

#ifdef __cplusplus
extern "C" {
#endif

#include "USR_VAR.h"


//-----------------------------------------------------------------------------
// adc
#define ADC_MEAN            (2048)

//-----------------------------------------------------------------------------
// ADC Result Define!

#define	ADC_VINV_R	        (int16)AdcaResultRegs.ADCRESULT0
#define	ADC_IINV_R	        (int16)AdcaResultRegs.ADCRESULT1
#define	ADC_VOUT_R          (int16)AdcaResultRegs.ADCRESULT2
#define	ADC_IOUT_R	        (int16)AdcaResultRegs.ADCRESULT3
#define	ADC_VBUS_PFC_R	    (int16)AdcaResultRegs.ADCRESULT4
#define	ADC_NTC_AMB	        (int16)AdcaResultRegs.ADCRESULT5
#define	ADC_1P65_REF        (int16)AdcaResultRegs.ADCRESULT6

#define	ADC_VINV_S	        (int16)AdcbResultRegs.ADCRESULT0
#define	ADC_IINV_S	        (int16)AdcbResultRegs.ADCRESULT1
#define	ADC_VOUT_T          (int16)AdcbResultRegs.ADCRESULT2
#define	ADC_IOUT_S	        (int16)AdcbResultRegs.ADCRESULT3

#define	ADC_VBUS_PFC_S      (int16)AdccResultRegs.ADCRESULT0
#define	ADC_NTC_B	        (int16)AdccResultRegs.ADCRESULT1
#define	ADC_NTC_I	        (int16)AdccResultRegs.ADCRESULT2

#define	ADC_VINV_T	        (int16)AdcdResultRegs.ADCRESULT0
#define	ADC_IINV_T	        (int16)AdcdResultRegs.ADCRESULT1
#define	ADC_VOUT_S          (int16)AdcdResultRegs.ADCRESULT2
#define	ADC_IOUT_T	        (int16)AdcdResultRegs.ADCRESULT3
#define	ADC_VBUS_PFC_T	    (int16)AdcdResultRegs.ADCRESULT4



#define START_ADC_MEAN_CNT  (5)

//for calculate 1.5v mean value
extern struct _AdcMean
{
    int32   i32SumIpRVolt;
    int32   i32SumIpRAbsVolt;
    int32   i32SumInv1Vol;      // It is the Value Of "Power On"!
    int32   i32SumOPVolt;
    int32   i32SumOPCurr;

    int16	i16IpRVolt;
    int16	i16IpRAbsVolt;
    int16   i16IPCurr;
    int16   i16InvVolt_R;         // It is the Value Of "Power On"!
    int16   i16OPVolt;
    int16   i16OpCurr_R;
    int16	i16OPCurr_Buffer;
    int16   i16DC1Off;
    int16   i16MeanCnt_IpV;
    int16   i16StartCnt;

	unsigned StartMeanOK:	1;
}sADC_Mean;

extern struct _AdcMeasure
{
    int16 i16IpVolt_R;
    int16 i16InvVolt_R;
    int16 i16InvCurr_R;
    int16 i16OpVolt_R;
    int16 i16OpCurr_R;
    int16 i16RectBus_R;
    int16 i16D2dBus_R;
    int16 i16TempPFC_R;
    int16 i16TempINV_R;

    int16 i16IpVolt_S;
    int16 i16InvVolt_S;
    int16 i16InvCurr_S;
    int16 i16OpVolt_S;
    int16 i16OpCurr_S;
    int16 i16RectBus_S;
    int16 i16D2dBus_S;
    int16 i16TempPFC_S;
    int16 i16TempINV_S;

    int16 i16IpVolt_T;
    int16 i16InvVolt_T;
    int16 i16InvCurr_T;
    int16 i16OpVolt_T;
    int16 i16OpCurr_T;
    int16 i16RectBus_T;
    int16 i16D2dBus_T;
    int16 i16TempPFC_T;
    int16 i16TempINV_T;

    int16 i16TempEnv;			// Environment Temperature
    
    int16 i1P65Ref;
    
    int16 i16OpVolt_RS;
    int16 i16OpVolt_ST;
    int16 i16OpVolt_TR;

}sADC_Measure;

extern struct _AdcSum
{
    Uint32 u32IpVolt_R;
    Uint32 u32InvVolt_R;
    Uint32 u32OpVolt_R;
    Uint32 u32OpCurr_R;
    int32  i32OpWatt_R;
    Uint32 u32RectBus_R;
    Uint32 u32D2dBus_R;
    Uint32 u32TempPFC_R;
    Uint32 u32TempINV_R;

    Uint32 u32IpVolt_S;
    Uint32 u32InvVolt_S;
    Uint32 u32OpVolt_S;
    Uint32 u32OpCurr_S;
    int32  i32OpWatt_S;
    Uint32 u32RectBus_S;
    Uint32 u32D2dBus_S;
    Uint32 u32TempPFC_S;
    Uint32 u32TempINV_S;

    Uint32 u32IpVolt_T;
    Uint32 u32InvVolt_T;
    Uint32 u32OpVolt_T;
    Uint32 u32OpCurr_T;
    int32  i32OpWatt_T;
    Uint32 u32RectBus_T;
    Uint32 u32D2dBus_T;
    Uint32 u32TempPFC_T;
    Uint32 u32TempINV_T;

    Uint32 u32TempEnv;
    
    Uint32 u321P65;
    
    Uint32 u32OpVolt_RS;
    Uint32 u32OpVolt_ST;
    Uint32 u32OpVolt_TR;
    
}sAdcSum,sAdcSumBuff;

extern struct _AdcCal
{
    int16   i16IpRmsVolt_R;           //0.1V
    int16   i16Inv1RMSVol_R;          //0.1V
    int16   i16OPRMSVol_R;            //0.1V
    int16   i16OPRMSCurr_R;           //0.1A
    int32   i32OPWatt_R;              //1w
    int32   i32OPVA_R;                //1VA
    int16   i16RectBus_R;             //0.1V
    int16   i16D2dBus_R;              //0.1V
    Uint16  u16InvMovingVol_R;
    Uint16  u16Temp_INV1_R;
    Uint16  u16Temp_PFC1_R;

    int16   i16IpRmsVolt_S;           //0.1V    NO
    int16   i16Inv1RMSVol_S;          //0.1V
    int16   i16OPRMSVol_S;            //0.1V
    int16   i16OPRMSCurr_S;           //0.1A
    int32   i32OPWatt_S;              //1w
    int32   i32OPVA_S;                //1VA
    int16   i16RectBus_S;             //0.1V    NO
    int16   i16D2dBus_S;              //0.1V
    Uint16  u16InvMovingVol_S;
    Uint16  u16Temp_INV1_S;
    Uint16  u16Temp_PFC1_S;

    int16   i16IpRmsVolt_T;           //0.1V
    int16   i16Inv1RMSVol_T;          //0.1V
    int16   i16OPRMSVol_T;            //0.1V
    int16   i16OPRMSCurr_T;           //0.1A
    int32   i32OPWatt_T;              //1w
    int32   i32OPVA_T;                //1VA
    int16   i16RectBus_T;             //0.1V
    int16   i16D2dBus_T;              //0.1V
    Uint16  u16InvMovingVol_T;
    Uint16  u16Temp_INV1_T;
    Uint16  u16Temp_PFC1_T;

    Uint16  u16Temp_Env;

    Uint16  u16PfcADCCalCntBuff;
    Uint16  u16InvADCCalCntBuff;
    
    int16 i1P65Mean;
}sAdcCal;

extern struct _sRMS
{
    Uint16	u16InvVolt_R;    //0.1V
    Uint16	u16OpVolt_R;    //0.1V
    Uint16	u16OpCurr_R;
    Uint16	u16IpVolt_R;
    int32	i32OpWatt_R;
    Uint32	u32OpVa_R;
    Uint16	u16LoadPercent_R;
    Uint16	wOPI_PF_R;
    Uint16	wOPI_CF_R;
    Uint16	u16RectBus_R;         //1V
    Uint16	u16D2dBus_R;
    int16	i16Temperature_PFC_R;
    int16	i16Temperature_INV_R;

    Uint16	u16InvVolt_S;    //0.1V
    Uint16	u16OpVolt_S;    //0.1V
    Uint16	u16OpCurr_S;
    Uint16	u16IpVolt_S;
    int32	i32OpWatt_S;
    Uint32	u32OpVa_S;
    Uint16	u16LoadPercent_S;
    Uint16	wOPI_PF_S;
    Uint16	wOPI_CF_S;
    Uint16	u16RectBus_S;         //1V
    Uint16	u16D2dBus_S;
    int16	i16Temperature_PFC_S;
    int16	i16Temperature_INV_S;

    Uint16	u16InvVolt_T;    //0.1V
    Uint16	u16OpVolt_T;    //0.1V
    Uint16	u16OpCurr_T;
    Uint16	u16IpVolt_T;
    int32	i32OpWatt_T;
    Uint32	u32OpVa_T;
    Uint16	u16LoadPercent_T;
    Uint16	wOPI_PF_T;
    Uint16	wOPI_CF_T;
    Uint16	u16RectBus_T;         //1V
    Uint16	u16D2dBus_T;
    int16	i16Temperature_PFC_T;
    int16	i16Temperature_INV_T;

    int16	i16Temperature_AMB;
    Uint16	wIpTrTemperature;
    Uint16	wOpTrTemperature;

    Uint16	u16AcFreq;
    
    Uint16	u16LoadPercent_Max;
    Uint16	u16OpCurr_Max;
    
    Uint32	u32OpWatt_R;
    Uint32	u32OpWatt_S;
    Uint32	u32OpWatt_T;    
    
    Uint32  u32TotalKWH_R;          // 0.01KWh
    Uint32  u32TotalKWH_S;          // 0.01KWh
    Uint32  u32TotalKWH_T;          // 0.01KWh
    Uint32  u32TotalKWH;            // 0.1KWh
}sRMS ;


extern  union   UN_ADC_SP_FLAG
{
    Uint16  u16All;
    struct  ST_ADC_SP_FLAG
    {
        unsigned  u1CalIpVolt       : 1;    // Bit 0:
        unsigned  u1CalInvVolt      : 1;    // Bit 1:
        unsigned  u1CalOpVolt       : 1;    // Bit 2:
        unsigned  u1CalOPWatt       : 1;    // Bit 3:
        unsigned  u1CalBusVolt      : 1;    // Bit 4:
        unsigned  u1CalTempEnv      : 1;    // Bit 5:
        unsigned  u1CalTempPfc      : 1;    // Bit 6:
        unsigned  u1CalTempInv      : 1;    // Bit 7:

        unsigned                    : 1;    // Bit 8:
        unsigned                    : 1;    // Bit 9:
        unsigned                    : 1;    // Bit 10:
        unsigned                    : 1;    // Bit 11:
        unsigned                    : 1;    // Bit 12:
        unsigned                    : 1;    // Bit 13:
        unsigned                    : 1;    // Bit 14:
        unsigned                    : 1;    // Bit 15:
    } sBit;
} sAdcOk;



//flag
extern void IsrAdcSampling(void);
extern void IsrAdcSum(void);

extern void TaskRmsCalc(void);

extern void GetFrequency(void);
extern void FrequencyAcInputCheck(void);
extern void CalculateKWH(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

