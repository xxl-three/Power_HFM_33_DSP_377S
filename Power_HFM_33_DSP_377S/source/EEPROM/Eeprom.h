/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Eeprom.h

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef Eeprom_H
#define Eeprom_H

#include "usr_var.h"

//******************************************************************************
#define EEPROM_93LC86
//#define FRAM_FM25L16


/******************************************************************************
 EEPROM BIT ASSIGN
 93LC56 =  128x16 Bits
 93LC66 =  256x16 Bits
 93LC76 =  512x16 Bits
 93LC86 = 1024x16 Bits
******************************************************************************/
#ifdef  EEPROM_93LC86
//#define EE_READ         0x80            // 93LC66 READ          command opcode
//#define EE_WRITE        0x40            // 93LC66 WRITE         command opcode
//#define EE_EWEN         0x00            // 93LC66 WRITE ENABLE  command opcode
//#define EWEN_ADR        0xC0            // 93LC66 WRITE ENABLE  command address
//#define EE_EWDS         0x00            // 93LC66 WRITE DISABLE command opcode
//#define EWDS_ADR        0x00            // 93LC66 WRITE DISABLE command address
//#define EE_ERAL         0x00            // 93LC66 ERASE ALL     command opcode
//#define ERAL_ADR        0x80            // 93LC66 ERASE ALL     command address

#define EE_READ         0x80            // 93LC86 READ          command opcode
#define EE_WRITE        0x40            // 93LC86 WRITE         command opcode
#define EE_EWEN         0x00            // 93LC86 WRITE ENABLE  command opcode
#define EWEN_ADR        0x300           // 93LC86 WRITE ENABLE  command address
#define EE_EWDS         0x00            // 93LC86 WRITE DISABLE command opcode
#define EWDS_ADR        0x00            // 93LC86 WRITE DISABLE command address
#define EE_ERAL         0x00            // 93LC86 ERASE ALL     command opcode
#define ERAL_ADR        0x200           // 93LC86 ERASE ALL     command address
#endif

/******************************************************************************
 FRAM BIT ASSIGN
 FM25L16 = 2048x8 Bits
******************************************************************************/
#ifdef  FRAM_FM25L16
#define EE_READ         0x03            // FM25L16 READ          command opcode
#define EE_WRITE        0x02            // FM25L16 WRITE         command opcode
#define EE_EWEN         0x06            // FM25L16 WRITE ENABLE  command opcode
#define EWEN_ADR        0x00            // Useless
#define EE_EWDS         0x04            // FM25L16 WRITE DISABLE command opcode
#define EWDS_ADR        0x00            // Useless
#define EE_ERAL         0x22            // My Defined
#define ERAL_ADR        0x00            // Useless
#endif

//******************************************************************************
#define EEPROM_TOTAL_WORDS						76			//words not index(255)

#define QUEUE_FULL                  0       //FIFO满置0
#define QUEUE_EMPTY                 1       //FIFO空置1
#define QUEUE_OPERATE_OK            2       //队列操作完成 赋值为2

//event log
#define EVENT_LOG_WORDS_NUM						10			//use 10 word to record 144 event code
#define EVENT_LOG_INNER_WORDS_NUM				4
#define EVENT_LOG_BUFFER_NUM					80			//the max number of event log happened at the same time
#define EVENT_LOG_TRANS_NUM						10


#define	EEP_ADDR_PASSWORD						0                  
#define	EEP_ADDR_RUN_TIME_LIMIT					1                  
#define	EEP_ADDR_SOURCE_CONFIG					2                  
#define	EEP_ADDR_OUTPUT_MODE					3           
#define	EEP_ADDR_RATING_POWER					4              
#define	EEP_ADDR_OUTPUT_CURRENT_LIMIT			5              
#define	EEP_ADDR_CURRENT_PROGRAM_MEMORY			6              
#define EEP_ADDR_PRODUCT_ID						7              
#define	EEP_ADDR_POWER_RECYCLE_LEVEL   			8
#define	EEP_ADDR_IPEAK						    9
#define	EEP_ADDR_LOAD_ENERGY_MEMORY_HIGH		10                 
#define	EEP_ADDR_LOAD_ENERGY_MEMORY_LOW			11      
#define EEP_ADDR_POWER_RECYCLE                  12           
#define	EEP_ADDR_SR_PHASE						13             
#define	EEP_ADDR_TR_PHASE						14             
#define	EEP_ADDR_OFFSET						    15


