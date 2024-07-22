/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : modbus.h

VERSION  : V0.0

DATE     : 2018/06/06

AUTHOR   : HUI

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "F28x_Project.h"     // DSP2833x Headerfile Include File

#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */

#define ILLEGAL_FUNCTION        1
#define ILLEGAL_DATA_ADDRESS    2
#define ILLEGAL_DATA_VALUE      3
#define SERVER_DEVICE_FAILURE   4
#define PRAGROM_DATA_ERROR      5
#define SERVER_DEVICE_BUSY      6
#define PRAGROM_FUNC_DISABLE    7
#define MEMORY_PARITY_ERROR     8

// *** Start define supported register types ***
//#define MBUS_SUPPORT_DISCRETE_INPUT
//#define MBUS_SUPPORT_COILS
#define MBUS_SUPPORT_HOLDING_REGS
//#define MBUS_SUPPORT_INPUT_REGS
// *** End define supported register types ***

// *** Start define supported functions ***
//#define MBUS_SUPPORT_F01  0x01    //Read Coils
//#define MBUS_SUPPORT_F02  0x02    //Read Discrete Inputs
#define MBUS_SUPPORT_F03    0x03    //Read Holding Registers
//#define MBUS_SUPPORT_F04  0x04    //Read Input Registers
//#define MBUS_SUPPORT_F05  0x05    //Write Single Coil
#define MBUS_SUPPORT_F06    0x06    //Write Single Register
//#define MBUS_SUPPORT_F0F  0x0F    //Write Multiple Coils
#define MBUS_SUPPORT_F10    0x10    //Write Multiple registers
//
#define MBUS_SUPPORT_FUNCTION_MASK  0xA4
// *** End define supported functions ***

// *** Start define parameters for registers ***
// 1. define number of discrete inputs/coils/holding registers/input registers
// 2. define begin and end address of discrete inputs/coils/holding registers/input registers
#ifdef MBUS_SUPPORT_DISCRETE_INPUT
    #define MBUS_MAX_DISCREATE_INPUTS   48
    #define MBUS_DI_START_ADDR          0xA000
    #define MBUS_DI_END_ADDR            (MBUS_DI_START_ADDR+MBUS_MAX_DISCREATE_INPUTS-1)
#endif

#ifdef MBUS_SUPPORT_COILS
    #define MBUS_MAX_COILS              16
    #define MBUS_COILS_START_ADDR       0xB000
    #define MBUS_COILS_END_ADDR         (MBUS_COILS_START_ADDR+MBUS_MAX_COILS-1)
#endif

#ifdef MBUS_SUPPORT_HOLDING_REGS

    // identifiers, read only
    #define MBUS_HREGS_LEN_IDS              10
    #define MBUS_HREGS_SADDR_IDS            0x0000
    #define MBUS_HREGS_EADDR_IDS            (MBUS_HREGS_SADDR_IDS+MBUS_HREGS_LEN_IDS-1)
    #define MBUS_HREGS_SIDX_IDS             0
    // states, read only
    #define MBUS_HREGS_LEN_STATES           5
    #define MBUS_HREGS_SADDR_STATES         0x1000
    #define MBUS_HREGS_EADDR_STATES         (MBUS_HREGS_SADDR_STATES+MBUS_HREGS_LEN_STATES-1)
    #define MBUS_HREGS_SIDX_STATES          (MBUS_HREGS_SIDX_IDS+MBUS_HREGS_LEN_IDS)
    // alarms, read only
    #define MBUS_HREGS_LEN_ALARMS           10
    #define MBUS_HREGS_SADDR_ALARMS         0x2000
    #define MBUS_HREGS_EADDR_ALARMS         (MBUS_HREGS_SADDR_ALARMS+MBUS_HREGS_LEN_ALARMS-1)
    #define MBUS_HREGS_SIDX_ALARMS          (MBUS_HREGS_SIDX_STATES+MBUS_HREGS_LEN_STATES)
    // measurements, read only
    #define MBUS_HREGS_LEN_MEASUREMENTS     60  //2020/08/06 50 change to 60
    #define MBUS_HREGS_SADDR_MEASUREMENTS   0x3000
    #define MBUS_HREGS_EADDR_MEASUREMENTS   (MBUS_HREGS_SADDR_MEASUREMENTS+MBUS_HREGS_LEN_MEASUREMENTS-1)
    #define MBUS_HREGS_SIDX_MEASUREMENTS    (MBUS_HREGS_SIDX_ALARMS+MBUS_HREGS_LEN_ALARMS)
    // configuration, read/write
    #define MBUS_HREGS_LEN_CONF             70
    #define MBUS_HREGS_SADDR_CONF           0x4000
    #define MBUS_HREGS_EADDR_CONF           (MBUS_HREGS_SADDR_CONF+MBUS_HREGS_LEN_CONF-1)
    #define MBUS_HREGS_SIDX_CONF            (MBUS_HREGS_SIDX_MEASUREMENTS+MBUS_HREGS_LEN_MEASUREMENTS)
    // commands, single-write only
    #define MBUS_HREGS_LEN_COMMANDS         204 //一组完整的编程数据，3/3机器总共有十组
    #define MBUS_HREGS_SADDR_COMMANDS       0x5000
    #define MBUS_HREGS_EADDR_COMMANDS       (MBUS_HREGS_SADDR_COMMANDS+MBUS_HREGS_LEN_COMMANDS-1)
    #define MBUS_HREGS_SIDX_COMMANDS        (MBUS_HREGS_SIDX_CONF+MBUS_HREGS_LEN_CONF)

    #define MBUS_MAX_HOLDING_REGISTERS      (MBUS_HREGS_LEN_IDS+MBUS_HREGS_LEN_STATES+MBUS_HREGS_LEN_ALARMS+MBUS_HREGS_LEN_MEASUREMENTS+MBUS_HREGS_LEN_CONF+MBUS_HREGS_LEN_COMMANDS)

    //不需要分配Memory的地址
    #define MBUS_HREGS_LEN_THD              102
    #define MBUS_HREGS_SADDR_THD            0x4100
    #define MBUS_HREGS_EADDR_THD            (MBUS_HREGS_SADDR_THD+MBUS_HREGS_LEN_THD-1)

    #define MBUS_HREGS_LEN_SINEWAVE         360
    #define MBUS_HREGS_SADDR_SINEWAVE       0x4500
    #define MBUS_HREGS_EADDR_SINEWAVE       (MBUS_HREGS_SADDR_SINEWAVE+MBUS_HREGS_LEN_SINEWAVE-1)

    #define MBUS_HREGS_LEN_ADJUST           24
    #define MBUS_HREGS_SADDR_ADJUST         0x4A00
    #define MBUS_HREGS_EADDR_ADJUST         (MBUS_HREGS_SADDR_ADJUST+MBUS_HREGS_LEN_ADJUST-1)
