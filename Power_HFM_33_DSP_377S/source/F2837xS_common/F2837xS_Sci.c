//###########################################################################
//
// FILE:   F2837xS_Sci.c
//
// TITLE:  F2837xS SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File
#include "USR_VAR.h"

//---------------------------------------------------------------------------
// InitSci:
// This function initializes the SCI(s) to a known state.
//---------------------------------------------------------------------------
void InitSci(void)
{
    //scia
    SciaRegs.SCICCR.all = 0x0007;	// 1 stop bit,  No loopback  (8bit data) parity disable

    SciaRegs.SCICTL1.all = 0x0003;	// enable TX, RX,
    // Disable RX ERR, TXWAKE, SLEEP
    SciaRegs.SCICTL2.all = 0x0000;	// default

    //SciaRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR(baud) = 50M / (2400*8) - 1 = 2603
    //SciaRegs.SCILBAUD.bit.BAUD = 0x002B;
    SciaRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
    SciaRegs.SCILBAUD.bit.BAUD = 0x008A;
    //SciaRegs->SCIHBAUD.bit.BAUD = 0x0001;		// 19200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (19200*8) - 1 = 324
    //SciaRegs->SCILBAUD.bit.BAUD = 0x0044;
    //SciaRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 115200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (115200*8) - 1 = 53
    //SciaRegs.SCILBAUD.bit.BAUD = 0x0035;
    
    //-> Initialize SCI FIFO
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x204f;
    SciaRegs.SCIFFCT.all = 0x0;
    
    SciaRegs.SCICTL1.all = 0x0023;	// Relinquish SCI from Reset


    //scib
    ScibRegs.SCICCR.all = 0x0007;	// 1 stop bit,  No loopback

    ScibRegs.SCICTL1.all = 0x0003;	// enable TX, RX,
    // Disable RX ERR, TXWAKE, SLEEP
    ScibRegs.SCICTL2.all = 0x0000;	// default

    //ScibRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (2400*8) - 1 = 2603
    //ScibRegs.SCILBAUD.bit.BAUD = 0x002B;
    ScibRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
    ScibRegs.SCILBAUD.bit.BAUD = 0x008A;
    //ScibRegs->SCIHBAUD.bit.BAUD = 0x0001;		// 19200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (19200*8) - 1 = 324
    //ScibRegs->SCILBAUD.bit.BAUD = 0x0044;
    //ScibRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 115200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (115200*8) - 1 = 53
    //ScibRegs.SCILBAUD.bit.BAUD = 0x0035;
    
    //-> Initialize SCI FIFO
    ScibRegs.SCIFFTX.all = 0xE040;
    ScibRegs.SCIFFRX.all = 0x204f;
    ScibRegs.SCIFFCT.all = 0x0;
    
    ScibRegs.SCICTL1.all = 0x0023;	// Relinquish SCI from Reset

    //scic
    ScicRegs.SCICCR.all = 0x0007;	// 1 stop bit,  No loopback

    ScicRegs.SCICTL1.all = 0x0003;	// enable TX, RX,
    // Disable RX ERR, TXWAKE, SLEEP
    ScicRegs.SCICTL2.all = 0x0000;	// default

    //ScicRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (2400*8) - 1 = 2603
    //ScicRegs.SCILBAUD.bit.BAUD = 0x002B;
    ScicRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
    ScicRegs.SCILBAUD.bit.BAUD = 0x008A;
    //ScicRegs->SCIHBAUD.bit.BAUD = 0x0001;		// 19200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (19200*8) - 1 = 324
    //ScicRegs->SCILBAUD.bit.BAUD = 0x0044;
    //ScicRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 115200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (115200*8) - 1 = 53
    //ScicRegs.SCILBAUD.bit.BAUD = 0x0035;
    
    //-> Initialize SCI FIFO
    ScicRegs.SCIFFTX.all = 0xE040;
    ScicRegs.SCIFFRX.all = 0x204f;
    ScicRegs.SCIFFCT.all = 0x0;
    
    ScicRegs.SCICTL1.all = 0x0023;	// Relinquish SCI from Reset

    //scid
    ScidRegs.SCICCR.all = 0x0007;	// 1 stop bit,  No loopback

    ScidRegs.SCICTL1.all = 0x0003;	// enable TX, RX,
    // Disable RX ERR, TXWAKE, SLEEP
    ScidRegs.SCICTL2.all = 0x0000;	// default

    //ScidRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (2400*8) - 1 = 2603
    //ScidRegs.SCILBAUD.bit.BAUD = 0x002B;
    ScidRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
    ScidRegs.SCILBAUD.bit.BAUD = 0x008A;
    //ScidRegs->SCIHBAUD.bit.BAUD = 0x0001;		// 19200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (19200*8) - 1 = 324
    //ScidRegs->SCILBAUD.bit.BAUD = 0x0044;
    //ScidRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 115200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (115200*8) - 1 = 53
    //ScidRegs.SCILBAUD.bit.BAUD = 0x0035;
    
    //-> Initialize SCI FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;
    
    ScidRegs.SCICTL1.all = 0x0023;	// Relinquish SCI from Reset
    
    

}

