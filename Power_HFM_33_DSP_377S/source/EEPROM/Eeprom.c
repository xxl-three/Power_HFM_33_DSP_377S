/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Eeprom.c

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/
#include "math.h"
#include "USR_VAR.H"
#include "Eeprom.h"
#include "RTC.h"
#include "E2pDriver.h"
#include "modbus.h"
#include "Measurement.h"

//******************************************************************************
Uint16   u16E2pCommand;                         // EEPROM COMMAND register
Uint16   u16E2pAddress;                        // EEPROM ADDRESS register
Uint16   u16E2pData;                        // EEPROM DATA register

union UN_E2pFlag sEepromFlag;

struct S_Eeprom sEeprom;
union   UN_EE_FLAG sEeOperateFlag;             // EEPROM flag

ee_data_structure sEE_Data_In_Ram;

Uint16 	u16Ex_EEPROM_Default_Count;

//event log
Uint16	wEvent_Log_Words_Num_Index;
Uint16	wEvent_Log_Buffer[EVENT_LOG_BUFFER_NUM];
Uint16	wEvent_Log_Total_Count;
Uint16	wEvent_Log_Current_Count;
Uint16	wEvent_Log_Index;			//0~499 in eeprom. if > 499 wrap to 0
Uint16	wPre_Event_Log_Index;
Uint16	wEvent_Log_Inner_Index;		//0~3 words.
Uint16 	pwEvent_Log_Address;
Uint16 	wEvent_Log_Data;
Uint16 	u16Event_Log_Update_Count;



/********************************************************************************************/
/* Default_EEPROM		默认														    	*/
/* 																						  	*/
/* 																                           	*/
/*                                                                                        	*/
/*                                                                                        	*/
/********************************************************************************************/
void EEPROM_Default(void)
{
    //int16* ptemp_eeprom_in_ram;
    int16 i;

    u16GainDcOffset = DCOFFSET;
    u16IpeakProtect = 50;
    u16PowerRecycle = 0;
    sSet.u16OutputMode.u16Word = 0;
    sSet.u16SRPhaseAngle = 120;
    sSet.u16TRPhaseAngle = 240;
    
    sGain_R.wTEMP = GAIN_TEMP;
    sGain_R.wBUS = GAIN_BUS;
    sGain_R.wINV = GAIN_INV;
    sGain_R.wOPV = GAIN_OPV;
    sGain_R.wOPI = GAIN_OPI;
    sGain_R.wWAT = GAIN_WAT;
    sGain_R.wPWM = GAIN_PWM;
    sGain_R.wAngle = GAIN_ANGLE;
    sGain_R.wIPV = GAIN_IPV;

    sGain_S.wTEMP = GAIN_TEMP;
    sGain_S.wBUS = GAIN_BUS;
    sGain_S.wINV = GAIN_INV;
    sGain_S.wOPV = GAIN_OPV;
    sGain_S.wOPI = GAIN_OPI;
    sGain_S.wWAT = GAIN_WAT;
    sGain_S.wPWM = GAIN_PWM;
    sGain_S.wAngle = GAIN_ANGLE;
    sGain_S.wIPV = GAIN_IPV;

    sGain_T.wTEMP = GAIN_TEMP;
    sGain_T.wBUS = GAIN_BUS;
    sGain_T.wINV = GAIN_INV;
    sGain_T.wOPV = GAIN_OPV;
    sGain_T.wOPI = GAIN_OPI;
    sGain_T.wWAT = GAIN_WAT;
    sGain_T.wPWM = GAIN_PWM;
    sGain_T.wAngle = GAIN_ANGLE;
    sGain_T.wIPV = GAIN_IPV;

    sSet.u16RemainingRunTime = 0xFFFF;

    CONFIG_FLAG.WORD = 0x0000;
    CONFIG_FLAG.sBit.InverterSoftstartEnable = 1;
    CONFIG_FLAG.sBit.RunTimeLimitEnable = 0;

    //ptemp_eeprom_in_ram = (int16*)&sEE_Data_In_Ram;

    u8ProductID = 1;
    ExtEepromQueueIn(EEP_ADDR_PRODUCT_ID, u8ProductID);
    ExtEepromQueueIn(EEP_ADDR_IPEAK, u16IpeakProtect);
    ExtEepromQueueIn(EEP_ADDR_SR_PHASE,sSet.u16SRPhaseAngle);
    ExtEepromQueueIn(EEP_ADDR_TR_PHASE,sSet.u16TRPhaseAngle);
    ExtEepromQueueIn(EEP_ADDR_OFFSET, u16GainDcOffset);
    ExtEepromQueueIn(EEP_ADDR_POWER_RECYCLE, u16PowerRecycle);
    ExtEepromQueueIn(EEP_ADDR_POWER_RECYCLE_LEVEL, 0);

    ExtEepromQueueIn(GAIN_R_START_ADDRESS, sGain_R.wTEMP  );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 1, sGain_R.wBUS   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 2, sGain_R.wINV   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 3, sGain_R.wOPV   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 4, sGain_R.wOPI   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 5, sGain_R.wWAT   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 6, sGain_R.wPWM   );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 7, sGain_R.wAngle );
    ExtEepromQueueIn(GAIN_R_START_ADDRESS + 8, sGain_R.wIPV   );

    ExtEepromQueueIn(GAIN_S_START_ADDRESS, sGain_S.wTEMP  );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 1, sGain_S.wBUS   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 2, sGain_S.wINV   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 3, sGain_S.wOPV   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 4, sGain_S.wOPI   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 5, sGain_S.wWAT   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 6, sGain_S.wPWM   );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 7, sGain_S.wAngle );
    ExtEepromQueueIn(GAIN_S_START_ADDRESS + 8, sGain_S.wIPV   );

    ExtEepromQueueIn(GAIN_T_START_ADDRESS, sGain_T.wTEMP  );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 1, sGain_T.wBUS   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 2, sGain_T.wINV   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 3, sGain_T.wOPV   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 4, sGain_T.wOPI   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 5, sGain_T.wWAT   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 6, sGain_T.wPWM   );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 7, sGain_T.wAngle );
    ExtEepromQueueIn(GAIN_T_START_ADDRESS + 8, sGain_T.wIPV   );

    ExtEepromQueueIn(EEP_ADDR_RUN_TIME_LIMIT, sSet.u16RemainingRunTime);
    ExtEepromQueueIn(EEP_ADDR_SOURCE_CONFIG, CONFIG_FLAG.WORD);
    

    ExtEepromQueueIn(EEP_ADDR_PASSWORD, 0xA5A5);



    //Initial_Variable();

    //sEepromFlag.sBit.u1ReturnDefaultValue = 1;
}


