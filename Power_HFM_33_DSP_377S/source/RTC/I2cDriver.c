/*****************************************************************************

PROJECT  : RT_5~10KVA

COMPILER : Code Composer Studio V7.1.0.00016

MODULE   : I2cDriver.c

VERSION  : V0.0

DATE     : 2018/4/13 

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/

#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "I2cDriver.h"
//----------------------------------------------------------------------------

//Uint16	u16WriteTimeOutCnt100us;
 
struct I2CMSG sRtcI2cMsg;

//----------------------------------------------------------------------------
// I2C EEPROM Read
// 
//----------------------------------------------------------------------------
Uint16 IsRtcReadComplete(void)
{
    if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
	{
		I2caRegs.I2CCNT  = sRtcI2cMsg.NumOfBytes;
			
        I2caRegs.I2CDXR.bit.DATA = sRtcI2cMsg.MemoryLowAddr;
		
		I2caRegs.I2CMDR.all = 0x2620;
		//I2caRegs.I2CMDR.bit.TRX = 1;      // set as dummy transmitter
		//I2caRegs.I2CMDR.bit.STP = 0;      // DO NOT request stop condition
		//I2caRegs.I2CMDR.bit.STT = 1;      // set start of the new frame
		
		sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;	
	}
	else if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
	{	
	    if (I2caRegs.I2CSTR.bit.SCD)
	    {
	        //stop bit is sent, due to no stop command is nacked
		    I2caRegs.I2CSTR.bit.SCD = 1;
		    sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_READ_FAIL;
		} 
        else if (I2caRegs.I2CSTR.bit.ARDY)    // register access ready
	    {
		    //此状况代表read指令被NACK(之前的写入动作还未完成)
	        //因此发出一个STOP指令结束这次读的command
	        if (I2caRegs.I2CSTR.bit.NACK)    
		    {
			    I2caRegs.I2CMDR.bit.STP = 1;      // request stop condition
		        I2caRegs.I2CSTR.bit.NACK = 1;
		    }
		    else
		    {
			    I2caRegs.I2CCNT = E2P_READ_DATA_BYTE_CNT_PER_CMD;			  //read 2 bytes

			    I2caRegs.I2CMDR.all = 0x2C20;	
		        //I2caRegs.I2CMDR.bit.TRX = 0;    // set as receiver
			    //I2caRegs.I2CMDR.bit.STP = 1;
		        //I2caRegs.I2CMDR.bit.STT = 1;    // set start of the new frame
			
			    sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_READ_BUSY;
		    }
		}
	}
	else if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_READ_BUSY)
	{
	    if (I2caRegs.I2CSTR.bit.SCD)
		{
		    I2caRegs.I2CSTR.bit.SCD = 1;	  //clear

            if (I2caRegs.I2CSTR.bit.NACK)    
            {
                I2caRegs.I2CSTR.bit.NACK = 1;
                sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_READ_FAIL;
            }
            else
            {
			    sRtcI2cMsg.MsgBuffer[0] = I2caRegs.I2CDRR.bit.DATA;
			    //sRtcI2cMsg.MsgBuffer[1] = I2caRegs.I2CDRR.bit.DATA;

		        sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
		    }
		} 
	}
	
	if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE || 
	    sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_READ_FAIL)
	{
	    return 1;
	}
	else
	{
	    return 0;
	}
}

//----------------------------------------------------------------------------
// I2C EEPROM Write
// 
//----------------------------------------------------------------------------
Uint16 IsRtcWriteComplete(void)
{
    if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_WITHSTOP)
	{
        I2caRegs.I2CCNT = E2P_WRITE_DATA_BYTE_CNT_PER_CMD + I2C_ADDR_BYTE;
        I2caRegs.I2CDXR.bit.DATA = sRtcI2cMsg.MemoryLowAddr;
		I2caRegs.I2CDXR.bit.DATA = sRtcI2cMsg.MsgBuffer[0];
				
		I2caRegs.I2CMDR.all = 0x6E20;
		//I2caRegs.I2CMDR.bit.TRX = 1;      // set as transmitter
		//I2caRegs.I2CMDR.bit.STP = 1;      // request stop condition
		//I2caRegs.I2CMDR.bit.STT = 1;      // set start of the new frame
		
		sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
	}
	else if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_WRITE_BUSY)
	{
	    //stop bit is sent
	    if (I2caRegs.I2CSTR.bit.SCD)
		{
		    I2caRegs.I2CSTR.bit.SCD = 1;	  //clear
		    
		   	if (I2caRegs.I2CSTR.bit.NACK)
		    {
		        //fail
		        I2caRegs.I2CSTR.bit.NACK = 1;
		        sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_WRITE_FAIL;
		    }
		    else
		    {
		        sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
		    }
		}
	}
	
	if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE ||
	    sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_WRITE_FAIL)
	{
	    return 1;
	}
	else
	{
	    return 0;
	}
}

//----------------------------------------------------------------------------
// @fn          I2C_IsIdle
//
// @brief       Check if E2P idle for read/write operation
//
// @note        
//
// @return      1: idle
//              0: busy
//----------------------------------------------------------------------------
Uint16 I2C_IsIdle(void)
{	
	//sRtcI2cMsg.SlaveAddress = I2C_SLAVE_ADDR;
	//Does not yet exceed "E2P write cycle time" or I2C is in busy --> not idle
	if (//u16WriteTimeOutCnt100us || 
	    sRtcI2cMsg.MsgStatus != I2C_MSGSTAT_INACTIVE || 
        I2caRegs.I2CSTR.bit.BB)
	{
		return 0;	
	}
	else
	{
		sRtcI2cMsg.SlaveAddress = I2C_SLAVE_ADDR;
		return 1;	
	}
}

//-----------------------------------------------------------------------------
// @fn          I2C_Read
//
// @brief       To read data from EEPROM
//
// @note        必须于timebase interrupt enable后才能于main routine呼叫
//              Cant' be called from ISR
// @param[in]   *ptE2PDriverObj     pointer of EEPROM driver obj
//
// @param[in]   u16Addr             MCU register struct pointer       
//
// @param[in]   puData              the pointer to read data
//
// @return      success or fail of read operation
//              E2P_DRIVER_SUCCESS
//              E2P_DRIVER_FAIL
//-----------------------------------------------------------------------------
Uint16 I2C_Read(Uint16 u16Addr, Uint16* pu16Data)
{
	Uint16 u16Local100us;
	Uint16 u16TimeOutCnt;
	Uint32 u32EEPROMAddr;
	

	if (!I2C_IsIdle())
	{
		return E2P_DRIVER_FAIL;
	}
	else
	{
		I2caRegs.I2CSAR.bit.SAR = sRtcI2cMsg.SlaveAddress;

    	sRtcI2cMsg.NumOfBytes = I2C_ADDR_BYTE;
		sRtcI2cMsg.MemoryLowAddr = (u16Addr & 0x00FF);

    	sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP; 
    	
		//init local time & set read time out counter
		u16Local100us = u16MicroSec;
		u16TimeOutCnt = E2P_I2C_READ_TIMEOUT_100US;
		
    	while (!IsRtcReadComplete())
    	{
    	    //time out
       		if (u16Local100us != u16MicroSec)
       		{
       			u16Local100us = u16MicroSec;
       			if (u16TimeOutCnt)
       			{
       				u16TimeOutCnt--;
       				
       				if (u16TimeOutCnt == 0)
       				{
       					break;
       				}
       			}
       		}
       		
       		SetWDGClock();
    	}
    	
    	//return
    	if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE)
    	{
    	    //return read data
    	    *pu16Data = ((Uint16)sRtcI2cMsg.MsgBuffer[0]);
    	    
    		return E2P_DRIVER_SUCCESS;
    	}
    	else if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_READ_FAIL)
    	{
    	    sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    	    return E2P_DRIVER_FAIL;
    	}
    	else
    	{
    		//time out --> to reinit I2C
    		
			sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    		return E2P_DRIVER_FAIL;
    	}
    }
}
//-----------------------------------------------------------------------------
// @fn          I2C_Write
//
// @brief       To write data to EEPROM
//
// @note        必须于timebase interrupt enable后才能于main routine呼叫
//              Cant' be called from ISR
// @param[in]   *ptE2PDriverObj     pointer of EEPROM driver obj
//
// @param[in]   u16Addr             MCU register struct pointer       
//
// @param[in]   u16Data             data to be writtern
//
// @return      success or fail of write operation
//              E2P_DRIVER_SUCCESS
//              E2P_DRIVER_FAIL
//-----------------------------------------------------------------------------
Uint16 I2C_Write(Uint16 u16Addr,Uint16 u16Data)
{
    Uint16 u16Local100us;
	Uint16 u16TimeOutCnt;
	
    if (!I2C_IsIdle())
    {
        return E2P_DRIVER_FAIL;
    }
	else
	{
		I2caRegs.I2CSAR.bit.SAR = sRtcI2cMsg.SlaveAddress;

		sRtcI2cMsg.MemoryLowAddr = (u16Addr & 0x00FF);
	
		sRtcI2cMsg.MsgBuffer[0] = (u16Data & 0x00FF);
		
		//set write cycle time
		//u16WriteTimeOutCnt100us = E2P_WRITE_CYCLE_TIME_100US;
		sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_WITHSTOP;
		
	    //init local time & set read time out counter
		u16Local100us = u16MicroSec;
		u16TimeOutCnt = E2P_I2C_WRITE_TIMEOUT_100US;
		
    	while (!IsRtcWriteComplete())
    	{
    	    //time out
       		if (u16Local100us != u16MicroSec)
       		{
       			u16Local100us = u16MicroSec;
       			if (u16TimeOutCnt)
       			{
       				u16TimeOutCnt--;
       				
       				if (u16TimeOutCnt == 0)
       				{
       					break;
       				}
       			}
       		}
       		
       		SetWDGClock();
    	}
    	
    	//return
    	if (sRtcI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE)
    	{
    	    return E2P_DRIVER_SUCCESS;
    	}
    	else
    	{
    	    sRtcI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    	    return E2P_DRIVER_FAIL;
    	}
	}
}
