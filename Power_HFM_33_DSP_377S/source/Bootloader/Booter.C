/*---------------------------------------------------------------------------------------
Coding   ：ANSI 简体中文GBK
PROJECT  : Source

COMPILER : Code Composer Studio V8.2.0.00007

MODULE   : BOOTER.C

VERSION  : V0.0

DATE     : 2018/10/05

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


---------------------------------------------------------------------------------------*/

#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "F021_F2837xS_C28x.h"
#include "main.h"
#include "mainUPS.h"
#include "Booter.h"

#define DSP_CLOCK_FREQ              (200)   // in MHz

// #BEEP
#define BOOT_BUZZER_ON              (EPwm12Regs.AQCSFRC.bit.CSFB = 0)
#define BOOT_BUZZER_OFF             (EPwm12Regs.AQCSFRC.bit.CSFB = 1)
#define BOOT_BUZ_PWM_HALF_PERIOD    (41666)     // 1000000/2.4k/0.005/2 = 41666

//---------------------------------------------------------------------------------------
// Variable Define
//---------------------------------------------------------------------------------------
#pragma DATA_SECTION(u32FlashAddr ,"BOOTER_RAM");
#pragma DATA_SECTION(RamAddr ,"BOOTER_RAM");
#pragma DATA_SECTION(FlashData ,"BOOTER_RAM");
#pragma DATA_SECTION(FlashCount ,"BOOTER_RAM");
#pragma DATA_SECTION(FlashFlag ,"BOOTER_RAM");
Uint32  u32FlashAddr;                      // Flash address
Uint16  *RamAddr;                        // RAM address
Uint16  FlashData;
Uint16  FlashCount;
Uint16  FlashFlag;

#pragma DATA_SECTION(u32BootTimeBaseUsec ,"BOOTER_RAM");
#pragma DATA_SECTION(BootTimeBase100ms ,"BOOTER_RAM");
#pragma DATA_SECTION(BootTimeBase1s ,"BOOTER_RAM");
#pragma DATA_SECTION(Ecommand ,"BOOTER_RAM");
#pragma DATA_SECTION(RxCheckSum ,"BOOTER_RAM");
#pragma DATA_SECTION(RxTimeOutCnt ,"BOOTER_RAM");
#pragma DATA_SECTION(NoRxDataCnt ,"BOOTER_RAM");
#pragma DATA_SECTION(u8BootRxCount ,"BOOTER_RAM");
#pragma DATA_SECTION(u8BootRxLength ,"BOOTER_RAM");
Uint32  u32BootTimeBaseUsec;            // Timebase, micro sec
Uint16  BootTimeBase100ms;              // Timebase, 100ms
Uint16  BootTimeBase1s;                 // Timebase, 1   s
Uint16  Ecommand;                       // Command
Uint16  RxCheckSum;                     // RX check sum
Uint16  RxTimeOutCnt;                   // No RX data time out count
Uint16  NoRxDataCnt;                    // No RX data in count, for reset RX COUNT
Uint8   u8BootRxCount;
Uint8   u8BootRxLength;

#pragma DATA_SECTION(ByteNumber ,"BOOTER_RAM");
#pragma DATA_SECTION(PageNumber ,"BOOTER_RAM");
#pragma DATA_SECTION(u32StartProgramAddress ,"BOOTER_RAM");
#pragma DATA_SECTION(Data1 ,"BOOTER_RAM");
#pragma DATA_SECTION(Data2 ,"BOOTER_RAM");
Uint16  ByteNumber;                     // Byte number
Uint16  PageNumber;                     // Page number
Uint32  u32StartProgramAddress;         // Start Address
Uint16  Data1;                          // Data1
Uint16  Data2;                          // Data2


#pragma DATA_SECTION(u8BootTxCount ,"BOOTER_RAM");
#pragma DATA_SECTION(pu8BootTxIndex ,"BOOTER_RAM");
#pragma DATA_SECTION(u8BootRxBuffer ,"BOOTER_RAM");
#pragma DATA_SECTION(u8BootTxBuffer ,"BOOTER_RAM");
Uint8   u8BootTxCount;
Uint8  *pu8BootTxIndex;
Uint8   u8BootRxBuffer[BOOT_RX_BUFF_LEN];
Uint8   u8BootTxBuffer[BOOT_TX_BUFF_LEN];

#pragma DATA_SECTION(SectorMask ,"BOOTER_RAM");
#pragma DATA_SECTION(pu16DataBufferAddr ,"BOOTER_RAM");
#pragma DATA_SECTION(u16DataBufferSizeInWords ,"BOOTER_RAM");
#pragma DATA_SECTION(Status ,"BOOTER_RAM");
#pragma DATA_SECTION(u16ProgBuzzerCnt ,"BOOTER_RAM");
//#pragma DATA_SECTION(FlashStatus ,"BOOTER_RAM");
Uint16  SectorMask;                     // Sector will be erased
Uint16  *pu16DataBufferAddr;                       // Pointer to the buffer of 16 bit data
Uint16  u16DataBufferSizeInWords;                         // Number of 16 bit value
Uint16  Status;                         // Return status
Uint16	u16ProgBuzzerCnt;
//FLASH_ST FlashStatus;                   // Flash status structure

#define WORDS_IN_FLASH_BUFFER   80      // Programming data buffer, Words
#pragma DATA_SECTION(u16ProgramBuffer ,"BOOTER_RAM");
Uint16  u16ProgramBuffer[WORDS_IN_FLASH_BUFFER];


#pragma DATA_SECTION(SciFlag ,"BOOTER_RAM");
#pragma DATA_SECTION(ProgramFlag ,"BOOTER_RAM");
#pragma DATA_SECTION(FailFlag ,"BOOTER_RAM");
union   UN_SciFlag SciFlag;             // SCI flag
union   UN_ProgramFlag ProgramFlag;     // Program Flag
union   UN_FailFlag FailFlag;           // Fail Flag

#define FLASHABLE_BANK0_SECTOR_SIZE   (12)  
#pragma DATA_SECTION(pu32FlashableFlashBank0Sector, "BOOTER_RAM");
Uint32    pu32FlashableFlashBank0Sector[FLASHABLE_BANK0_SECTOR_SIZE];

#define FLASHABLE_BANK1_SECTOR_SIZE   (14)  
#pragma DATA_SECTION(pu32FlashableFlashBank1Sector, "BOOTER_RAM");
Uint32    pu32FlashableFlashBank1Sector[FLASHABLE_BANK1_SECTOR_SIZE];

#pragma DATA_SECTION(BootPassWord, "PASSWORD_ROM");
const unsigned int BootPassWord[] = {BOOT_PASSWORD, BOOT_KEYWORD};