/********************************************************************************************
*ROUTINE:
*DESCRIPTION:
*
*
*
*********************************************************************************************/
Uint16 External_EEPROM_Read(int16 addr)
{
    E2P_Read(addr, &u16E2pData);

    return u16E2pData;
}

//----------------------------------------------------------------------------
// 检测读取的EEPROM中保存的gain参数，
// 如果超出默认范围，则恢复Default值
//----------------------------------------------------------------------------
void CheckGainErrorLimit(Uint16 *u16Gain, Uint16 u16DefaultGain)
{
    Uint16 u16LowLimit, u16HighLimit;

    u16LowLimit = (Uint16)(u16DefaultGain*0.7F);
    if(u16DefaultGain > 50000)   //65535/1.3=50411.5
    {
        u16HighLimit = 65535;
    }
    else
    {
        u16HighLimit = (Uint16)(u16DefaultGain*1.3F);
    }

    if((*u16Gain > u16HighLimit) || (*u16Gain < u16LowLimit))
    {
        *u16Gain = u16DefaultGain;
    }
}

//----------------------------------------------------------------------------
//上电读取EEPROM中的所有数据
// 
//----------------------------------------------------------------------------
void Read_Ex_EEPROM_Parameter(void)
{
    int16  i;
    int16* ptemp_eeprom_in_ram;
    Uint16 u16Data, startAddress;
    Uint16 i_uint_HZ;
    Uint32 u32Data;

    u16Data = External_EEPROM_Read(EEP_ADDR_PASSWORD);
    if(u16Data != 0xA5A5)
    {
        EEPROM_Default();
    }
    else
    {
        CONFIG_FLAG.WORD = External_EEPROM_Read(EEP_ADDR_SOURCE_CONFIG);
        sSet.u16RemainingRunTime = External_EEPROM_Read(EEP_ADDR_RUN_TIME_LIMIT);
        
        sPdRunTime.u16RunTimeDate = External_EEPROM_Read(EEP_ADDR_RUNTIME_DATE);
        sPdRunTime.sHourMinute.u16All = External_EEPROM_Read(EEP_ADDR_RUNTIME_HOUR_MINUTE);

        sRMS.u32TotalKWH_R = External_EEPROM_Read(EEP_ADDR_ENERGY_R_LO);
        u16Data = External_EEPROM_Read(EEP_ADDR_ENERGY_R_HI);
        sRMS.u32TotalKWH_R += (Uint32)u16Data << 16;
        
        sRMS.u32TotalKWH_S = External_EEPROM_Read(EEP_ADDR_ENERGY_S_LO);
        u16Data = External_EEPROM_Read(EEP_ADDR_ENERGY_S_HI);
        sRMS.u32TotalKWH_S += (Uint32)u16Data << 16;
        
        sRMS.u32TotalKWH_T = External_EEPROM_Read(EEP_ADDR_ENERGY_T_LO);
        u16Data = External_EEPROM_Read(EEP_ADDR_ENERGY_T_HI);
        sRMS.u32TotalKWH_T += (Uint32)u16Data << 16;

        u16Data = External_EEPROM_Read(EEP_ADDR_PRODUCT_ID);
        if(u16Data == 0 || u16Data > 100)
        {
            u16Data = 1;
        }
        u8ProductID = u16Data;

        u16Data = External_EEPROM_Read(EEP_ADDR_IPEAK);
        u16IpeakProtect = u16Data;
        
        sSet.u16OutputMode.u16Word = External_EEPROM_Read(EEP_ADDR_OUTPUT_MODE);    
		if(sSet.u16OutputMode.sByte.u8HiByte > 1)
		    sSet.u16OutputMode.sByte.u8HiByte = 0;
		if(sSet.u16OutputMode.sByte.u8LowByte > 1)
		    sSet.u16OutputMode.sByte.u8LowByte = 0;
        
        u16Data = External_EEPROM_Read(EEP_ADDR_SR_PHASE);
        if(u16Data <= 360)
        {
            sSet.u16SRPhaseAngle = u16Data;
        }
        else
        {
            sSet.u16SRPhaseAngle = 120;
        }
        
        u16Data = External_EEPROM_Read(EEP_ADDR_TR_PHASE);
        if(u16Data <= 360)
        {
            sSet.u16TRPhaseAngle = u16Data;
        }
        else
        {
            sSet.u16TRPhaseAngle = 240;
        }

        u16Data = External_EEPROM_Read(EEP_ADDR_OFFSET);
        if(u16Data >= 2024 && u16Data <= 2072)
        {
            u16GainDcOffset = u16Data;
        }
        else
        {
            u16GainDcOffset = DCOFFSET;
        }
        
        u16Data = External_EEPROM_Read(EEP_ADDR_POWER_RECYCLE);
        if(u16Data == 1)
            u16PowerRecycle = 1;
        else
            u16PowerRecycle = 0;
            
        u16Data = External_EEPROM_Read(EEP_ADDR_POWER_RECYCLE_LEVEL);    
        if(u16Data <= 3)
            u16PowerRecycleLevel = u16Data;
        else
            u16PowerRecycleLevel = 0;

        //load the programming information form the memory
        u16Data = External_EEPROM_Read(EEP_ADDR_CURRENT_PROGRAM_MEMORY);

        if(u16Data == 0 || u16Data > PROGRAM_MEMORY_NUMBER)
        {
            u16Data = 1;
        }

        //copy the first 2 word eeprom data to ram
        //ptemp_eeprom_in_ram = (int16*)&sProgramming.u16Deep;
        //startAddress = PROGRAM_START_ADDRESS + (u16Data - 1) * PROGRAM_DATA_LENGTH;
        //for(i = 0; i < PROGRAM_DATA_LENGTH; i++)
        //{
        //    *(ptemp_eeprom_in_ram + i) = External_EEPROM_Read(startAddress + i);
        //}

        //copy the gain data to ram
        ptemp_eeprom_in_ram = (int16*)&sGain_R.wTEMP;
        for(i = 0; i < GAIN_DATA_LENGTH; i++)
        {
            *(ptemp_eeprom_in_ram + i) = External_EEPROM_Read(GAIN_R_START_ADDRESS + i);
        }

        if(sGain_R.wTEMP > (GAIN_TEMP * 2))
        {
            sGain_R.wTEMP = 10;
        }        
        CheckGainErrorLimit(&sGain_R.wBUS,  GAIN_BUS);
        CheckGainErrorLimit(&sGain_R.wINV,  GAIN_INV);
        CheckGainErrorLimit(&sGain_R.wOPV,  GAIN_OPV);
        CheckGainErrorLimit(&sGain_R.wOPI,  GAIN_OPI);
        CheckGainErrorLimit(&sGain_R.wWAT,  GAIN_WAT);
        CheckGainErrorLimit(&sGain_R.wPWM,  GAIN_PWM);
        CheckGainErrorLimit(&sGain_R.wIPV,  GAIN_IPV);
        if(sGain_R.wAngle > 359)
        {
            sGain_R.wAngle = GAIN_ANGLE;
        }
        

        ptemp_eeprom_in_ram = (int16*)&sGain_S.wTEMP;
        for(i = 0; i < GAIN_DATA_LENGTH; i++)
        {
            *(ptemp_eeprom_in_ram + i) = External_EEPROM_Read(GAIN_S_START_ADDRESS + i);
        }

        if(sGain_S.wTEMP > (GAIN_TEMP * 2))
        {
            sGain_S.wTEMP = 10;
        }
        CheckGainErrorLimit(&sGain_S.wBUS,  GAIN_BUS);
        CheckGainErrorLimit(&sGain_S.wINV,  GAIN_INV);
        CheckGainErrorLimit(&sGain_S.wOPV,  GAIN_OPV);
        CheckGainErrorLimit(&sGain_S.wOPI,  GAIN_OPI);
        CheckGainErrorLimit(&sGain_S.wWAT,  GAIN_WAT);
        CheckGainErrorLimit(&sGain_S.wPWM,  GAIN_PWM);
        CheckGainErrorLimit(&sGain_S.wIPV,  GAIN_IPV);
        if(sGain_S.wAngle > 359)
        {
            sGain_S.wAngle = GAIN_ANGLE;
        }
        

        ptemp_eeprom_in_ram = (int16*)&sGain_T.wTEMP;
        for(i = 0; i < GAIN_DATA_LENGTH; i++)
        {
            *(ptemp_eeprom_in_ram + i) = External_EEPROM_Read(GAIN_T_START_ADDRESS + i);
        }

        if(sGain_T.wTEMP > (GAIN_TEMP * 2))
        {
            sGain_T.wTEMP = 10;
        }
        CheckGainErrorLimit(&sGain_T.wBUS,  GAIN_BUS);
        CheckGainErrorLimit(&sGain_T.wINV,  GAIN_INV);
        CheckGainErrorLimit(&sGain_T.wOPV,  GAIN_OPV);
        CheckGainErrorLimit(&sGain_T.wOPI,  GAIN_OPI);
        CheckGainErrorLimit(&sGain_T.wWAT,  GAIN_WAT);
        CheckGainErrorLimit(&sGain_T.wPWM,  GAIN_PWM);
        CheckGainErrorLimit(&sGain_T.wIPV,  GAIN_IPV);
        if(sGain_T.wAngle > 359)
        {
            sGain_T.wAngle = GAIN_ANGLE;
        }
        
        //读取谐波数据
        HarmSuperData.u16HarmConfig1 = External_EEPROM_Read(HARM_START_ADDRESS);
        //HarmSuperData.u16Command = External_EEPROM_Read(HARM_START_ADDRESS+1);
        //copy the gain data to ram
        ptemp_eeprom_in_ram = (int16*)&HarmSuperData.i16Harmrms[0];
        for(i = 0; i < (HARM_DATA_LENGTH-2); i++)
        {
            u16Data = External_EEPROM_Read(HARM_START_ADDRESS + 2 + i);
            if(u16Data <= 100)
                *(ptemp_eeprom_in_ram + i) = u16Data;
            else
                *(ptemp_eeprom_in_ram + i) = 0;
            
        }
    }
}

