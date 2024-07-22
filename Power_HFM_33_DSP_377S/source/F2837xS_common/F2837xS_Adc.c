//###########################################################################
//
// FILE:   F2837xS_Adc.c
//
// TITLE:  F2837xS Adc Support Functions.
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



//
// AdcSetMode - Set the resolution and signalmode for a given ADC. This will
//              ensure that the correct trim is loaded.
//
void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
    Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
    Uint16 adcOffsetTrim;         //temporary ADC offset trim

    //
    //re-populate INL trim
    //
    CalAdcINL(adc);

    if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP))
    {
        //
        //offset trim function is programmed into OTP, so call it
        //

        //
        //calculate the index into OTP table of offset trims and call
        //function to return the correct offset trim
        //
        adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
        adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
    }
    else
    {
        //
        //offset trim function is not populated, so set offset trim to 0
        //
        adcOffsetTrim = 0;
    }

    //
    //Apply the resolution and signalmode to the specified ADC.
    //Also apply the offset trim and, if needed, linearity trim correction.
    //
    switch(adc)
    {
        case ADC_ADCA:
            AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
                AdcaRegs.rsvd5[3] = 0x7000;
            }
        break;
        case ADC_ADCB:
            AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
                AdcbRegs.rsvd5[3] = 0x7000;
            }
        break;
        case ADC_ADCC:
            AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
                AdccRegs.rsvd5[3] = 0x7000;
            }
        break;
        case ADC_ADCD:
            AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_RESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
                AdcdRegs.rsvd5[3] = 0x7000;
            }
        break;
    }
}

//
// CalAdcINL - Loads INL trim values from OTP into the trim registers of the
//             specified ADC. Use only as part of AdcSetMode function, since
//             linearity trim correction is needed for some modes.
//
void CalAdcINL(Uint16 adc)
{
    switch(adc)
    {
        case ADC_ADCA:
            if(0xFFFF != *((Uint16*)CalAdcaINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcaINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCB:
            if(0xFFFF != *((Uint16*)CalAdcbINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcbINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCC:
            if(0xFFFF != *((Uint16*)CalAdccINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdccINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCD:
            if(0xFFFF != *((Uint16*)CalAdcdINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcdINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
    }
}

#define ACQPS_TIME    (64)

static void SetADCaSOC(void)
{
    // channel selection           
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;      //                 
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;      //                 
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;      //                 
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;      //                  
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;      //                 
    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;      //                 
    AdcaRegs.ADCSOC6CTL.bit.CHSEL = 14;     // 1.65v                 
    //AdcaRegs.ADCSOC7CTL.bit.CHSEL = 0;                              


    // Acquisition window
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_TIME; //SOC0 will use sample duration of 15 SYSCLK cycles (at least 1 ADCCLK)
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC6CTL.bit.ACQPS = ACQPS_TIME;
    AdcaRegs.ADCSOC7CTL.bit.ACQPS = ACQPS_TIME;


    // trigger source
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    //AdcaRegs.ADCSOC7CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA

}

static void SetADCbSOC(void)
{
    // channel selection
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;          // 
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;          // 
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;          // 
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;          // 
    
    
    // Acquisition window
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_TIME; //SOC0 will use sample duration of 1 SYSCLK cycles (at least 1 ADCCLK)
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_TIME;
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_TIME;
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_TIME;
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_TIME;


    // trigger source
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA

}

static void SetADCcSOC(void)
{
    // channel selection
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;  // 
    AdccRegs.ADCSOC1CTL.bit.CHSEL = 3;  // 
    AdccRegs.ADCSOC2CTL.bit.CHSEL = 4;  // 
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 14; // 
    AdccRegs.ADCSOC4CTL.bit.CHSEL = 14; // 


    // Acquisition window
    AdccRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_TIME; //SOC0 will use sample duration of 1 SYSCLK cycles (at least 1 ADCCLK)
    AdccRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_TIME;
    AdccRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_TIME;
    AdccRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_TIME;
    AdccRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_TIME;
    AdccRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_TIME;


    // trigger source
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA

}

static void SetADCdSOC(void)
{
    // channel selection
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;     // 
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;     // 
    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;     // 
    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;     // 
    AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;     // 
      
                                          
    // Acquisition window
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_TIME; //SOC0 will use sample duration of 1 SYSCLK cycles (at least 1 AdcdLK)
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_TIME;
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_TIME;
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_TIME;
    AdcdRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_TIME;
    AdcdRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_TIME;


    // trigger source
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = 0x0B; //trigger on ePWM4 SOCA

}

static void SetADCSOC(void)
{
    SetADCaSOC();
    SetADCbSOC();
    SetADCcSOC();
    SetADCdSOC();
}

//---------------------------------------------------------------------------
// InitAdc:
// This function initializes ADC to a known state.
//---------------------------------------------------------------------------
void InitAdc(void)
{
    EALLOW;

    // setting ADCCLK
    AdcaRegs.ADCCTL2.bit.PRESCALE = 8; // ADCCLK = (SYSCLK)200M / 5 = 40MHz
    AdcbRegs.ADCCTL2.bit.PRESCALE = 8;
    AdccRegs.ADCCTL2.bit.PRESCALE = 8;
    AdcdRegs.ADCCTL2.bit.PRESCALE = 8;

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    SetADCSOC();

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
    
}

void MyAdcInit(void)
{
    /*-> User Application <-*/
    EALLOW;
    // only one EOC can trigger interrupt
    
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 6; //end of EOC6 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0;   //disable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 1;
    
    EDIS;
}
//
// End of file
//
