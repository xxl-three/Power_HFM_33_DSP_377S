//###########################################################################
//
// FILE:   F2837xS_device.h
//
// TITLE:  F2837xS Device Definitions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 (Patch) $
// $Release Date: March 3 2017 $
// $Copyright: Copyright (C) 2014-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef F2837xS_DEVICE_H
#define F2837xS_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define _FLASH    // for SysCtrl to initialize flash and run on RAM
#define   CPU1  1    // define which CPU

#define   TARGET   1

//
// User To Select Target Device:
//
#define   F28_2837xS    TARGET

//
// Common CPU Definitions:
//
extern __cregister volatile unsigned int IFR;
extern __cregister volatile unsigned int IER;

#define  EINT   __asm(" clrc INTM")
#define  DINT   __asm(" setc INTM")
#define  ERTM   __asm(" clrc DBGM")
#define  DRTM   __asm(" setc DBGM")
#ifndef  EALLOW
#define  EALLOW __asm(" EALLOW")
#endif
#ifndef  EDIS
#define  EDIS   __asm(" EDIS")
#endif
#define  ESTOP0 __asm(" ESTOP0")

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#ifndef C28X_BIT0
#define C28X_BIT0    0x00000001
#endif

#ifndef C28X_BIT1
#define C28X_BIT1    0x00000002
#endif

#ifndef C28X_BIT2
#define C28X_BIT2    0x00000004
#endif

#ifndef C28X_BIT3
#define C28X_BIT3    0x00000008
#endif

#ifndef C28X_BIT4
#define C28X_BIT4    0x00000010
#endif

#ifndef C28X_BIT5
#define C28X_BIT5    0x00000020
#endif

#ifndef C28X_BIT6
#define C28X_BIT6    0x00000040
#endif

#ifndef C28X_BIT7
#define C28X_BIT7    0x00000080
#endif

#ifndef C28X_BIT8
#define C28X_BIT8    0x00000100
#endif

#ifndef C28X_BIT9
#define C28X_BIT9    0x00000200
#endif

#ifndef C28X_BIT10
#define C28X_BIT10   0x00000400
#endif

#ifndef C28X_BIT11
#define C28X_BIT11   0x00000800
#endif

#ifndef C28X_BIT12
#define C28X_BIT12   0x00001000
#endif

#ifndef C28X_BIT13
#define C28X_BIT13   0x00002000
#endif

#ifndef C28X_BIT14
#define C28X_BIT14   0x00004000
#endif

#ifndef C28X_BIT15
#define C28X_BIT15   0x00008000
#endif

#ifndef C28X_BIT16
#define C28X_BIT16   0x00010000
#endif

#ifndef C28X_BIT17
#define C28X_BIT17   0x00020000
#endif

#ifndef C28X_BIT18
#define C28X_BIT18   0x00040000
#endif

#ifndef C28X_BIT19
#define C28X_BIT19   0x00080000
#endif

#ifndef C28X_BIT20
#define C28X_BIT20   0x00100000
#endif

#ifndef C28X_BIT21
#define C28X_BIT21   0x00200000
#endif

#ifndef C28X_BIT22
#define C28X_BIT22   0x00400000
#endif

#ifndef C28X_BIT23
#define C28X_BIT23   0x00800000
#endif

#ifndef C28X_BIT24
#define C28X_BIT24   0x01000000
#endif

#ifndef C28X_BIT25
#define C28X_BIT25   0x02000000
#endif

#ifndef C28X_BIT26
#define C28X_BIT26   0x04000000
#endif

#ifndef C28X_BIT27
#define C28X_BIT27   0x08000000
#endif

#ifndef C28X_BIT28
#define C28X_BIT28   0x10000000
#endif

#ifndef C28X_BIT29
#define C28X_BIT29   0x20000000
#endif

#ifndef C28X_BIT30
#define C28X_BIT30   0x40000000
#endif

#ifndef C28X_BIT31
#define C28X_BIT31   0x80000000
#endif