/********************************************************************************************
*DESCRIPTION: Update external eeprom mechanism
*	because writing 93lc86 needs 3ms, we should not writing data in a main cycle.
* 	we should seperate the data into several main loops
*
*
*********************************************************************************************/
void External_EEPROM_Update(void)
{
    int16* ptemp_eeprom_in_ram;
    static Uint16 u16Result;
    int16 i;

    if(sEepromFlag.sBit.u1ReturnDefaultValue)
    {
        //copy eeprom data to ram
        ptemp_eeprom_in_ram = (int16*)&sEE_Data_In_Ram;

        //write password 0xffff first and after writing all eeprom data, write 0xa5a5 to password
        if(u16Ex_EEPROM_Default_Count >= EEPROM_TOTAL_WORDS)
        {
            sEE_Data_In_Ram.wPassword = 0xA5A5;
            u16Ex_EEPROM_Default_Count = 0;
            u16Result = E2P_Write(u16Ex_EEPROM_Default_Count, *(ptemp_eeprom_in_ram + u16Ex_EEPROM_Default_Count));
            if(u16Result == E2P_DRIVER_SUCCESS)
            {
                u16Ex_EEPROM_Default_Count = 0;
                sEepromFlag.sBit.u1ReturnDefaultValue = 0;
            }
        }
        else
        {
            u16Result = E2P_Write(u16Ex_EEPROM_Default_Count, *(ptemp_eeprom_in_ram + u16Ex_EEPROM_Default_Count));
            if(u16Result == E2P_DRIVER_SUCCESS)
            {
                u16Ex_EEPROM_Default_Count++;
            }
        }
    }
    else if(sEepromFlag.sBit.u1EraseAlarmHistory)
    {
        E2P_Write(u16Ex_EEPROM_Default_Count, 0xFFFF);
        u16Event_Log_Update_Count++;

        if(u16Event_Log_Update_Count == 1024)
        {
            sEepromFlag.sBit.u1EraseAlarmHistory = 0;
        }
    }
    else if(sEeprom.u16Count > 0)
    {
        if(ExtEepromQueueOut() == QUEUE_OPERATE_OK)
        {
            E2P_Write(sEeprom.u16OutData[0], sEeprom.u16OutData[1]);
        }
    }

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Check_EEPROM_Operating_Finished(void)
{
    static Uint16  u16EepromOperatingFinishedCnt;

    if (sEepromFlag.sBit.u1ReturnDefaultValue ||
        sEepromFlag.sBit.u1EraseAlarmHistory ||
        sEeprom.u16Count > 0 //||
        //sEvent.u16Count > 0
       )
    {
        u16EepromOperatingFinishedCnt = 0;
        sEeOperateFlag.sBit.u1Finished = 0;
    }
    else
    {
        if (u16EepromOperatingFinishedCnt < 10)
        {
            u16EepromOperatingFinishedCnt++;
        }
        else
        {
            sEeOperateFlag.sBit.u1Finished = 1;
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Uint16 ExtEepromQueueIn(Uint16 u16Index, Uint16 u16Value)
{
    if((sEeprom.u16Front == sEeprom.u16Rear) && (sEeprom.u16Count == EEPROM_DATA_FIFO_SIZE)) //判断如果队列满了
    {
        return QUEUE_FULL;    //返回队列满的标志
    }

    sEeprom.u16FIFO[sEeprom.u16Rear][0] = u16Index;
    sEeprom.u16FIFO[sEeprom.u16Rear][1] = u16Value;

    sEeprom.u16Rear = ((sEeprom.u16Rear + 1) % EEPROM_DATA_FIFO_SIZE);
    sEeprom.u16Count++;

    Check_EEPROM_Operating_Finished();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Uint16 ExtEepromQueueOut(void)
{
    if((sEeprom.u16Front == sEeprom.u16Rear) && (sEeprom.u16Count == 0))   // empty
    {
        return QUEUE_EMPTY;
    }
    else
    {
        // out
        sEeprom.u16OutData[0] = sEeprom.u16FIFO[sEeprom.u16Front][0];
        sEeprom.u16OutData[1] = sEeprom.u16FIFO[sEeprom.u16Front][1];
        sEeprom.u16Front = ((sEeprom.u16Front + 1) % EEPROM_DATA_FIFO_SIZE);
        sEeprom.u16Count--;

        return QUEUE_OPERATE_OK;
    }
}

/*
void Update_Event_Log_Index(void)
{
	if(wPre_Event_Log_Index != wEvent_Log_Index)
	{
		wPre_Event_Log_Index = wEvent_Log_Index;
		sEE_Data_In_Ram.wEvent_Log_Index = wEvent_Log_Index;

		External_EEPROM_Queue_Add(90, sEE_Data_In_Ram.wEvent_Log_Index);
	}
}

void Event_Log_Queue_Add(Uint16 wEventCode)
{
	wEvent_Log_Buffer[wEvent_Log_Total_Count] = wEventCode;
	if(wEvent_Log_Total_Count < EVENT_LOG_BUFFER_NUM )
		wEvent_Log_Total_Count++;
}
*/

void Updata_Power_Energy(void)
{
    Uint16 data;

    data = (Uint16)(sRMS.u32TotalKWH_R >> 16);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_R_HI, data);
    data = (Uint16)(sRMS.u32TotalKWH_R&0x0000FFFF);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_R_LO, data);
    
    data = (Uint16)(sRMS.u32TotalKWH_S >> 16);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_S_HI, data);
    data = (Uint16)(sRMS.u32TotalKWH_S&0x0000FFFF);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_S_LO, data);
    
    data = (Uint16)(sRMS.u32TotalKWH_T >> 16);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_T_HI, data);
    data = (Uint16)(sRMS.u32TotalKWH_T&0x0000FFFF);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_T_LO, data);

}

