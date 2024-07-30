/*****************************************************************************
Coding   ??ANSI ????????GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : modbus.c

VERSION  : V0.0

DATE     : 2018/06/06

AUTHOR   : HUI

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#include <string.h>
#include "modbus.h"
#include "Crc.h"
#include "USR_VAR.H"
#include "eeprom.h"
#include "RTC.h"
#include "booter.h"
#include "Measurement.h"
#include "WaveLog.h"

//----------------------------------------------------------------------------
const unsigned char ifSet[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char   u8ProductID;              //for MODBUS protocol used
unsigned int usTimerT35_50us;



Uint16 u16Address;
Uint32 u32Address;
Uint16 u16Index;
Uint16 u16Quantity;
Uint16 u16OutputValue; // for write single coil & write single holding register用于写单线圈和写单保持寄存器
Uint16 u16Offset;
Uint8  u8RealPosHi, u8ReplyPosHi;
Uint16 u16ErrorCode;
Uint16 u16CurAddr;
int16 *pi16StartInRam;


_StructModbusRxTx sModbusRxTx[MBUS_INTERFACE_NUM];
_StructModbusData sModbusData;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void MBus_TimeOutInit(unsigned int ulBaudRate)
//该函数根据波特率初始化超时计时器。对于高于 19200 的波特率，使用固定的 1750 微秒计时器值。否则，计算字符时间的 3.5 倍。
{
    // If baudrate > 19200 then we should use the fixed timer values
    // t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
    if(ulBaudRate > 19200)
    {
        usTimerT35_50us = 35;       // 1750us.	counting unit:50us
    }
    else
    {
        // The timer reload value for a character is given by:
        //
        // ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
        //             = 11 * Ticks_per_1s / Baudrate
        //             = 220000 / Baudrate
        // The reload for t3.5 is 1.5 times this value and similary
        // for t3.5.
        usTimerT35_50us = (7UL * 220000UL) / (2UL * ulBaudRate);
    }
    usTimerT35_50us = 6;
}

//----------------------------------------------------------------------------
//该函数初始化指定端口的接收和发送参数
//----------------------------------------------------------------------------
void MBus_InitialRxTx(Uint16 u16MbusPortIndex)
{
    sModbusRxTx[u16MbusPortIndex].u8InterfaceID = u16MbusPortIndex;
    sModbusRxTx[u16MbusPortIndex].u16RxTimer = 0;
    sModbusRxTx[u16MbusPortIndex].i16LenRx = 0;
    sModbusRxTx[u16MbusPortIndex].i16LenTx = 0;
    sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;
    sModbusRxTx[u16MbusPortIndex].u8fState = MBUS_STATE_RX;
    sModbusRxTx[u16MbusPortIndex].u8fRxOK = 0;

}

//----------------------------------------------------------------------------
// Description: Initialize parameters of modbus data
// Arg: void
// Return: void
// 该函数初始化 MODBUS 数据，包括离散输入、线圈、保持寄存器和输入寄存器。
//----------------------------------------------------------------------------
void MBus_InitialModbusData(void)
{
    int iMax = 0;
    int i = 0;

    // initialize discrete inputs
    #ifdef MBUS_SUPPORT_DISCRETE_INPUT
    iMax = MBUS_MAX_DISCREATE_INPUTS % 8 == 0 ? MBUS_MAX_DISCREATE_INPUTS / 8 : MBUS_MAX_DISCREATE_INPUTS / 8 + 1;
    for(i = 0; i < iMax; i++)
    {
        sModbusData.discreteInputs[i] = 0;
    }
    #endif

    // initialize coils
    #ifdef MBUS_SUPPORT_COILS
    iMax = MBUS_MAX_COILS % 8 == 0 ? MBUS_MAX_COILS / 8 : MBUS_MAX_COILS / 8 + 1;
    for(i = 0; i < iMax; i++)
    {
        sModbusData.coils[i] = 0;
    }
    #endif

    #ifdef MBUS_SUPPORT_HOLDING_REGS
    // initialize holding registers
    //***********************************************
    // - identifiers data
    //***********************************************
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  0] = POWER_RATING; //Rating KW*10
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  1] = FIRMWARE_VERSION; //Firmware main version
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  2] = HIGH_OP_VOLT_PROG_LIMIT;  //
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  3] = DETAIL_MODEL_TYPE * 100 + 33; //model type 3/3
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  4] = HIGH_OP_VOLT_LIMIT;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  5] = LOW_OP_CURRENT_LIMIT;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  6] = LOW_OP_VOLT_LIMIT;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS +  7] = OP_FREQ_UNIT;

    //   - word 3-7: Serial #
    //for(i = 0; i < 5; i++)
    //          sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_IDS+3+i] = (unsigned int)SerialNumber[2*i]+(unsigned int)SerialNumber[2*i+1]*256;

    // - status
    for(i = MBUS_HREGS_SIDX_STATES; i < (MBUS_HREGS_SIDX_STATES + MBUS_HREGS_LEN_STATES); i++)
    {
        sModbusData.u16HoldingRegs[i] = 0;
    }
    // - alarms
    for(i = MBUS_HREGS_SIDX_ALARMS; i < (MBUS_HREGS_SIDX_ALARMS + MBUS_HREGS_LEN_ALARMS); i++)
    {
        sModbusData.u16HoldingRegs[i] = 0;
    }
    // - measurements
    for(i = MBUS_HREGS_SIDX_MEASUREMENTS; i < (MBUS_HREGS_SIDX_MEASUREMENTS + MBUS_HREGS_LEN_MEASUREMENTS); i++)
    {
        sModbusData.u16HoldingRegs[i] = 0xFFFF;
    }
    // - configurations
    for(i = MBUS_HREGS_SIDX_CONF; i < (MBUS_HREGS_SIDX_CONF + MBUS_HREGS_LEN_CONF); i++)
    {
        sModbusData.u16HoldingRegs[i] = 0xFFFF;
    }
    /*
    // date/time and power share are not present in NETYS PR
    // date/time
    for(i = MBUS_HREGS_SIDX_DATETIME; i < (MBUS_HREGS_SIDX_DATETIME+MBUS_HREGS_LEN_DATETIME); i++)
        sModbusData.u16HoldingRegs[i] = 0xFFFF;
    // power share

    */

    #endif // #ifdef MBUS_SUPPORT_HOLDING_REGS

    #ifdef MBUS_SUPPORT_INPUT_REGS
    // initialize input registers
    for(i = 0; i < MBUS_MAX_INPUT_REGISTERS; i++)
    {
        sModbusData.u16InputRegs[i] = 0xFFFF;
    }
    #endif
}


//----------------------------------------------------------------------------
// Description: Initialize parameters of modbus
// Arg: void
// Return: void
// 该函数初始化 MODBUS，包括超时计时器、接收/发送参数和 MODBUS 数据。
//----------------------------------------------------------------------------
void MBus_InitialModbus(void)
{
    MBus_TimeOutInit(9600);

    // initialize rx/tx parameters
    #ifdef  MBUS_INTERFACE_0
    MBus_InitialRxTx(0);
    #endif

    #ifdef  MBUS_INTERFACE_1
    MBus_InitialRxTx(1);
    #endif

    #ifdef  MBUS_INTERFACE_2
    MBus_InitialRxTx(2);
    #endif

    MBus_InitialModbusData();
}





