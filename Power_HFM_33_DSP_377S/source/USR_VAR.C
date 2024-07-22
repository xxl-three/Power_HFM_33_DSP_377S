/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : USR_VAR.c

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/

#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "USR_VAR.h"

/*****************************************************************************
   Variables
*****************************************************************************/
Uint16  u16MicroSec;					    // Mico second
Uint16  u16Mini1ms;                         // Mini second, 1 ms
Uint16  u16Mini10ms;                        // Mini second, 10 ms
Uint16  u16Mini50ms;                        // Mini second, 50 ms
Uint16  u16Mini100ms;                       // Mini second, 100 ms
Uint16  u16Second;                          // Second
Uint16  u16Minute;                          // Minute
Uint16  u16Hour;                            // Hour

Uint16   u16DisplayTime;                       // DISPLAY LCD time temp.

Uint16   u16DcbusHighCnt1, u16DcbusHighCnt2, u16DcbusLowCnt1, u16DcbusLowCnt2;                    // DC BUS + fail count

Uint8    AC_FREQ_CNT;                    // AC Frequency out of range count, backup

Uint16   wGAIN_ADD_R,wGAIN_ADD_S,wGAIN_ADD_T;                     // PWM gain add constant
Uint16   u16CalPwmGain_R,u16CalPwmGain_S,u16CalPwmGain_T;
Uint16   u16BusDelayTime;                 // Check DCBUS delay time

Uint16	 u16LcdBacklightOnTime;		// LCD backlight on tiem

Uint16   u16DelayOn;

Uint32   dwOPI_PEAK_TMP;              	// Output I A/D sum1 temp
Uint32   dwOPI_PEAK_SUM;          		// Output I A/D sum1

_sGain 	sGain_R,sGain_S,sGain_T;

Uint16 u16IpeakProtect;
Uint16 u16GainDcOffset;
Uint16 u16PowerRecycle;
Uint16 u16PowerRecycleLevel;

Uint32	dwPwmGainTemp_R;
Uint32	dwPwmGainTemp_S;
Uint32	dwPwmGainTemp_T;

Uint16  wBusTemp;
Uint16  u16BusOvpCnt;

_sProgram		sProgramming;
_sProgRun		sProgRun;
struct	_sLVRT 	sLVRT;

Uint16	u16SineWaveIndex;
Uint32	u32IsrTimeBase;
Uint32	u32TimeBase;
Uint32	u32TimeBaseRem;

struct	_sSet sSet;

nu16HiLoByte_t	wTempVaule;

Uint16	wRGain,wSGain,wTGain;
Uint16	wG1RGain,wG1SGain,wG1TGain;

Uint16  u16IndexR,u16IndexS,u16IndexT;

nu32HiLoWord_t iMathDoubleWord_G1R;

Uint32 u32TempLoadEnergy_R,u32TempLoadEnergy_S,u32TempLoadEnergy_T;


struct _sPdRunTime sPdRunTime, sNowRunTime;

Uint16	u16InvIPeakProtectCnt, u16InvErrorCnt_R, u16InvErrorDelayCnt,u16Inv316ScProtectCnt;
Uint16	wInvOpVolError;
Uint16	u16RelayDelayCnt, u16OverVoltageCnt_R, u16SetupModeCnt, u16RepairModeCnt;
Uint16  u16SoftstartCnt;
int16   i16TemperatureOffset;

Uint16 u16LastInvAdcVolt;
Uint16	u16ProgramErrorCode;    

int32 i32Inv1RMSAmpVref_R,i32Inv1RMSAmpVref_S,i32Inv1RMSAmpVref_T;
int32 i32Inv1AmpVref_R,i32Inv1AmpVref_S,i32Inv1AmpVref_T;
Uint16 u16RmsErrorDelayCnt;

Uint32  u32EndIsrTimeCnt, u32StartIsrTimeCnt, u32CpuIsrTime,u32IsrTimeMax;       // Mico second

Uint32   u32timeisrStartIsrTimeCnt, u32timeisrEndIsrTimeCnt;
Uint32   u16timeisrLastCpuIsrTime;

int16 i16DebugIndex,i16DebugData[4][360]; 

struct _sHarmSuperDataRegs HarmSuperData;
Uint32 u32TotalHarmValue;

float32 f32SinewaveTable[360];
int16 i16SinewaveTable[360];
/*****************************************************************************/
union	UN_PASSWORD_FLAG PASSWORD_FLAG;	// SETUP PASSWORD
union   UN_ADC_FLAG ADC_FLAG;           // ADC flag

union   UN_CONFIG CONFIG_FLAG;

union   UN_FAULT1_WARN FAULT1_FLAG;

union   UN_WARN_FLAG WARN_FLAG;         // Warning flag

union   UN_AC_FLAG AC_FLAG;             // ACIN flag

union   UN_BUS_FLAG BUS_FLAG;           // DC BUS flag

