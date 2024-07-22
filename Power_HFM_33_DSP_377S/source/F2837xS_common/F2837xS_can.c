//###########################################################################
//
// FILE:   F2837xS_can.c
//
// TITLE:  F2837xS CAN Support Functions.
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

#define CAN_MSG_ID              0x1111
#define CAN_TX_MSG_OBJ          1
#define CAN_RX_MSG_OBJ          2
#define CAN_MAX_BIT_DIVISOR     (13)   // The maximum CAN bit timing divisor
#define CAN_MIN_BIT_DIVISOR     (5)    // The minimum CAN bit timing divisor
#define CAN_MAX_PRE_DIVISOR     (1024) // The maximum CAN pre-divisor
#define CAN_MIN_PRE_DIVISOR     (1)    // The minimum CAN pre-divisor
#define CAN_BTR_BRP_M           (0x3F)
#define CAN_BTR_BRPE_M          (0xF0000)

//
// Globals
//
unsigned char ucTXMsgData[4] = {0x1, 0x2, 0x3, 0x4}; // TX Data
unsigned char ucRXMsgData[4] = {0, 0, 0, 0};         // RX Data
uint32_t messageSize = sizeof(ucTXMsgData);          // Message Size (DLC)
volatile unsigned long msgCount = 0; // A counter that keeps track of the
                                     // number of times the transmit was
                                     // successful.
volatile unsigned long errFlag = 0;  // A flag to indicate that some
                                     // transmission error occurred.

static const uint16_t canBitValues[] =
{
    0x1100, // TSEG2 2, TSEG1 2, SJW 1, Divide 5
    0x1200, // TSEG2 2, TSEG1 3, SJW 1, Divide 6
    0x2240, // TSEG2 3, TSEG1 3, SJW 2, Divide 7
    0x2340, // TSEG2 3, TSEG1 4, SJW 2, Divide 8
    0x3340, // TSEG2 4, TSEG1 4, SJW 2, Divide 9
    0x3440, // TSEG2 4, TSEG1 5, SJW 2, Divide 10
    0x3540, // TSEG2 4, TSEG1 6, SJW 2, Divide 11
    0x3640, // TSEG2 4, TSEG1 7, SJW 2, Divide 12
    0x3740  // TSEG2 4, TSEG1 8, SJW 2, Divide 13
};

typedef enum
{
        //! Transmit message object.
        MSG_OBJ_TYPE_TRANSMIT,

        //! Receive message object.
        MSG_OBJ_TYPE_RECEIVE
}
msgObjType;

//
// InitCAN - Initializes the CAN-A controller after reset.
//
void InitCAN(void)
{
	int16_t iMsg;

	//
    // Place CAN controller in init state, regardless of previous state.  This
    // will put controller in idle, and allow the message object RAM to be
    // programmed.
	//
	CanaRegs.CAN_CTL.bit.Init = 1;
	CanaRegs.CAN_CTL.bit.SWR = 1;

	//
    // Wait for busy bit to clear
	//
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Clear the message value bit in the arbitration register.  This indicates
    // the message is not valid and is a "safe" condition to leave the message
    // object.  The same arb reg is used to program all the message objects.
    //
    CanaRegs.CAN_IF1CMD.bit.DIR = 1;
    CanaRegs.CAN_IF1CMD.bit.Arb = 1;
    CanaRegs.CAN_IF1CMD.bit.Control = 1;

    CanaRegs.CAN_IF1ARB.all = 0;

    CanaRegs.CAN_IF1MCTL.all = 0;

    CanaRegs.CAN_IF2CMD.bit.DIR = 1;
    CanaRegs.CAN_IF2CMD.bit.Arb = 1;
    CanaRegs.CAN_IF2CMD.bit.Control = 1;

    CanaRegs.CAN_IF2ARB.all = 0;

    CanaRegs.CAN_IF2MCTL.all = 0;

    //
    // Loop through to program all 32 message objects
    //
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        //
    	// Wait for busy bit to clear
    	//
        while(CanaRegs.CAN_IF1CMD.bit.Busy)
        {
        }

        //
        // Initiate programming the message object
        //
        CanaRegs.CAN_IF1CMD.bit.MSG_NUM = iMsg;

        //
        // Wait for busy bit to clear
        //
        while(CanaRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        //
        // Initiate programming the message object
        //
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = iMsg + 1;
    }

    //
    // Acknowledge any pending status interrupts.
    //
    volatile uint32_t discardRead = CanaRegs.CAN_ES.all;
    
/*    
    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.
    // In this example, the CAN bus is set to 500 kHz.
    //
    // Consult the data sheet for more information about
    // CAN peripheral clocking.
    //
    uint32_t status = setCANBitRate(200000000, 500000);
    
    //
    // Enable test mode and select external loopback
    //
    //CanaRegs.CAN_CTL.bit.Test = 1;
    //CanaRegs.CAN_TEST.bit.EXL = 1;

    //
    // Initialize the message object that will be used for sending CAN
    // messages.
    //
    setupMessageObject(CAN_TX_MSG_OBJ, CAN_MSG_ID, MSG_OBJ_TYPE_TRANSMIT);

    //
    // Initialize the message object that will be used for receiving CAN
    // messages.
    //
    setupMessageObject(CAN_RX_MSG_OBJ, CAN_MSG_ID, MSG_OBJ_TYPE_RECEIVE);

    //
    // Enable the CAN for operation.
    //
    CanaRegs.CAN_CTL.bit.Init = 0;
*/
}