//---------------------------------------------------------------------------------------
// BOOT LOADER
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(Bloader, "BOOTLOAD_APP_ENTRY");
void Bloader(void)
{       
    // Copy Flash API code to RAM
    MemCopy_Boot(&BootRamfunsRunStart, &BootRamfunsLoadStart, (size_t)&BootRamfunsLoadSize);
    MemCopy_Boot(&FlashApiRunStart, &FlashApiLoadStart, (size_t)&FlashApiLoadSize);
    
    pu32FlashableFlashBank0Sector[0]  = 0x084000;   //sector C
    pu32FlashableFlashBank0Sector[1]  = 0x086000;   //sector D
    pu32FlashableFlashBank0Sector[2]  = 0x088000;   //sector E
    pu32FlashableFlashBank0Sector[3]  = 0x090000;   //sector F
    pu32FlashableFlashBank0Sector[4]  = 0x098000;   //sector G
    pu32FlashableFlashBank0Sector[5]  = 0x0A0000;   //sector H
    pu32FlashableFlashBank0Sector[6]  = 0x0A8000;   //sector I
    pu32FlashableFlashBank0Sector[7]  = 0x0B0000;   //sector J
    pu32FlashableFlashBank0Sector[8]  = 0x0B8000;   //sector K
    pu32FlashableFlashBank0Sector[9]  = 0x0BA000;   //sector L
    pu32FlashableFlashBank0Sector[10] = 0x0BC000;   //sector M
    pu32FlashableFlashBank0Sector[11] = 0x0BE000;   //sector N
    
    pu32FlashableFlashBank1Sector[0]  = 0x0C0000;   //Sector O 
    pu32FlashableFlashBank1Sector[1]  = 0x0C2000;   //Sector P 
    pu32FlashableFlashBank1Sector[2]  = 0x0C4000;   //Sector Q 
    pu32FlashableFlashBank1Sector[3]  = 0x0C6000;   //Sector R 
    pu32FlashableFlashBank1Sector[4]  = 0x0C8000;   //Sector S 
    pu32FlashableFlashBank1Sector[5]  = 0x0D0000;   //Sector T 
    pu32FlashableFlashBank1Sector[6]  = 0x0D8000;   //Sector U 
    pu32FlashableFlashBank1Sector[7]  = 0x0E0000;   //Sector V 
    pu32FlashableFlashBank1Sector[8]  = 0x0E8000;   //Sector W 
    pu32FlashableFlashBank1Sector[9]  = 0x0F0000;   //Sector X 
    pu32FlashableFlashBank1Sector[10] = 0x0F8000;   //Sector Y 
    pu32FlashableFlashBank1Sector[11] = 0x0FA000;   //Sector Z 
    pu32FlashableFlashBank1Sector[12] = 0x0FC000;   //Sector AA
    pu32FlashableFlashBank1Sector[13] = 0x0FE000;   //Sector AB
    
    
        
    while (1)
    {
        SciReceiveData();                         // Receive data
        SciParser();                              // Parser data
        SciTransmitData();                        // Transmit data

        Boot_TimeBase();                          // Timebase

        CheckStartProgram();                      // Check start program
        Erase_All_Flash();                        // Erase all flash memory
        Program_Flash();                          // Program flash memory
        Finish_Program();                         // Finish Program flash memory
        CheckTimeOut();                           // check time out for no RX data in programming

        if (ProgramFlag.sBit.ProgramOver)          // Program over ?
        {
            ProgramFlag.u16All = 0;
            SciFlag.u16All = 0;
            FailFlag.u16All = 0;
            //BootAppEntry();
            
            EALLOW;
            WdRegs.WDCR.all = 0x0028;             // WD enable
            EDIS;
            while (1)
            {
                asm(" NOP");                        // wait for WD reset
            }
        }
    }
}

//---------------------------------------------------------------------------------------
//   Boot Time Base启动时间基数
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(Boot_TimeBase, "BOOTLOAD_APP");
void Boot_TimeBase(void)
{
    //CPU_READY_TOGGLE;                            // Toggle WDG clock

    u32BootTimeBaseUsec++;
    if (u32BootTimeBaseUsec >= 28570)                // 100 ms ? Bus Frequency=150MHz
    {
        u32BootTimeBaseUsec = 0;

        RxTimeOutCnt++;                           //+100ms, RX data time out count

        //if (GpioDataRegs.GPCDAT.bit.GPIO69)
        //{
        //    TEST3_OUT_0;
        //}
        //else
        //{
        //    TEST3_OUT_1;
        //}

        NoRxDataCnt++;                            //+100ms, RX data in count
        if (NoRxDataCnt >= 20)                    //>=2S ?
        {
            NoRxDataCnt = 0;                       //clear count
            u8BootRxCount = 0;                          //reset RX COUNT
        }

        BootTimeBase100ms++;
        if (BootTimeBase100ms >= 10)              // 1 s ?
        {
            BootTimeBase100ms = 0;

            BootTimeBase1s++;

            // SET LCD BACKLIGHT
            //LED_DRY_FLAG.sBit.LCD_BKLIT = 1;        // LCD BACKLIGHT on
        }

        ProgBuzzer();
    }
}

//---------------------------------------------------------------------------------------
//   Clear Time Base明确时间基础
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(CLR_TimeBase, "BOOTLOAD_APP");
void CLR_TimeBase(void)
{
    u32BootTimeBaseUsec = 0;
    BootTimeBase100ms = 0;
    BootTimeBase1s = 0;
}

//---------------------------------------------------------------------------------------
//  Time Delay 5 usec 时间延迟 5usec
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(TimeDelayMicroSec, "BOOTLOAD_APP");
void TimeDelayMicroSec(void)
{
    Uint16 microsec = 0;

    while (microsec < 40)        // Delay 5us, Bus Frequency=150MHz
    {
        microsec++;
    }

}

//---------------------------------------------------------------------------------------
//ROUTINE NAME : INIT_PIM_BootINIT PIM启动
//INPUT/OUTPUT : None
//DESCRIPTION  : Port initialization
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(INIT_PIM_Boot, "BOOTLOAD_APP");
void INIT_PIM_Boot(void)
{
    EALLOW;

    // GPIO 31~0 , 3322 2222 2222 1111 1111 1100 0000 0000
    // GPIO 31~0 , 1098 7654 3210 9876 5432 1098 7654 3210
    // FUNCTION  , 1111 1111 0000 2200 0000 1111 1111 1111  , 0:GPIO, 1/2/3:Peripheral
    // DIRECTION , 1010 0000 0011 0100 0000 1111 1111 1111  , 0:In,   1:Out
    // DATA      , 0011 0000 0000 1111 0110 0000 0000 0000  , Initial value

    // GPIO 63~32, 6666 5555 5555 5544 4444 4444 3333 3333
    // GPIO 63~32, 3210 9876 5432 1098 7654 3210 9876 5432
    // FUNCTION  , 1100 0000 0000 0000 0000 0000 0000 0000  , 0:GPIO, 1/2/3:Peripheral
    // DIRECTION , 1000 1111 1111 1100 1000 1101 1111 1110  , 0:In,   1:Out
    // DATA      , 1111 0000 0100 0010 0110 0010 0010 0001  , Initial value

    // GPIO 95~64, 9999 9988 8888 8888 7777 7777 7766 6666
    // GPIO 95~64, 5432 1098 7654 3210 9876 5432 1098 7654
    // FUNCTION  , 0000 0000 0000 0000 0000 0000 0000 0000  , 0:GPIO, 1/2/3:Peripheral
    // DIRECTION , 0000 0000 0000 0111 1111 1110 1110 0000  , 0:In,   1:Out
    // DATA      , 0000 0000 0000 0000 0001 1000 0100 1001  , Initial value


    //Set GPIO Initial Data
    GpioDataRegs.GPADAT.all = 0x00000000;       // GPIO Data GPIO00-GPIO31
    GpioDataRegs.GPBDAT.all = 0x00000000;       // GPIO Data GPIO32-GPIO63
    GpioDataRegs.GPCDAT.all = 0x00000000;       // GPIO Data GPIO64-GPIO95
    GpioDataRegs.GPDDAT.all = 0x00000000;
    GpioDataRegs.GPEDAT.all = 0x00000000;

    // GPIO Direction
    GpioCtrlRegs.GPADIR.all = 0xD7FFFFFF;       //0xECFFFFFF;   //31~0
    GpioCtrlRegs.GPBDIR.all = 0xB3EFB7A3;       //0xBB5D64E3;   //63~32
    GpioCtrlRegs.GPCDIR.all = 0xFFFC3B04;       //0xFD799FFE;   //95~64
    GpioCtrlRegs.GPDDIR.all = 0xFFFFFFFF;
    GpioCtrlRegs.GPEDIR.all = 0xFFFFFFFF;
    

    // Init Gpio42/43 for SCI A 
    // Enable internal pull-up for the selected pins 
    // Pull-ups can be enabled or disabled disabled by the user.  
    // This will enable the pullups for the specified pins.
    // Set qualification for selected pins to asynch only 
    // Inputs are synchronized to SYSCLKOUT by default.  
    // This will select asynch (no qualification) for the selected pins.
    //GpioCtrlRegs.GPBQSEL1.bit.GPIO43 = 3;
    //GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = 3;
    //GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;
    //GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = 3;
    //GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;
    //GpioCtrlRegs.GPBDIR.bit.GPIO42 =1;
    //GpioCtrlRegs.GPBDIR.bit.GPIO43 =0;
    
    // Init Gpio46/47 for SCI D 
    GpioCtrlRegs.GPBQSEL1.bit.GPIO46 = 3;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO47 = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 2;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO46 = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO47 =1;
    GpioCtrlRegs.GPBDIR.bit.GPIO46 =0;
    
    // BEEP
    GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO23 as EPWM12B (For BEEP)
    
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;     // x0 = /1 of PLLSYSCLK
    // x1 = /2 of PLLSYSLCK (default on reset)
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;           // Stop all the TB clocks
    
    EPwm12Regs.TBPRD = BOOT_BUZ_PWM_HALF_PERIOD;
    EPwm12Regs.AQCSFRC.bit.CSFB = 1;
    EPwm12Regs.TBCTR = 0;
    EPwm12Regs.CMPA.bit.CMPA = 0;
    EPwm12Regs.CMPB.bit.CMPB = BOOT_BUZ_PWM_HALF_PERIOD / 2;
    EPwm12Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;
    EPwm12Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
    EPwm12Regs.AQCTLA.bit.CAU  = AQ_CLEAR;
    EPwm12Regs.AQCTLA.bit.CAD  = AQ_SET;
    EPwm12Regs.AQCTLB.bit.CBU  = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.CBD  = AQ_SET;
    
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;          // enable the TB clocks

    EDIS;
}

