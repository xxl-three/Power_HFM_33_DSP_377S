;//###########################################################################
;//
;// FILE:  F2837xS_CodeStartBranch.asm
;//
;// TITLE: Branch for redirecting code execution after boot.
;//
;// For these examples, code_start is the first code that is executed after
;// exiting the boot ROM code.
;//
;// The codestart section in the linker cmd file is used to physically place
;// this code at the correct memory location.  This section should be placed
;// at the location the BOOT ROM will re-direct the code to.  For example,
;// for boot to FLASH this code will be located at 0x3f7ff6.
;//
;// In addition, the example F2837xS projects are setup such that the codegen
;// entry point is also set to the code_start label.  This is done by linker
;// option -e in the project build options.  When the debugger loads the code,
;// it will automatically set the PC to the "entry point" address indicated by
;// the -e linker option.  In this case the debugger is simply assigning the PC,
;// it is not the same as a full reset of the device.
;//
;// The compiler may warn that the entry point for the project is other then
;//  _c_init00.  _c_init00 is the C environment setup and is run before
;// main() is entered. The code_start code will re-direct the execution
;// to _c_init00 and thus there is no worry and this warning can be ignored.
;//
;//###########################################################################
;// $TI Release: F2837xS Support Library v210 $
;// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
;// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
;//             http://www.ti.com/ ALL RIGHTS RESERVED $
;//###########################################################################

***********************************************************************

WD_DISABLE  .set  1    ;set to 1 to disable WD, else set to 0

    .ref _c_int00
    .ref _RamClear            ; for jump to bootloader
    .ref _BootAppEntry
    .ref _main  
    .global code_start        
    .global  __stack
    .global  cinit

;***********************************************************************
;* Function: codestart section
;*
;* Description: Branch to code starting point
;***********************************************************************

    .sect "codestart"

code_start:
    .if WD_DISABLE == 1
       LB wd_disable       ;Branch to watchdog disable code
    .else
        LB _c_int00         ;Branch to start of boot._asm in RTS library
    .endif

;end codestart section

;***********************************************************************
;* Function: wd_disable
;*
;* Description: Disables the watchdog timer
;***********************************************************************
    .if WD_DISABLE == 1

;    .text
    .sect "BOOTLOAD_APP"
wd_disable:
    SETC     OBJMODE        ;Set OBJMODE for 28x object code
    EALLOW                  ;Enable EALLOW protected register access
    MOVZ     DP, #7029h>>6  ;Set data page for WDCR register
    MOV      @7029h, #0068h ;Set WDDIS bit in WDCR to disable WD
    EDIS                    ;Disable EALLOW protected register access
    ;LB _c_int00            ;Branch to start of boot._asm in RTS library
    LB _BootAppEntry
    
    
    ADDB     SP, #4      
    MOVL     XAR4, #0x00E000
    MOVL     *-SP[2], XAR4  
             
    MOV      *-SP[3], #0     
    CMP      *-SP[3], #0x7000 
    SB       C$L2, HIS       
             
C$L1:        
    MOVL     XAR4, *-SP[2]  
    MOVZ     AR0, *-SP[3]   
    MOV      *+XAR4[AR0], #0
    INC      *-SP[3]         
    CMP      *-SP[3], #0x7000 
    SB       C$L1, LO        
             
C$L2:        
    SUBB     SP, #4     
    
	LB _c_int00            ;Branch to start of boot._asm in RTS library
	;LB _main            ;Branch to start of boot.asm in RTS library

    .endif

;end wd_disable

    .end

;//
;// End of file.
;//
