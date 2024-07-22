/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : EepromDriver.c

VERSION  : V0.0

DATE     : 2018/6/6 

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/
#include "F28x_Project.h"
#include "E2pDriver.h"
//----------------------------------------------------------------------------

//Uint16	u16WriteTimeOutCnt100us;
 
struct I2CMSG sI2cMsg;

//----------------------------------------------------------------------------
// I2C EEPROM Init
// 
//----------------------------------------------------------------------------
void InitMCU_I2C(void)
{   
    I2caRegs.I2CSAR.bit.SAR = I2C_SLAVE_ADDR;		// Slave address - EEPROM control code
    sI2cMsg.SlaveAddress = I2C_SLAVE_ADDR;
    
	I2caRegs.I2CMDR.bit.IRS = 0;

    // 150M / (I2CPSC+1)((I2CCLKL+5)+(I2CCLKH+5)) = 150M / 15*25 = 400k
    I2caRegs.I2CPSC.all = 14;            // Prescaler - need 7-12 Mhz on module clk   150/(14+1) = 10MHz
    I2caRegs.I2CCLKL = 8;               // NOTE: must be non zero
    I2caRegs.I2CCLKH = 7;               // NOTE: must be non zero
    //I2caRegs.I2CIER.all = 0x24;       // Enable SCD & ARDY __interrupts

    I2caRegs.I2CMDR.all = 0x0020;       // Take I2C out of reset
   									    // Stop I2C when suspended

    I2caRegs.I2CFFTX.all = 0x6000;	    // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2000;	    // Enable RXFIFO, clear RXFFINT,

}