//---------------------------------------------------------------------------------------
// INIT SCI1 2400 BAUD
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(InitSCI2400, "BOOTLOAD_APP");
void InitSCI2400(void)
{
    // Initialize SCI-A:
    ScidRegs.SCICCR.all = 0x0007;       // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    ScidRegs.SCICTL1.all = 0x0003;      // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScidRegs.SCICTL2.all = 0x0000;      // default
    ScidRegs.SCIHBAUD.bit.BAUD = 0x000A;		// 2400 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (2400*8) - 1 = 2603
	ScidRegs.SCILBAUD.bit.BAUD = 0x002B;

    // Initialize SCI-A FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;

    ScidRegs.SCICTL1.all = 0x0023;
}

//---------------------------------------------------------------------------------------
// INIT SCI1 9600 BAUD
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(InitSCI9600, "BOOTLOAD_APP");
void InitSCI9600(void)
{
    // Initialize SCI-A:
    ScidRegs.SCICCR.all = 0x0007;       // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    ScidRegs.SCICTL1.all = 0x0003;      // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScidRegs.SCICTL2.all = 0x0000;      // default
    ScidRegs.SCIHBAUD.bit.BAUD = 0x0002;		// 9600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (9600*8) - 1 = 650
	ScidRegs.SCILBAUD.bit.BAUD = 0x008A;

    // Initialize SCI-A FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;

    ScidRegs.SCICTL1.all = 0x0023;
}

//---------------------------------------------------------------------------------------
// INIT SCI1 19200 BAUD
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(InitSCI19200, "BOOTLOAD_APP");
void InitSCI19200(void)
{
    // Initialize SCI-A:
    ScidRegs.SCICCR.all = 0x0007;       // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    ScidRegs.SCICTL1.all = 0x0003;      // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScidRegs.SCICTL2.all = 0x0000;      // default
    ScidRegs.SCIHBAUD.bit.BAUD = 0x0001;		// 19200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (19200*8) - 1 = 324
	ScidRegs.SCILBAUD.bit.BAUD = 0x0044;

    // Initialize SCI-A FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;

    ScidRegs.SCICTL1.all = 0x0023;
}

//---------------------------------------------------------------------------------------
// INIT SCI1 57600 BAUD
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(InitSCI57600, "BOOTLOAD_APP");
void InitSCI57600(void)
{
    // Initialize SCI-A:
    ScidRegs.SCICCR.all = 0x0007;       // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    ScidRegs.SCICTL1.all = 0x0003;      // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScidRegs.SCICTL2.all = 0x0000;      // default
    ScidRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 57600 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (57600*8) - 1 = 107
    ScidRegs.SCILBAUD.bit.BAUD = 0x006B;

    // Initialize SCI-A FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;

    ScidRegs.SCICTL1.all = 0x0023;
}

//---------------------------------------------------------------------------------------
// INIT SCI1 115200 BAUD
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(InitSCI115200, "BOOTLOAD_APP");
void InitSCI115200(void)
{
    // Initialize SCI-A:
    ScidRegs.SCICCR.all = 0x0007;       // 1 stop bit,  No loopback
    // No parity,8 char bits,               
    // async mode, idle-line protocol
    ScidRegs.SCICTL1.all = 0x0003;      // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    ScidRegs.SCICTL2.all = 0x0000;      // default
    ScidRegs.SCIHBAUD.bit.BAUD = 0x0000;		// 115200 baud @LSPCLK = 200 / 4 = 50MHz. BRR = 50M / (115200*8) - 1 = 53
    ScidRegs.SCILBAUD.bit.BAUD = 0x0035;

    // Initialize SCI-A FIFO
    ScidRegs.SCIFFTX.all = 0xE040;
    ScidRegs.SCIFFRX.all = 0x204f;
    ScidRegs.SCIFFCT.all = 0x0;

    ScidRegs.SCICTL1.all = 0x0023;
}

//---------------------------------------------------------------------------------------
// SCI RECEIVE
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(SciReceive, "BOOTLOAD_APP");
void SciReceive(void)
{
    u8BootRxCount = 0;
}

//---------------------------------------------------------------------------------------
// SCI TRANSMIT
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(SciTransmit, "BOOTLOAD_APP");
void SciTransmit(void)
{
    pu8BootTxIndex = u8BootTxBuffer;              // TX buffer start address

    SciFlag.sBit.TxDataEnable = 1;                // Tx data enable
}

//---------------------------------------------------------------------------------------
// SCI RECEIVE DATA
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(SciReceiveData, "BOOTLOAD_APP");
void SciReceiveData(void)
{
    if (ScidRegs.SCIFFRX.bit.RXFFST != 0)                           	// Receive FIFO empty ?
    {
        SciFlag.sBit.RxDataIn = 1;                                   	// set RX data

        u8BootRxBuffer[u8BootRxCount] = ScidRegs.SCIRXBUF.bit.SAR;      // store to buffer

        NoRxDataCnt = 0;                                  	            // clear count

        u8BootRxCount++;
        if (u8BootRxCount >= BOOT_RX_BUFF_LEN)                          // >= buffer length ?
        {
            u8BootRxCount = 0;
        }
    }

}

