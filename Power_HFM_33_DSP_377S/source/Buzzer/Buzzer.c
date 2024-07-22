/*****************************************************************************

PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : Buzzer.c

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/
#include "F28x_Project.h"   // Device Headerfile and Examples Include File
#include "USR_VAR.H"
#include "eeprom.H"
#include "Buzzer.h"

//----------------------------------------------------------------------------
Uint16  u16BeepOnCnt;                   // Beep on count
Uint16  u16BeepOffCnt;                  // Beep off count
Uint16  u16BeepOnCntBuf;                // Beep on temp. count
Uint16  u16BeepOffCntBuf;               // Beep off temp. count

//----------------------------------------------------------------------------
// Golbal Variable
//----------------------------------------------------------------------------
union   UN_BEEP_FLAG sBeepFlag;                     // BEEP flag
union   UN_BEEP_SOUND sBeepSound;                   // BEEP sound select

Uint16  u16BuzzerTestTime;                  // Buzzer Test time

//----------------------------------------------------------------------------
// BEEPER DURATION
// Run it every 0.1s
//----------------------------------------------------------------------------
void BuzzerBeepOperation(void)
{
    //Buzzer Test
    if (sBeepFlag.sBit.u1BeepTest)           // Buzzer Test ?
    {
        if (u16BuzzerTestTime)               // Test time over ?
        {
            u16BuzzerTestTime--;

            BUZZER_ON;
        }
        else
        {
            sBeepFlag.sBit.u1BeepTest = 0;
            BUZZER_OFF;
        }
        return;
    }

    if (sBeepSound.sBit.u1OneBeep0p2s  ||
        sBeepSound.sBit.u1OneBeep5s   ||
        sBeepFlag.sBit.u1BiBi)
    {
        //do nothing
    }
    else if (sBeepSound.sBit.u1BeepSlow    ||
             sBeepSound.sBit.u1BeepSlow5s  ||
             sBeepSound.sBit.u1BeepSlow10s ||
             sBeepSound.sBit.u1BeepMid     ||
             sBeepSound.sBit.u1BeepFast)
    {
        //if (sEepDataInRam.sEeFlag2.sBit.AudibleAlarmDisable)
        {
            BUZZER_OFF;
            return;
        }
    }
    else
    {
        BUZZER_OFF;
        return;
    }
/*
    // Beep pulse ON/OFF
    if (u16BeepOffCntBuf == 0)
    {
        if (u16BeepOnCntBuf == 0)
        {
            if (sBeepFlag.sBit.u1BiBi)
            {
                if (sBeepFlag.sBit.u1BiBiTmp)
                {
                    sBeepFlag.sBit.u1BiBiTmp = 0;          // clear flag, beep again
                    u16BeepOnCntBuf = u16BeepOnCnt;        // restore beep on count
                    u16BeepOffCntBuf = u16BeepOffCnt;      // restore beep off count
                }
                else
                {
                    sBeepFlag.sBit.u1BiBi = 0;
                    BUZZER_OFF;
                    return;
                }
            }
            else
            {
                if (sBeepSound.sBit.u1OneBeep0p2s ||
                    sBeepSound.sBit.u1OneBeep5s)
                {
                    sBeepSound.sBit.u1OneBeep0p2s = 0;
                    sBeepSound.sBit.u1OneBeep5s  = 0;
                    BUZZER_OFF;
                    return;
                }

                u16BeepOnCntBuf = u16BeepOnCnt;             // restore beep on count
                u16BeepOffCntBuf = u16BeepOffCnt;           // restore beep off count
            }
        }

        if (u16BeepOnCntBuf)
        {
            u16BeepOnCntBuf--;

            if (!sEepDataInRam.sEeFlag2.sBit.AudibleAlarmDisable 	&&   // Audible alarm Enable ? 0=enable	1=disable
                !sBeepFlag.sBit.u1BeepPause  						&&
                sBeepSound.u16All)
            {
                BUZZER_ON;
            }
            else
            {
                BUZZER_OFF;
            }
        }
    }
    else
    {
        u16BeepOffCntBuf--;
        BUZZER_OFF;
    }
    */
    // Beep pulse ON/OFF
    if (u16BeepOnCntBuf == 0)
    {
        if (u16BeepOffCntBuf == 0)
        {
            if (sBeepFlag.sBit.u1BiBi)
            {
                if (sBeepFlag.sBit.u1BiBiTmp)
                {
                    sBeepFlag.sBit.u1BiBiTmp = 0;          // clear flag, beep again
                    u16BeepOnCntBuf = u16BeepOnCnt;        // restore beep on count
                    u16BeepOffCntBuf = u16BeepOffCnt;      // restore beep off count
                }
                else
                {
                    sBeepFlag.sBit.u1BiBi = 0;
                    BUZZER_OFF;
                    return;
                }
            }
            else
            {
                if (sBeepSound.sBit.u1OneBeep0p2s ||
                    sBeepSound.sBit.u1OneBeep5s)
                {
                    sBeepSound.sBit.u1OneBeep0p2s = 0;
                    sBeepSound.sBit.u1OneBeep5s  = 0;
                    BUZZER_OFF;
                    return;
                }

                u16BeepOnCntBuf = u16BeepOnCnt;             // restore beep on count
                u16BeepOffCntBuf = u16BeepOffCnt;           // restore beep off count
            }
        }
        else
        {
            u16BeepOffCntBuf--;
            BUZZER_OFF;
        }
    }
    else
    {
        if (u16BeepOnCntBuf)
        {
            u16BeepOnCntBuf--;

            if (//!sEepDataInRam.sEeFlag2.sBit.AudibleAlarmDisable 	&&   // Audible alarm Enable ? 0=enable	1=disable
                !sBeepFlag.sBit.u1BeepPause  						&&
                sBeepSound.u16All)
            {
                BUZZER_ON;
            }
            else
            {
                BUZZER_OFF;
            }
        }
        else
        {
            BUZZER_OFF;
        }
    }
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void BuzzerBeepCom(void)
{
    u16BeepOnCntBuf = u16BeepOnCnt;                   // store to u16BeepOnCntBuf
    u16BeepOffCntBuf = u16BeepOffCnt;                 // store to u16BeepOffCntBuf
}