void Clear_Power_Energy(void)
{
    sRMS.u32TotalKWH_R = 0;
    sRMS.u32TotalKWH_R = 0;
    sRMS.u32TotalKWH_R = 0;
    sRMS.u32TotalKWH = 0;
    
    ExtEepromQueueIn(EEP_ADDR_ENERGY_R_HI, 0);    
    ExtEepromQueueIn(EEP_ADDR_ENERGY_R_LO, 0);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_S_HI, 0);    
    ExtEepromQueueIn(EEP_ADDR_ENERGY_S_LO, 0);
    ExtEepromQueueIn(EEP_ADDR_ENERGY_T_HI, 0);    
    ExtEepromQueueIn(EEP_ADDR_ENERGY_T_LO, 0); 
   
}

void Updata_PD_RunTime(void)
{
    Uint16 data;

    
    ExtEepromQueueIn(EEP_ADDR_RUNTIME_DATE, sPdRunTime.u16RunTimeDate);
    
    ExtEepromQueueIn(EEP_ADDR_RUNTIME_HOUR_MINUTE, sPdRunTime.sHourMinute.u16All);
    
}


void Clear_PD_RunTime(void)
{
    sPdRunTime.u16RunTimeDate = 0;
    sPdRunTime.sHourMinute.u16All = 0;
    sPdRunTime.sSecondNone.u16All = 0;
    
    ExtEepromQueueIn(EEP_ADDR_RUNTIME_DATE, sPdRunTime.u16RunTimeDate);    
    ExtEepromQueueIn(EEP_ADDR_RUNTIME_HOUR_MINUTE, sPdRunTime.sHourMinute.u16All);
   
}



