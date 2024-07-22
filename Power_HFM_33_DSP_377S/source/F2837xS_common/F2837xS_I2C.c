//###########################################################################
//
// FILE:   F2837xS_I2C.c
//
// TITLE:  F2837xS I2C Initialization & Support Functions.
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
//---------------------------------------------------------------------------
// Example: I2cAGpioConfig(), I2cBGpioConfig()
//---------------------------------------------------------------------------
// These functions configures GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//

#ifdef CPU1
//
// I2cAGpioConfig - Configure I2CA GPIOs
//                  'I2caDataClkPin' should be assign with one of the
//                   possible I2C_A SDA - SDL GPIO pin Use defined Macros from
//                   "F2837xS_I2c_defines.h" for  assignment
//
void I2cAGpioConfig(Uint16 I2caDataClkPin)
{
    EALLOW;

    switch(I2caDataClkPin)
    {
        case I2C_A_GPIO0_GPIO1:

            //
            // Enable internal pull-up for the selected I2C pins
            // Enable pull-up for GPIO0 (SDAA)
            // Enable pull-up for GPIO1 (SDLA)
            //
            GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;
            GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;

            //
            // Set qualification for the selected I2C pins
            // Async/no qualification (I/ps sync to SYSCLKOUT by default)
            //
            GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 3;
            GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;

            //
            // Configure which of the possible GPIO pins will be I2C_A pins
            // using GPIO regs
            // Configure GPIO0 for SDAA operation
            // Configure GPIO0 for SDAA operation
            //
            GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 1;
            GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 2;

            //
            // Configure GPIO1 for SDLA operation
            // Configure GPIO1 for SDLA operation
            //
            GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 1;
            GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 2;

            break;

        case I2C_A_GPIO32_GPIO33:
            //
            // Enable internal pull-up for the selected I2C pins
            //
            GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
            GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;

            //
            // Set qualification for the selected I2C pins
            //
            GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;
            GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;

            //
            // Configure which of the possible GPIO pins will be I2C_A pins
            // using GPIO regs
            //
            GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;
            GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;

            break;

        case I2C_A_GPIO42_GPIO43:
            //
            // Enable internal pull-up for the selected I2C pins
            //

            //
            // Set qualification for the selected I2C pins
            //

            //
            // Configure which of the possible GPIO pins will be I2C_A pins
            // using GPIO regs
            //
            break;

        case I2C_A_GPIO91_GPIO92:
            //
            // Enable internal pull-up for the selected I2C pins
            //
            GpioCtrlRegs.GPCPUD.bit.GPIO91 = 0;
            GpioCtrlRegs.GPCPUD.bit.GPIO92 = 0;

            //
            // Set qualification for the selected I2C pins
            //
               GpioCtrlRegs.GPCQSEL2.bit.GPIO91 = 3;
               GpioCtrlRegs.GPCQSEL2.bit.GPIO92 = 3;

            //
            // Configure which of the possible GPIO pins will be I2C_A pins
            // using GPIO regs
            //
               GpioCtrlRegs.GPCGMUX2.bit.GPIO91 = 1;
            GpioCtrlRegs.GPCMUX2.bit.GPIO91 = 2;
               GpioCtrlRegs.GPCGMUX2.bit.GPIO92 = 1;
            GpioCtrlRegs.GPCMUX2.bit.GPIO92 = 2;
            break;

        case I2C_A_GPIO63104_GPIO105:
            //
            // Enable internal pull-up for the selected I2C pins
            //

            //
            // Set qualification for the selected I2C pins
            //

            //
            // Configure which of the possible GPIO pins will be I2C_A pins
            // using GPIO regs
            //
            break;

        default:

            break;

    } // End of Switch
    EDIS;
}



void InitI2cGpio()
{
	/*
    GPIO_SetupPinMux(32, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(33, GPIO_OUTPUT, GPIO_PULLUP | GPIO_ASYNC);
    GPIO_SetupPinOptions(32, GPIO_OUTPUT, GPIO_PULLUP | GPIO_ASYNC);
    */
    I2cAGpioConfig(I2C_A_GPIO32_GPIO33);
}

//
// I2C_Init - Initialize I2C settings
//

void InitI2c()
{

    I2caRegs.I2CSAR.all = 0x0050;       // Slave address - RTC control code

    // 200M / (I2CPSC+1)((I2CCLKL+5)+(I2CCLKH+5)) = 200M / 25*20 = 400k
    I2caRegs.I2CPSC.all = 24;		    // Prescaler - need 7-12 Mhz on module clk   200/(24+1) = 8MHz
    I2caRegs.I2CCLKL = 6;               // NOTE: must be non zero(6+5)*1/8m=1.375ns
    I2caRegs.I2CCLKH = 4;               // NOTE: must be non zero
    //I2caRegs.I2CIER.all = 0x24;       // Enable SCD & ARDY __interrupts

    // Stop I2C when suspended
    I2caRegs.I2CFFTX.all = 0x6000;      // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2000;      // Enable RXFIFO, clear RXFFINT,
    
    I2caRegs.I2CMDR.all = 0x0020;       // Take I2C out of reset
    
    
}


#endif


//
// End of file
//