//---------------------------------------------------------------------------------------
// SCI TRANSMIT DATA
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(SciTransmitData, "BOOTLOAD_APP");
void SciTransmitData(void)
{

    if (!SciFlag.sBit.TxDataEnable)               // Tx data enable ?
    {
        return;
    }

    if (ScidRegs.SCIFFTX.bit.TXFFST == 0)        // Transmit FIFO empty ?
    {
        ScidRegs.SCITXBUF.bit.TXDT = *pu8BootTxIndex++;          // get data from TX buffer
        u8BootTxCount--;
    }

    if (u8BootTxCount == 0)                   // TX data over ?
    {
        while (!ScidRegs.SCICTL2.bit.TXEMPTY) // Transmit u16ProgramBuffer and Shift register empty ?
        {
            //CPU_READY_TOGGLE;                            // Toggle WDG clock
        }

        SciFlag.sBit.TxDataEnable = 0;     // Tx data disable
    }
}

//---------------------------------------------------------------------------------------
// SCI PARSER sci解析
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(SciParser, "BOOTLOAD_APP");
void SciParser(void)
{
    Fapi_StatusType oReturnCheck;
    Fapi_FlashStatusWordType oFlashStatusWord;
    
    if (!SciFlag.sBit.RxDataIn)           // RX data in ?
    {
        return;
    }

    SciFlag.sBit.RxDataIn = 0;

    if (u8BootRxCount < 3)                // < 3 data ?
    {
        return;
    }

    if (u8BootRxCount == 3)
    {
        u8BootRxLength = u8BootRxCount + u8BootRxBuffer[2] + 1; // total length
    }


    if (u8BootRxCount < u8BootRxLength)            // receive over ?
    {
        return;
    }

    RxCheckSum = u8BootRxBuffer[u8BootRxCount - 1];  // get receive check sum data

    if (RxCheckSum != GetCheckSum((Uint16*)&u8BootRxBuffer, u8BootRxCount - 1)) // RX check sum and data correct ?
    {
        TR_CheckSumError();               // check sum error
        SciTransmit();
        SciReceive();                     // reset RX counter
        return;
    }

    RxTimeOutCnt = 0;                    // clear RX data time out count

    Ecommand = u8BootRxBuffer[1];               	// get command
    switch(Ecommand)
    {
        case 3:                   		// 0x03, Sector/Option erase
            {
                if (u8BootRxBuffer[4] == 0x06)           	// 0x06, check sum
                {
                    // Unlock CSM
                    CsmUnlock_Boot();

                    // Copy Flash setup code to RAM
                    //MemCopy_Boot(&BootRamfunsLoadStart, &BootRamfunsRunStart, (size_t)&BootRamfunsLoadSize);
                    
                    // Copy Flash API code to RAM
                    //MemCopy_Boot(&FlashApiRunStart, &FlashApiLoadStart, (size_t)&FlashApiLoadSize);
                    
                    if(ProgramFlag.sBit.EraseOK)
                    {
                        RxTimeOutCnt = 0;                // clear count
                    
                        TR_Success();                    // Response success
                    }
                    else
                    {
                        ProgramFlag.sBit.Erasing = 1;   // set Erasing flag
                    }
                }
                break;
            }
        case 6:                  // 0x06, Checksum computation
            {
                if ((u8BootRxBuffer[3] == 0x00) &&       // 0x00, check sum
                    (u8BootRxBuffer[4] == 0x00))         // 0x00,
                {
                    EALLOW;
                    //
                    //Give pump ownership to FMC0
                    //
                    FlashPumpSemaphoreRegs.PUMPREQUEST.all = 0x5A5A0000;
                    
    	            oReturnCheck = Fapi_initializeAPI(F021_CPU0_W0_BASE_ADDRESS, DSP_CLOCK_FREQ);
    	            oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    	
                    // program Password and UPS ID
                    u32FlashAddr = BootPassWord;   // Password address
                    u16ProgramBuffer[0] = BOOT_PASSWORD;             // 0x55AA
                    u16ProgramBuffer[1] = BOOT_KEYWORD;              // 0x8010
                    //pu16DataBufferAddr = (Uint16 *)&u16ProgramBuffer;
                    u16DataBufferSizeInWords = 2;             // 2
                    
                    oReturnCheck = Fapi_issueProgrammingCommand((Uint32 *)u32FlashAddr,
													u16ProgramBuffer,
													u16DataBufferSizeInWords,
													0,
													0,
													Fapi_DataOnly);
													
					while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
					
					//
                    // Enable ECC
                    //
                    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;
					
					EDIS;
                    
                    oReturnCheck = Fapi_doVerifyBy16bits((Uint32 *)u32FlashAddr,u16DataBufferSizeInWords,u16ProgramBuffer,&oFlashStatusWord);
                    
                    if (oReturnCheck == Fapi_Status_Success)
                    {
                        CLR_TimeBase();                     // clear time base count

                        SciFlag.sBit.EndOfData = 1;          // End of data
                        ProgramFlag.sBit.ProgramOK = 1;      // Set Programm OK flag

                        TR_Success();                       // Response success
                    }
                    else
                    {
                        TR_FAIL();    // Response FAIL
                    }
                }
                break;
            }
        case 8:                  // 0x08, Get start & end address
            {
                if (u8BootRxBuffer[3] == 0x09)           // 0x09, check sum
                {
                    u8BootTxBuffer[0] = 0x01;
                    u8BootTxBuffer[1] = 0x08;
                    u8BootTxBuffer[2] = 0x06;
                    u8BootTxBuffer[3] = 0x00;             // Start address low
                    u8BootTxBuffer[4] = 0x40;             // Start address middle
                    u8BootTxBuffer[5] = 0x08;             // Start address high   , 0x084000
                    u8BootTxBuffer[6] = 0xFF;             // End   address low
                    u8BootTxBuffer[7] = 0xFF;             // End   address middle
                    u8BootTxBuffer[8] = 0x0F;             // End   address high   , 0x0FFFFF
                    u8BootTxBuffer[9] = 0x64;             // check sum

                    u8BootTxCount = 10;
                    SciTransmit();
                }
                break;
            }
        case 9:                  // 0x09, Long address block programming
            {
                ByteNumber = u8BootRxBuffer[3];          		// get Byte number
                //if (u8BootRxBuffer[3] & 0x01)            	// Odd ?
                //{
                //   ByteNumber++;                  			// Byte number +1
                //   u8BootRxBuffer[u8BootRxCount-1] = 0xFF;    // fill 0xFF in last byte for odd number
                //}

                PageNumber = u8BootRxBuffer[4];          		// get Page number
                Data1 = (Uint16)u8BootRxBuffer[5];
                Data2 = ((Uint16)u8BootRxBuffer[6] << 8) | Data1;                // 16 bits, address
                u32StartProgramAddress = ((Uint32)PageNumber << 16) | (Uint32)Data2; // 32 bits, Page+address

                SciFlag.sBit.ReadyToProgram = 1;

                ProgramFlag.sBit.Programming = 1;  			// Set Programming flag

                break;
            }
        case 0x0A:				// 0x0A, Request UPS into programming mode
            {
                if ((u8BootRxBuffer[3] == 0x55) &&       		// 0x55,
                    (u8BootRxBuffer[4] == 0xAA) &&       		// 0xAA,
                    (u8BootRxBuffer[5] == (BOOT_KEYWORD>>8)) &&         // BOOT_KEYWORD    0x8010
                    (u8BootRxBuffer[6] == (BOOT_KEYWORD&0x00FF)) &&     // 
                    (u8BootRxBuffer[7] == BOOT_CHECKSUM))         		// 0x92, Check Sum( LowByte(0x55 + 0xAA + 0x80 + 0x10 + 0x01 + 0x0A + 0x04) )
                {
                    SciFlag.sBit.PROGRAM_MODE = 1;  			// start program mode
                }
                else
                {
                    TR_ErrorFile();    // Response Error file
                }
                break;
            }
        case 0x0C:				// 0x0C, Get Boot Loader Programming Status
            {
                if (u8BootRxBuffer[3] == 0x0D)           		// 0x0D, check sum
                {
                    u8BootTxBuffer[0] = 0x01;
                    u8BootTxBuffer[1] = 0x0C;
                    u8BootTxBuffer[2] = 0x01;

                    if (ProgramFlag.sBit.Programming)       	// In Programming ?
                    {
                        u8BootTxBuffer[3] = 0x01;    // In Programming
                    }
                    else
                    {
                        u8BootTxBuffer[3] = 0x00;    // Ready to Start Programming
                    }

                    u8BootTxBuffer[4] = u8BootTxBuffer[0] + u8BootTxBuffer[1] + u8BootTxBuffer[2] + u8BootTxBuffer[3]; // check sum

                    u8BootTxCount = 5;
                    SciTransmit();
                }
                break;
            }
        default:
            break;
    }

    SciReceive();							// reset RX counter
}