//
// For Portability, User Is Recommended To Use the C99 Standard integer types
//
#if !defined(__TMS320C28XX_CLA__)
#include <assert.h>
#include <stdarg.h>
#endif //__TMS320C28XX_CLA__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//
// C++ Bool Compatibility
//
#if defined(__cplusplus)
typedef bool _Bool;
#endif

//
// C99 defines boolean type to be _Bool, but this doesn't match the format of
// the other standard integer types.  bool_t has been defined to fill this gap.
//
typedef _Bool bool_t;

//
//used for a bool function return status
//
typedef _Bool status_t;

#ifndef SUCCESS
#define SUCCESS     true
#endif

#ifndef FAIL
#define FAIL        false
#endif

//
// The following data types are included for compatibility with legacy code,
// they are not recommended for use in new software.  Please use the C99
// types included above
//
#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef char                int8;
typedef unsigned char       Uint8;
typedef int             	int16;
typedef long            	int32;
typedef long long			int64;
typedef unsigned int    	Uint16;
typedef unsigned long   	Uint32;
typedef unsigned long long	Uint64;
typedef float           	float32;
typedef long double     	float64;

//unsign
typedef union
{
    Uint16 u16Word;
    struct hilow_usbyte
    {
        Uint16  u8LowByte         : 8;    // Bit  0~7:
        Uint16  u8HiByte          : 8;    // Bit  8~15:
    }sByte;
}nu16HiLoByte_t;

//sign
typedef union
{
    int32 i32DbWord;
    struct hilow_word
    {
        int16 i16LowWord;
        int16 i16HiWord;
    }sWord;
}ni32HiLoWord_t;

//unsign
typedef union
{
    Uint32 u32DbWord;
    struct hilow_usword
    {
        Uint16 u16LowWord;
        Uint16 u16HiWord;
    }sWord;
}nu32HiLoWord_t;
#endif

//
// The following data types are for use with byte addressable peripherals.
// See compiler documentation on the byte_peripheral type attribute.
//
#ifndef __TMS320C28XX_CLA__
#if __TI_COMPILER_VERSION__ >= 16006000
typedef unsigned int bp_16 __attribute__((byte_peripheral));
typedef unsigned long bp_32 __attribute__((byte_peripheral));
#endif
#endif

//
// Include All Peripheral Header Files:
//
#include "F2837xS_adc.h"
#include "F2837xS_analogsubsys.h"
#include "F2837xS_cla.h"
#include "F2837xS_cmpss.h"
#include "F2837xS_cputimer.h"
#include "F2837xS_dac.h"
#include "F2837xS_dcsm.h"
#include "F2837xS_dma.h"
#include "F2837xS_ecap.h"
#include "F2837xS_emif.h"
#include "F2837xS_epwm.h"                // Enhanced PWM
#include "F2837xS_epwm_xbar.h"
#include "F2837xS_eqep.h"
#include "F2837xS_flash.h"
#include "F2837xS_gpio.h"                // General Purpose I/O Registers
#include "F2837xS_i2c.h"
#include "F2837xS_input_xbar.h"
#include "F2837xS_mcbsp.h"
#include "F2837xS_memconfig.h"
#include "F2837xS_nmiintrupt.h"          // NMI Interrupt Registers
#include "F2837xS_output_xbar.h"
#include "F2837xS_piectrl.h"             // PIE Control Registers
#include "F2837xS_pievect.h"
#include "F2837xS_sci.h"
#include "F2837xS_sdfm.h"
#include "F2837xS_spi.h"
#include "F2837xS_sysctrl.h"             // System Control/Power Modes
#include "F2837xS_upp.h"
#include "F2837xS_xbar.h"
#include "F2837xS_xint.h"                // External Interrupts

//
// byte_peripheral attribute is only supported on the C28
//
#ifndef __TMS320C28XX_CLA__
#if __TI_COMPILER_VERSION__ >= 16006000
#include "F2837xS_can.h"
#endif
#endif


#ifdef __cplusplus
}
#endif                                  // extern "C"

#endif                                  // end of F2837xS_DEVICE_H definition

//
// End of file.
//