void InitCanGpio(void)
{
    //
    // Setup GPIO pin mux for CAN-A TX/RX and CAN-B TX/RX
    //
    GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 6); //GPIO62 -  CANRXA
    GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 6); //GPIO63 - CANTXA
    GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PUSHPULL);  
    
    GPIO_SetupPinMux(39, GPIO_MUX_CPU1, 6); //GPIO39 -  CANRXB
    GPIO_SetupPinOptions(39, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(38, GPIO_MUX_CPU1, 6);  //GPIO38 - CANTXB
    GPIO_SetupPinOptions(38, GPIO_OUTPUT, GPIO_PUSHPULL);  
    
}

//
// setCANBitRate - Set the CAN bit rate based on device clock (Hz)
//                 and desired bit rate (Hz)
//
uint32_t setCANBitRate(uint32_t sourceClock, uint32_t bitRate)
{
    uint32_t desiredRatio;
    uint32_t canBits;
    uint32_t preDivide;
    uint32_t regValue;
    uint16_t canControlValue;

    //
    // Calculate the desired clock rate.
    //
    desiredRatio = sourceClock / bitRate;

    //
    // Make sure that the Desired Ratio is not too large.  This enforces the
    // requirement that the bit rate is larger than requested.
    //
    if((sourceClock / desiredRatio) > bitRate)
    {
        desiredRatio += 1;
    }

    //
    // Check all possible values to find a matching value.
    //
    while(desiredRatio <= CAN_MAX_PRE_DIVISOR * CAN_MAX_BIT_DIVISOR)
    {
        //
        // Loop through all possible CAN bit divisors.
        //
        for(canBits = CAN_MAX_BIT_DIVISOR;
            canBits >= CAN_MIN_BIT_DIVISOR;
            canBits--)
        {
            //
            // For a given CAN bit divisor save the pre divisor.
            //
            preDivide = desiredRatio / canBits;

            //
            // If the calculated divisors match the desired clock ratio then
            // return these bit rate and set the CAN bit timing.
            //
            if((preDivide * canBits) == desiredRatio)
            {
                //
                // Start building the bit timing value by adding the bit timing
                // in time quanta.
                //
                regValue = canBitValues[canBits - CAN_MIN_BIT_DIVISOR];

                //
                // To set the bit timing register, the controller must be
                // placed
                // in init mode (if not already), and also configuration change
                // bit enabled.  The state of the register should be saved
                // so it can be restored.
                //
                canControlValue = CanaRegs.CAN_CTL.all;
                CanaRegs.CAN_CTL.bit.Init = 1;
                CanaRegs.CAN_CTL.bit.CCE = 1;

                //
                // Now add in the pre-scalar on the bit rate.
                //
                regValue |= ((preDivide - 1) & CAN_BTR_BRP_M) |
                            (((preDivide - 1) << 10) & CAN_BTR_BRPE_M);

                //
                // Set the clock bits in the and the bits of the
                // pre-scalar.
                //
                CanaRegs.CAN_BTR.all = regValue;

                //
                // Restore the saved CAN Control register.
                //
                CanaRegs.CAN_CTL.all = canControlValue;

                //
                // Return the computed bit rate.
                //
                return(sourceClock / ( preDivide * canBits));
            }
        }

        //
        // Move the divisor up one and look again.  Only in rare cases are
        // more than 2 loops required to find the value.
        //
        desiredRatio++;
    }
    return 0;
}

