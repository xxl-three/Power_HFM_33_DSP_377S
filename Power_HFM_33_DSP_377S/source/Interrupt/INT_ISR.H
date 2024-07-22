/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : INT_ISR.H

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : 

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef INT_ISR_H
#define INT_ISR_H


extern ni32HiLoWord_t si32InvPwmDutyR,si32InvPwmDutyS,si32InvPwmDutyT;    
extern ni32HiLoWord_t si32InvPwmDutyRn,si32InvPwmDutySn,si32InvPwmDutyTn;   
//----------------------------------------------------------------------------
// Golbal functions
//----------------------------------------------------------------------------
extern void IsrInverterController(void);
extern void SystemControl_ISR(void);

extern void IsrInverterPWM_A(void);
extern void IsrInverterPWM_B(void);

extern void Scia_ISR(void);
//extern void Scib_ISR(void);
extern void Scic_ISR(void);
extern void Scid_ISR(void);

#endif


