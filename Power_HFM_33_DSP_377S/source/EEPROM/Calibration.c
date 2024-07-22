/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Calibration.c

VERSION  : V0.0

DATE     : 2021/07/19

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/
#include "USR_VAR.H"
#include "calibration.h"
#include "math.h"
#include "Eeprom.h"
#include "E2pDriver.h"
#include "modbus.h"
#include "Measurement.h"

//----------------------------------------------------------------------------
// 检查GAIN的范围是否合理,如果不合理,维持原来的值
//----------------------------------------------------------------------------
Uint16 ChkEepromModbusSet(Uint16 DefaultGain,Uint16 oldGain,Uint16 *SetGain)
{
	Uint16 u16HighLimit,u16LowLimit;
	 
	u16HighLimit = (Uint16)(DefaultGain*1.3F);
	u16LowLimit = (Uint16)(DefaultGain*0.7F);
	
	if(*SetGain<u16LowLimit || *SetGain>u16HighLimit)
	{
		*SetGain = oldGain; 
	}

	return *SetGain;
}

//----------------------------------------------------------------------------
// calc new gain
//----------------------------------------------------------------------------
Uint16 GainCalc(Uint16 u16Address, Uint16 u16Data)
{
    static Uint16 u16NewGain,u16EepromAddress;   
    //Uint16 u16TemperatureIndex;

    switch(u16Address)
    {
        //------------------------------------------
        //R相参数校准	
        case 0x4A01:    //TEMPERATURE
            //u16NewGain = (Uint16)((Uint32)sEepDataInRam.u16GainExtBattTemp1*sADC_Cal.u16Temp_Ext1/u16EXT_BATT_TABLE[u16TemperatureIndex]);
            //sGain_R.wTEMP = ChkEepromModbusSet(GAIN_R_START_ADDRESS,&u16NewGain);
            //ExtEepromQueueIn(GAIN_R_START_ADDRESS, sGain_R.wTEMP);
            break;
            
        case 0x4A02:    //BUS
        	u16NewGain = (Uint16)((Uint32)sGain_R.wBUS*u16Data/sRMS.u16D2dBus_R);            
            sGain_R.wBUS = ChkEepromModbusSet(GAIN_BUS,sGain_R.wBUS,&u16NewGain);
            ExtEepromQueueIn(GAIN_R_START_ADDRESS + 1, sGain_R.wBUS);
        	break;
        	        	
        case 0x4A03:    //INV
        	u16NewGain = (Uint16)((Uint32)sGain_R.wINV*u16Data/sRMS.u16InvVolt_R);            
            sGain_R.wINV = ChkEepromModbusSet(GAIN_INV,sGain_R.wINV,&u16NewGain);
            ExtEepromQueueIn(GAIN_R_START_ADDRESS + 2, sGain_R.wINV);
        	break;
        	
        case 0x4A04:    //OPV
        	u16NewGain = (Uint16)((Uint32)sGain_R.wOPV*u16Data/sRMS.u16OpVolt_R);            
            sGain_R.wOPV = ChkEepromModbusSet(GAIN_OPV,sGain_R.wOPV,&u16NewGain);
            ExtEepromQueueIn(GAIN_R_START_ADDRESS + 3, sGain_R.wOPV);
        	break;
        	
        case 0x4A05:    //OPI
        	u16NewGain = (Uint16)((Uint32)sGain_R.wOPI*u16Data/sRMS.u16OpCurr_R);            
            sGain_R.wOPI = ChkEepromModbusSet(GAIN_OPI,sGain_R.wOPI,&u16NewGain);
            ExtEepromQueueIn(GAIN_R_START_ADDRESS + 4, sGain_R.wOPI);
        	break;
        
        case 0x4A06:    //WAT
        	u16NewGain = (Uint16)((Uint32)sGain_R.wWAT*u16Data/sRMS.u32OpWatt_R);            
            sGain_R.wWAT = ChkEepromModbusSet(GAIN_WAT,sGain_R.wWAT,&u16NewGain);
            ExtEepromQueueIn(GAIN_R_START_ADDRESS + 5, sGain_R.wWAT);
        	break;
        
        //------------------------------------------
        //S相参数校准	
        case 0x4A0A:
        	u16NewGain = (Uint16)((Uint32)sGain_S.wBUS*u16Data/sRMS.u16D2dBus_S);            
            sGain_S.wBUS = ChkEepromModbusSet(GAIN_BUS,sGain_S.wBUS,&u16NewGain);
            ExtEepromQueueIn(GAIN_S_START_ADDRESS + 1, sGain_S.wBUS);
        	break;
        	        	
        case 0x4A0B:
        	u16NewGain = (Uint16)((Uint32)sGain_S.wINV*u16Data/sRMS.u16InvVolt_S);            
            sGain_S.wINV = ChkEepromModbusSet(GAIN_INV,sGain_S.wINV,&u16NewGain);
            ExtEepromQueueIn(GAIN_S_START_ADDRESS + 2, sGain_S.wINV);
        	break;
        	
        case 0x4A0C:
        	u16NewGain = (Uint16)((Uint32)sGain_S.wOPV*u16Data/sRMS.u16OpVolt_S);            
            sGain_S.wOPV = ChkEepromModbusSet(GAIN_OPV,sGain_S.wOPV,&u16NewGain);
            ExtEepromQueueIn(GAIN_S_START_ADDRESS + 3, sGain_S.wOPV);
        	break;
        	
        case 0x4A0D:
        	u16NewGain = (Uint16)((Uint32)sGain_S.wOPI*u16Data/sRMS.u16OpCurr_S);            
            sGain_S.wOPI = ChkEepromModbusSet(GAIN_OPI,sGain_S.wOPI,&u16NewGain);
            ExtEepromQueueIn(GAIN_S_START_ADDRESS + 4, sGain_S.wOPI);
        	break;
        
        case 0x4A0E:
        	u16NewGain = (Uint16)((Uint32)sGain_S.wWAT*u16Data/sRMS.u32OpWatt_S);            
            sGain_S.wWAT = ChkEepromModbusSet(GAIN_WAT,sGain_S.wWAT,&u16NewGain);
            ExtEepromQueueIn(GAIN_S_START_ADDRESS + 5, sGain_S.wWAT);
        	break;	
            
        //------------------------------------------
        //T相参数校准
        case 0x4A12:
        	u16NewGain = (Uint16)((Uint32)sGain_T.wBUS*u16Data/sRMS.u16D2dBus_T);            
            sGain_T.wBUS = ChkEepromModbusSet(GAIN_BUS,sGain_T.wBUS,&u16NewGain);
            ExtEepromQueueIn(GAIN_T_START_ADDRESS + 1, sGain_T.wBUS);
        	break;
        	        	
        case 0x4A13:
        	u16NewGain = (Uint16)((Uint32)sGain_T.wINV*u16Data/sRMS.u16InvVolt_T);            
            sGain_T.wINV = ChkEepromModbusSet(GAIN_INV,sGain_T.wINV,&u16NewGain);
            ExtEepromQueueIn(GAIN_T_START_ADDRESS + 2, sGain_T.wINV);
        	break;
        	
        case 0x4A14:
        	u16NewGain = (Uint16)((Uint32)sGain_T.wOPV*u16Data/sRMS.u16OpVolt_T);            
            sGain_T.wOPV = ChkEepromModbusSet(GAIN_OPV,sGain_T.wOPV,&u16NewGain);
            ExtEepromQueueIn(GAIN_T_START_ADDRESS + 3, sGain_T.wOPV);
        	break;
        	
        case 0x4A15:
        	u16NewGain = (Uint16)((Uint32)sGain_T.wOPI*u16Data/sRMS.u16OpCurr_T);            
            sGain_T.wOPI = ChkEepromModbusSet(GAIN_OPI,sGain_T.wOPI,&u16NewGain);
            ExtEepromQueueIn(GAIN_T_START_ADDRESS + 4, sGain_T.wOPI);
        	break;
        
        case 0x4A16:
        	u16NewGain = (Uint16)((Uint32)sGain_T.wWAT*u16Data/sRMS.u32OpWatt_T);            
            sGain_T.wWAT = ChkEepromModbusSet(GAIN_WAT,sGain_T.wWAT,&u16NewGain);
            ExtEepromQueueIn(GAIN_T_START_ADDRESS + 5, sGain_T.wWAT);
        	break;     
        	
        default:
            break;
    }
    return u16NewGain;
}