union   UN_WATT_FLAG WATT_FLAG;         // Watt flag

union   UN_FREQ_FLAG FREQ_FLAG;         	// Frequency Flag

union   UN_DryContactFlag DryContactFlag;         // DryContact flag
union   UN_SourceControlFlag SourceControlFlag, LastSourceControlFlag;

union   UN_TIMES_FLAG sTimesFlag;

union   UN_SYS_FLAG sSystemFlag;                    // System flag  

/******************************************************************************/
//DEFAULT SERIAL NUMBER
const Uint8 DEF_SERIAL_NUMBER[]  =       {"0123456789    "};

//Range:-30C~150C
const Uint16 u16NTC_HS_TABLE[TEMPERATURE_HS_LEN] =
{
    91   ,
    98   ,
    104  ,
    111  ,
    119  ,
    127  ,
    135  ,
    144  ,
    154  ,
    164  ,
    174  ,
    185  ,
    196  ,
    208  ,
    221  ,
    234  ,
    248  ,
    263  ,
    278  ,
    294  ,
    311  , 
    328  , 
    346  , 
    365  , 
    385  , 
    405  , 
    426  , 
    448  , 
    471  , 
    495  , 
    519  , 
    545  , 
    571  , 
    598  , 
    626  , 
    654  , 
    684  , 
    714  , 
    745  , 
    777  , 
    810  , 
    844  , 
    878  , 
    914  , 
    949  , 
    986  , 
    1023 , 
    1061 , 
    1100 , 
    1139 , 
    1179 , 
    1219 , 
    1260 , 
    1301 , 
    1343 , 
    1385 , 
    1428 , 
    1470 , 
    1513 , 
    1557 , 
    1600 , 
    1644 , 
    1687 , 
    1731 , 
    1775 , 
    1818 , 
    1862 , 
    1906 , 
    1949 , 
    1992 , 
    2035 , 
    2078 , 
    2120 , 
    2162 , 
    2204 , 
    2245 , 
    2286 , 
    2326 , 
    2366 , 
    2406 , 
    2445 , 
    2483 , 
    2521 , 
    2558 , 
    2595 , 
    2631 , 
    2667 , 
    2702 , 
    2736 , 
    2770 , 
    2803 , 
    2835 , 
    2867 ,
    2898 ,
    2928 ,
    2958 ,
    2987 ,
    3016 ,
    3044 ,
    3071 ,
    3097 ,
    3123 ,
    3149 ,
    3174 ,
    3198 ,
    3221 ,
    3244 ,
    3267 ,
    3288 ,
    3310 ,
    3330 ,
    3351 ,
    3370 ,
    3389 ,
    3408 ,
    3426 ,
    3444 ,
    3461 ,
    3478 ,
    3494 ,
    3510 ,
    3525 ,
    3540 ,
    3555 ,
    3569 ,
    3583 ,
    3596 ,
    3609 ,
    3622 ,
    3634 ,
    3646 ,
    3658 ,
    3669 ,
    3680 ,
    3691 ,
    3701 ,
    3711 ,
    3721 ,
    3731 ,
    3740 ,
    3749 ,
    3758 ,
    3766 ,
    3774 ,
    3782 ,
    3790 ,
    3798 ,
    3805 ,
    3812 ,
    3819 ,
    3826 ,
    3833 ,
    3839 ,
    3845 ,
    3851 ,
    3857 ,
    3863 ,
    3869 ,
    3874 ,
    3879 ,
    3884 ,
    3889 ,
    3894 ,
    3899 ,
    3904 ,
    3908 ,
    3912 ,
    3917 ,
    3921 ,
    3925 ,
    3929 ,
    3933 ,
    3936 ,
    3940 ,
    3943 ,
    3947 ,
    3950 ,
    3953 ,
    3956 ,
    3960 ,
    3963 

};