#endif

#ifdef MBUS_SUPPORT_INPUT_REGS
    #define MBUS_MAX_INPUT_REGISTERS    285
    #define MBUS_IR_START_ADDR          0x6000
    #define MBUS_IR_END_ADDR            (MBUS_IR_START_ADDR+MBUS_MAX_INPUT_REGISTERS-1)
#endif
// *** End define parameters for registers ***

// *** define send/receive buffer size ***
#define MBUS_MAX_SEND_BUF   255
#define MBUS_MAX_RECV_BUF   128

// *** define modbus operation state ***
#define MBUS_STATE_RX       0
#define MBUS_STATE_TX       1
#define MBUS_STATE_PROCESS  2

// *** define number of modbus tx/rx interfaces ***
#define MBUS_INTERFACE_NUM  3
#define MBUS_INTERFACE_0
#define MBUS_INTERFACE_1
#define MBUS_INTERFACE_2

// *** Set Rx timeout to 10 ms(value will be 100 if timer is 100us). ***
#define MBUS_RX_TIMOUT  2

// *** define structure for rx/tx parameters ***
typedef struct
{
    unsigned char u8InterfaceID;
    // buffer for receiving modbus data
    unsigned char u8BufRx[MBUS_MAX_RECV_BUF];
    // buffer for sending modbus data
    unsigned char u8BufTx[MBUS_MAX_SEND_BUF];
    // flag to indicate port state(receival state or send state)
    unsigned char u8fState;
    // flag to indicate if receival is OK
    unsigned char u8fRxOK;
    // timeout counter for receiving modbus data, this value need to be add in timer interrupt
    unsigned int u16RxTimer;
    // length of receival data
    int i16LenRx;
    // length of send data
    int i16LenTx;
    // current index in send buffer
    int i16IndexTx;
    //
} _StructModbusRxTx;

// *** define structure for storeing register data ***
typedef struct
{
    #ifdef MBUS_SUPPORT_DISCRETE_INPUT
    unsigned char discreteInputs[MBUS_MAX_DISCREATE_INPUTS / 8 + 1];
    #endif

    #ifdef MBUS_SUPPORT_COILS
    unsigned char coils[MBUS_MAX_COILS];
    #endif

    #ifdef MBUS_SUPPORT_HOLDING_REGS
    unsigned int u16HoldingRegs[MBUS_MAX_HOLDING_REGISTERS];
    #endif

    #ifdef MBUS_SUPPORT_INPUT_REGS
    unsigned int u16InputRegs[MBUS_MAX_INPUT_REGISTERS];
    #endif
} _StructModbusData;


// variables
extern  unsigned char   u8ProductID;          //for MODBUS protocol used

extern _StructModbusRxTx sModbusRxTx[MBUS_INTERFACE_NUM];
extern _StructModbusData sModbusData;

//----------------------------------------------------------------------------
// Golbal functions
//----------------------------------------------------------------------------
extern void MBus_InitialRxTx(Uint16 u16MbusPortIndex);
extern void MBus_InitialModbus(void);
extern void MBus_InitialModbusData(void);

extern void Isr_MBus_HandleMBusTx(unsigned int u16MbusPortIndex, unsigned int *pSciTxBuf);
extern void Isr_MBus_HandleMBusRx(unsigned int u16MbusPortIndex, unsigned char u8SciData);

extern void Isr_Check_MBus_TimeOut(unsigned int u16MbusPortIndex);

extern void TaskModbusCommunication(void);
#endif