//---------------------------------------------------------------------------------------
// 
// CHECK START PROGRAM 检查启动程序
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(CheckStartProgram, "BOOTLOAD_APP");
void CheckStartProgram(void)
{
    if (SciFlag.sBit.PROGRAM_MODE)                // response ack command
    {
        SciFlag.sBit.PROGRAM_MODE = 0;

        u8BootTxBuffer[0] = 0x01;
        u8BootTxBuffer[1] = 0x0A;
        u8BootTxBuffer[2] = 0x02;              	    // length
        u8BootTxBuffer[3] = 0x10;
        u8BootTxBuffer[4] = 0x02;               	// 0x02, 9600 Baud
        //u8BootTxBuffer[4] = 0x03;             	// 0x03, 19200 Baud
        //u8BootTxBuffer[4] = 0x04;             	// 0x04, 38400 Baud
        //u8BootTxBuffer[4] = 0x05;             	// 0x05, 57600 Baud
        //u8BootTxBuffer[4] = 0x06;             	// 0x05, 115200 Baud
        u8BootTxBuffer[5] = u8BootTxBuffer[0] + u8BootTxBuffer[1] + u8BootTxBuffer[2] + u8BootTxBuffer[3] + u8BootTxBuffer[4];

        u8BootTxCount = 6;
        SciTransmit();

        while (SciFlag.sBit.TxDataEnable)          // Tx data over ?
        {
            SciTransmitData();                     // Transmit data

            //CPU_READY_TOGGLE;                    // Toggle WDG clock
        }
        
        ProgramFlag.u16All = 0;

        InitSCI9600();                            // set 9600 Baud
        //InitSCI19200();                    		// set 19200 Baud
        //InitSCI57600();                    		// set 57600 Baud        
        //InitSCI115200();                    		// set 115200 Baud        
        SciReceive();                             // reset RX counter
    }
}

//---------------------------------------------------------------------------------------
// GET CHECK SUM DATA
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(GetCheckSum, "BOOTLOAD_APP");
Uint16 GetCheckSum(Uint16 *addr, Uint16 count)
{
    Uint16 sum = 0;

    while (count)                        // over ?
    {
        sum += *addr++;

        count--;
    }

    sum &= 0x00FF;                       // mask high byte
    return (sum);
}

//---------------------------------------------------------------------------------------
// TRANSMIT CHECK SUM ERROR
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(TR_CheckSumError, "BOOTLOAD_APP");
void TR_CheckSumError(void)
{
    u8BootTxBuffer[0] = 0x15;                   // NAK
    u8BootTxBuffer[1] = Ecommand;               // previous ECMD
    u8BootTxBuffer[2] = 0;                      // Data length
    u8BootTxBuffer[3] = 0x15 + u8BootTxBuffer[1]; // Checksum
    u8BootTxCount = 4;
}

//---------------------------------------------------------------------------------------
// TRANSMIT OPERATION SUCCESS
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(TR_Success, "BOOTLOAD_APP");
void TR_Success(void)
{
    u8BootTxBuffer[0] = 0x01;             	// start of Header2
    u8BootTxBuffer[1] = Ecommand;         	// previous ECMD
    u8BootTxBuffer[2] = 0x01;              	// Data length
    u8BootTxBuffer[3] = 0x00;               	// Status code, Operation successful
    u8BootTxBuffer[4] = u8BootTxBuffer[0] + u8BootTxBuffer[1] + u8BootTxBuffer[2] + u8BootTxBuffer[3]; // Checksum
    u8BootTxCount = 5;

    SciTransmit();                       	// Transmit enbale
}

//---------------------------------------------------------------------------------------
// TRANSMIT OPERATION FAIL
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(TR_FAIL, "BOOTLOAD_APP");
void TR_FAIL(void)
{
    u8BootTxBuffer[0] = 0x01;             	// start of Header
    u8BootTxBuffer[1] = Ecommand;         	// previous ECMD
    u8BootTxBuffer[2] = 0x01;             	// Data length
    u8BootTxBuffer[3] = 0x10;             	// Status code, Operation failed
    u8BootTxBuffer[4] = u8BootTxBuffer[0] + u8BootTxBuffer[1] + u8BootTxBuffer[2] + u8BootTxBuffer[3]; // Checksum
    u8BootTxCount = 5;

    SciTransmit();                       	// Transmit enbale
}

//---------------------------------------------------------------------------------------
// TRANSMIT ERROR FILE
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(TR_ErrorFile, "BOOTLOAD_APP");
void TR_ErrorFile(void)
{
    u8BootTxBuffer[0] = 0x01;              	// start of Header
    u8BootTxBuffer[1] = Ecommand;          	// previous ECMD
    u8BootTxBuffer[2] = 0x01;              	// Data length
    u8BootTxBuffer[3] = 0x30;              	// Status code, Error file
    u8BootTxBuffer[4] = u8BootTxBuffer[0] + u8BootTxBuffer[1] + u8BootTxBuffer[2] + u8BootTxBuffer[3]; // Checksum
    u8BootTxCount = 5;

    SciTransmit();                       	// Transmit enbale
}

//---------------------------------------------------------------------------------------
// CHECK TIME OUT
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(CheckTimeOut, "BOOTLOAD_APP");
void CheckTimeOut(void)
{
    if ((ProgramFlag.sBit.EraseOK      && 	// Erase OK ?
         !ProgramFlag.sBit.Programming) || 	// Programming ?
        (ProgramFlag.sBit.Programming  && 	// Programming ?
         !ProgramFlag.sBit.ProgramOK))     	// not Program OK  ?
    {
        if (RxTimeOutCnt >= SEC_5S)       	// >= 5S ?
        {
            ProgramFlag.u16All = 0;

            InitSCI9600();
            SciReceive();                  	// reset RX counter
        }
    }
    else
    {
        RxTimeOutCnt = 0;                 	// clear count
    }

    if (ScidRegs.SCIRXST.bit.RXERROR) 		// Receive Error ?
    {
        ProgramFlag.u16All = 0;

        InitSCI9600();                 		// set 2400 Baud
        SciReceive();                  		// reset RX counter
    }
}

