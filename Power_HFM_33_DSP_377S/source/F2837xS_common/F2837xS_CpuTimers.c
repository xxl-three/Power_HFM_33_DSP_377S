//###########################################################################
//
// FILE:    F2837xS_CpuTimers.c
//
// TITLE:   CPU 32-bit Timers Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"

//
// Globals
//
struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;

//
// InitCpuTimers - This function initializes all three CPU timers to a known
//                 state.
//
void InitCpuTimers(void)
{
    //
    // CPU Timer 0
    // Initialize address pointers to respective timer registers:
    //
    CpuTimer0.RegsAddr = &CpuTimer0Regs;

    //
    // Initialize timer period to maximum:
    //
    CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;

    //
    // Make sure timer is stopped:
    //
    CpuTimer0Regs.TCR.bit.TSS = 1;

    //
    // Reload all counter register with period value:
    //
    CpuTimer0Regs.TCR.bit.TRB = 1;

    //
    // Reset interrupt counters:
    //
    CpuTimer0.InterruptCount = 0;

    //
    // Initialize address pointers to respective timer registers:
    //
    CpuTimer1.RegsAddr = &CpuTimer1Regs;
    CpuTimer2.RegsAddr = &CpuTimer2Regs;

    //
    // Initialize timer period to maximum:
    //
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;

    //
    // Make sure timers are stopped:
    //
    CpuTimer1Regs.TCR.bit.TSS = 1;
    CpuTimer2Regs.TCR.bit.TSS = 1;

    //
    // Reload all counter register with period value:
    //
    CpuTimer1Regs.TCR.bit.TRB = 1;
    CpuTimer2Regs.TCR.bit.TRB = 1;

    //
    // Reset interrupt counters:
    //
    CpuTimer1.InterruptCount = 0;
    CpuTimer2.InterruptCount = 0;
}

//
// ConfigCpuTimer - This function initializes the selected timer to the period
//                  specified by the "Freq" and "Period" parameters. The "Freq"
//                  is entered as "MHz" and the period in "uSeconds". The timer
//                  is held in the stopped state after configuration.
//
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Count, int type )
{
    //Uint32 temp;

    //
    // Initialize timer period:
    //
    Timer->CPUFreqInMHz = Freq;
    Timer->PeriodInUSec = Count*8;    //in 0.1 us  25000*0.8us=20ms,50hz
    //temp = (long) (Freq * Period);

    //
    // Counter decrements PRD+1 times each period
    //
    Timer->RegsAddr->PRD.all = Count;

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    //if(type == 0)
        Timer->RegsAddr->TPR.all  = 0;      //   5ns
    //else
    //    Timer->RegsAddr->TPR.all  = 19;     //   5ns*(19+1)=100ns=0.1us
    Timer->RegsAddr->TPRH.all  = 0;

    //
    // Initialize timer control register:
    //
    Timer->RegsAddr->TCR.bit.TSS = 1;     // 1 = Stop timer, 0 = Start/Restart
                                          // Timer
    Timer->RegsAddr->TCR.bit.TRB = 1;     // 1 = reload timer
    Timer->RegsAddr->TCR.bit.SOFT = 0;
    Timer->RegsAddr->TCR.bit.FREE = 0;    // Timer Free Run Disabled
    Timer->RegsAddr->TCR.bit.TIE = 0;     // 0 = Disable/ 1 = Enable Timer
                                          // Interrupt
    //
    // Reset interrupt counter:
    //
    Timer->InterruptCount = 0;
}


//
// End of file
//
