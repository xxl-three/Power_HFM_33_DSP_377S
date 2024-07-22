/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : EepromDriver.h

VERSION  : V0.0

DATE     : 2018/6/6

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef EepromDriver_H
#define EepromDriver_H

#include "F28x_Project.h"  // Device Headerfile and Examples Include File
#include "USR_VAR.H"
//-----------------------------------------------------------------------------

#define I2C_MSGSTAT_READ_FAIL         0xF001
#define I2C_MSGSTAT_WRITE_FAIL        0xF010

//-----------------------------------------------------------------------------
//EEPROM slave address def
#define I2C_DEVICE_ADDR                   (0)            // A1 = 0, A2 = 0
#define I2C_SLAVE_ADDR                    ((0x0050 + (I2C_DEVICE_ADDR  << 1)))   //7 bit of slave address

//+ user configurable
#define I2C_ADDR_TWO_BYTE		//for the E2P which with 2 bytes of address; comment for single byte address E2P chip

#define WOrRSuccessDetect

#ifdef I2C_ADDR_TWO_BYTE
#define I2C_ADDR_BYTE    (2)
#else
#define I2C_ADDR_BYTE    (1)
#endif

//+ non user configurable
#define E2P_READ_DATA_BYTE_CNT_PER_CMD   (2)
#define E2P_WRITE_DATA_BYTE_CNT_PER_CMD  (2)


#define E2P_I2C_READ_TIMEOUT_100US	(3 + 1)		//400us add 100us for time tolerance
#define E2P_I2C_WRITE_TIMEOUT_100US (3 + 1)     //wait max 400us add 100us for time tolerance
#define E2P_WRITE_CYCLE_TIME_100US	(60+1)	    //6000us add 100us for time tolerance



#define E2P_DRIVER_SUCCESS	(I2C_TRUE)
#define E2P_DRIVER_FAIL		(I2C_FALSE)

//-----------------------------------------------------------------------------
extern void InitMCU_I2C(void);
extern Uint16 E2P_Read(Uint16 u16Addr, Uint16* pu16Data);
extern Uint16 E2P_Write(Uint16 u16Addr, Uint16 u16Data);

#endif