//---------------------------------------------------------------------------------------
// WAITING LOOP
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(WaitToDisplay, "BOOTLOAD_APP");
void WaitToDisplay(Uint16 second)
{
    //CLR_TimeBase();                      // clear time base count
    //
    //while (BootTimeBase1s < second)      // < second ? delay time to display message
    //{
    //    TimeDelayMicroSec();              // delay time for timebase :=5us, other function time :=15us, 5+15=20us
    //
    //    SCREEN_LCD();                     // select LCD screen
    //    DISPLAY_LCD();                    // LCD display routine
    //    Boot_TimeBase();                  // Timebase
    //}
}

//---------------------------------------------------------------------------------------
// ERASE ALL FLASH MEMORY
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(Erase_All_Flash, "BOOTLOAD_APP");
void Erase_All_Flash(void)
{
    int16   i16Count;
    Fapi_StatusType oReturnCheck;
    
    if (ProgramFlag.sBit.EraseOK)         // Erase OK ?
    {
        return;                           // exit
    }

    if (ProgramFlag.sBit.Erasing)         // Erasing ?
    {
        EALLOW;
        
        //
        //Give pump ownership to FMC0
        //
        FlashPumpSemaphoreRegs.PUMPREQUEST.all = 0x5A5A0000;

    	oReturnCheck = Fapi_initializeAPI(F021_CPU0_W0_BASE_ADDRESS, DSP_CLOCK_FREQ);
    	oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);

        // Erase flashable area flash
        // Erase is performed from the last sector because the password is located at the end of flash address
        for (i16Count = 0; i16Count < FLASHABLE_BANK0_SECTOR_SIZE; i16Count++)
        {
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (Uint32 *)pu32FlashableFlashBank0Sector[i16Count]);
            while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}
        }
        
        /*
        //
        //Give pump ownership to FMC1
        //
        FlashPumpSemaphoreRegs.PUMPREQUEST.all = 0x5A5A0001;
        
    	oReturnCheck = Fapi_initializeAPI(F021_CPU0_W1_BASE_ADDRESS, DSP_CLOCK_FREQ);
    	oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank1);

        // Erase flashable area flash
        // Erase is performed from the last sector because the password is located at the end of flash address
        for (i16Count = (FLASHABLE_BANK1_SECTOR_SIZE - 1); i16Count >= 0; i16Count--)
        {
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (Uint32 *)pu32FlashableFlashBank1Sector[i16Count]);
            while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}
        }
        */
        
        Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;
        
        EDIS;
        

        ProgramFlag.sBit.EraseOK = 1;     // Erase OK ?

        RxTimeOutCnt = 0;                // clear count

        TR_Success();                    // Response success
    }
}
//---------------------------------------------------------------------------------------
// Is In Flashable Range
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(IsInFlashableRange, "BOOTLOAD_APP");
Uint8 IsInFlashableRange(Uint32 u32Addr, Uint16 u16Len)
{
    if (u32Addr >= APPLICATION_FLASH_STARTING_ADDR
        && u32Addr <= APPLICATION_FLASH_END_ADDR
        && (u32Addr + (u16Len >> 1)) <= APPLICATION_FLASH_END_ADDR + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------
// Programming Flash Memory
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(Program_Flash, "BOOTLOAD_APP");
void Program_Flash(void)
{
    Uint16 i_uint,loop;
    
    Fapi_StatusType oReturnCheck;
    Fapi_FlashStatusWordType oFlashStatusWord;

    if (!ProgramFlag.sBit.Programming)    // Programming ?
    {
        return;
    }

    if (!SciFlag.sBit.ReadyToProgram)     // Ready to Program ?
    {
        return;
    }

    SciFlag.sBit.ReadyToProgram = 0;      // clear Ready to Program flag

    if ((u32StartProgramAddress ==  0x088000)  ||   // 0x088000, Password ?
        ((u32StartProgramAddress >= 0x080000) &&   // BOOTROM RANGE: >= 0x080000 and <= 0x083FFF ?
         (u32StartProgramAddress <= 0x083FFF)))
    {
        TR_Success();                     // Response success
        return;                           // exit
    }

    // Copy data to u16ProgramBuffer
    RamAddr = (Uint16 *)&u8BootRxBuffer[7];     // RX dara address
    FlashCount = ByteNumber;             // data number
    i_uint = 0;
    while (FlashCount)
    {
        Data1 = *RamAddr;                 // High byte
        RamAddr++;
        Data2 = *RamAddr;                 // Low byte
        RamAddr++;

        u16ProgramBuffer[i_uint] = (Data1 << 8) | Data2;
        FlashCount--;
        i_uint++;
    }

    // program flash
    u32FlashAddr = u32StartProgramAddress;  // flash address
    //pu16DataBufferAddr = (Uint16 *)&u16ProgramBuffer;
    u16DataBufferSizeInWords = ByteNumber >> 1;            // divide 2
        
    EALLOW;
    
    //
    //Give pump ownership to FMC0
    //
    //FlashPumpSemaphoreRegs.PUMPREQUEST.all = 0x5A5A0000;
    //
    //oReturnCheck = Fapi_initializeAPI(F021_CPU0_W0_BASE_ADDRESS, DSP_CLOCK_FREQ);
    //oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    	
    if (IsInFlashableRange(u32FlashAddr, u16DataBufferSizeInWords))
    {
        /*
        oReturnCheck = Fapi_issueProgrammingCommand((Uint32 *)u32FlashAddr,
		        										u16ProgramBuffer,
		        										u16DataBufferSizeInWords,
		        										0,
		        										0,
		        										Fapi_DataOnly);
                
		while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
		*/        
	    
	    for(loop = 0; loop < u16DataBufferSizeInWords; loop++)
        {
		    oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)(u32FlashAddr+loop),
		    											u16ProgramBuffer+loop,
		    											1,
		    											0,
		    											0,
		    											Fapi_DataOnly);    //Fapi_DataOnly
            
		    while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
		    
		    // Verify flash
            oReturnCheck = Fapi_doVerifyBy16bits((uint32 *)(u32FlashAddr+loop),1,u16ProgramBuffer+loop,&oFlashStatusWord);
		    
		    if(oReturnCheck != Fapi_Status_Success)
		    {
		        TR_FAIL();    // Response fail
		        break;
		    }
	    }
	    
    }
    EDIS;
    

    //if (oReturnCheck == Fapi_Status_Success)
    {
        TR_Success();    // Response success
    }
    //else
    //{
    //    TR_FAIL();    // Response fail
    //}
}


//---------------------------------------------------------------------------------------
// Finish Program Flash
//---------------------------------------------------------------------------------------
#pragma CODE_SECTION(Finish_Program, "BOOTLOAD_APP");
void Finish_Program(void)
{
    if (!SciFlag.sBit.EndOfData)          // End of data ?
    {
        return;    // exit
    }

    if (BootTimeBase1s == 0)             // 1S ? delay time to display message
    {
        return;
    }

    ProgramFlag.sBit.Verifying = 1;       // Verifying ?
    //WaitToDisplay(1);                    // Wait 1S to display

    ProgramFlag.sBit.VerifyOK = 1;        // VerifyOK ?
    //WaitToDisplay(1);                    // Wait 1S to display

    ProgramFlag.sBit.ProgramFinish = 1;   // Program finish
    //WaitToDisplay(2);                    // Wait 2S to display

    ProgramFlag.sBit.ProgramOver = 1;     // Program over and exit
    //WaitToDisplay(2);                    // Wait 2S to display
}