//----------------------------------------------------------------------------
//Function code 0x06/0x10 write data process该函数处理写操作，根据地址执行相应的操作，如设置输出电压、频率、限流等。
//----------------------------------------------------------------------------
void Function_WrProcess(unsigned short u16Address0)
{
    static Uint16 i;
    
    /*switch(u16Address0)
    {
        case 0x1000: //????duty????

            break;

        case 0x1001: //??????????
            if((u16OutputValue & 0x5A00) == 0x5A00)
            {
                sStatusFlag.sBit.u1HardWareTest = 1;
                sStatusFlag.sBit.u2HardWarePWM = (u16OutputValue >> 4) & 0x0003;
                sStatusFlag.sBit.u1HardWareIO = u16OutputValue & 0x0001;
            }
            else
            {
                sStatusFlag.sBit.u1HardWareTest = 0;
                sStatusFlag.sBit.u2HardWarePWM = 0;
                sStatusFlag.sBit.u1HardWareIO = 0;
            }
            break;

        case 0x1002://charge duty control
            break;

        case 0x1003:

            break;

        case 0x1004:    //???????
            if(u16OutputValue == 1)
            {
                SetTransferPoint();
            }
            else if(u16OutputValue == 10)
            {
                //RS232_Parameter.sBit.u1SaveWavelogAtZero = 1;
            }
            else    //0
            {
                ClearTransferPoint();
            }
            break;


        case 0x1005:     //EPO???????5S??????

            break;

        case 0x1006:     //set charge current level low

            break;

        default:
            u16ErrorCode = ILLEGAL_DATA_ADDRESS;
            break;
    }*/

    switch(u16Address0)
    {
        case 0x4000: // 0x4000+0    output voltage setting  T phase voltage
            if(sSet.u16OutputMode.sByte.u8HiByte == 0)
            {
                //do nothing
            }
            else if(u16OutputValue >= SMALL_OP_VOLT_LIMIT && u16OutputValue <= HIGH_OP_VOLT_LIMIT)
            {
                if(sSet.u16OutputVoltage_T != u16OutputValue)
                {
                    DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                }
                sSet.u16OutputVoltage_T = u16OutputValue;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4001: // 0x4000+1    output voltage setting  S phase voltage
            if(sSet.u16OutputMode.sByte.u8HiByte == 0)
            {
                //do nothing
            }
            else if(u16OutputValue >= SMALL_OP_VOLT_LIMIT && u16OutputValue <= HIGH_OP_VOLT_LIMIT)
            {
                if(sSet.u16OutputVoltage_S != u16OutputValue)
                {
                    DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                }
                sSet.u16OutputVoltage_S = u16OutputValue;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4002: // 0x4000+2    output voltage setting  R phase voltage
            if(u16OutputValue >= SMALL_OP_VOLT_LIMIT && u16OutputValue <= HIGH_OP_VOLT_LIMIT)
            {
                if(sSet.u16OutputVoltage_R != u16OutputValue)
                {
                    DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                }
                sSet.u16OutputVoltage_R = u16OutputValue;
                if(sSet.u16OutputMode.sByte.u8HiByte == 0)
                {
                    sSet.u16OutputVoltage_S = u16OutputValue;
                    sSet.u16OutputVoltage_T = u16OutputValue;
                }
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4003: // output frequency setting
            if(u16OutputValue >= LOW_OP_FREQ_LIMIT && u16OutputValue <= HIGH_OP_FREQ_LIMIT)
            {
                if(sSet.u16OutputFrequency != u16OutputValue)
                {
                    DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                }
                sSet.u16OutputFrequency = u16OutputValue;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4004: // output current limit
            if(u16OutputValue >= 0 && u16OutputValue <= LOW_OP_CURRENT_LIMIT)
            {
                sSet.u16OutputCurrentLimit = u16OutputValue;
                ExtEepromQueueIn(EEP_ADDR_OUTPUT_CURRENT_LIMIT, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4005: // output on angle
            if(u16OutputValue >= 0 && u16OutputValue <= 360)
            {
                sSet.u16OutputOnAngle = u16OutputValue;
                Adjust_Angle_Offset();
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4007: // output on/off control
            if(u16OutputValue == 1)
            {
                #ifndef DEBUG_OPEN_LOOP
                if(!CHK_INVERTER_ENABLE_R &&
                   sSet.u16OutputVoltage_R >= SMALL_OP_VOLT_LIMIT &&
                   sSet.u16OutputVoltage_S >= SMALL_OP_VOLT_LIMIT &&
                   sSet.u16OutputVoltage_T >= SMALL_OP_VOLT_LIMIT &&
                   (CHK_INPUT_RELAY_ON || SourceControlFlag.sBit.RepairOn) &&
                   u16DelayOn == 0 &&
                   FAULT1_FLAG.WORD == 0)
                #endif
                {
                    SourceControlFlag.sBit.OutputOn = 1;
                }

                //LCD_FLAG.sBit.ShowProgramRunStatus = 0;
                //SourceControlFlag.sBit.ShowProgramRunStatus = 0;

                SourceControlFlag.sBit.VSET = 0;
                SourceControlFlag.sBit.FSET = 0;
            }
            /*else if(u16OutputValue == 0x10 &&
                    (CHK_INPUT_RELAY_ON || SourceControlFlag.sBit.RepairOn) &&
                    u16DelayOn == 0 &&
                    FAULT1_FLAG.WORD == 0)
            {
                if(CONFIG_FLAG.sBit.ProgramRunEnable)
                {
                    if(!SourceControlFlag.sBit.OutputOn)
                    {
                        Program_Start();
                        if(FAULT1_FLAG.sBit.ProgramParaError)
                        {
                            u16ErrorCode = PRAGROM_DATA_ERROR;
                        }
                    }
                }
                else
                {
                    u16ErrorCode = PRAGROM_FUNC_DISABLE;    //function disable
                }
            }*/
            else if(u16OutputValue == 0)
            {
                SetOutputOff();

                SourceControlFlag.sBit.VSET = 0;
                SourceControlFlag.sBit.FSET = 0;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4008:
            wTempVaule.u16Word = u16OutputValue;
            if(!SourceControlFlag.sBit.OutputOn)
            {
                if(wTempVaule.sByte.u8HiByte == 1 &&
                   wTempVaule.sByte.u8LowByte >= 1 && wTempVaule.sByte.u8LowByte <= PROGRAM_MEMORY_NUMBER)      //load program data from eeprom
                {
                    pi16StartInRam = (int *)&sProgramming.u16Deep;
                    u16CurAddr = PROGRAM_START_ADDRESS + (wTempVaule.sByte.u8LowByte - 1) * PROGRAM_DATA_LENGTH;
                    for(i = 0; i < PROGRAM_DATA_LENGTH; i++)
                    {
                        *(pi16StartInRam + i) = External_EEPROM_Read(u16CurAddr + i);
                    }
                    MBus_UpdateProgramArea();
                    sSet.wLoadSaveProgramMemoryIndex.u16Word = wTempVaule.u16Word;
                    ExtEepromQueueIn(EEP_ADDR_CURRENT_PROGRAM_MEMORY, u16OutputValue);  //updata the memory u16Index according to current program data
                }
                else if(wTempVaule.sByte.u8HiByte == 2 &&
                        wTempVaule.sByte.u8LowByte >= 1 && wTempVaule.sByte.u8LowByte <= PROGRAM_MEMORY_NUMBER)     //save program data into eeprom
                {
                    Save_Program_data(wTempVaule.sByte.u8LowByte);
                    sSet.wLoadSaveProgramMemoryIndex.u16Word = wTempVaule.u16Word;
                    ExtEepromQueueIn(EEP_ADDR_CURRENT_PROGRAM_MEMORY, u16OutputValue);  //updata the memory u16Index according to current program data
                }
                else
                {
                    u16ErrorCode = ILLEGAL_DATA_VALUE;
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;
        case 0x4009:
            if(!SourceControlFlag.sBit.OutputOn)
            {
                wTempVaule.u16Word = u16OutputValue;
                if(wTempVaule.sByte.u8HiByte <= 1 && wTempVaule.sByte.u8LowByte <= 1)
                {
                    sSet.u16OutputMode.u16Word = wTempVaule.u16Word;
                    ExtEepromQueueIn(EEP_ADDR_OUTPUT_MODE, u16OutputValue);
                    if(sSet.u16OutputMode.sByte.u8HiByte == 0)
                    {
                        sSet.u16OutputVoltage_S = sSet.u16OutputVoltage_R;
                        sSet.u16OutputVoltage_T = sSet.u16OutputVoltage_R;
                    }
                    if(sSet.u16OutputMode.sByte.u8LowByte == 0) //输出相位差（0：标准120度，1：用户自定义
                    {
                        sSet.u16SRPhaseAngle = 120;
                        sSet.u16TRPhaseAngle = 240;
                        ExtEepromQueueIn(EEP_ADDR_SR_PHASE, sSet.u16SRPhaseAngle);
                        ExtEepromQueueIn(EEP_ADDR_TR_PHASE, sSet.u16TRPhaseAngle);
                    }
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;
        case 0x400A:
            if(PASSWORD_FLAG.sBit.WriteTimeLimitEnable)
            {
                sSet.u16RemainingRunTime = u16OutputValue;
                ExtEepromQueueIn(EEP_ADDR_RUN_TIME_LIMIT, u16OutputValue);
                if(sSet.u16RemainingRunTime == 0xFFFF)
                {
                    CONFIG_FLAG.sBit.RunTimeLimitEnable = 0;
                }
                else
                {
                    CONFIG_FLAG.sBit.RunTimeLimitEnable = 1;
                }
                ExtEepromQueueIn(EEP_ADDR_SOURCE_CONFIG, u16OutputValue);
            }
            break;
        case 0x400B:
            CONFIG_FLAG.WORD = u16OutputValue;
            ExtEepromQueueIn(EEP_ADDR_SOURCE_CONFIG, u16OutputValue);
            break;
        case 0x400C:
            switch(u16OutputValue)
            {
                case 0x01:
                    if(!SourceControlFlag.sBit.OutputOn)
                    {
                        EEPROM_Default();
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                    break;

                case 0x02:
                    if(!SourceControlFlag.sBit.OutputOn)
                    {
                        Read_Ex_EEPROM_Parameter();
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                    break;

                case 0x03:
                    FAULT1_FLAG.WORD = 0;
                    break;

                case 0x04:
                    SourceControlFlag.sBit.RemoteControlOn = 0;
                    break;

                case 0x10:
                    Clear_PD_RunTime();
                    Clear_Power_Energy();
                    break;

                case 0x11:
                    Clear_PD_RunTime();
                    break;

                case 0x12:
                    Clear_Power_Energy();
                    break;

                case 0x5A:
                    if(!SourceControlFlag.sBit.OutputOn)
                    {
                        CONFIG_FLAG.sBit.LcdEntryIntoFirmwareUpgrade = 1;
                    }
                    break;

                default:
                    u16ErrorCode = SERVER_DEVICE_BUSY;
                    break;
            }
            break;
        case 0x400D:    //S-R Phase  (Default:120)  Range：0~359
            if(sSet.u16OutputMode.sByte.u8LowByte == 1) //输出相位差（0：标准120度，1：用户自定义
            {
                if(!SourceControlFlag.sBit.OutputOn)    //静态：0~360度
                {
                    if(u16OutputValue < 360)
                    {
                        sSet.u16SRPhaseAngle = u16OutputValue;
                        ExtEepromQueueIn(EEP_ADDR_SR_PHASE, u16OutputValue);
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                }
                else                                    //动态：0~30度
                {
                    if(u16OutputValue >= (120 - 30) && u16OutputValue <= (120 + 30))
                    {
                        if(sSet.u16SRPhaseAngle != u16OutputValue)
                        {
                            DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                        }
                        sSet.u16SRPhaseAngle = u16OutputValue;
                        //ExtEepromQueueIn(EEP_ADDR_SR_PHASE,u16OutputValue);   //2019/8/16 动态调整时不存储相角
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;
        case 0x400E:    //T-R Phase  (Default:240)  Range：0~359
            if(sSet.u16OutputMode.sByte.u8LowByte == 1) //输出相位差（0：标准120度，1：用户自定义
            {
                if(!SourceControlFlag.sBit.OutputOn)
                {
                    if(u16OutputValue < 360)
                    {
                        sSet.u16TRPhaseAngle = u16OutputValue;
                        ExtEepromQueueIn(EEP_ADDR_TR_PHASE, u16OutputValue);
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                }
                else                                    //动态：0~30度
                {
                    if(u16OutputValue >= (240 - 30) && u16OutputValue <= (240 + 30))
                    {
                        if(sSet.u16TRPhaseAngle != u16OutputValue)
                        {
                            DryContactFlag.sBit.ManualVoltFreqUpDown = 1;
                        }
                        sSet.u16TRPhaseAngle = u16OutputValue;
                        //ExtEepromQueueIn(EEP_ADDR_TR_PHASE,u16OutputValue);   //2019/8/16 动态调整时不存储相角
                    }
                    else
                    {
                        u16ErrorCode = ILLEGAL_DATA_VALUE;
                    }
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;
        case 0x400F:
            if(!SourceControlFlag.sBit.OutputOn)
            {
                if(u16OutputValue > 0 && u16OutputValue < 255)
                {
                    u8ProductID = u16OutputValue;

                    ExtEepromQueueIn(EEP_ADDR_PRODUCT_ID, u16OutputValue);
                }
                else
                {
                    u16ErrorCode = ILLEGAL_DATA_VALUE;
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;
        case 0x4010:
            sSet.u16LimitYear = u16OutputValue;
            ExtEepromQueueIn(0x30, u16OutputValue);
            break;
        case 0x4011:
            sSet.u16LimitMonthDay = u16OutputValue;
            ExtEepromQueueIn(0x31, u16OutputValue);
            break;
        case 0x4012:
            if(u16OutputValue == 0x8001)
            {
                PASSWORD_FLAG.sBit.WriteTimeLimitEnable = 1;
            }
            else
            {
                PASSWORD_FLAG.sBit.WriteTimeLimitEnable = 0;
            }
            break;
        case 0x401E:
            if(u16OutputValue == 0x0001)
            {
                SetTransferPoint();
            }
            else if(u16OutputValue == 0x0002)
            {
                ClearTransferPoint();
            }
            break;
        case 0x401F:    //upgrade the ac source firmware
            if(!SourceControlFlag.sBit.OutputOn)
            {
                if(u16OutputValue == BOOT_PASSWORD)
                {
                    SciFlag.sBit.PROGRAM_MODE = 1;              // set programming mode
                }
                else
                {
                    u16ErrorCode = ILLEGAL_DATA_VALUE;
                }
            }
            else
            {
                u16ErrorCode = SERVER_DEVICE_BUSY;
            }
            break;

        //updata R Phase GAIN value
        case 0x4020:
            if(u16OutputValue <= (GAIN_TEMP << 1))
            {
                sGain_R.wTEMP = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4021:
            if(u16OutputValue < (GAIN_BUS << 1) && u16OutputValue > (GAIN_BUS >> 1))
            {
                sGain_R.wBUS = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 1, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4022:
            if(u16OutputValue < (GAIN_INV << 1) && u16OutputValue > (GAIN_INV >> 1))
            {
                sGain_R.wINV = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 2, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4023:
            if(u16OutputValue < (GAIN_OPV << 1) && u16OutputValue > (GAIN_OPV >> 1))
            {
                sGain_R.wOPV = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 3, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4024:
            if(u16OutputValue < (GAIN_OPI << 1) && u16OutputValue > (GAIN_OPI >> 1))
            {
                sGain_R.wOPI = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 4, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4025:
            if(u16OutputValue < (GAIN_WAT << 1) && u16OutputValue > (GAIN_WAT >> 1))
            {
                sGain_R.wWAT = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 5, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4026:
            if(u16OutputValue < (GAIN_PWM * 1.3) && u16OutputValue > (GAIN_PWM * 0.7))
            {
                sGain_R.wPWM = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 6, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4027:
            if(u16OutputValue < 360)
            {
                sGain_R.wAngle = u16OutputValue;
                ExtEepromQueueIn(GAIN_R_START_ADDRESS + 7, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        //更新系统时间
        /*case 0x4028:
            sCurrent_Time.bMonth = u16OutputValue;
            break;

        case 0x4029:
            sCurrent_Time.bHour = u16OutputValue;
            break;

        case 0x402A:
            sCurrent_Time.bSec = u16OutputValue;
            Set_RTC_Time();
            break;
        */
        //updata S Phase GAIN value
        case 0x402B:
            if(u16OutputValue <= (GAIN_TEMP << 1))
            {
                sGain_S.wTEMP = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x402C:
            if(u16OutputValue < (GAIN_BUS << 1) && u16OutputValue > (GAIN_BUS >> 1))
            {
                sGain_S.wBUS = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 1, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x402D:
            if(u16OutputValue < (GAIN_INV << 1) && u16OutputValue > (GAIN_INV >> 1))
            {
                sGain_S.wINV = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 2, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x402E:
            if(u16OutputValue < (GAIN_OPV << 1) && u16OutputValue > (GAIN_OPV >> 1))
            {
                sGain_S.wOPV = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 3, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x402F:
            if(u16OutputValue < (GAIN_OPI << 1) && u16OutputValue > (GAIN_OPI >> 1))
            {
                sGain_S.wOPI = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 4, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4030:
            if(u16OutputValue < (GAIN_WAT << 1) && u16OutputValue > (GAIN_WAT >> 1))
            {
                sGain_S.wWAT = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 5, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4031:
            if(u16OutputValue < (GAIN_PWM * 1.3) && u16OutputValue > (GAIN_PWM * 0.7))
            {
                sGain_S.wPWM = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 6, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4032:
            if(u16OutputValue < 360)
            {
                sGain_S.wAngle = u16OutputValue;
                ExtEepromQueueIn(GAIN_S_START_ADDRESS + 7, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        //updata T Phase GAIN value
        case 0x4033:
            if(u16OutputValue <= (GAIN_TEMP << 1))
            {
                sGain_T.wTEMP = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4034:
            if(u16OutputValue < (GAIN_BUS << 1) && u16OutputValue > (GAIN_BUS >> 1))
            {
                sGain_T.wBUS = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 1, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4035:
            if(u16OutputValue < (GAIN_INV << 1) && u16OutputValue > (GAIN_INV >> 1))
            {
                sGain_T.wINV = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 2, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4036:
            if(u16OutputValue < (GAIN_OPV << 1) && u16OutputValue > (GAIN_OPV >> 1))
            {
                sGain_T.wOPV = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 3, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4037:
            if(u16OutputValue < (GAIN_OPI << 1) && u16OutputValue > (GAIN_OPI >> 1))
            {
                sGain_T.wOPI = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 4, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4038:
            if(u16OutputValue < (GAIN_WAT << 1) && u16OutputValue > (GAIN_WAT >> 1))
            {
                sGain_T.wWAT = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 5, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x4039:
            if(u16OutputValue < (GAIN_PWM * 1.3) && u16OutputValue > (GAIN_PWM * 0.7))
            {
                sGain_T.wPWM = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 6, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x403A:
            if(u16OutputValue < 360)
            {
                sGain_T.wAngle = u16OutputValue;
                ExtEepromQueueIn(GAIN_T_START_ADDRESS + 7, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x403B:
            if(u16OutputValue <= 100)
            {
                u16IpeakProtect = u16OutputValue;
                ExtEepromQueueIn(EEP_ADDR_IPEAK, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x403C:
            if(sLVRT.wTestTime > 0 && sLVRT.wTestTime <= 5000)
            {
                sLVRT.wTimeCount = 0;
                sLVRT.sFunction.WORD = u16OutputValue;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        case 0x403D:
            if(u16OutputValue >= 0 && u16OutputValue <= 5000)
            {
                sLVRT.wTestTime = u16OutputValue;
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;
        case 0x4040:
            if(u16OutputValue >= 0 && u16OutputValue <= 1)
            {
                u16PowerRecycle = u16OutputValue;
                ExtEepromQueueIn(EEP_ADDR_POWER_RECYCLE, u16OutputValue);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;
        case 0x4041:
            if(u16OutputValue >= 0 && u16OutputValue <= 3)
            {
                u16PowerRecycleLevel = u16OutputValue;
                ExtEepromQueueIn(EEP_ADDR_POWER_RECYCLE_LEVEL, u16PowerRecycleLevel);
            }
            else
            {
                u16ErrorCode = ILLEGAL_DATA_VALUE;
            }
            break;

        default:
            u16ErrorCode = ILLEGAL_DATA_ADDRESS;
            break;
    }
}


//----------------------------------------------------------------------------
// read coils
//----------------------------------------------------------------------------
void Function_01(Uint16 u16MbusPortIndex)
{
    /*if(u16Quantity > MBUS_MAX_COILS) // u16Quantity is lager than number that device supported
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address < MBUS_COILS_START_ADDR) || (u16Address > MBUS_COILS_END_ADDR)) // start u16Address is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address + u16Quantity - 1) > MBUS_COILS_END_ADDR) // start u16Address+u16Quantity is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else
    {
        // byte count
        //pModbus->strModbusRxTx.u8BufTx[2] = u16Quantity/8;
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity >> 3;

        if(u16Quantity % 8 != 0)
        {
            sModbusRxTx[u16MbusPortIndex].u8BufTx[2]++;
        }

        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)  // fulfill discrete inputs value
        {
            if(u16Offset % 8 == 0)
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset >> 3] = 0;
            }

            // find exact position in upsInfo_Modbus.coils[]
            u8RealPosHi = (u16Address + u16Offset - MBUS_COILS_START_ADDR) >> 3; // u16Index of coils[]
            u8ReplyPosHi = 3 + u16Offset >> 3; // u16Index of pMBusRxTx->u8BufTx

            if(sModbusData.coils[u8RealPosHi]&ifSet[(u16Address + u16Offset - MBUS_COILS_START_ADDR) % 8])
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[u8ReplyPosHi] |= ifSet[u16Offset % 8];
            }
        }
    }*/
}

//----------------------------------------------------------------------------
// read discrete inputs
//----------------------------------------------------------------------------
void Function_02(Uint16 u16MbusPortIndex)
{
    /*if(u16Quantity > MBUS_MAX_DISCREATE_INPUTS) // u16Quantity is lager than number that device supported
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address < MBUS_DI_START_ADDR) || (u16Address > MBUS_DI_END_ADDR)) // start u16Address is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address + u16Quantity - 1) > MBUS_DI_END_ADDR) // start u16Address+u16Quantity is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity >> 3; // byte count

        if(u16Quantity % 8 != 0)
        {
            sModbusRxTx[u16MbusPortIndex].u8BufTx[2]++;
        }

        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++) // fulfill discrete inputs value
        {
            if(u16Offset % 8 == 0)
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset >> 3] = 0;
            }

            // find exact position in upsInfo_Modbus.discreteInputs[]
            u8RealPosHi = (u16Address + u16Offset - MBUS_DI_START_ADDR) >> 3; // u16Index of discreteInputs[]
            u8ReplyPosHi = 3 + u16Offset >> 3; // u16Index of pMBusRxTx->u8BufTx

            if(sModbusData.discreteInputs[u8RealPosHi]&ifSet[(u16Address + u16Offset - MBUS_DI_START_ADDR) % 8])
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[u8ReplyPosHi] |= ifSet[u16Offset % 8];
            }
        }
    }*/
}

//----------------------------------------------------------------------------
// read holding registers
//----------------------------------------------------------------------------
void Function_03(Uint16 u16MbusPortIndex)
{
    if(u16Quantity > 123) // modbus protocol PDU max support 253 bytes
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    // identifiers, 12 words
    else if((u16Address >= MBUS_HREGS_SADDR_IDS) && (u16Address <= MBUS_HREGS_EADDR_IDS) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_IDS))
    {
        u16Index = MBUS_HREGS_SIDX_IDS + u16Address - MBUS_HREGS_SADDR_IDS;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    // states, 4 words
    else if((u16Address >= MBUS_HREGS_SADDR_STATES) && (u16Address <= MBUS_HREGS_EADDR_STATES) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_STATES))
    {
        u16Index = MBUS_HREGS_SIDX_STATES + u16Address - MBUS_HREGS_SADDR_STATES;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    // alarms, 4 words
    else if((u16Address >= MBUS_HREGS_SADDR_ALARMS) && (u16Address <= MBUS_HREGS_EADDR_ALARMS) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_ALARMS))
    {
        u16Index = MBUS_HREGS_SIDX_ALARMS + u16Address - MBUS_HREGS_SADDR_ALARMS;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    // measurements, 48 words
    else if((u16Address >= MBUS_HREGS_SADDR_MEASUREMENTS) && (u16Address <= MBUS_HREGS_EADDR_MEASUREMENTS) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_MEASUREMENTS))
    {
        u16Index = MBUS_HREGS_SIDX_MEASUREMENTS + u16Address - MBUS_HREGS_SADDR_MEASUREMENTS;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    // configurations, 32 words
    else if((u16Address >= MBUS_HREGS_SADDR_CONF) && (u16Address <= MBUS_HREGS_EADDR_CONF) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_CONF))
    {
        u16Index = MBUS_HREGS_SIDX_CONF + u16Address - MBUS_HREGS_SADDR_CONF;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    // command table, 10 words
    else if((u16Address >= MBUS_HREGS_SADDR_COMMANDS) && (u16Address <= MBUS_HREGS_EADDR_COMMANDS) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_COMMANDS))
    {
        u16Index = MBUS_HREGS_SIDX_COMMANDS + u16Address - MBUS_HREGS_SADDR_COMMANDS;
        pi16StartInRam = (int *)&sModbusData.u16HoldingRegs[u16Index];
    }
    else if((u16Address >= MBUS_HREGS_SADDR_THD) && (u16Address <= MBUS_HREGS_EADDR_THD)  && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_THD))
    {
        u16Index = u16Address - MBUS_HREGS_SADDR_THD;
        pi16StartInRam = (int *)(&HarmSuperData.u16HarmConfig1 + u16Index);
    }
    else if((u16Address >= MBUS_HREGS_SADDR_SINEWAVE) && (u16Address <= MBUS_HREGS_EADDR_SINEWAVE)  && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_SINEWAVE))
    {
        u16Index = u16Address - MBUS_HREGS_SADDR_SINEWAVE;
        pi16StartInRam = (int *)(&i16SinewaveTable[0] + u16Index);
    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }

    if(u16ErrorCode == 0)
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity * 2; // byte count
        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
        {
            // if register value is 0x1234, the first byte sent is 0x12, than 0x34
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2] = (*(pi16StartInRam + u16Offset)) >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2 + 1] = (*(pi16StartInRam + u16Offset)) & 0x00FF;
        }
    }
}

//----------------------------------------------------------------------------
// read input registers
//----------------------------------------------------------------------------
void Function_04(Uint16 u16MbusPortIndex)
{
    /*if(u16Quantity > 126) // modbus protocol 砏﹚戈程??? 253 bytes
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address < MBUS_IR_START_ADDR) || (u16Address > MBUS_IR_END_ADDR)) // start u16Address is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address + u16Quantity - 1) > MBUS_IR_END_ADDR) // start u16Address+u16Quantity is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity * 2; // byte count
        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
        {
            // if register value is 0x1234, the first byte sent is 0x12, than 0x34
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2] = (sModbusData.u16InputRegs[u16Address + u16Offset - MBUS_IR_START_ADDR]) >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2 + 1] = (sModbusData.u16InputRegs[u16Address + u16Offset - MBUS_IR_START_ADDR]) & 0x00FF;
        }
    }*/
}

//----------------------------------------------------------------------------
// write single coil
//----------------------------------------------------------------------------
void Function_05(Uint16 u16MbusPortIndex)
{
    /*u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[4];
    u16OutputValue = u16OutputValue << 8;
    u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[5];

    if((u16OutputValue != 0x0000) && (u16OutputValue != 0xFF00))
    {
        u16ErrorCode = 0xFF;    // error value for setting coils
    }
    else if(u16Address < MBUS_COILS_START_ADDR || u16Address > MBUS_COILS_END_ADDR)
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else
    {
        switch(u16Address)
        {
            case MBUS_COILS_START_ADDR: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            case MBUS_COILS_START_ADDR+1: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            case MBUS_COILS_START_ADDR+2: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            case MBUS_COILS_START_ADDR+3: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            case MBUS_COILS_START_ADDR+4: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            case MBUS_COILS_START_ADDR+5: // ...
                // *** If this request is rejected, need to set u16ErrorCode to 0x04

                if(u16OutputValue == 0xFF00)
                    ;
                else
                    ;
                break;
            // *** You need to add codes here to deal with all cases you are interested!
            // case xxx:
            //     break;
            // ...
            // ...
            default:

                break;
        }
    }*/
}

//----------------------------------------------------------------------------
// write single holding register
//----------------------------------------------------------------------------
void Function_06(Uint16 u16MbusPortIndex)
{
    static Uint16 i;

    u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[4];
    u16OutputValue = u16OutputValue << 8;
    u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[5];

    // commands, 1 word
    if((u16Address >= MBUS_HREGS_SADDR_CONF) && (u16Address <= MBUS_HREGS_EADDR_CONF))
    {
        if(u16MbusPortIndex < 2)
        {
            SourceControlFlag.sBit.RemoteControlOn = 1;
        }

        Clear_Set_Flag();

        Function_WrProcess(u16Address);

    }
    else if((u16Address >= MBUS_HREGS_SADDR_THD) && (u16Address <= MBUS_HREGS_EADDR_THD))
    {
        if(!SourceControlFlag.sBit.OutputOn)
        {
            u16Index = u16Address - MBUS_HREGS_SADDR_THD;
            pi16StartInRam = (int *)(&HarmSuperData.u16HarmConfig1 + u16Index);

            if(u16Index == 0)
            {
                HarmSuperData.u16Command = 0x0001;
            }
            else if(u16Index >= 2 && u16Index <= HARM_TOTAL_NUM)
            {
                if(u16OutputValue > 100)
                {
                    u16OutputValue = 0;
                }
            }

            //updata the program data into ram
            *(pi16StartInRam) = u16OutputValue;
            //save the program data into EEPROM
            ExtEepromQueueIn(HARM_START_ADDRESS + u16Index, u16OutputValue);
        }
        else
        {
            u16ErrorCode = SERVER_DEVICE_BUSY;
        }
    }
    else if((u16Address >= MBUS_HREGS_SADDR_COMMANDS) && (u16Address <= MBUS_HREGS_EADDR_COMMANDS))
    {
        if(u16MbusPortIndex < 2)
        {
            SourceControlFlag.sBit.RemoteControlOn = 1;
        }

        Clear_Set_Flag();
        if(!SourceControlFlag.sBit.OutputOn)
        {
            pi16StartInRam = (int *)&sProgramming.u16Deep;

            //updata the program data into ram
            sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  u16Address - MBUS_HREGS_SADDR_COMMANDS] = u16OutputValue;
            *(pi16StartInRam + u16Address - MBUS_HREGS_SADDR_COMMANDS) = u16OutputValue;
            //save the program data into EEPROM
            //ExtEepromQueueIn(u16Address - MBUS_HREGS_SADDR_COMMANDS + PROGRAM_START_ADDRESS + i,u16OutputValue);
        }
        else
        {
            u16ErrorCode = SERVER_DEVICE_BUSY;
        }
    }
    else if((u16Address >= MBUS_HREGS_SADDR_ADJUST) && (u16Address <= MBUS_HREGS_EADDR_ADJUST))
    {
        GainCalc(u16Address, u16OutputValue);
    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
}

//----------------------------------------------------------------------------
// write multiple coils
//----------------------------------------------------------------------------
void Function_0F(Uint16 u16MbusPortIndex)
{
    /*Uint16 i;

    if((u16Quantity == 0) || (u16Quantity > 0x07B0)) //
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if(((u16Quantity % 8 == 0) && (u16Quantity / 8 != sModbusRxTx[u16MbusPortIndex].u8BufRx[6])) || ((u16Quantity % 8 != 0) && (u16Quantity / 8 + 1 != sModbusRxTx[u16MbusPortIndex].u8BufRx[6])))
    {
        u16ErrorCode = 0xFF;
    }
    else if((u16Address < MBUS_COILS_START_ADDR) || (u16Address > MBUS_COILS_END_ADDR)) // start u16Address is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address + u16Quantity - 1) > MBUS_COILS_END_ADDR) // start u16Address+u16Quantity is error
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else
    {
        for(i = 0; i < u16Quantity; i++)
        {
            u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[7 + i / 8] & (0x01 << (i % 8));

            switch(u16Address)
            {
                case MBUS_COILS_START_ADDR: // ...
                    // *** If this request is rejected, need to set u16ErrorCode to 0x04
                    break;
                case MBUS_COILS_START_ADDR+1: // ...
                    // *** If this request is rejected, need to set u16ErrorCode to 0x04
                    break;
                case MBUS_COILS_START_ADDR+2: // ...
                    // *** If this request is rejected, need to set u16ErrorCode to 0x04
                    break;
                // *** You need to add codes here to deal with all cases you are interested!
                // case xxx:
                //     break;
                // ...
                // ...
                default:

                    ;
            }

            u16Address += 1;
        }
    }*/
}

//----------------------------------------------------------------------------
// write multiple registers
//----------------------------------------------------------------------------
void Function_10(Uint16 u16MbusPortIndex)
{
    static Uint16 i;

    // commands, 1 word
    //if((u16Address >= MBUS_HREGS_SADDR_COMMANDS) && (u16Address <= MBUS_HREGS_EADDR_COMMANDS*PROGRAM_MEMORY_NUMBER)  && ((u16Address+u16Quantity-1) <= MBUS_HREGS_EADDR_COMMANDS*PROGRAM_MEMORY_NUMBER))
    if((u16Address >= MBUS_HREGS_SADDR_COMMANDS) && (u16Address <= MBUS_HREGS_EADDR_COMMANDS)  && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_COMMANDS))
    {
        if(u16MbusPortIndex < 2)
        {
            SourceControlFlag.sBit.RemoteControlOn = 1;
        }

        Clear_Set_Flag();
        if(!SourceControlFlag.sBit.OutputOn)
        {
            pi16StartInRam = (int *)&sProgramming.u16Deep;
            for(i = 0; i < u16Quantity; i++)
            {
                u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[7 + i * 2];
                u16OutputValue = u16OutputValue << 8;
                u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[8 + i * 2];

                //updata the program data into ram
                sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  u16Address - MBUS_HREGS_SADDR_COMMANDS + i] = u16OutputValue;
                *(pi16StartInRam + u16Address - MBUS_HREGS_SADDR_COMMANDS + i) = u16OutputValue;
                //save the program data into EEPROM
                //ExtEepromQueueIn(u16Address - MBUS_HREGS_SADDR_COMMANDS + PROGRAM_START_ADDRESS + i,u16OutputValue);
            }
        }
        else
        {
            u16ErrorCode = SERVER_DEVICE_BUSY;
        }
    }
    else if((u16Address >= MBUS_HREGS_SADDR_CONF + 0x20) && (u16Address <= MBUS_HREGS_EADDR_CONF)  && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_CONF))
    {
        for(i = 0; i < u16Quantity; i++)
        {
            u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[7 + i * 2];
            u16OutputValue = u16OutputValue << 8;
            u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[8 + i * 2];

            Function_WrProcess(u16Address + i);
        }
    }
    else if((u16Address >= MBUS_HREGS_SADDR_THD) && (u16Address <= MBUS_HREGS_EADDR_THD)  && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_THD))
    {
        if(!SourceControlFlag.sBit.OutputOn)
        {
            u16Index = u16Address - MBUS_HREGS_SADDR_THD;
            pi16StartInRam = (int *)(&HarmSuperData.u16HarmConfig1 + u16Index);
            for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
            {
                u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[7 + u16Offset * 2];
                u16OutputValue = u16OutputValue << 8;
                u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[8 + u16Offset * 2];

                if((u16Index + u16Offset) >= 2 && (u16Index + u16Offset) <= HARM_TOTAL_NUM)
                {
                    if(u16OutputValue > 100)
                    {
                        u16OutputValue = 0;
                    }
                }

                //updata the program data into ram
                *(pi16StartInRam + u16Offset) = u16OutputValue;
                //save the program data into EEPROM
                ExtEepromQueueIn(HARM_START_ADDRESS + u16Index + u16Offset, u16OutputValue);
            }

            if(u16Index == 0)
            {
                HarmSuperData.u16Command = 0x0001;
            }
        }
        else
        {
            u16ErrorCode = SERVER_DEVICE_BUSY;
        }
    }
    else if((u16Address >= MBUS_HREGS_SADDR_ADJUST) && (u16Address <= MBUS_HREGS_EADDR_ADJUST) && ((u16Address + u16Quantity - 1) <= MBUS_HREGS_EADDR_ADJUST))
    {
        for(i = 0; i < u16Quantity; i++)
        {
            u16OutputValue = sModbusRxTx[u16MbusPortIndex].u8BufRx[7 + i * 2];
            u16OutputValue = u16OutputValue << 8;
            u16OutputValue += sModbusRxTx[u16MbusPortIndex].u8BufRx[8 + i * 2];

            GainCalc(u16Address + i, u16OutputValue);
        }
    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
}

//----------------------------------------------------------------------------
// read EEPROM registers
//----------------------------------------------------------------------------
void Function_61(Uint16 u16MbusPortIndex)
{
    if((u16Address >= 0) && (u16Address <= 1023)  && ((u16Address + u16Quantity - 1) <= 1023))
    {

    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }

    if(u16ErrorCode == 0)
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity * 2; // byte count
        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
        {
            u16OutputValue = External_EEPROM_Read(u16Address + u16Offset);
            // if register value is 0x1234, the first byte sent is 0x12, than 0x34
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2] = u16OutputValue >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2 + 1] = u16OutputValue & 0x00FF;
        }
    }
}

//----------------------------------------------------------------------------
// read wavelog registers
//----------------------------------------------------------------------------
void Function_62(Uint16 u16MbusPortIndex)
{
    if(u16Quantity > 123) // modbus protocol PDU max support 253 bytes
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if((u16Address < (ROW_NUM * COL_NUM + 10))  && ((u16Address + u16Quantity - 1) < (ROW_NUM * COL_NUM + 10)))
    {
        pi16StartInRam = (int *)(&sWave_Data_In_Ram.u16IndexCnt + u16Address);
    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }

    if(u16ErrorCode == 0)
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity * 2; // byte count
        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
        {
            // if register value is 0x1234, the first byte sent is 0x12, than 0x34
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2] = (*(pi16StartInRam + u16Offset)) >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2 + 1] = (*(pi16StartInRam + u16Offset)) & 0x00FF;
        }
    }
}

//----------------------------------------------------------------------------
// read DSP ram
//----------------------------------------------------------------------------
void Function_63(Uint16 u16MbusPortIndex)
{
    u32Address = (((Uint32)u16Quantity >> 8) << 16) + u16Address;
    u16Quantity = u16Quantity & 0x00FF;

    u16Index = u16Address - 0;

    if(u16Quantity > 123) // modbus protocol PDU max support 253 bytes
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }
    else if(u32Address <= 0x0007FF || (u32Address >= 0x008000 && u32Address <= 0x017FFF))
    {
        pi16StartInRam = (int *)u32Address;
    }
    else
    {
        u16ErrorCode = ILLEGAL_DATA_ADDRESS;
    }


    if(u16ErrorCode == 0)
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16Quantity * 2; // byte count
        sModbusRxTx[u16MbusPortIndex].i16LenTx = 3 + sModbusRxTx[u16MbusPortIndex].u8BufTx[2]; // 3=ID+function code+byte count
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;

        for(u16Offset = 0; u16Offset < u16Quantity; u16Offset++)
        {
            // if register value is 0x1234, the first byte sent is 0x12, than 0x34
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2] = (*(pi16StartInRam + u16Offset)) >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3 + u16Offset * 2 + 1] = (*(pi16StartInRam + u16Offset)) & 0x00FF;
        }
    }
}


//----------------------------------------------------------------------------
// Description: Parse modbus packet
// Arg:
//      StructModbus *pModbus: modbus information
// Return: void
//----------------------------------------------------------------------------
void MBus_ParseModbusProtocol(Uint16 u16MbusPortIndex)
{
    static unsigned int u16crcValue;
    static int i;

    u16ErrorCode = 0;

    u16Address = ((Uint16)sModbusRxTx[u16MbusPortIndex].u8BufRx[2] << 8) + sModbusRxTx[u16MbusPortIndex].u8BufRx[3];
    u16Quantity = ((Uint16)sModbusRxTx[u16MbusPortIndex].u8BufRx[4] << 8) + sModbusRxTx[u16MbusPortIndex].u8BufRx[5];

    switch(sModbusRxTx[u16MbusPortIndex].u8BufRx[1])
    {
        //case 0x01: // read coils
        //    Function_01(u16MbusPortIndex);
        //    break;

        //case 0x02: // read discrete inputs
        //    Function_02(u16MbusPortIndex);
        //    break;

        case 0x03: // read holding registers
            Function_03(u16MbusPortIndex);
            break;

        //case 0x04: // read input registers
        //    Function_04(u16MbusPortIndex);
        //    break;

        //case 0x05: // write single coil
        //    Function_05(u16MbusPortIndex);
        //    break;

        case 0x06: // write single holding register
            Function_06(u16MbusPortIndex);
            break;

        //case 0x0F: // write multiple coils
        //    Function_0F(u16MbusPortIndex);
        //    break;

        case 0x10: // write multiple registers
            Function_10(u16MbusPortIndex);
            break;

        case 0x61: // read EEPROM registers
            Function_61(u16MbusPortIndex);
            break;

        case 0x62: // read wavelog registers
            Function_62(u16MbusPortIndex);
            break;

        case 0x63: // read DSP ram
            Function_63(u16MbusPortIndex);
            break;

        default: //
            u16ErrorCode = ILLEGAL_FUNCTION;
            break;
    }

    if(!(sModbusRxTx[u16MbusPortIndex].u8BufRx[0] == 0x0 && sModbusRxTx[u16MbusPortIndex].u8BufRx[1] == 0x06))
    {
        sModbusRxTx[u16MbusPortIndex].u8BufTx[0] = sModbusRxTx[u16MbusPortIndex].u8BufRx[0];

        if(((sModbusRxTx[u16MbusPortIndex].u8BufRx[1] >= 0x01) && (sModbusRxTx[u16MbusPortIndex].u8BufRx[1] <= 0x04)) ||
           ((sModbusRxTx[u16MbusPortIndex].u8BufRx[1] >= 0x61) && (sModbusRxTx[u16MbusPortIndex].u8BufRx[1] <= 0x64)))
        {
            if(u16ErrorCode == 0)       // normal
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1];
            }
            else                    // error situation
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1] | 0x80;
                sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16ErrorCode;
                sModbusRxTx[u16MbusPortIndex].i16LenTx = 3;
            }
        }
        else if((sModbusRxTx[u16MbusPortIndex].u8BufRx[1] == 0x05) || (sModbusRxTx[u16MbusPortIndex].u8BufRx[1] == 0x06)) // write single coil or write holding register
        {
            if(u16ErrorCode == 0)
            {
                memcpy(sModbusRxTx[u16MbusPortIndex].u8BufTx, sModbusRxTx[u16MbusPortIndex].u8BufRx, sModbusRxTx[u16MbusPortIndex].i16LenRx - 2); // -2 => crc
                sModbusRxTx[u16MbusPortIndex].i16LenTx = sModbusRxTx[u16MbusPortIndex].i16LenRx - 2;
            }
            else
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1] | 0x80; // Exception Code
                sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16ErrorCode; // sub-code
                sModbusRxTx[u16MbusPortIndex].i16LenTx = 3;
            }
        }
        else if((sModbusRxTx[u16MbusPortIndex].u8BufRx[1] == 0x0F) || (sModbusRxTx[u16MbusPortIndex].u8BufRx[1] == 0x10)) // write multiple coil/registers
        {
            if(u16ErrorCode == 0)
            {
                memcpy(sModbusRxTx[u16MbusPortIndex].u8BufTx, sModbusRxTx[u16MbusPortIndex].u8BufRx, 6);
                sModbusRxTx[u16MbusPortIndex].i16LenTx = 6;
            }
            else
            {
                sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1] | 0x80;
                sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16ErrorCode;
                sModbusRxTx[u16MbusPortIndex].i16LenTx = 3;
            }
        }
        else
        {
            sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1] | 0x80;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = u16ErrorCode;
            sModbusRxTx[u16MbusPortIndex].i16LenTx = 3;
        }

        u16crcValue = CRC16(sModbusRxTx[u16MbusPortIndex].u8BufTx, sModbusRxTx[u16MbusPortIndex].i16LenTx);
        sModbusRxTx[u16MbusPortIndex].u8BufTx[sModbusRxTx[u16MbusPortIndex].i16LenTx + 1] = (u16crcValue >> 8) & 0x00FF;
        sModbusRxTx[u16MbusPortIndex].u8BufTx[sModbusRxTx[u16MbusPortIndex].i16LenTx] = u16crcValue & 0x00FF;
        sModbusRxTx[u16MbusPortIndex].i16LenTx += 2; // add crc length
        sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;
        sModbusRxTx[u16MbusPortIndex].u8fState = MBUS_STATE_TX;
    }
    else
    {
        MBus_InitialRxTx(u16MbusPortIndex);
    }
}


//----------------------------------------------------------------------------
// Description: Check and parse modbus packet(check CRC value, UPS ID, modbus commands, ...)
// Arg:
//      StructModbus *pModbus: modbus information
// Return: void
//----------------------------------------------------------------------------
void MBus_HandleMBusRx(Uint16 u16MbusPortIndex, unsigned char byUpsID)
{
    static Uint16 u16crcValue;

    if((sModbusRxTx[u16MbusPortIndex].u8fState == MBUS_STATE_RX) && (sModbusRxTx[u16MbusPortIndex].u8fRxOK == 1))
    {
        u16crcValue = CRC16(sModbusRxTx[u16MbusPortIndex].u8BufRx, sModbusRxTx[u16MbusPortIndex].i16LenRx);

        // check ups ID
        if((sModbusRxTx[u16MbusPortIndex].u8BufRx[0] != 0) &&
           (sModbusRxTx[u16MbusPortIndex].u8BufRx[0] != byUpsID))
        {
            MBus_InitialRxTx(u16MbusPortIndex);
            return;
        }
        else if(u16crcValue != 0)
        {
            sModbusRxTx[u16MbusPortIndex].u8BufTx[0] = sModbusRxTx[u16MbusPortIndex].u8BufRx[0];
            // error code is original function code + 0x80;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[1] = sModbusRxTx[u16MbusPortIndex].u8BufRx[1] | 0x80;
            // exception code(Bad CRC)
            sModbusRxTx[u16MbusPortIndex].u8BufTx[2] = 0x03;
            u16crcValue = CRC16(sModbusRxTx[u16MbusPortIndex].u8BufTx, 3);
            sModbusRxTx[u16MbusPortIndex].u8BufTx[4] = u16crcValue >> 8;
            sModbusRxTx[u16MbusPortIndex].u8BufTx[3] = u16crcValue & 0x00FF;
            sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;
            sModbusRxTx[u16MbusPortIndex].i16LenTx = 5;
            sModbusRxTx[u16MbusPortIndex].u8fState = MBUS_STATE_TX;
            return;
        }
        else    // crc ok!!!
        {
            sModbusRxTx[u16MbusPortIndex].u8fState = MBUS_STATE_PROCESS;
            MBus_ParseModbusProtocol(u16MbusPortIndex);

            sModbusRxTx[u16MbusPortIndex].u8fRxOK = 0;
        }
    }
}

//----------------------------------------------------------------------------
// Description: Tx modbus reply packet
// Arg:
//      StructModbus *pModbus: modbus information
// Return: void
//----------------------------------------------------------------------------
void Isr_MBus_HandleMBusRx(unsigned int u16MbusPortIndex, unsigned char u8SciData)
{
    if((sModbusRxTx[u16MbusPortIndex].u8fState == MBUS_STATE_RX) && (sModbusRxTx[u16MbusPortIndex].u8fRxOK == 0))
    {
        // codes to receive modbus data through sci or any other interfaces
        if((sModbusRxTx[u16MbusPortIndex].i16LenRx < MBUS_MAX_RECV_BUF))
        {
            sModbusRxTx[u16MbusPortIndex].u8BufRx[sModbusRxTx[u16MbusPortIndex].i16LenRx++] = u8SciData;

            sModbusRxTx[u16MbusPortIndex].u16RxTimer = 0;
        }
    }
}
//----------------------------------------------------------------------------
// Description: Tx modbus reply packet
// Arg:
//      StructModbus *pModbus: modbus information
// Return: void
//----------------------------------------------------------------------------
void Isr_MBus_HandleMBusTx(unsigned int u16MbusPortIndex, unsigned int *pSciTxBuf)
{
    // codes to send modbus reply packet
    if(sModbusRxTx[u16MbusPortIndex].u8fState == MBUS_STATE_TX)
    {
        if(sModbusRxTx[u16MbusPortIndex].i16IndexTx < sModbusRxTx[u16MbusPortIndex].i16LenTx)
        {
            *pSciTxBuf = sModbusRxTx[u16MbusPortIndex].u8BufTx[sModbusRxTx[u16MbusPortIndex].i16IndexTx];
            sModbusRxTx[u16MbusPortIndex].i16IndexTx++;
        }
        else
        {
            sModbusRxTx[u16MbusPortIndex].i16LenRx = 0;
            sModbusRxTx[u16MbusPortIndex].u16RxTimer = 0;
            sModbusRxTx[u16MbusPortIndex].i16IndexTx = 0;
            sModbusRxTx[u16MbusPortIndex].i16LenTx = 0;
            sModbusRxTx[u16MbusPortIndex].u8fState = MBUS_STATE_RX;
            sModbusRxTx[u16MbusPortIndex].u8fRxOK = 0;
        }
    }
}


//----------------------------------------------------------------------------
// Function Name : MBus_UpdateDataArea
// Description : updates parameters of modbus
// Arg: void
// Timebase : 100ms
//----------------------------------------------------------------------------
void MBus_UpdateDataArea(void)
{
    static Uint8 i_offset;
    static Uint16 bUpdateMeasureData;
    static Uint16 bUpdateProgramData;

    //***********************************************
    // - alarm data
    //***********************************************
    if(CHK_DRY_ONPUT1_1 && DryContactFlag.sBit.u1Input1)
    {
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  0] = FAULT1_FLAG.WORD | 0x4000;
    }
    else
    {
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  0] = FAULT1_FLAG.WORD;
    }
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  1] = BUS_FLAG.WORD;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  2] = SourceControlFlag.WORD;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  3] = sProgRun.u16CycleCnt;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  4] = sProgRun.u16Index + 1;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  5] = sProgRun.u32TimeCnt >> 16;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_ALARMS +  6] = sProgRun.u32TimeCnt;


    //***********************************************
    // - measurement data
    //***********************************************

    if(bUpdateMeasureData == 0)
    {
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  0] = sProgramming.u16Deep;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  1] = sProgramming.u16Start;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  2] = sProgramming.u16End;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  3] = sProgramming.u16Cycles;

        bUpdateMeasureData++;
    }
    else if(bUpdateMeasureData == 1)
    {
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  0] = sAdcCal.i16D2dBus_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  1] = 0;//sRMS.wBUSN

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  2] = sRMS.u16InvVolt_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  3] = sRMS.u16OpVolt_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  4] = sRMS.u16OpCurr_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  5] = sRMS.i32OpWatt_R >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  6] = (sRMS.i32OpWatt_R & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  7] = sRMS.u32OpVa_R >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  8] = sRMS.u32OpVa_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS +  9] = sRMS.wOPI_PF_R * 256 + sRMS.wOPI_CF_R;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 10] = sRMS.i16Temperature_INV_R;
        if(SourceControlFlag.sBit.OutputOn)
        {
            sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 11] = sSet.u16OutputFrequency;
        }
        else
        {
            sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 11] = 0;
        }

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 12] = sRMS.u16InvVolt_S;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 13] = sRMS.u16OpVolt_S;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 14] = sRMS.u16OpCurr_S;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 15] = sRMS.i32OpWatt_S >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 16] = (sRMS.i32OpWatt_S & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 17] = sRMS.u32OpVa_S >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 18] = sRMS.u32OpVa_S;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 19] = sRMS.wOPI_PF_S * 256 + sRMS.wOPI_CF_S;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 20] = sRMS.i16Temperature_INV_S;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 21] = sRMS.u16InvVolt_T;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 22] = sRMS.u16OpVolt_T;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 23] = sRMS.u16OpCurr_T;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 24] = sRMS.i32OpWatt_T >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 25] = (sRMS.i32OpWatt_T & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 26] = sRMS.u32OpVa_T >> 16;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 27] = sRMS.u32OpVa_T;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 28] = sRMS.wOPI_PF_T * 256 + sRMS.wOPI_CF_T;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 29] = sRMS.i16Temperature_INV_T;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 30] = (i32Inv1AmpVref_R & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 31] = (i32Inv1AmpVref_S & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 32] = (i32Inv1AmpVref_T & 0x0000FFFF);

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 33] = (i32Inv1AmpVref_R >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 34] = (i32Inv1AmpVref_S >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 35] = (i32Inv1AmpVref_T >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 36] = sRMS.i16Temperature_AMB;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 37] = sRMS.u16LoadPercent_Max;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 41] = sPdRunTime.u16RunTimeDate;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 42] = sPdRunTime.sHourMinute.u16All;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 43] = sNowRunTime.u16RunTimeDate;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 44] = sNowRunTime.sHourMinute.u16All;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 45] = (sRMS.u32TotalKWH >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 46] = (sRMS.u32TotalKWH & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 47] = (sRMS.u32TotalKWH_R >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 48] = (sRMS.u32TotalKWH_R & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 49] = (sRMS.u32TotalKWH_S >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 50] = (sRMS.u32TotalKWH_S & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 51] = (sRMS.u32TotalKWH_T >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 52] = (sRMS.u32TotalKWH_T & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 53] = (u32CpuIsrTime >> 16);           //only for debug isr time
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 54] = (u32CpuIsrTime & 0x0000FFFF);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 55] = (u32IsrTimeMax >> 16);
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_MEASUREMENTS + 56] = (u32IsrTimeMax & 0x0000FFFF);


        bUpdateMeasureData++;
    }
    else if(bUpdateMeasureData == 2)
    {
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x20] = sGain_R.wTEMP;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x21] = sGain_R.wBUS;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x22] = sGain_R.wINV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x23] = sGain_R.wOPV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x24] = sGain_R.wOPI;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x25] = sGain_R.wWAT;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x26] = sGain_R.wPWM;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x27] = sGain_R.wAngle;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x2B] = sGain_S.wTEMP;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x2C] = sGain_S.wBUS;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x2D] = sGain_S.wINV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x2E] = sGain_S.wOPV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x2F] = sGain_S.wOPI;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x30] = sGain_S.wWAT;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x31] = sGain_S.wPWM;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x32] = sGain_S.wAngle;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x33] = sGain_T.wTEMP;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x34] = sGain_T.wBUS;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x35] = sGain_T.wINV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x36] = sGain_T.wOPV;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x37] = sGain_T.wOPI;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x38] = sGain_T.wWAT;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x39] = sGain_T.wPWM;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x3A] = sGain_T.wAngle;

        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x3B] = u16IpeakProtect;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x40] = u16PowerRecycle;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0x41] = u16PowerRecycleLevel;

        bUpdateMeasureData++;
    }
    else if(bUpdateMeasureData == 3)
    {
        bUpdateProgramData++;
        if(bUpdateProgramData >= 20)
        {
            bUpdateProgramData = 0;
        }

        i_offset = bUpdateProgramData * 10;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  4 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wR_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  5 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wR_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  6 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wS_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  7 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wS_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  8 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wT_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  9 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wT_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 10 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wFreq_Start;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 11 + i_offset] = sProgramming.u16Step[bUpdateProgramData].wFreq_End;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 12 + i_offset] = sProgramming.u16Step[bUpdateProgramData].u16TimeHigh;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 13 + i_offset] = sProgramming.u16Step[bUpdateProgramData].u16TimeLow;

        bUpdateMeasureData = 0;
    }
    else
    {
        bUpdateMeasureData = 0;
    }

    //***********************************************
    // - configuration data
    //***********************************************
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  0] = sSet.u16OutputVoltage_T;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  1] = sSet.u16OutputVoltage_S;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  2] = sSet.u16OutputVoltage_R;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  3] = sSet.u16OutputFrequency;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  4] = sSet.u16OutputCurrentLimit;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  5] = sSet.u16OutputOnAngle;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  6] = sSet.u16OutputRange;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  7] = SourceControlFlag.sBit.OutputOn;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  8] = sSet.wLoadSaveProgramMemoryIndex.u16Word;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  9] = sSet.u16OutputMode.u16Word;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  10] = sSet.u16RemainingRunTime;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  11] = CONFIG_FLAG.WORD;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  12] = 0;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  13] = sSet.u16SRPhaseAngle;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  14] = sSet.u16TRPhaseAngle;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  16] = sSet.u16LimitYear;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_CONF +  17] = sSet.u16LimitMonthDay;
}