void InitSciGpio()
{
    InitSciaGpio();
    InitScicGpio();
    InitScidGpio();
}

void InitSciaGpio()
{
    /*   EALLOW;

    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.
    // Set qualification for selected pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.

        GpioCtrlRegs.GPEQSEL1.bit.GPIO136 = 3;
        GpioCtrlRegs.GPEGMUX1.bit.GPIO135 = 1;
    	GpioCtrlRegs.GPEMUX1.bit.GPIO135 = 1;
    	GpioCtrlRegs.GPEGMUX1.bit.GPIO136 = 1;
    	GpioCtrlRegs.GPEMUX1.bit.GPIO136 = 1;
        GpioCtrlRegs.GPEDIR.bit.GPIO135 =1;
        GpioCtrlRegs.GPEDIR.bit.GPIO136 =0;

        EDIS;
    */
    GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15);
    GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_ASYNC | GPIO_OPENDRAIN);
    GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15);
    GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_PULLUP);

}

void InitScicGpio()
{
    /*   EALLOW;

    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //SNMP
        GpioCtrlRegs.GPEQSEL1.bit.GPIO139 = 3;
        GpioCtrlRegs.GPEGMUX1.bit.GPIO139 = 1;
    	GpioCtrlRegs.GPEMUX1.bit.GPIO139 = 1;
    	GpioCtrlRegs.GPEGMUX1.bit.GPIO140 = 1;
    	GpioCtrlRegs.GPEMUX1.bit.GPIO140 = 1;
        GpioCtrlRegs.GPEDIR.bit.GPIO139 =0;
        GpioCtrlRegs.GPEDIR.bit.GPIO140 =1;

        EDIS;
     */
    GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_ASYNC | GPIO_OPENDRAIN);
    GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PULLUP);
}

void InitScidGpio()
{
    /*  EALLOW;
    //MINICARD
    // Enable internal pull-up for the selected pins //
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.

    //  GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
    //  GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;    // Enable pull-up for GPIO63 (SCITXDC)

    // Set qualification for selected pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.

       GpioCtrlRegs.GPEQSEL1.bit.GPIO138 = 3;
       GpioCtrlRegs.GPEGMUX1.bit.GPIO137 = 1;
    GpioCtrlRegs.GPEMUX1.bit.GPIO137 = 1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO138 = 1;
    GpioCtrlRegs.GPEMUX1.bit.GPIO138 = 1;
       GpioCtrlRegs.GPEDIR.bit.GPIO138 =0;
       GpioCtrlRegs.GPEDIR.bit.GPIO137 =1;

       EDIS;
     */
    GPIO_SetupPinMux(46, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(46, GPIO_INPUT, GPIO_ASYNC | GPIO_OPENDRAIN);
    GPIO_SetupPinMux(47, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(47, GPIO_OUTPUT, GPIO_PULLUP);
}

//----------------------------------------------------------------------------
// Timebase: 100ms
//----------------------------------------------------------------------------
/*void ChangeSciaBaudrate(void)
{
    static Uint16 u16DelayCnt;

    if(sTimesFlag.sBit.u1DelayChangeUBR)
    {
        if(u16DelayCnt < 10)
        {
            u16DelayCnt++;
        }
        else
        {
            switch(sTimesFlag.sBit.u1DelayChangeUBR)
            {
                case 2:
                    SciaRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
                    SciaRegs.SCILBAUD.bit.BAUD = 0x008A;
                    break;

                case 1:
                //Lint: pass through
                default:
                    SciaRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (2400*8) - 1 = 2603
                    SciaRegs.SCILBAUD.bit.BAUD = 0x002B;
                    break;
            }
            
            sTimesFlag.sBit.u1DelayChangeUBR = 0;
        }
    }
    else
    {
        u16DelayCnt = 0;
    }


}*/

// End of file
//
