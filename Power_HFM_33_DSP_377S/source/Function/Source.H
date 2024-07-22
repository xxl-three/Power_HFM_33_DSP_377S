
/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Source.h

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef ACSOURCE_H
#define ACSOURCE_H

#include "usr_var.h"

extern void TimeBaseSystem(void);

extern void Adjust_Pwm_Gain(unsigned char RST);
extern void Adjust_Frequency(void);

extern void Check_DCBUS_VOL(void);

extern void GET_FREQ(void);
extern Uint16 FREQ_COM(Uint16 i_uint);

extern Uint8 ASC2BYTE(Uint16 i_uint);

extern void TimeBase100ms(void);
extern void TimeBase1Sec(void);
extern void TimeBase1Min(void);
extern void RunTime(void);

extern void Check_VFSet_Mode_Time(void);
extern void CHECK_FREQUENCY(void);
extern void CLR_FLAG(void);
extern void CLR_ALL_FLAG(void);
extern void INIT_VAR(void);

extern void Program_Stop(void);
extern void Program_Start(void);
extern void Program_Start_Data_init(void);
extern void Program_Running(void);

extern void Check_Over_Current(void);
extern void Check_Over_Load(void);
extern void Check_Over_Voltage(void);
extern void Check_Over_Temperature(void);
extern void Check_Updata_Eeprom(void);
extern void Check_Repair_Mode(void);

extern void Adjust_Angle_Offset(void);
extern unsigned int Get_Temperature(unsigned int wTempAdc, unsigned char type);
extern void Input_Relay_Control(void);
extern void FanSpeedControl(void);
extern void IpeakControl(void);

extern void Clear_Set_Flag(void);

extern void InvRmsVolController(void);

extern void TaskDryContact(void);
extern void SetOutputOff(void);

#endif