void MBus_UpdateProgramArea(void)
{
    Uint8 i_byte, i_offset;

    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  0] = sProgramming.u16Deep;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  1] = sProgramming.u16Start;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  2] = sProgramming.u16End;
    sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS +  3] = sProgramming.u16Cycles;
    for(i_byte = 0; i_byte < 20; i_byte++)
    {
        i_offset = i_byte * 10;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 4 + i_offset] = sProgramming.u16Step[i_byte].wR_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 5 + i_offset] = sProgramming.u16Step[i_byte].wR_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 6 + i_offset] = sProgramming.u16Step[i_byte].wS_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 7 + i_offset] = sProgramming.u16Step[i_byte].wS_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 8 + i_offset] = sProgramming.u16Step[i_byte].wT_StartVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 9 + i_offset] = sProgramming.u16Step[i_byte].wT_EndVolt;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 10 + i_offset] = sProgramming.u16Step[i_byte].wFreq_Start;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 11 + i_offset] = sProgramming.u16Step[i_byte].wFreq_End;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 12 + i_offset] = sProgramming.u16Step[i_byte].u16TimeHigh;
        sModbusData.u16HoldingRegs[MBUS_HREGS_SIDX_COMMANDS + 13 + i_offset] = sProgramming.u16Step[i_byte].u16TimeLow;
    }
}