#define	GAIN_START_ADDRESS						0x10
#define	GAIN_DATA_LENGTH						9
#define	GAIN_R_START_ADDRESS					0x10
#define	GAIN_S_START_ADDRESS					0x1A
#define	GAIN_T_START_ADDRESS					0x24

#define	EEP_ADDR_RUNTIME_DATE					50
#define	EEP_ADDR_RUNTIME_HOUR_MINUTE			51
#define	EEP_ADDR_ENERGY_R_HI			        52
#define	EEP_ADDR_ENERGY_R_LO			        53
#define	EEP_ADDR_ENERGY_S_HI			        54
#define	EEP_ADDR_ENERGY_S_LO			        55
#define	EEP_ADDR_ENERGY_T_HI			        56
#define	EEP_ADDR_ENERGY_T_LO			        57

#define	HARM_START_ADDRESS					    0x64
#define	HARM_DATA_LENGTH						102

#define	PROGRAM_START_ADDRESS					0x150
#define	PROGRAM_ITEM_NUMBER						20
#define	PROGRAM_DATA_LENGTH						204
#define	PROGRAM_MEMORY_NUMBER					5			

#define EEPROM_DATA_FIFO_SIZE                   250
//******************************************************************************

extern  union   UN_E2pFlag
{
    Uint16  u16All;
    struct  ST_E2pFlag
    {
        Uint16  u1ReturnDefaultValue        : 1;  // Bit 0: set eeprom to default value
        Uint16  u1EraseAlarmHistory      	: 1;
        Uint16  u1Ready                    : 1;
    } sBit;
} sEepromFlag;                    // Parallel Bus flag



//----------------------------------------------------------------------------
// Eeprom
//----------------------------------------------------------------------------
extern struct S_Eeprom
{
    Uint16 u16OutData[2];

    Uint16 u16Front;     //队列头
    Uint16 u16Rear;      //队列尾
    Uint16 u16Count;     //队列计数

    Uint16 u16FIFO[EEPROM_DATA_FIFO_SIZE][2];

} sEeprom;


extern  union   UN_EE_FLAG
{
    Uint16  u16All;
    struct  ST_EE_FLAG
    {
        unsigned  u1EE_CHECK      : 1;    // Check EEPROM on UPS
        unsigned  u1EE_ON         : 1;    // EEPROM have on board
        unsigned  u1EE_WRID       : 1;    // Write new u8UPSID to EEPROM
        unsigned  u1EE_CLS        : 1;    // Erase EEPROM
        unsigned  u1EE_DFT        : 1;    // Return Default data
        unsigned  u1ErrorData     : 1;    // Error data for write to EEPROM
        unsigned  u1Finished      : 1;
    } sBit;
} sEeOperateFlag;                // EEPROM flag

//eeprom
typedef struct
{
    //eeprom protocol
    Uint16 wPassword;		// word 0
    _sProgram wProgram[10];			// word 1~64	total:64 words
} ee_data_structure;

//eeprom
extern ee_data_structure 	sEE_Data_In_Ram;
//external eeprom update
extern Uint16 		u16Ex_EEPROM_Default_Count;

extern Uint16 		u16Event_Log_Update_Count;

//******************************************************************************
extern Uint16 External_EEPROM_Read(int addr);

extern Uint16 ExtEepromQueueIn(Uint16 u16Index, Uint16 u16Value);

extern void Read_Ex_EEPROM_Parameter(void);
extern void External_EEPROM_Update(void);

extern void Updata_Power_Energy(void);
extern void Load_Program_data(int num);
extern void Save_Program_data(int num);

extern void EepromTask(void);

#endif
