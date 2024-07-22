#ifndef MAINUPS_H
#define MAINUPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "F2837xS_Device.h"     // F2837xS Headerfile Include File

//timer
#define T_2SEC              (20)


typedef union
{
	Uint32 u32All;
	struct ST_MAIN_CTL
	{
    unsigned parameterOK				:1;		// sBit:00 
    unsigned My_Initial					:1;     // sBit:01 
    unsigned UPS_Standby				:1;     // sBit:02 
    unsigned ForceIpSCR_On				:1;		// sBit:03  When No Bat UPS On, Ip SCR On Before PFC On
    unsigned UPS_Off					:1;     // sBit:04 
    unsigned UPS_On_AfterDisEnergize	:1;     // sBit:05 
    unsigned u1PFC_ON					:1;  	// sBit:06 
    unsigned u1PFC_OK					:1;  	// sBit:07 
    	                                     
    unsigned INV_ON						:1;    	// sBit:08 
    unsigned INV_OK						:1;    	// sBit:09
    unsigned DisBus_Finish				:1;   	// sBit:10
    unsigned Ac_To_Dc					:1;   	// sBit:11
    unsigned Dc_To_Ac					:1;   	// sBit:12
    unsigned syncPFCIn					:1;   	// sBit:13
    unsigned syncIn						:1;   	// sBit:14
    unsigned syncBusIn					:1;   	// sBit:15
                                            
    	                                    
    unsigned u1InPhaseMode				:1;		// sBit:16
    unsigned syncInPhaMode				:1;		// sBit:17
    unsigned phaLocked					:1;		// sBit:18
    unsigned syncInPhaLocked			:1;     // sBit:19
    unsigned op50Hz						:1;		// sBit:20
    unsigned SysfreeRun					:1;		// sBit:21
    unsigned SyncBusfreeRun				:1;		// sBit:22
    unsigned inParallel					:1;		// sBit:23
    	                    			        
    unsigned freqConvMode				:1;		// sBit:24
    unsigned autoBypSearched			:1;     // sBit:25
    unsigned master						:1;		// sBit:26
	unsigned OpCurMovingSum_Start		:1;		// sBit:27											// sBit:27
    unsigned u1ControllerTimebase10ms	:1; 	// sBit:28
    unsigned u1ForceDcbusResoftstart	:1; 	// sBit:29
    unsigned u1OnGenerator              :1;     // sBit:30
	}sBit;                                   
}main_control;

extern union UN_MY_FAULT_FLAG
{
	Uint16  u16All;
    struct  ST_MY_FLAG
	{
    	unsigned SysPLL_Fail     	:1;     // Bit0, Bypass I/P Fail
    	unsigned SyncBus_Fail    	:1;     // Bit1, Sync_Bus I/P Fail
    	unsigned Reserved2			:1;     // Bit2,
    	unsigned Reserved3       	:1;     // 
    	unsigned Reserved4      	:1;     // 
    	unsigned Reserved5     	    :1;     // 
    	unsigned Reserved6    		:1;     //
    	unsigned Reserved7      	:1;     // 
    }sBit;
}Sync_Fault;             					// My_Fault flag

//system
extern Uint32 u32SysTimeBaseps;
extern Uint16 u16SysTime1ms, wSysTime20ms, u16SysTime100ms, u16SysTime1s;

//flag
extern main_control sMainCtrlFlag;

//const
extern const int i16SineTable[];

//----------------------------------------------------------------------------

void mainUPS(void);
void SysTimeRoutine(void);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  