//----------------------------------------------------------------------------
// I2C EEPROM Read
// 
//----------------------------------------------------------------------------
Uint16 IsReadComplete(void)
{
    if (sI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
	{
		I2caRegs.I2CCNT  = sI2cMsg.NumOfBytes;
		
		#ifdef I2C_ADDR_TWO_BYTE			
		I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MemoryHighAddr;
		#endif
		
        I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MemoryLowAddr;
		
		I2caRegs.I2CMDR.all = 0x2620;
		//I2caRegs.I2CMDR.bit.TRX = 1;      // set as dummy transmitter
		//I2caRegs.I2CMDR.bit.STP = 0;      // DO NOT request stop condition
		//I2caRegs.I2CMDR.bit.STT = 1;      // set start of the new frame
		
		sI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;	
	}
	else if (sI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
	{	
	    if (I2caRegs.I2CSTR.bit.SCD)
	    {
	        //stop bit is sent, due to no stop command is nacked
		    I2caRegs.I2CSTR.bit.SCD = 1;
		    sI2cMsg.MsgStatus = I2C_MSGSTAT_READ_FAIL;
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
			
			    sI2cMsg.MsgStatus = I2C_MSGSTAT_READ_BUSY;
		    }
		}
	}
	else if (sI2cMsg.MsgStatus == I2C_MSGSTAT_READ_BUSY)
	{
	    if (I2caRegs.I2CSTR.bit.SCD)
		{
		    I2caRegs.I2CSTR.bit.SCD = 1;	  //clear

            if (I2caRegs.I2CSTR.bit.NACK)    
            {
                I2caRegs.I2CSTR.bit.NACK = 1;
                sI2cMsg.MsgStatus = I2C_MSGSTAT_READ_FAIL;
            }
            else
            {
			    sI2cMsg.MsgBuffer[0] = I2caRegs.I2CDRR.bit.DATA;
			    sI2cMsg.MsgBuffer[1] = I2caRegs.I2CDRR.bit.DATA;

		        sI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
		    }
		} 
	}
	
	if (sI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE || 
	    sI2cMsg.MsgStatus == I2C_MSGSTAT_READ_FAIL)
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
Uint16 IsWriteComplete(void)
{
    if (sI2cMsg.MsgStatus == I2C_MSGSTAT_SEND_WITHSTOP)
	{
        I2caRegs.I2CCNT = E2P_WRITE_DATA_BYTE_CNT_PER_CMD + I2C_ADDR_BYTE;
		#ifdef I2C_ADDR_TWO_BYTE			  
		I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MemoryHighAddr;
		#endif
        I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MemoryLowAddr;
		I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MsgBuffer[0];
		I2caRegs.I2CDXR.bit.DATA = sI2cMsg.MsgBuffer[1];
				
		I2caRegs.I2CMDR.all = 0x6E20;
		//I2caRegs.I2CMDR.bit.TRX = 1;      // set as transmitter
		//I2caRegs.I2CMDR.bit.STP = 1;      // request stop condition
		//I2caRegs.I2CMDR.bit.STT = 1;      // set start of the new frame
		
		sI2cMsg.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
	}
	else if (sI2cMsg.MsgStatus == I2C_MSGSTAT_WRITE_BUSY)
	{
	    //stop bit is sent
	    if (I2caRegs.I2CSTR.bit.SCD)
		{
		    I2caRegs.I2CSTR.bit.SCD = 1;	  //clear
		    
		   	if (I2caRegs.I2CSTR.bit.NACK)
		    {
		        //fail
		        I2caRegs.I2CSTR.bit.NACK = 1;
		        sI2cMsg.MsgStatus = I2C_MSGSTAT_WRITE_FAIL;
		    }
		    else
		    {
		        sI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
		    }
		}
	}
	
	if (sI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE ||
	    sI2cMsg.MsgStatus == I2C_MSGSTAT_WRITE_FAIL)
	{
	    return 1;
	}
	else
	{
	    return 0;
	}
}

//----------------------------------------------------------------------------
// @fn          E2P_IsIdle
//
// @brief       Check if E2P idle for read/write operation
//
// @note        
//
// @return      1: idle
//              0: busy
//----------------------------------------------------------------------------
Uint16 E2P_IsIdle(void)
{	
	//sI2cMsg.SlaveAddress = I2C_SLAVE_ADDR;
	//Does not yet exceed "E2P write cycle time" or I2C is in busy --> not idle
	if (//u16WriteTimeOutCnt100us || 
	    sI2cMsg.MsgStatus != I2C_MSGSTAT_INACTIVE || 
        I2caRegs.I2CSTR.bit.BB)
	{
		return 0;	
	}
	else
	{
		sI2cMsg.SlaveAddress = I2C_SLAVE_ADDR;
		return 1;	
	}
}

//-----------------------------------------------------------------------------
// @fn          E2P_Read
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
Uint16 E2P_Read(Uint16 u16Addr, Uint16* pu16Data)
{
	Uint16 u16Local100us;
	Uint16 u16TimeOutCnt;
	Uint32 u32EEPROMAddr;
	

	if (!E2P_IsIdle())
	{
		return E2P_DRIVER_FAIL;
	}
	else
	{
		u32EEPROMAddr = (Uint32)u16Addr << 1;    // 每个word有两个byte，记忆体位址要两倍
		
    	
		if (u32EEPROMAddr & 0xFFFF0000)   //需要写入第17bit的address，不同的EEPROM，实现方式不一样
		{
		    I2caRegs.I2CSAR.bit.SAR = sI2cMsg.SlaveAddress + 1;		// Slave address
		}
		else
		{
			I2caRegs.I2CSAR.bit.SAR = sI2cMsg.SlaveAddress;
		}

    	sI2cMsg.NumOfBytes = I2C_ADDR_BYTE;
		#ifdef I2C_ADDR_TWO_BYTE	
		sI2cMsg.MemoryHighAddr = ((u32EEPROMAddr & 0xFFFF) >> 8);
		#endif
		sI2cMsg.MemoryLowAddr = (u32EEPROMAddr & 0x00FF);

    	sI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP; 
    	
		//init local time & set read time out counter
		u16Local100us = u16MicroSec;
		u16TimeOutCnt = E2P_I2C_READ_TIMEOUT_100US;
		
    	while (!IsReadComplete())
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
    	}
    	
    	//return
    	if (sI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE)
    	{
    	    Uint16   u16Data;
    	    
    	    //return read data
    	    u16Data = ((Uint16)sI2cMsg.MsgBuffer[0] << 8);
    	    u16Data += sI2cMsg.MsgBuffer[1];
    	    *pu16Data = u16Data;
    	    
    		return E2P_DRIVER_SUCCESS;
    	}
    	else if (sI2cMsg.MsgStatus == I2C_MSGSTAT_READ_FAIL)
    	{
    	    sI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    	    return E2P_DRIVER_FAIL;
    	}
    	else
    	{
    		//time out --> to reinit I2C
    		
			sI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    		return E2P_DRIVER_FAIL;
    	}
    }
}
//-----------------------------------------------------------------------------
// @fn          E2P_Write
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
Uint16 E2P_Write(Uint16 u16Addr,Uint16 u16Data)
{
    Uint16 u16Local100us;
	Uint16 u16TimeOutCnt;
	Uint32 u32EEPROMAddr;
	
    if (!E2P_IsIdle())
    {
        return E2P_DRIVER_FAIL;
    }
	else
	{
		u32EEPROMAddr = (Uint32)u16Addr << 1;    // 每个word有两个byte，记忆体位址要两倍

		if (u32EEPROMAddr & 0xFFFF0000)   //需要写入第17bit的address，不同的EEPROM，实现方式不一样
		{
		    I2caRegs.I2CSAR.bit.SAR = sI2cMsg.SlaveAddress + 1;		// Slave address
		}
		else
		{
			I2caRegs.I2CSAR.bit.SAR = sI2cMsg.SlaveAddress;
		}

		#ifdef I2C_ADDR_TWO_BYTE	
		sI2cMsg.MemoryHighAddr = ((u32EEPROMAddr & 0xFFFF) >> 8);
		#endif
		sI2cMsg.MemoryLowAddr = (u32EEPROMAddr & 0x00FF);
	
		sI2cMsg.MsgBuffer[0] = (u16Data >> 8);
		sI2cMsg.MsgBuffer[1] = (u16Data & 0x00FF);
		
		//set write cycle time
		//u16WriteTimeOutCnt100us = E2P_WRITE_CYCLE_TIME_100US;
		sI2cMsg.MsgStatus = I2C_MSGSTAT_SEND_WITHSTOP;
		
	    //init local time & set read time out counter
		u16Local100us = u16MicroSec;
		u16TimeOutCnt = E2P_I2C_WRITE_TIMEOUT_100US;
		
    	while (!IsWriteComplete())
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
    	}
    	
    	//return
    	if (sI2cMsg.MsgStatus == I2C_MSGSTAT_INACTIVE)
    	{
    	    return E2P_DRIVER_SUCCESS;
    	}
    	else
    	{
    	    sI2cMsg.MsgStatus = I2C_MSGSTAT_INACTIVE;
    	    return E2P_DRIVER_FAIL;
    	}
	}
}