//---------------------------------------------------------------------------
// InitSysCtrl:
// This function initializes the System Control registers to a known state.
// - Disables the watchdog
// - Set the PLLCR for proper SYSCLKOUT frequency
// - Set the pre-scaler for the high and low frequency peripheral clocks
// - Enable the clocks to the peripherals
//---------------------------------------------------------------------------
#pragma CODE_SECTION(InitSysCtrl_Boot, "BOOTLOAD_APP");
void InitSysCtrl_Boot(void)
{

#ifdef _FLASH
    //
    // Copy time critical code and Flash setup code to RAM. This includes the
    // following functions: InitFlash()
    //
    // The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the device .cmd file.
    //
    //MemCopy_Boot(&BootRamfunsLoadStart, &BootRamfunsLoadEnd, &BootRamfunsRunStart);
    MemCopy_Boot(&BootRamfunsRunStart, &BootRamfunsLoadStart, (size_t)&BootRamfunsLoadSize);
    //
    // Call Flash Initialization to setup flash waitstates. This function must
    // reside in RAM.
    //
    InitFlash_Bank0_Boot();
    InitFlash_Bank1_Boot();
#endif

    //
    //      *IMPORTANT*
    //
    // The Device_cal function, which copies the ADC & oscillator calibration
    // values from TI reserved OTP into the appropriate trim registers, occurs
    // automatically in the Boot ROM. If the boot ROM code is bypassed during
    // the debug process, the following function MUST be called for the ADC and
    // oscillators to function according to specification. The clocks to the
    // ADC MUST be enabled before calling this function.
    //
    // See the device data manual and/or the ADC Reference Manual for more
    // information.
    //
    EALLOW;

    //
    // Enable pull-ups on unbonded IOs as soon as possible to reduce power
    // consumption.
    //
   // GPIO_EnableUnbondedIOPullups();


    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

    //
    // Check if device is trimmed
    //
    if(*((Uint16 *)0x5D1B6) == 0x0000)
    {
        //
        // Device is not trimmed--apply static calibration values
        //
        AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
    }

    CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
    EDIS;

    //
    // Initialize the PLL control: SYSPLLMULT and SYSCLKDIVSEL.
    //
    // Defined options to be passed as arguments to this function are defined
    // in F2837xS_Examples.h.
    //
    // Note: The internal oscillator CANNOT be used as the PLL source if the
    // PLLSYSCLK is configured to frequencies above 194 MHz.
    //
    //  PLLSYSCLK = (XTAL_OSC) * (IMULT + FMULT) / (PLLSYSCLKDIV)
    //

    //InitSysPll_Boot(XTAL_OSC, IMULT_40, FMULT_0, PLLCLK_BY_2); //10M OSC
    InitSysPll_Boot(XTAL_OSC,IMULT_20,FMULT_0,PLLCLK_BY_2);    //20M OSC

    //
    // Turn on all peripherals
    //
    InitPeripheralClocks_Boot();
}


//---------------------------------------------------------------------------
// Example: InitFlash:
// This function initializes the Flash Control registers
//                   CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
//---------------------------------------------------------------------------
#pragma CODE_SECTION(InitFlash_Bank0_Boot, "Boot_ramfuncs");
void InitFlash_Bank0_Boot(void)
{
    EALLOW;

    //
    // Set VREADST to the proper value for the flash banks to power up
    // properly. This sets the bank power up delay.
    //
    Flash0CtrlRegs.FBAC.bit.VREADST = 0x14;

    //
    // At reset bank and pump are in sleep. A Flash access will power up the
    // bank and pump automatically.
    //
    // After a Flash access, bank and pump go to low power mode (configurable
    // in FBFALLBACK/FPAC1 registers) if there is no further access to flash.
    //
    // Power up Flash bank and pump. This also sets the fall back mode of
    // flash and pump as active.
    //
    Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
    Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3;

    //
    // Disable Cache and prefetch mechanism before changing wait states
    //
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

    //
    // Set waitstates according to frequency
    //
    //      *CAUTION*
    // Minimum waitstates required for the flash operating at a given CPU rate
    // must be characterized by TI. Refer to the datasheet for the latest
    // information.
    //
#if CPU_FRQ_200MHZ
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;
#endif

#if CPU_FRQ_150MHZ
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
#endif

#if CPU_FRQ_120MHZ
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
#endif

    //
    // Enable Cache and prefetch mechanism to improve performance of code
    // executed from Flash.
    //
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1;

    //
    // At reset, ECC is enabled. If it is disabled by application software and
    // if application again wants to enable ECC.
    //
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;

    EDIS;

    //
    // Force a pipeline flush to ensure that the write to the last register
    // configured occurs before returning.
    //
    __asm(" RPT #7 || NOP");
}

#pragma CODE_SECTION(InitFlash_Bank1_Boot, "Boot_ramfuncs");
void InitFlash_Bank1_Boot(void)
{
    EALLOW;

    //
    // Set VREADST to the proper value for the flash banks to power up
    // properly. This sets the bank power up delay.
    //
    Flash1CtrlRegs.FBAC.bit.VREADST = 0x14;

    //
    // At reset bank and pump are in sleep. A Flash access will power up the
    // bank and pump automatically.
    //
    // After a Flash access, bank and pump go to low power mode (configurable
    // in FBFALLBACK/FPAC1 registers) if there is no further access to flash.
    //
    // Power up Flash bank and pump. This also sets the fall back mode of
    // flash and pump as active.
    //
    Flash1CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
    Flash1CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3;

    //
    // Disable Cache and prefetch mechanism before changing wait states
    //
    Flash1CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
    Flash1CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

    //
    // Set waitstates according to frequency
    //
    //      *CAUTION*
    // Minimum waitstates required for the flash operating at a given CPU rate
    // must be characterized by TI. Refer to the datasheet for the latest
    // information.
    //
#if CPU_FRQ_200MHZ
    Flash1CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;
#endif

#if CPU_FRQ_150MHZ
    Flash1CtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
#endif

#if CPU_FRQ_120MHZ
    Flash1CtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
#endif

    //
    // Enable Cache and prefetch mechanism to improve performance of code
    // executed from Flash.
    //
    Flash1CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
    Flash1CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1;

    //
    // At reset, ECC is enabled. If it is disabled by application software and
    // if application again wants to enable ECC.
    //
    Flash1EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;

    EDIS;

    //
    // Force a pipeline flush to ensure that the write to the last register
    // configured occurs before returning.
    //
    __asm(" RPT #7 || NOP");
}

//---------------------------------------------------------------------------
// Example: InitPll:
//---------------------------------------------------------------------------
// This function initializes the PLLCR register.