//
// setupMessageObject - Setup message object as Transmit or Receive
//
void setupMessageObject(uint32_t objID, uint32_t msgID, msgObjType msgType)
{
    //
    // Wait for busy bit to clear.
    //
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Clear and Write out the registers to program the message object.
    //
    CanaRegs.CAN_IF1CMD.all = 0;
    CanaRegs.CAN_IF1MSK.all = 0;
    CanaRegs.CAN_IF1ARB.all = 0;
    CanaRegs.CAN_IF1MCTL.all = 0;

    //
    // Set the Control, Mask, and Arb bit so that they get transferred to the
    // Message object.
    //
    CanaRegs.CAN_IF1CMD.bit.Control = 1;
    CanaRegs.CAN_IF1CMD.bit.Arb = 1;
    CanaRegs.CAN_IF1CMD.bit.Mask = 1;
    CanaRegs.CAN_IF1CMD.bit.DIR = 1;

    //
    // Set direction to transmit
    //
    if(msgType == MSG_OBJ_TYPE_TRANSMIT)
    {
        CanaRegs.CAN_IF1ARB.bit.Dir = 1;
    }

    //
    // Set Message ID (this example assumes 11 bit ID mask)
    //
    CanaRegs.CAN_IF1ARB.bit.ID = msgID;
    CanaRegs.CAN_IF1ARB.bit.MsgVal = 1;

    //
    // Set the data length since this is set for all transfers.  This is
    // also a single transfer and not a FIFO transfer so set EOB bit.
    //
    CanaRegs.CAN_IF1MCTL.bit.DLC = messageSize;
    CanaRegs.CAN_IF1MCTL.bit.EoB = 1;

    //
    // Transfer data to message object RAM
    //
    CanaRegs.CAN_IF1CMD.bit.MSG_NUM = objID;
}


//
// sendCANMessage - Transmit data from the specified message object
//
void sendCANMessage(uint32_t objID)
{
    //
    // Wait for busy bit to clear.
    //
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Write data to transfer into DATA-A and DATA-B interface registers
    //
    uint16_t index;
    for(index = 0; index < messageSize; index++)
    {
        switch(index)
        {
            case 0:
                CanaRegs.CAN_IF1DATA.bit.Data_0 = ucTXMsgData[index];
                break;
            case 1:
                CanaRegs.CAN_IF1DATA.bit.Data_1 = ucTXMsgData[index];
                break;
            case 2:
                CanaRegs.CAN_IF1DATA.bit.Data_2 = ucTXMsgData[index];
                break;
            case 3:
                CanaRegs.CAN_IF1DATA.bit.Data_3 = ucTXMsgData[index];
                break;
            case 4:
                CanaRegs.CAN_IF1DATB.bit.Data_4 = ucTXMsgData[index];
                break;
            case 5:
                CanaRegs.CAN_IF1DATB.bit.Data_5 = ucTXMsgData[index];
                break;
            case 6:
                CanaRegs.CAN_IF1DATB.bit.Data_6 = ucTXMsgData[index];
                break;
            case 7:
                CanaRegs.CAN_IF1DATB.bit.Data_7 = ucTXMsgData[index];
                break;
        }
    }

    //
    // Set Direction to write and set DATA-A/DATA-B to be transfered to
    // message object
    //
    CanaRegs.CAN_IF1CMD.all = 0x830000;

    //
    // Set Tx Request Bit
    //
    CanaRegs.CAN_IF1CMD.bit.TXRQST = 1;

    //
    // Transfer the message object to the message object specified by
    // objID.
    //
    CanaRegs.CAN_IF1CMD.bit.MSG_NUM = objID;
}

//
// getCANMessage - Check the message object for new data.
//                 If new data, data written into array and return true.
//                 If no new data, return false.
//
bool getCANMessage(uint32_t objID)
{
    bool status;

    //
    // Set the Message Data A, Data B, and control values to be read
    // on request for data from the message object.
    //
    CanaRegs.CAN_IF2CMD.all = 0;
    CanaRegs.CAN_IF2CMD.bit.Control = 1;
    CanaRegs.CAN_IF2CMD.bit.DATA_A = 1;
    CanaRegs.CAN_IF2CMD.bit.DATA_B = 1;

    //
    // Transfer the message object to the message object IF register.
    //
    CanaRegs.CAN_IF2CMD.bit.MSG_NUM = objID;

    //
    // Wait for busy bit to clear.
    //
    while(CanaRegs.CAN_IF2CMD.bit.Busy)
    {
    }

    //
    // See if there is new data available.
    //
    if(CanaRegs.CAN_IF2MCTL.bit.NewDat == 1)
    {
        //
        // Read out the data from the CAN registers.
        //
        uint16_t index;
        for(index = 0; index < messageSize; index++)
        {
            switch(index)
            {
                case 0:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATA.bit.Data_0;
                break;
                case 1:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATA.bit.Data_1;
                break;
                case 2:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATA.bit.Data_2;
                break;
                case 3:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATA.bit.Data_3;
                break;
                case 4:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATB.bit.Data_4;
                break;
                case 5:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATB.bit.Data_5;
                break;
                case 6:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATB.bit.Data_6;
                break;
                case 7:
                    ucRXMsgData[index] = CanaRegs.CAN_IF2DATB.bit.Data_7;
                break;
            }
        }

        //
        // Clear New Data Flag
        //
        CanaRegs.CAN_IF2CMD.bit.TxRqst = 1;

        //
        // Wait for busy bit to clear.
        //
        while(CanaRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        //
        // Transfer the message object to the message object IF register.
        //
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = objID;

        status = true;
    }
    else
    {
        status = false;
    }

    return(status);
}

//
// End of file
//
