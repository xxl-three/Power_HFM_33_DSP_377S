//###########################################################################
//
// FILE:   F2837xS_ECap.c
//
// TITLE:  F2837xS eCAP Initialization & Support Functions.
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

#include "USR_VAR.H"

//
// InitECap - This function initializes the eCAP(s) to a known state.
//
void InitECap(void)
{
    // Initialize eCAP1/2/3/4/5/6
   
    ECap1Regs.ECEINT.all = 0x0000;              // Disable All Capture Interrupts
    ECap1Regs.ECCLR.all  = 0xFFFF;              // Clear All Capture Interrupt Flags
    ECap1Regs.ECCTL1.bit.CAPLDEN   = 0;         // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;         // Make sure the counter is stopped
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;       // Continuous mode
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;         // Stop at 1 events
    ECap1Regs.ECCTL1.bit.CAP1POL = 0;           // 1:Falling Edge; 0:Rising Edge
    ECap1Regs.ECCTL1.bit.CTRRST1 = 0;           // Absolute Time Stamp
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;          // Disable sync in
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;         // Pass through
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;           // Enable capture units
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;         // Start Counter
    
    //tbd...
    
    ECap2Regs.ECEINT.all = 0x0000;              // Disable All Capture Interrupts
    ECap2Regs.ECCLR.all  = 0xFFFF;              // Clear All Capture Interrupt Flags
    ECap2Regs.ECCTL1.bit.CAPLDEN   = 0;         // Disable CAP1-CAP4 register loads
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;         // Make sure the counter is stopped
    ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;       // Continuous mode
    ECap2Regs.ECCTL2.bit.STOP_WRAP = 0;         // Stop at 1 events
    ECap2Regs.ECCTL1.bit.CAP1POL = 0;           // 1:Falling Edge; 0:Rising Edge
    ECap2Regs.ECCTL1.bit.CTRRST1 = 0;           // Absolute Time Stamp
    ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;          // Disable sync in
    ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0;         // Pass through
    ECap2Regs.ECCTL1.bit.CAPLDEN = 1;           // Enable capture units
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;         // Start Counter
 
   
    /*
    ECap4Regs.ECEINT.all = 0x0000;              // Disable All Capture Interrupts
    ECap4Regs.ECCLR.all  = 0xFFFF;              // Clear All Capture Interrupt Flags
    ECap4Regs.ECCTL1.bit.CAPLDEN   = 0;         // Disable CAP1-CAP4 register loads
    ECap4Regs.ECCTL2.bit.TSCTRSTOP = 0;         // Make sure the counter is stopped
    ECap4Regs.ECCTL2.bit.CONT_ONESHT = 0;       // Continuous mode
    ECap4Regs.ECCTL2.bit.STOP_WRAP = 0;         // Stop at 1 events
    ECap4Regs.ECCTL1.bit.CAP1POL = 1;           // 1:Falling Edge; 0:Rising Edge
    ECap4Regs.ECCTL1.bit.CTRRST1 = 0;           // Absolute Time Stamp
    ECap4Regs.ECCTL2.bit.SYNCI_EN = 0;          // Disable sync in
    ECap4Regs.ECCTL2.bit.SYNCO_SEL = 0;         // Pass through
    ECap4Regs.ECCTL1.bit.CAPLDEN = 1;           // Enable capture units
    ECap4Regs.ECCTL2.bit.TSCTRSTOP = 1;         // Start Counter
    */
    
    //使用ECap3Regs的APWM模式来产生相位角	
    ECap3Regs.ECCTL2.bit.CAP_APWM = 1;      // Enable APWM mode
    ECap3Regs.ECCLR.all = 0x0FF;            // Clear pending __interrupts
    ECap3Regs.CAP1 = sSet.u32OpPeriod;		// active registers：period
    ECap3Regs.CAP2 = sSet.u32OpPeriod>>1;	// active registers：compare
    ECap3Regs.CAP3 = sSet.u32OpPeriod;		// shadow registers：period
    ECap3Regs.CAP4 = sSet.u32OpPeriod>>1;	// shadow registers：compare
    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start counters
    
}

//
// InitECapGpio - This function initializes GPIO pins to function as ECAP pins
//                Each GPIO pin can be configured as a GPIO pin or up to 3
//                different peripheral functional pins. By default all pins
//                come up as GPIO inputs after reset.
//                Caution:
//                For each eCAP peripheral
//                Only one GPIO pin should be enabled for ECAP operation.
//                Comment out other unwanted lines.
//
void InitECapGpio()
{   
    InitECap1Gpio(80); //INPUT L1 ZERO SYNC(default)
    InitECap2Gpio(81); //INPUT L2 ZERO SYNC(default)
}

//
// InitECap1Gpio - Initialize ECAP1 GPIOs
//
void InitECap1Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT7SELECT = pin;         // Set eCAP1 source to GPIO-pin
    EDIS;
}

//
// InitECap2Gpio - Initialize ECAP2 GPIOs
//
void InitECap2Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT8SELECT = pin;         // Set eCAP2 source to GPIO-pin
    EDIS;
}

//
// InitECap3Gpio - Initialize ECAP3 GPIOs
//
void InitECap3Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT9SELECT = pin;         // Set eCAP3 source to GPIO-pin
    EDIS;
}

//
// InitECap4Gpio - Initialize ECAP4 GPIOs
//
void InitECap4Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT10SELECT = pin;         // Set eCAP4 source to GPIO-pin
    EDIS;
}

//
// InitECap5Gpio - Initialize ECAP5 GPIOs
//
void InitECap5Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT11SELECT = pin;         // Set eCAP5 source to GPIO-pin
    EDIS;
}

//
// InitECap6Gpio - Initialize ECAP6 GPIOs
//
void InitECap6Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT12SELECT = pin;         // Set eCAP6 source to GPIO-pin
    EDIS;
}

//
// InitAPwm1Gpio - Initialize EPWM1 GPIOs
//
void InitAPwm1Gpio()
{
    EALLOW;
    OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX0 = 3; // Select ECAP1.OUT on Mux0
    OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX0 = 1;  // Enable MUX0 for ECAP1.OUT
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 3;    // Select OUTPUTXBAR3 on GPIO5
    EDIS;
}

//
// End of file
//