#pragma CODE_SECTION(InitSysPll_Boot, "BOOTLOAD_APP");
void InitSysPll_Boot(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
{
    Uint16 temp_divsel;

    if((clock_source == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)    &&
       (imult         == ClkCfgRegs.SYSPLLMULT.bit.IMULT)           &&
       (fmult         == ClkCfgRegs.SYSPLLMULT.bit.FMULT)           &&
       (divsel        == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
        //everything is set as required, so just return
        return;
    }

    // Make sure the PLL is not running in limp mode
    if (ClkCfgRegs.MCDCR.bit.MCLKSTS != 0)
    {
        EALLOW;
        // OSCCLKSRC2 failure detected. PLL running in limp mode.
        // Re-enable missing clock logic.
        ClkCfgRegs.MCDCR.bit.MCLKCLR = 1;
        EDIS;
        // Replace this line with a call to an appropriate
        // SystemShutdown(); function.
        __asm("        ESTOP0");     // Uncomment for debugging purposes
    }

    if(clock_source != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)
    {
        switch (clock_source)
        {
            case INT_OSC1:
                //SysIntOsc1Sel_Boot();
                EALLOW;
                ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 2;     // Clk Src = INTOSC1
                EDIS;
                break;

            case INT_OSC2:
                //SysIntOsc2Sel_Boot();
                EALLOW;
                ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF = 0;       // Turn on INTOSC2
                ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0;     // Clk Src = INTOSC2
                EDIS;
                break;

            case XTAL_OSC:
                //SysXtalOscSel_Boot();
                EALLOW;
                ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF = 0;          // Turn on XTALOSC
                ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;     // Clk Src = XTAL
                EDIS;
                break;
        }
    }

    // first modify the PLL multipliers
    if(imult != ClkCfgRegs.SYSPLLMULT.bit.IMULT || fmult != ClkCfgRegs.SYSPLLMULT.bit.FMULT)
    {
        EALLOW;
        //modfiy dividers to maximum to reduce the inrush current
        ClkCfgRegs.SYSPLLMULT.bit.IMULT = imult;     //Setting integer multiplier
        ClkCfgRegs.SYSPLLMULT.bit.FMULT = fmult;      //Setting fractional multiplier
        ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 1;                 //Enable SYSPLL
        EDIS;

        //Wait for the SYSPLL lock
        while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
        {
            // Uncomment to service the watchdog
            // ServiceDog_Boot();
        }
    }

    //increase the freq. of operation in steps to avoid any VDD fluctuations
    temp_divsel = divsel + 3;
    if(temp_divsel > PLLCLK_BY_126)
    {
        temp_divsel = PLLCLK_BY_126;
    }

    while(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV != divsel)
    {
        EALLOW;
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = temp_divsel;
        EDIS;
        temp_divsel = temp_divsel - 1;
        if(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV != divsel)
        {
            //DELAY_US(15L);    //Notice：don't call this function, can't use the function in ram!!!
            asm(" RPT #255 || NOP");
        }
    }

    //Enable PLLSYSCLK is fed from system PLL clock
    EALLOW;
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;
    EDIS;


}

//--------------------------------------------------------------------------
// Example: InitPeripheralClocks:
//---------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
// First the high and low clock prescalers are set
// Second the clocks are enabled to each peripheral.
// To reduce power, leave clocks to unused peripherals disabled
//
// Note: If a peripherals clock is not enabled then you cannot
// read or write to the registers for that peripheral

#pragma CODE_SECTION(InitPeripheralClocks_Boot, "BOOTLOAD_APP");
void InitPeripheralClocks_Boot(void)
{
    EALLOW;

    CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
    CpuSysRegs.PCLKCR0.bit.DMA = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;
    CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    CpuSysRegs.PCLKCR1.bit.EMIF1 = 1;
    CpuSysRegs.PCLKCR1.bit.EMIF2 = 1;

    //CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
    //CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;
    //
    //CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
    //CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
    //CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;
    //CpuSysRegs.PCLKCR3.bit.ECAP4 = 1;
    //CpuSysRegs.PCLKCR3.bit.ECAP5 = 1;
    //CpuSysRegs.PCLKCR3.bit.ECAP6 = 1;
    //
    //CpuSysRegs.PCLKCR4.bit.EQEP1 = 0;
    //CpuSysRegs.PCLKCR4.bit.EQEP2 = 0;
    //CpuSysRegs.PCLKCR4.bit.EQEP3 = 0;

    CpuSysRegs.PCLKCR6.bit.SD1 = 1;
    CpuSysRegs.PCLKCR6.bit.SD2 = 1;

    CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
    CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
    CpuSysRegs.PCLKCR7.bit.SCI_C = 1;
    CpuSysRegs.PCLKCR7.bit.SCI_D = 1;

    CpuSysRegs.PCLKCR8.bit.SPI_A = 0;
    CpuSysRegs.PCLKCR8.bit.SPI_B = 0;
    CpuSysRegs.PCLKCR8.bit.SPI_C = 0;

    CpuSysRegs.PCLKCR9.bit.I2C_A = 1;
    CpuSysRegs.PCLKCR9.bit.I2C_B = 1;

    CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
    CpuSysRegs.PCLKCR10.bit.CAN_B = 1;

    CpuSysRegs.PCLKCR11.bit.McBSP_A = 1;
    CpuSysRegs.PCLKCR11.bit.McBSP_B = 1;
    CpuSysRegs.PCLKCR11.bit.USB_A = 0;

    CpuSysRegs.PCLKCR12.bit.uPP_A = 1;

    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

    CpuSysRegs.PCLKCR14.bit.CMPSS1 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS3 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS4 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS5 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS6 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS7 = 0;
    CpuSysRegs.PCLKCR14.bit.CMPSS8 = 0;

    CpuSysRegs.PCLKCR16.bit.DAC_A = 0;
    CpuSysRegs.PCLKCR16.bit.DAC_B = 0;
    CpuSysRegs.PCLKCR16.bit.DAC_C = 0;

    EDIS;
}

//--------------------------------------------------------------------------
//          This function will copy the specified memory contents from
//          one location to another.
//
//          Uint16 *SourceAddr        Pointer to the first word to be moved
//                                    SourceAddr < SourceEndAddr
//          Uint16* SourceEndAddr     Pointer to the last word to be moved
//          Uint16* DestAddr          Pointer to the first destination word
//
//          No checks are made for invalid memory locations or that the
//          end address is > then the first start address.
//--------------------------------------------------------------------------
#pragma CODE_SECTION(MemCopy_Boot, "BOOTLOAD_APP");
void MemCopy_Boot(Uint16 *Runstart, Uint16 *LoadStart, Uint16 len)
{
    Uint16 i;
    
    for(i = 0;i<len;i++)
    {
        *Runstart++ = *LoadStart++;
    }
}


#pragma CODE_SECTION(CsmUnlock_Boot, "BOOTLOAD_APP");
//---------------------------------------------------------------------------------------
// Example_CsmUnlock
// Unlock the code security module (CSM)
// Parameters:
// Return Value:
//          STATUS_SUCCESS         CSM is unlocked
//          STATUS_FAIL_UNLOCK     CSM did not unlock
//---------------------------------------------------------------------------------------
Uint16 CsmUnlock_Boot()
{
    volatile Uint16 temp;
    
    //
    // Load the key registers with the current password. The 0xFFFF's are dummy
    // passwords.  User should replace them with the correct password for the
    // DSP.
    //
    EALLOW;
    DcsmZ1Regs.Z1_CSMKEY0 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY1 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY2 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY3 = 0xFFFFFFFF;

    DcsmZ2Regs.Z2_CSMKEY0 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY1 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY2 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY3 = 0xFFFFFFFF;
    EDIS;
}

#pragma CODE_SECTION(ProgBuzzer, "BOOTLOAD_APP");
//---------------------------------------------------------------------------------------
//Function Name:ProgBuzzer
//Argument [none]:
//Returns [none]:
//Description:
//---------------------------------------------------------------------------------------
void ProgBuzzer(void)
{
    u16ProgBuzzerCnt++;

    if(u16ProgBuzzerCnt >= 20)
    {
        u16ProgBuzzerCnt = 0;
    }

    if(u16ProgBuzzerCnt >= 18 )
    {
        BOOT_BUZZER_ON;
    }
    else
    {
        BOOT_BUZZER_OFF;
    }
}