//------------------------------------------------------------------------------
// check MODBUS time out
//Set Rx timeout to T35
//Timebase:50us
//------------------------------------------------------------------------------
void Isr_Check_MBus_TimeOut(unsigned int u16MbusPortIndex)
{
    if(sModbusRxTx[u16MbusPortIndex].i16LenRx > 0 &&
       sModbusRxTx[u16MbusPortIndex].u8fState == MBUS_STATE_RX &&
       sModbusRxTx[u16MbusPortIndex].u8fRxOK == 0)
    {
        if(sModbusRxTx[u16MbusPortIndex].u16RxTimer >= usTimerT35_50us)
        {
            if(sModbusRxTx[u16MbusPortIndex].i16LenRx >= MB_SER_PDU_SIZE_MIN)
            {
                sModbusRxTx[u16MbusPortIndex].u16RxTimer = 0;
                sModbusRxTx[u16MbusPortIndex].u8fRxOK = 1;
            }
            else
            {
                sModbusRxTx[u16MbusPortIndex].i16LenRx = 0;
            }
        }
        else
        {
            sModbusRxTx[u16MbusPortIndex].u16RxTimer += 1;
        }
    }
}


//----------------------------------------------------------------------------
//main loop
//----------------------------------------------------------------------------
void TaskModbusCommunication(void)
{
    MBus_UpdateDataArea();

    #ifdef  MBUS_INTERFACE_0
    MBus_HandleMBusRx(0, u8ProductID); //SCI0
    #endif

    #ifdef  MBUS_INTERFACE_1
    MBus_HandleMBusRx(1, u8ProductID);   //SCI1
    #endif

    #ifdef  MBUS_INTERFACE_2
    MBus_HandleMBusRx(2, u8ProductID); //SCI2
    #endif
}

