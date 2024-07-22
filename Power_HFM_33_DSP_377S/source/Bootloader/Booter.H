/*****************************************************************************

PROJECT  : Source

COMPILER : Code Composer Studio V8.2.0.00007

MODULE   : BOOTER.H 

VERSION  : V0.0

DATE     : 2018/10/05

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef BOOTER_H
#define BOOTER_H

#include "USR_VAR.h"

#define _EnableInterrupts()         EINT
#define _DisableInterrupts()        DINT
/******************************************************************************
 Constant Define
******************************************************************************/
#define BOOT_PASSWORD   0x55AA                  // 0x55AA, Password
#define BOOT_KEYWORD    0x8014                  // 0x8014, SplitPhase 16kva Keyword
#define BOOT_CHECKSUM   ((0x55 + 0xAA + (BOOT_KEYWORD>>8) + (BOOT_KEYWORD&0x00FF) + 0x01 + 0x0A + 0x04)&0x00FF)

#define BOOT_RX_BUFF_LEN 80
#define BOOT_TX_BUFF_LEN 20

#define APPLICATION_FLASH_STARTING_ADDR         (0x00084000)
#define APPLICATION_FLASH_END_ADDR              (0x000BFFFF)
//#pragma DATA_SEG BOOTER_RAM
/******************************************************************************
 Variable Define
******************************************************************************/
//extern  Uint32  *pu32FlashAddr;                      // Flash address
//extern  Uint16  *RamAddr;                        // RAM address
//extern  Uint16  FlashData;
//extern  Uint16  FlashCount;
//extern  Uint16  FlashFlag;

//extern  Uint32  u32BootTimeBaseUsec;               // Timebase, micro sec
//extern  Uint16  BootTimeBase100ms;              // Timebase, 100ms
//extern  Uint16  BootTimeBase1s;                 // Timebase, 1   s
//extern  Uint16  LcdTime;                        // LCD character delay time
//extern  Uint16  DspTime;                        // LCD display   delay time
//extern  Uint16  Ecommand;                       // Command
//extern  Uint16  RxCheckSum;                     // RX check sum
//extern  Uint16  RxTimeOutCnt;                   // No RX data time out count
//extern  Uint16  NoRxDataCnt;                    // No RX data in count, for reset RX COUNT

//extern  Uint16  ByteNumber;                     // Byte number
//extern  Uint16  PageNumber;                     // Page number
//extern  Uint32  StartAddress;                   // Start Address
//extern  Uint16  Data1;                          // Data1
//extern  Uint16  Data2;                          // Data2

extern  Uint16  BootRamfunsLoadStart;
extern  Uint16  BootRamfunsLoadSize  ;
extern  Uint16  BootRamfunsLoadEnd  ;
extern  Uint16  BootRamfunsRunStart ;
extern  Uint16  BootRamfunsRunSize   ;
extern  Uint16  BootRamfunsRunEnd    ;

 
extern Uint16 FlashApiLoadStart;
extern Uint16 FlashApiLoadEnd;
extern Uint16 FlashApiLoadSize;
extern Uint16 FlashApiRunStart;
extern Uint16 FlashApiRunEnd;
extern Uint16 FlashApiRunSize;
 
//extern  Uint16  SectorMask;
//extern  Uint16  *pu16DataBufferAddr;
//extern  Uint16  u16DataBufferSizeInWords;
//extern  Uint16  Status;
//extern  FLASH_ST FlashStatus;

//extern  Uint16  Buffer[WORDS_IN_FLASH_BUFFER];


extern  union   UN_SciFlag
        {
        Uint16  u16All;
        struct  ST_SciFlag
        {
        unsigned TxDataEnable    :1;             // Transmit Data
        unsigned RxDataIn        :1;             // Receive  Data input
        unsigned ReadyToProgram  :1;             // Ready to Program
        unsigned EndOfData       :1;             // End   of Data
        unsigned PROGRAM_MODE    :1;             // Entry Program mode flag
        }       sBit;
        }       SciFlag;                        // SCI flag

extern  union   UN_ProgramFlag
        {
        Uint16  u16All;
        struct  ST_ProgramFlag
        {
        unsigned ProgramOver     :1;             // Finish Program and exit
        unsigned ProgramFinish   :1;             // Finish Program
        unsigned Erasing         :1;             // Erasing
        unsigned EraseOK         :1;             // Erase OK
        unsigned Programming     :1;             // Programming
        unsigned ProgramOK       :1;             // Program OK
        unsigned Verifying       :1;             // Verifying
        unsigned VerifyOK        :1;             // Verify OK
            
        unsigned u1FlashBank     :1;             // Verify OK
        }       sBit;
        }       ProgramFlag;                    // Program Flag

extern  union   UN_FailFlag
        {
        Uint16  u16All;
        struct  ST_FailFlag
        {
        unsigned ErrorFail       :1;             // Error   Fail
        unsigned EraseFail       :1;             // Erase   Fail
        unsigned ProgramFail     :1;             // Program Fail
        unsigned VerifyFail      :1;             // Verify  Fail
        }       sBit;
        }       FailFlag;                       // Fail Flag

//#pragma CONST_SEG PASSWORD_ROM
extern const unsigned int BootPassWord[];


//#pragma CODE_SEG BOOTLOAD_APP
extern void Bloader(void);
extern void INIT_PIM_Boot(void);

void MemCopy_Boot(Uint16 *Runstart, Uint16 *LoadStart, Uint16 len);

void Boot_TimeBase(void);
void CLR_TimeBase(void);
void TimeDelayMicroSec(void);


extern void InitSCI2400(void);
extern void InitSCI9600(void);
extern void InitSCI19200(void);

void SciReceive(void);
void SciTransmit(void);
void SciReceiveData(void);
void SciTransmitData(void);
void SciParser(void);

void CheckStartProgram(void);

Uint16 GetCheckSum(Uint16 *addr,Uint16 count);

void TR_CheckSumError(void);
void TR_Success(void);
void TR_FAIL(void);
void TR_ErrorFile(void);

void CheckTimeOut(void);
//extern void WaitToDisplay(Uint16 second);

void Erase_All_Flash(void);
void Program_Flash(void);
void Finish_Program(void);

void InitSysCtrl_Boot(void);
void InitSysPll_Boot(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel);
void InitPeripheralClocks_Boot(void);
Uint16 CsmUnlock_Boot();

void ProgBuzzer(void);


#endif
