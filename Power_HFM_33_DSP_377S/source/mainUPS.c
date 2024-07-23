/*****************************************************************************
Coding   ��ANSI ��������GBK
PROJECT  : Source

COMPILER : Code Composer Studio V8.2.0.00007

MODULE   : mainUPS.c

VERSION  : V0.0

DATE     : 2018/10/05

AUTHOR   : ZHOU;

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :

******************************************************************************/
#include "F28x_Project.h"
#include "math.h"
#include "mainUPS.h"
#include "USR_VAR.H"
#include "Booter.h"
#include "Buzzer.h"
//----------------------------------------------------------------------------

//flag

//----------------------------------------------------------------------------
extern Uint16 ebssStart[];    //The symbol is created by the linker. Refer to the *.cmd file.
extern Uint16 ebssEnd[];      //The symbol is created by the linker. Refer to the *.cmd file.
extern Uint16 claDataStart[];
extern Uint16 claDataEnd[];

extern void c_int00(void);  //The function is defined in rts2800_ml.lib
void My_Init(void);
void RamClear(void);
#pragma CODE_SECTION(UserAppEntry, "USER_APP_ENTRY");
void UserAppEntry(void)
{
    EALLOW;
    //DcsmCommonRegs.FLSEM.all = 0x0A503;
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0x00;//����flash�Զ�����
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;//��д�ȴ�����3��ʱ��
    Flash0CtrlRegs.FBAC.all = 0x14;//��������20��ʱ��
    //Flash0CtrlRegs.FRD_INTF_CTRL.all = 0x3;
    EDIS;//��Ȩ�Ĵ�������
    
    RamClear(); //�������ram�ڴ棿
    c_int00();  //������������ȷ��ʼ��
}

