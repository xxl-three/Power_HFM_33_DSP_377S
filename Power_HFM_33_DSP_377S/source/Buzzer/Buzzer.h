
/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Buzzer.h

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef	BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "F28x_Project.h"   // Device Headerfile and Examples Include File

//----------------------------------------------------------------------------
#define BUZ_PWM_HALF_PERIOD   (31234)     // 1000000/2.4k/0.00667/2 = 31234


//----------------------------------------------------------------------------
// Golbal Variable
//---------------------------------------------------------------------------- 
extern  union   UN_BEEP_FLAG
{
    Uint16  u16All;
    struct  ST_BEEP_FLAG
    {
        unsigned u1BeepEnable    : 1;    // Beep enable flag
        unsigned u1BeepPause     : 1;    // Beep pause  flag
        unsigned u1BiBi          : 1;    // Beep Bi Bi       flag
        unsigned u1BiBiTmp       : 1;    // Beep Bi Bi temp. flag
        unsigned u1OnKeyBeep     : 1;    // Beep on          flag, when ON key press
        unsigned u1OffKeyBeep    : 1;    // Beep on          flag, when OFF key press
        unsigned u1SetupKeyBeep  : 1;    // Beep on          flag, when SETUP key press
        unsigned u1BeepTest      : 1;    // Beep Test        flag
            
        unsigned u1BeepPwmOn     : 1;
    }       sBit;
}       sBeepFlag;              // BEEP flag

extern  union   UN_BEEP_SOUND
{
    Uint16  u16All;
    struct  ST_BEEP_SOUND
    {
        unsigned u1OneBeep0p2s      : 1;    // Beep 0.2S
        unsigned u1OneBeep5s        : 1;    // Beep 5S
        unsigned u1BeepSlow10s      : 1;    // Slow beep 10S flag
        unsigned u1BeepSlow5s       : 1;    // Slow beep  5S flag
        unsigned u1BeepSlow         : 1;    // Slow beep  2S flag
        unsigned u1BeepMid          : 1;    // Mid  beep     flag
        unsigned u1BeepFast         : 1;    // Fast beep     flag
    }       sBit;
}       sBeepSound;             // BEEP sound select

extern  Uint16  u16BuzzerTestTime;              // Buzzer Test time

//----------------------------------------------------------------------------
// Golbal functions
//----------------------------------------------------------------------------
extern void BuzzerShort0p2sBeep(void);
extern void BuzzerShort5sBeep(void);
extern void BuzzerClearBeep(void);

extern void TaskBuzzerControl(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif 