//----------------------------------------------------------------------------
// BEEPER DURATION
//
// SLOW BEEP 10S: PERIOD  10S, ON 0.1S, OFF 9.9S
// SLOW BEEP  5S: PERIOD   5S, ON 0.1S, OFF 4.9S
// SLOW BEEP    : PERIOD   2S, ON 0.1S, OFF 1.9S
// MID  BEEP    : PERIOD   1S, ON 0.1S, OFF 0.9S
// FAST BEEP    : PERIOD 0.5S, ON 0.1S, OFF 0.4S
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Slow 10S Beep
//----------------------------------------------------------------------------
void BuzzerSlowBeep10s(void)
{
    if (sBeepSound.sBit.u1OneBeep5s)
    {
        return;
    }

    if (!sBeepSound.sBit.u1BeepSlow10s)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1BeepSlow10s = 1;
        sBeepFlag.sBit.u1BeepPause = 0;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 99;                    // off 9.9S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Slow 5S Beep
//----------------------------------------------------------------------------
void BuzzerSlowBeep5s(void)
{
    if (!sBeepSound.sBit.u1BeepSlow5s)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1BeepSlow5s = 1;
        sBeepFlag.sBit.u1BeepPause = 0;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 49;                    // off 4.9S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Slow Beep
//----------------------------------------------------------------------------
void BuzzerSlowBeep(void)
{
    if (sBeepSound.sBit.u1OneBeep5s)
    {
        return;
    }

    if (!sBeepSound.sBit.u1BeepSlow)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1BeepSlow = 1;
        sBeepFlag.sBit.u1BeepPause = 0;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 19;                    // off 1.9S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Mid Beep
//----------------------------------------------------------------------------
void BuzzerMiddleBeep(void)
{
    if (sBeepSound.sBit.u1OneBeep5s)
    {
        return;
    }

    if (!sBeepSound.sBit.u1BeepMid)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1BeepMid = 1;
        sBeepFlag.sBit.u1BeepPause = 0;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 9;                     // off 0.9S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Fast Beep
//----------------------------------------------------------------------------
void BuzzerFastBeep(void)
{
    if (sBeepSound.sBit.u1OneBeep5s)
    {
        return;
    }

    if (!sBeepSound.sBit.u1BeepFast)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1BeepFast = 1;
        sBeepFlag.sBit.u1BeepPause = 0;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 4;                     // off 0.4S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Bi Bi
//----------------------------------------------------------------------------
void BuzzerBibiBeep(void)
{
    if (!sBeepFlag.sBit.u1BiBi)          // Have BiBi ?
    {
        sBeepFlag.sBit.u1BiBi = 1;
        sBeepFlag.sBit.u1BiBiTmp = 1;
        u16BeepOnCnt = 1;                      // on 0.1S
        u16BeepOffCnt = 1;                     // off 0.1S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// 0.2S Beep
//----------------------------------------------------------------------------
void BuzzerShort0p2sBeep(void)
{
    if (!sBeepSound.sBit.u1OneBeep0p2s)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1OneBeep0p2s = 1;
        u16BeepOnCnt = 2;                      // on 0.2S
        u16BeepOffCnt = 0;                     // off 0.0S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// 5S Beep
//----------------------------------------------------------------------------
void BuzzerShort5sBeep(void)
{
    if (!sBeepSound.sBit.u1OneBeep5s)
    {
        sBeepSound.u16All = 0;
        sBeepSound.sBit.u1OneBeep5s = 1;
        u16BeepOnCnt = 50;                     // on 5.0S
        u16BeepOffCnt = 0;                     // off 0.0S
        BuzzerBeepCom();
    }
}

//----------------------------------------------------------------------------
// Clear Beep
//----------------------------------------------------------------------------
void BuzzerClearBeep(void)
{
    sBeepSound.u16All = 0;
    sBeepFlag.sBit.u1BeepPause = 0;
    u16BeepOnCnt = 0;                         // on 0.0S
    u16BeepOffCnt = 0;                        // off 0.0S
}



//----------------------------------------------------------------------------
// buzzer setup
//----------------------------------------------------------------------------
void BuzzerSetup(void)
{/*
    if (sBeepSound.sBit.u1OneBeep5s ||			// Beep 5S   over ?
        sBeepSound.sBit.u1OneBeep0p2s)			// Beep 0.2S over ?
    {
        // beep, do nothing
    }
    else if (sBackupFlag.sBit.u1TestInDcMode ||
             (sQualityFlag.sInput.sBit.u1InputLoss &&				// DCBUS discharge
              sQualityFlag.sBypassOp.sBit.u1BypassVoltAbnormal &&
              sUpsMode.sBit.BYPASS_MODE))
    {
        BuzzerClearBeep();
    }
    else if (sLoadPercentFlag.sBit.u1Load150 ||
             sLoadPercentFlag.sBit.u1Load125 ||
             (sUpsMode.sBit.DC_MODE && sQualityFlag.sBattery.sBit.u1BattLow))
    {
        BuzzerFastBeep();
    }
    else if ((sUpsMode.sBit.FAULT_MODE && !sQualityFlag.sBattery.sBit.u1BattShutdown) ||
             sIconLcd.fIconFlag.showErrCode         ||
             sVersionFlag.sBit.UpsCompatibleNOK	    ||      	// UPS1/UPS2 Version code Compatible NOK ?
             sVersionFlag.sBit.VACompatibleNOK		||          // UPS1/UPS2 VA Compatible NOK ?
             sCanModule.PAR_ERR.sBit.u1IdError      ||
             sParallelLoadFlag.sBit.u1LoadUnbalance ||
             sRedundancyFlag.sBit.u1RedundancyLost  ||
             sQualityFlag.sBattery.sBit.u1BattReverse ||
             sQualityFlag.sBattery.sBit.u1BattOverChg ||
             sQualityFlag.sCharger.sBit.u1ChgFail  ||
             sQualityFlag.sCharger.sBit.u1ChgRelayOpen ||
             sQualityFlag.sCharger.sBit.u1ChgRelayStick ||
             sQualityFlag.sFault.sBit.u1FanFail	 ||
             sQualityFlag.sWarning.sBit.u1OverloadAlarm ||
             sLoadPercentFlag.sBit.u1UserOverloadAlarm  ||
             sQualityFlag.sBattery.sBit.u1BattNeedReplace ||
             (!sBattTestFlag.sBit.TEST_IN && sQualityFlag.sBattery.sBit.u1BattTestFail) ||
             ((sQualityFlag.sBypassOp.sBit.u1BypassVoltAbnormal || sQualityFlag.sBypassOp.sBit.u1BypassOutofFreq) && (!sEepDataInRam.sEeFlag2.sBit.FREQ_CONVERTER) && !sUpsMode.sBit.INIT_MODE) ||   // Bypass Freq out of range and not lock ?
             //sQualityFlag.sOTP.sBit.u1HighTempWarning  ||
             //sQualityFlag.sSCR.sBit.u1BpsSCROpenFastFail ||
             sQualityFlag.sFault.sBit.u1IpFuseBroken ||
             sQualityFlag.sFault.sBit.u1IpFuse2Broken ||
             (sUpsMode.sBit.BYPASS_MODE &&  // 旁路模式输出关闭时告警
              !sEepDataInRam.sEeFlag2.sBit.FREQ_CONVERTER 	&& 	
              !sTimesFlag.sBit.u1TurnOffToStandby 	&&
              !sQualityFlag.sInput.sBit.u1InputLoss	&&	// AC Not Loss ?
              (u16BypassOffDelayBuzzerTimer100ms == 0)	&&	
              !sStaticFlag.sBit.u1OnBypass) ||          // Bypass SCR On ?
             sUpsMode.sBit.DC_MODE)
    {
        BuzzerSlowBeep();
    }
    else
    {
        BuzzerClearBeep();
    }
*/
}


//----------------------------------------------------------------------------
// timebase: 100ms
//----------------------------------------------------------------------------
void TaskBuzzerControl(void)
{
    BuzzerSetup();
    BuzzerBeepOperation();
}



