/*****************************************************************************

PROJECT  : Source

COMPILER : Code Composer Studio V8.2.0.00007

MODULE   : main.c

VERSION  : V0.0

DATE     : 2018/10/05

AUTHOR   : ZHOU;

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/

#include "F28x_Project.h"     // F28x Project Headerfile and Examples Include File
#include "mainUPS.h"
#include "USR_VAR.H"
#include "Booter.h"


/****************************************************************************
 * @fn          MainEntry
 *
 * @brief       Main entry
 *
 * @note        Located in bootloader area
****************************************************************************/
#pragma CODE_SECTION(BootAppEntry, "BOOTLOAD_MAIN");
void BootAppEntry(void)
{
    Uint16 data, u16Index, u16TmpCnt;
    Uint16 *flash;
    
    EALLOW;
    //DcsmCommonRegs.FLSEM.all = 0x0A503;
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;
    Flash0CtrlRegs.FBAC.all = 0x14;
    //Flash0CtrlRegs.FRD_INTF_CTRL.all = 0x3;
    EDIS;

	INIT_PIM_Boot();

	flash = (Uint16*)&BootPassWord;	    // Boot Password address
    data = *flash;

    //u16TmpCnt = 0;
    //for(u16Index = 0; u16Index < 10; u16Index++)
    //{
    //    if(CHK_KEY_ESC && CHK_KEY_ENTER)	// CHK_KEY_ON
    //    {
	//		u16TmpCnt++;
    //    }
    //}
    
    if(u16TmpCnt == 10)
    {
        data = 0;					// let the UPS enter into boot mode
    }

    if(data == BOOT_PASSWORD)		// Password ok ? 0x55AA
    {
        UserAppEntry();				// UPS main
    }
    else
    {
        InitSysCtrl_Boot();			// init System Control registers

        INIT_PIM_Boot();			// init Port
        InitSCI9600();				// set 9600 Baud
        
        Bloader();					// Boot Loader main
    }        
}

