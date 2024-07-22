//###########################################################################
//
// FILE:   F2837xS_EPwm.c
//
// TITLE:  F2837xS EPwm Initialization & Support Functions.
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
#include "USR_VAR.h"
#include "Buzzer.h"
//
// InitEPwmGpio - Initialize all EPWM modules' GPIOs
//
void InitEPwmGpio(void)
{
    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();
    InitEPwm5Gpio();
    InitEPwm6Gpio();
    InitEPwm7Gpio();
    InitEPwm11Gpio();
    InitEPwm12Gpio();
}

//
// InitEPwm1Gpio - Initialize EPWM1 GPIOs
//
void InitEPwm1Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO145 = 1;    // Disable pull-up on GPIO145 (EPWM1A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO146 = 1;    // Disable pull-up on GPIO146 (EPWM1B)

    //
    // Configure EPWM-1 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM1 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A (For PFC1+)
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B (For PFC1-)
    // GpioCtrlRegs.GPEMUX2.bit.GPIO145 = 1;   // Configure GPIO145 as EPWM1A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO146 = 1;   // Configure GPIO0146 as EPWM1B

    EDIS;
}

//
// InitEPwm2Gpio - Initialize EPWM2 GPIOs
//
void InitEPwm2Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO147 = 1;    // Disable pull-up on GPIO147 (EPWM2A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO148 = 1;    // Disable pull-up on GPIO148 (EPWM2B)

    //
    // Configure EPwm-2 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM2 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A (For PFC2+)
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B (For PFC2-)
    // GpioCtrlRegs.GPEMUX2.bit.GPIO147 = 1;   // Configure GPIO147 as EPWM2A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO148 = 1;   // Configure GPIO148 as EPWM2B

    EDIS;
}

//
// InitEPwm3Gpio - Initialize EPWM3 GPIOs
//
void InitEPwm3Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO149 = 1;    // Disable pull-up on GPIO149 (EPWM3A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO150 = 1;    // Disable pull-up on GPIO150 (EPWM3B)

    //
    // Configure EPwm-3 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM3 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO149 = 1;   // Configure GPIO149 as EPWM3A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO150 = 1;   // Configure GPIO150 as EPWM3B

    EDIS;
}

//
// InitEPwm4Gpio - Initialize EPWM4 GPIOs
//
void InitEPwm4Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO151 = 1;    // Disable pull-up on GPIO151 (EPWM4A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO152 = 1;    // Disable pull-up on GPIO152 (EPWM4B)

    //
    // Configure EPWM-4 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM4 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A (For INV1+)
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B (For INV1-)
    // GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 1;   // Configure GPIO151 as EPWM4A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO152 = 1;   // Configure GPIO152 as EPWM4B

    EDIS;
}

//
// InitEPwm5Gpio - Initialize EPWM5 GPIOs
//
void InitEPwm5Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO153 = 1;    // Disable pull-up on GPIO153 (EPWM5A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO154 = 1;    // Disable pull-up on GPIO154 (EPWM5B)

    //
    // Configure EPWM-5 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM5 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A (For INV2+)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B (For INV2-)
    // GpioCtrlRegs.GPEMUX2.bit.GPIO153 = 1;   // Configure GPIO153 as EPWM5A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO154 = 1;   // Configure GPIO0154 as EPWM5B

    EDIS;
}
//
// InitEPwm6Gpio - Initialize EPWM6 GPIOs
//
void InitEPwm6Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO155 = 1;    // Disable pull-up on GPIO155 (EPWM6A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO156 = 1;    // Disable pull-up on GPIO156 (EPWM6B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO155 = 1;   // Configure GPIO155 as EPWM6A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO156 = 1;   // Configure GPIO156 as EPWM6B

    EDIS;
}

//
// InitEPwm7Gpio - Initialize EPWM7 GPIOs
//
void InitEPwm7Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    //GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)
    // GpioCtrlRegs.GPEPUD.bit.GPIO157 = 1;    // Disable pull-up on GPIO157 (EPWM7A)
    // GpioCtrlRegs.GPEPUD.bit.GPIO158 = 1;    // Disable pull-up on GPIO158 (EPWM7B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A (For FAN1_PWM)
    //GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    // GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B

    EDIS;
}

//
// InitEPwm8Gpio - Initialize EPWM8 GPIOs
//
void InitEPwm8Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)
//  GpioCtrlRegs.GPEPUD.bit.GPIO159 = 1;    // Disable pull-up on GPIO159 (EPWM8A)
//  GpioCtrlRegs.GPFPUD.bit.GPIO160 = 1;    // Disable pull-up on GPIO160 (EPWM8B)

     //
     // Configure EPWM-6 pins using GPIO regs
     // This specifies which of the possible GPIO pins will be EPWM6 functional
     // pins.
     // Comment out other unwanted lines.
     //
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B
    // GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    // GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B

    EDIS;
}