void Load_Program_data(int16 num)
{
    int16  i;
    int16* ptemp_eeprom_in_ram;
    Uint16 startAddress;

    //save the index of the programming information into the memory
    //ExtEepromQueueIn(EEP_ADDR_CURRENT_PROGRAM_MEMORY,num);

    //copy the first 2 word eeprom data to ram
    ptemp_eeprom_in_ram = (int16*)&sProgramming.u16Deep;
    startAddress = PROGRAM_START_ADDRESS + (num - 1) * PROGRAM_DATA_LENGTH;
    for(i = 0; i < PROGRAM_DATA_LENGTH; i++)
    {
        *(ptemp_eeprom_in_ram + i) = External_EEPROM_Read(startAddress + i);
    }
}

void Save_Program_data(int16 num)
{
    int16  i;
    int16* ptemp_eeprom_in_ram;
    Uint16 startAddress;

    //copy the first 2 word eeprom data to ram
    ptemp_eeprom_in_ram = (int16*)&sProgramming.u16Deep;
    startAddress = PROGRAM_START_ADDRESS + (num - 1) * PROGRAM_DATA_LENGTH;
    for(i = 0; i < PROGRAM_DATA_LENGTH; i++)
    {
        ExtEepromQueueIn((startAddress + i), *(ptemp_eeprom_in_ram + i));
    }
}



//----------------------------------------------------------------------------
// 执行EEPROM更新的任务
// Timebase: main loop
//----------------------------------------------------------------------------
void EepromTask(void)
{
    //CheckEepromReady();

    //if(sEepromFlag.sBit.u1Ready)
    {
        External_EEPROM_Update();
        Check_EEPROM_Operating_Finished();
    }
}