void RamClear(void)
{
    Uint16 count;
    Uint16 *pu16RAMAddress;

    pu16RAMAddress = ebssStart;
    
    // MUST clear .ebss area before c_int00(),or rts2800_ml.lib may not work 

    for(count = 0; count<0x7000;count++)
    {
        *(pu16RAMAddress+count) = 0;
    }//����7000����ַ�����ram�ڴ�
    
    //pu16RAMAddress = claDataStart;
    //for(count = 0; count<0x1000;count++)
    //{
    //    *(pu16RAMAddress+count) = 0;
    //}
    
    
    /*pu16RAMAddress = (int *)0x001480;
    for(count = 0; count<0x0080;count++)
    {
        *(pu16RAMAddress+count) = 0;
    }
    
    pu16RAMAddress = (int *)0x001500;
    for(count = 0; count<0x0080;count++)
    {
        *(pu16RAMAddress+count) = 0;
    }*/
                   
}
//----------------------------------------------------------------------------
void main(void)
{
	static Uint32 u32mainStartIsrTimeCnt,u32mainEndIsrTimeCnt;
    static Uint32 u16mainLastCpuIsrTime;
    //  u32mainStartIsrTimeCnt������һ���޷���32λ�������͵ı��������ڼ�¼�жϿ�ʼʱ��ʱ����������������ڲ����жϴ�������ִ��ʱ����������ж���ص�ʱ����㡣
    //	u32mainEndIsrTimeCnt  ������һ���޷���32λ�������͵ı��������ڼ�¼�жϽ���ʱ��ʱ����������������ڲ����жϴ�������ִ��ʱ����������ж���ص�ʱ����㡣
    //	u16mainLastCpuIsrTime ������һ���޷���16λ�������͵ı��������ڼ�¼��һ��CPU�жϵ�ʱ�䡣���������ڼ�������CPU�ж�֮���ʱ������������CPU�ж���ص�ʱ����㡣

    //Step 1. ��ʼ��ϵͳ���ƣ� PLL�����Ź���ʹ������ʱ�� 
    // ���ʾ������������F2837xS_SysCtrl.c�ļ����ҵ���
    InitSysCtrl();

    // Step 2. Initialize GPIO:
    // This example function is found in the F2837xS_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    // ��2������ʼ��GPIO�� 
    /// ���ʾ������������F2837xS_Gpio.c�ļ����ҵ�����չʾ����ν�GPIO����ΪĬ��״̬��
    InitGpio();//-gpio.c
    //InitTzGpio();
    InitEPwmGpio();//��ʼ��epwm����ǿ�������ȵ��ƣ�����
    InitSciGpio();//��ʼ��spi������
    InitECapGpio();//��ʼ��ECAPģ�������
    InitI2cGpio();//��ʼ��12cģ�������

    //cb, stb, 38ma
    //ClearRam();

    // Step 3. ��������жϲ���ʼ��PIE������: 
    //         �ر�CPU�ж�
    DINT;


    // ��ʼ��PIE���ƼĴ���ΪĬ��״̬�� 
    // Ĭ��״̬�����е�PIE __interrupts���úͱ�־������� 
    // �ú�����F2837xS_PieCtrl.c�ļ����ҵ���
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    //�ر�CPU�жϲ��������CPU�жϱ�־:
    IER = 0x0000;
    IFR = 0x0000;


    // ��ʼ������ָ��shell Interruptָ���PIE������ 
    // �������(ISR) 
    // �⽫�����������ʹ__interrupt �ڱ�����û��ʹ�á�����ڵ��Ժ����á� 
  
    InitPieVectTable();

    //InitIntrupt();  //for Inv Ipeak detected
    // 
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //   EALLOW;  
    //   PieVectTable.EPWM1_INT = &epwm1_timer_isr;
    //   PieVectTable.EPWM2_INT = &epwm2_timer_isr;
    //   PieVectTable.EPWM3_INT = &epwm3_timer_isr;
    //   EDIS;    
    
    My_Init();                           // Initialization��ʼ��

 
    // ���� 4���û�ר�ô���:
    InitAdc();    //��ʼ��ģ��ת�������������ʣ��ο���ѹ��ͨ��    
    //����InitAdc()���õĺ���֮һ����SRAM���У������Ҫ�ڵ��øú���֮ǰ����������渴�Ƶ�SRAM
    MyAdcInit();//adc���棬����ģʽ
    
    InitEPwm();//��ʼ��pwm
    //InitECan();
    InitECap();//��ʼ��ecap���ⲿ������

    InitCpuTimers();   //������ֻ��ʼ��Cpu��ʱ����ʱ��Դ����������

    // Configure CPU-Timer 0 NOT interrupt every 16.67ms(default 60hz), system sync out signal(sync with HW)
    // 200MHz CPU Freq, count (in 0.1uSeconds)
    // ����CPU-Timer 0 NOT�ж�ÿ16.67ms(Ĭ��60hz)��ϵͳͬ���ź�(��Ӳ��ͬ��) 
    // 200MHz CPUƵ�ʣ�����(0.1 usseconds)
    ConfigCpuTimer(&CpuTimer0, 200, CPU_TIMER_ISR_CNT,0); 
    //����CPU��ʱ��0�Ĳ�����
    // ���У�& CpuTimer0��ʾ��CPU��ʱ��0�������ã�
    // 200��ʾ��ʱ����ʱ��Ƶ��Ϊ200MHz��
    // CPU_TIMER_ISR_CNT��ʾ��ʱ���жϵļ���ֵ��
    // 0��ʾ�������Զ����ع��ܡ�
    //for INV period(in "SineGen()")
    ConfigCpuTimer(&CpuTimer1, 200, SYS_SYNC_50HZ_CNT,1); 
    //����CPU��ʱ��1�Ĳ�����
    // ���У�& CpuTimer1��ʾ��CPU��ʱ��1�������ã�
    // 200��ʾ��ʱ����ʱ��Ƶ��Ϊ200MHz��
    // SYS_SYNC_50HZ_CNT��ʾ��ʱ���жϵļ���ֵ��
    // ������ͬ�����߽���ͬ����
        //sync with sync bus in
    //ConfigCpuTimer(&CpuTimer2, 200, SYS_SYNC_50HZ_CNT,1);
    CpuTimer0.RegsAddr->TCR.bit.TIE = 1; //����CPU��ʱ��0���ж�ʹ��λ������ʱ��0�����жϡ�
    StartCpuTimer0();//����CPU��ʱ��0����ʼ��ʱ��
    StartCpuTimer1();//����CPU��ʱ��1����ʼ��ʱ��
    //StartCpuTimer2();

    //communication
    InitSci(); 
    InitI2c();
    //InitCan();


    // Step 5. User specific code, enable interrupts:
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    //InitFlash();
    //����5���û��ض����룬�����ж�: 
    //����Flash Initialization������Flash�ȴ�״̬ //
    //�ú�������פ����RAM�� / / 
    //InitFlash ();


    // Step 6. IDLE loop. Just sit and loop forever (optional):
    //����6������ѭ����ֻ��������Զѭ��(��ѡ) :

    //---------------------------------------
    //InitBooterCAN();
   

    // Enable CPU INT1 which is connected to ADC INT:
    //IER |= (M_INT1 | M_INT12);            // Enable CPU Interrupt 1 and 12
    IER |= (M_INT1 | M_INT3);

    //Enable ADCA1_ISR in the PIE: Group 1 interrupt 1
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  //Enable PIE Group 1 ADCA1pie�ж�
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //Enable PIE Group 1 TIMER0
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    //PieCtrlRegs.PIEIER12.bit.INTx1 = 1; // Enable PIE Group 12 INT1(XINT3)
    //PieCtrlRegs.PIEIER12.bit.INTx2 = 1; // Enable PIE Group 12 INT2(XINT4)
    //PieCtrlRegs.PIEIER12.bit.INTx3 = 1; // Enable PIE Group 12 INT3(XINT5)
    
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;                       // Enable Global interrupt INTM//ȫ���жϡ�
    ERTM;                       // Enable Global realtime interrupt DBGM ???aʵʱ����

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    HarmDataInit();//��ʼ��г������-source
    Clear_Set_Flag();//����������������ñ�־λ-source
    INIT_VAR();//��ʼ������-source
    
    //---------------------------------------
    //1> Check & Read EEPROM;
    //2> If Read EEPROM Fail, Read Default Value!
    Read_Ex_EEPROM_Parameter();//����������ڼ��Ͷ�ȡ�ⲿEEPROM������-eeprom
    
    MBus_InitialModbus(); //���ڳ�ʼ��Modbusͨ��Э�顪modbus
    ClearTransferPoint(); //-wavelog������������������㡣�ڴ����п��ܴ���һЩ�������ݴ���Ļ����������������������ܻὫ��Щ���������������

    //-----------------------------------------------------------------------------
    // Main Loop��ѭ��
    //-----------------------------------------------------------------------------
    while(1)
    {
        //if(SourceControlFlag.sBit.RepairOn)
		//	MAIN_LOOP_1;

		TimeBaseSystem();//ʱ��У׼-source
		
		TaskRmsCalc();//������-measurement
		Scia_ISR();//isr�ж�
		Scic_ISR();
		Scid_ISR();
		TaskModbusCommunication();//���ƽ����ͨ�š�modbus
		
		HarmonicCalc(); //г������-source
		
		if(SourceControlFlag.sBit.OutputOn)
		{
			Check_Over_Current();//����
			Check_Over_Load();//����
			Check_Over_Temperature();//����
		}
		Check_DCBUS_VOL();     //ֱ��ĸ�ߵ�ѹ   
		TaskDryContact();//	�ɽӵ�����
   

        #ifdef DEBUG_OPEN_LOOP
        
		//-------------------------------------------
		//for debug use:fixed pwm duty
		//-------------------------------------------
		DINT;
		i32Inv1AmpVref_R = 60000;
		i32Inv1AmpVref_S = 60000;
		i32Inv1AmpVref_T = 60000;
		EINT;
		//INVERTER_ENABLE_R;
		//INVERTER_ENABLE_S;
		//INVERTER_ENABLE_T;
		//SourceControlFlag.sBit.OutputOn = 1;
		//-------------------------------------------
        PFC_ENABLE;
        #else
        
        Program_Running();//�����ѹ��Ƶ�ʵı仯
       
        
        InvRmsVolController();//�����ѹRMS������

		#endif

		Adjust_Frequency();//Ƶ�ʵ���-source

        //if(SourceControlFlag.sBit.RepairOn)
		//	MAIN_LOOP_0;

        if(SciFlag.sBit.PROGRAM_MODE)
        {
            if(!CHK_INVERTER_ENABLE)
            {
                DINT;
                INVERTER_DISABLE_R;
                INVERTER_DISABLE_S;
                INVERTER_DISABLE_T;
			    DC2DC_OFF;
			    //PFC_OFF;

                Bloader();
            }
            else
            {
                SciFlag.sBit.PROGRAM_MODE = 0;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
ROUTINE NAME : My_Init
INPUT/OUTPUT : None
DESCRIPTION  : Initialization
-----------------------------------------------------------------------------*/
void My_Init(void)
{
    sBeepSound.u16All = 0;
    
    // User variable initialization
    //���߶˵�ѹ�����r s t
    sSet.u16OutputVoltage_R = 1150;
    sSet.u16OutputVoltage_S = 1150;
    sSet.u16OutputVoltage_T = 1150;
    
    #ifdef HIGH_FREQ_OUTPUT
    sSet.u16OutputFrequency = NORMAL_OP_FREQ_LIMIT; //Ĭ��400.0Hz
    sSet.u32OpPeriod = (Uint32)(((Uint64)(2000000000)) / (sSet.u16OutputFrequency)); //��������
    #else
    sSet.u16OutputFrequency = NORMAL_OP_FREQ_LIMIT; //Ĭ��50.00Hz
    sSet.u32OpPeriod = (Uint32)(((Uint64)(20000000000)) / (sSet.u16OutputFrequency)); 
    #endif
       
    
    sSet.u16OutputRange = 2;			//�޸�Ĭ�ϳ���Ϊ�ߵ�
}