//Range:-20C~100C
const Uint16 u16NTC_AMB_TABLE[TEMPERATURE_AMB_LEN] =
{
    3579, 
    3557, 
    3534, 
    3510, 
    3486, 
    3460, 
    3435, 
    3408, 
    3381, 
    3353, 
    3325, 
    3295, 
    3265, 
    3235, 
    3204, 
    3172, 
    3139, 
    3106, 
    3073, 
    3038, 
    3004, 
    2968, 
    2933, 
    2897, 
    2860, 
    2823, 
    2786, 
    2749, 
    2711, 
    2673, 
    2634, 
    2595, 
    2557, 
    2518, 
    2478, 
    2439, 
    2400, 
    2361, 
    2321, 
    2282, 
    2243, 
    2203, 
    2164, 
    2125, 
    2087, 
    2048, 
    2010, 
    1972, 
    1934, 
    1896, 
    1859, 
    1822, 
    1785, 
    1749, 
    1713, 
    1678, 
    1643, 
    1609, 
    1575, 
    1541, 
    1508, 
    1475, 
    1443, 
    1411, 
    1380, 
    1349, 
    1319, 
    1290, 
    1261, 
    1232, 
    1204, 
    1176, 
    1149, 
    1123, 
    1097, 
    1071, 
    1046, 
    1022, 
    998, 
    974, 
    951, 
    929, 
    907, 
    885, 
    864, 
    844, 
    824, 
    804, 
    785, 
    766, 
    747, 
    729, 
    712, 
    695, 
    678, 
    662, 
    646, 
    630, 
    615, 
    600, 
    586, 
    572, 
    558, 
    545, 
    532, 
    519, 
    507, 
    495, 
    483, 
    472, 
    461, 
    450, 
    439, 
    429, 
    419, 
    409, 
    400, 
    390, 
    381, 
    373, 
    364 
};
/******************************************************************************/
#pragma DATA_SECTION(u8BootModel0 , "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel1 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel2 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel3 , "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel4 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel5 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel6 , "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel7 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel8 , "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel9 , "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel10, "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel11, "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel12, "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel13, "MODEL_ROM");
#pragma DATA_SECTION(u8BootModel14, "MODEL_ROM");   
#pragma DATA_SECTION(u8BootModel15, "MODEL_ROM");
const Uint8 u8BootModel0 			=   'T';	// 3相输出T,单相输出S.
const Uint8 u8BootModel1 			=   0x30 + (POWER_RATING/10000);	    
const Uint8 u8BootModel2 			=   0x30 + (POWER_RATING%10000)/1000;		
const Uint8 u8BootModel3 			=   0x30 + (POWER_RATING%1000)/100;		
const Uint8 u8BootModel4 			=   0x30 + (POWER_RATING%100)/10;	
const Uint8 u8BootModel5 			=   'K';	    
const Uint8 u8BootModel6 			=   0x30 + (HIGH_OP_VOLT_LIMIT/1000);	      	
const Uint8 u8BootModel7 			=   0x30 + (HIGH_OP_VOLT_LIMIT%1000)/100;
const Uint8 u8BootModel8 			=   0x30 + (HIGH_OP_VOLT_LIMIT%100)/10;	  	
const Uint8 u8BootModel9 			=   'V';	
#ifdef HIGH_FREQ_OUTPUT
const Uint8 u8BootModel10			=   0x30 + (NORMAL_OP_FREQ_LIMIT/1000);	   
const Uint8 u8BootModel11			=   0x30 + (NORMAL_OP_FREQ_LIMIT%1000)/100;
const Uint8 u8BootModel12			=   0x30 + (NORMAL_OP_FREQ_LIMIT%100)/10;	
#else
const Uint8 u8BootModel10			=   0x30 + (NORMAL_OP_FREQ_LIMIT/10000);	   
const Uint8 u8BootModel11			=   0x30 + (NORMAL_OP_FREQ_LIMIT%10000)/1000;
const Uint8 u8BootModel12			=   0x30 + (NORMAL_OP_FREQ_LIMIT%1000)/100;	
#endif
const Uint8 u8BootModel13			=   'H';	
const Uint8 u8BootModel14			=   'z';	
const Uint8 u8BootModel15			=   'a' + DETAIL_MODEL_TYPE;	


#pragma DATA_SECTION(u8FirmwareVersion0 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion1 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion2 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion3 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion4 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion5 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion6 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion7 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion8 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion9 , "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion10, "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion11, "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion12, "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion13, "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion14, "VERSION_ROM");
#pragma DATA_SECTION(u8FirmwareVersion15, "VERSION_ROM");
volatile const Uint8 u8FirmwareVersion0     =	' ';
volatile const Uint8 u8FirmwareVersion1     =	' ';
volatile const Uint8 u8FirmwareVersion2     =	' ';
volatile const Uint8 u8FirmwareVersion3     =	' ';
volatile const Uint8 u8FirmwareVersion4     =	' ';
volatile const Uint8 u8FirmwareVersion5     =	' ';
volatile const Uint8 u8FirmwareVersion6     =	' ';
volatile const Uint8 u8FirmwareVersion7     =	' ';
volatile const Uint8 u8FirmwareVersion8     =	' ';
volatile const Uint8 u8FirmwareVersion9     =	' ';
volatile const Uint8 u8FirmwareVersion10    =	' ';
volatile const Uint8 u8FirmwareVersion11    =	'V';
volatile const Uint8 u8FirmwareVersion12    =	0x30 + (FIRMWARE_VERSION/100);
volatile const Uint8 u8FirmwareVersion13    =	'.';
volatile const Uint8 u8FirmwareVersion14    =	0x30 + (FIRMWARE_VERSION%100)/10;
volatile const Uint8 u8FirmwareVersion15    =	0x30 + (FIRMWARE_VERSION%10);