//
// InitEPwm11Gpio - Initialize EPWM11 GPIOs
//
void InitEPwm11Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;     // Disable pull-up on GPIO20 (EPWM11A)
    //GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;     // Disable pull-up on GPIO166 (EPWM11B)
    //GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
    //GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    // Configure GPIO20 as EPWM11A
    //GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 5;    // Configure GPIO21 as EPWM11B
    //GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1;   // Configure GPIO165 as EPWM11A
    //GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1;   // Configure GPIO166 as EPWM11B

    EDIS;
}

//
// InitEPwm12Gpio - Initialize EPWM12 GPIOs
//
void InitEPwm12Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;    // Disable pull-up on GPIO22 (EPWM12A)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;    // Disable pull-up on GPIO23 (EPWM12B)
    //GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
    //GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    //GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 5;   // Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 5;   // Configure GPIO23 as EPWM12B (For BEEP)
    //GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1;   // Configure GPIO167 as EPWM12A
    //GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1;   // Configure GPIO168 as EPWM12B
    
    EDIS;
    
    //GPIO_SetupPinMux(23, GPIO_MUX_CPU1, 5);
    
}



//---------------------------------------------------------------------------
// InitEPwm:
// This function initializes the ePWM(s) to a known state.
//
//---------------------------------------------------------------------------
#define DEAD_BAND    (180)       //
void InitEPwm(void)
{
    // Initialize ePWM1/2/3/4/5/6/7/8/9/10/11/12
    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;     // x0 = /1 of PLLSYSCLK
    // x1 = /2 of PLLSYSLCK (default on reset)
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;           // Stop all the TB clocks

    // Time-Base Submodule Registers
    // INV-R1
    EPwm1Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm1Regs.TBCTR = 0;
    EPwm1Regs.CMPA.bit.CMPA = 0;
    EPwm1Regs.CMPB.bit.CMPB = 0;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;  // Count up down
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm1Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBD  = AQ_SET;

    // INV-R2
    EPwm2Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm2Regs.TBCTR = 0;
    EPwm2Regs.CMPA.bit.CMPA = 0;
    EPwm2Regs.CMPB.bit.CMPB = 0;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm2Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CBD  = AQ_SET;

    // INV-S1
    EPwm3Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm3Regs.TBCTR = 0;
    EPwm3Regs.CMPA.bit.CMPA = 0;
    EPwm3Regs.CMPB.bit.CMPB = 0;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm3Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CBD  = AQ_SET;

    // INV-S2
    EPwm4Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm4Regs.TBCTR = 0;
    EPwm4Regs.CMPA.bit.CMPA = 0;
    EPwm4Regs.CMPB.bit.CMPB = 0;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm4Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm4Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm4Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CBD  = AQ_SET;

    // INV-T1
    EPwm5Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm5Regs.TBCTR = 0;
    EPwm5Regs.CMPA.bit.CMPA = 0;
    EPwm5Regs.CMPB.bit.CMPB = 0;
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm5Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm5Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm5Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm5Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm5Regs.AQCTLB.bit.CBD  = AQ_SET;
    
    // INV-T2
    EPwm6Regs.TBPRD = INV_SW_HALF_PERIOD;
    EPwm6Regs.TBCTR = 0;
    EPwm6Regs.CMPA.bit.CMPA = 0;
    EPwm6Regs.CMPB.bit.CMPB = 0;
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm6Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm6Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm6Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.CBD  = AQ_SET;

    // FAN
    EPwm7Regs.TBPRD = FAN_PWM_PERIOD;
    //EPwm7Regs.AQCSFRC.bit.CSFA = 1;
    EPwm7Regs.TBCTR = 0;
    EPwm7Regs.CMPA.bit.CMPA = FAN_PWM_PERIOD / 2;
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm7Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
    EPwm7Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm7Regs.AQCTLA.bit.CAD  = AQ_SET;
    
    // INVERTER IPEAK
    EPwm11Regs.TBPRD = IPEAK_PWM_PERIOD;
    //EPwm11Regs.AQCSFRC.bit.CSFA = 1;
    EPwm11Regs.TBCTR = 0;
    EPwm11Regs.CMPA.bit.CMPA = IPEAK_PWM_PERIOD / 2;
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm11Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
    EPwm11Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm11Regs.AQCTLA.bit.CAD  = AQ_SET;

    // BEEP
    EPwm12Regs.TBPRD = BUZ_PWM_HALF_PERIOD;
    EPwm12Regs.AQCSFRC.bit.CSFB = 1;
    EPwm12Regs.TBCTR = 0;
    EPwm12Regs.CMPA.bit.CMPA = 0;
    EPwm12Regs.CMPB.bit.CMPB = BUZ_PWM_HALF_PERIOD / 2;
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm12Regs.TBCTL.bit.CTRMODE  = TB_COUNT_UPDOWN;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
    EPwm12Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm12Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm12Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.CBD  = AQ_SET;

    //======for sync reset====================
    //EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm9Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm10Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm11Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm12Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //
    //EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm5Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm7Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm8Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm9Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm10Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm11Regs.TBCTL.bit.PHSDIR = TB_UP;
    //EPwm12Regs.TBCTL.bit.PHSDIR = TB_UP;
    //
    //EPwm1Regs.TBPHS.bit.TBPHS = 0;
    //EPwm2Regs.TBPHS.bit.TBPHS = 0;
    //EPwm3Regs.TBPHS.bit.TBPHS = 0;
    //EPwm4Regs.TBPHS.bit.TBPHS = 0;
    //EPwm5Regs.TBPHS.bit.TBPHS = 0;
    //EPwm6Regs.TBPHS.bit.TBPHS = 0;
    //EPwm7Regs.TBPHS.bit.TBPHS = 0;
    //EPwm8Regs.TBPHS.bit.TBPHS = 0;
    //EPwm9Regs.TBPHS.bit.TBPHS = 0;
    //EPwm10Regs.TBPHS.bit.TBPHS = 0;
    //EPwm11Regs.TBPHS.bit.TBPHS = 0;
    //EPwm12Regs.TBPHS.bit.TBPHS = 0;
    //=======================================

    // 3 Level Inverter
    // Dead Band
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm1Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm1Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm1Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time

    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm2Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm2Regs.DBCTL.bit.OUTSWAP = 0x03;
    EPwm2Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm2Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time
    
    EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm3Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm3Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm3Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time
    
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm4Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm4Regs.DBCTL.bit.OUTSWAP = 0x03;
    EPwm4Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm4Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time

    EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm5Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm5Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm5Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time

    EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;          // 0b00: EPWMxA In (from the action-qualifier) is the source for both falling-edge and rising-edge delay.
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 0b10: Active high complementary (AHC). EPWMxB is inverted.
    EPwm6Regs.DBCTL.bit.OUT_MODE  = DB_FULL_ENABLE; // 0b11: DBM is fully enabled (i.e. both RED and FED active)
    EPwm6Regs.DBCTL.bit.OUTSWAP = 0x03;
    EPwm6Regs.DBRED.all = DEAD_BAND;                // TBCLK(1/(200M/(1(CLKDIV)*1(HSPCLKDIV)))) * DBRED = time = 0.6us (EE defined)
    EPwm6Regs.DBFED.all = DEAD_BAND;                // TBCLK * DBFED = time


    // Trip Zone
    //EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    //EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    //EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    //EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    //EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    //EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    /*
    // Event Trigger
    // ADC sampling point according to PWM timer
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;	// ET_CTR_ZERO: 0x1, Enable event time-base counter equal to zero. (TBCTR = 0x0000)
    // ET_CTR_PRD:	0x2, Enable event time-base counter equal to period(TBCTR = TBPRD)
    // ET_CTRU_CMPA:0x4, Enable event time-base counter equal to CMPA when the timer is incrementing or CMPC when the timer is incrementing
    EPwm1Regs.CMPC = 10;
    EPwm1Regs.ETSEL.bit.SOCASELCMP = 0;         // 0: Enable event time-base counter equal to CMPA
    // 1: Enable event time-base counter equal to CMPC
    //==============================================
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;        // 1: triggered every once

    EPwm1Regs.ETSEL.bit.SOCAEN = 1;             // before a new PWM cycle, start ADC convert
    */
    
    //EPWM1中断
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.INTEN = 1;
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;
    //EPWM2中断
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm2Regs.ETSEL.bit.INTEN = 1;
    EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;
    

    // for sync
    //SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = 5;        // EXTSYNCIN1 selected
    //SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = 0;        // EPWM1SYNCOUT selected
    //EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    //EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    //EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    //EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    //EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;          // enable the TB clocks
    EDIS;
    
    //移相全桥
    EPwm2Regs.TBPHS.bit.TBPHS = INV_SW_HALF_PERIOD; 
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;
    EPwm2Regs.TBCTL.bit.SWFSYNC = 1;
    EPwm2Regs.TBCTL.bit.PHSEN = 0;
    
    EPwm4Regs.TBPHS.bit.TBPHS = INV_SW_HALF_PERIOD; 
    EPwm4Regs.TBCTL.bit.PHSEN = 1;
    EPwm4Regs.TBCTL.bit.SYNCOSEL = 0;
    EPwm4Regs.TBCTL.bit.SWFSYNC = 1;
    EPwm4Regs.TBCTL.bit.PHSEN = 0;
    
    EPwm6Regs.TBPHS.bit.TBPHS = INV_SW_HALF_PERIOD; 
    EPwm6Regs.TBCTL.bit.PHSEN = 1;
    EPwm6Regs.TBCTL.bit.SYNCOSEL = 0;
    EPwm6Regs.TBCTL.bit.SWFSYNC = 1;
    EPwm6Regs.TBCTL.bit.PHSEN = 0;

}
