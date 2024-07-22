/*****************************************************************************
Coding   ：ANSI 简体中文GBK
PROJECT  : High Frequncy Ac source

COMPILER : Code Composer Studio V8.0.0.00016

MODULE   : USR_VAR.h

VERSION  : V0.0

DATE     : 2018/05/10

AUTHOR   : ZHOU

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

DESCRIPTION   :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

MODIFICATIONS :


******************************************************************************/

#ifndef USR_VAR_H
#define USR_VAR_H

#include "F28x_Project.h"   // Device Headerfile and Examples Include File

/*****************************************************************************
 Conditional Compiler
*****************************************************************************/

/*****************************************************************************
 Conditional Compiler
*****************************************************************************/
//#define 	    RTC_ENABLE				// Enable RTC function

//#define 	    KVA_SEL_SELECTABLE		// Selectable

//#define	        DEBUG_NO_PROTECT
//#define         DEBUG_OPEN_LOOP


//#define		DC_OFFSET_ENABLE

//#define 		POWER_RATING_3K  
//#define 		POWER_RATING_5K
//#define 		POWER_RATING_6K
//#define 		POWER_RATING_9K     
//#define 		POWER_RATING_30K
//#define 		POWER_RATING_30K_S01
//#define 		POWER_RATING_30K_S02
//#define 		POWER_RATING_45K_S01     
#define 		POWER_RATING_60K_S01
//#define 		POWER_RATING_90K
//#define 		POWER_RATING_90K_S01    //带过流保护时间
//#define 		POWER_RATING_100K_S01

#define			FIRMWARE_VERSION	9		//1.00*100=100

//----------------------------------------------------------------------------

#define         DELAY_ON_100MS_CNT      50      //N*100ms

//----------------------------------------------------------------------------
#define DSP_CLOCK_FREQ              (200)       // in MHz
//#define INV_PWM_FREQ                (10.0)      // in kHz
#define INV_PWM_FREQ                (8.0)      // in kHz

#define INV_SW_HALF_PERIOD          ((Uint16)((Uint32)DSP_CLOCK_FREQ * 1000 / INV_PWM_FREQ) / 2)      // up-down counter            //50us/0.01/2 = 2500(100MHz),  1000000/10k/0.005/2=10000L
#define INV_DUTY_HIGH_LIMIT         (int32)(((INV_SW_HALF_PERIOD/2.)*0.98))
#define INV_DUTY_LOW_LIMIT          (int32)(-((INV_SW_HALF_PERIOD/2.)*0.98))

#define FAN_PWM_PERIOD              INV_SW_HALF_PERIOD
#define IPEAK_PWM_PERIOD            (INV_SW_HALF_PERIOD/2)
#define BUZZER_SW_HALF_PERIOD       (31234)     // 1000000/2.4k/0.00667/2 = 31234

#define SYS_SYNC_50HZ_CNT           (25000L)   // 25ms
#define CPU_TIMER_ISR_CNT           (11111L)   // 125us 

 
                                    

//*****************************************************************************
// Power rating
//*****************************************************************************
#ifdef POWER_RATING_3K
    #define POWER_RATING			40
    #define DETAIL_MODEL_TYPE		0    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		11389
    #define GAIN_BUS   		  		7832
    #define GAIN_INV   		 		8489  
    #define GAIN_OPV   		 		8489
    #define GAIN_OPI   		 		3175
    #define GAIN_WAT   		 		10785
    #define GAIN_PWM   		 		7955	// ~=65536*0.1214
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			15          // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			18
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.0v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 150.0v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 300.0v    
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 300.0v
    
    #define	HIGH_OP_CURRENT_LIMIT	100		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	100	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        1
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			8   //(0.1A)
    #define	NO_WATT_POINT			80
    #define	NO_VA_POINT				80
    
    #define	HSINK_OTP				70
    #define	AMB_OTP					60
    
    //Power
    #define	OP_POWER_PF				100				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_5K
    #define POWER_RATING			50
    #define DETAIL_MODEL_TYPE		0    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		11389
    #define GAIN_BUS   		  		7832
    #define GAIN_INV   		 		8489  
    #define GAIN_OPV   		 		8489
    #define GAIN_OPI   		 		3175
    #define GAIN_WAT   		 		10785
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			15          // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			18
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.0v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 150.0v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 300.0v
    
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 300.0v
    
    #define	HIGH_OP_CURRENT_LIMIT	145		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	145	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        1
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			8   //(0.1A)
    #define	NO_WATT_POINT			80
    #define	NO_VA_POINT				80
    
    #define	HSINK_OTP				70
    #define	AMB_OTP					60
    
    //Power
    #define	OP_POWER_PF				100				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)
    
#elif defined POWER_RATING_6K
    #define POWER_RATING			60
    #define DETAIL_MODEL_TYPE		0    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		11389
    #define GAIN_BUS   		  		7832
    #define GAIN_INV   		 		8489  
    #define GAIN_OPV   		 		8489
    #define GAIN_OPI   		 		3175
    #define GAIN_WAT   		 		10785
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			15          // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			18
    
    #define	SMALL_OP_VOLT_LIMIT		0		// 0.0v
    #define	LOW_OP_VOLT_LIMIT	    1500	// 150.0v
    #define	HIGH_OP_VOLT_LIMIT	    3000	// 300.0v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1500	// 300.0v
    
    #define	HIGH_OP_CURRENT_LIMIT	250		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	250	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        1
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_OTP				70
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				100				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_9K
    #define POWER_RATING			90
    #define DETAIL_MODEL_TYPE		0    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		8615
    #define GAIN_INV   		 		9338  
    #define GAIN_OPV   		 		9338
    #define GAIN_OPI   		 		8995
    #define GAIN_WAT   		 		18455
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			15          // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			18
    
    #define	SMALL_OP_VOLT_LIMIT		0		// 0.0v
    #define	LOW_OP_VOLT_LIMIT	    1500	// 150.0v
    #define	HIGH_OP_VOLT_LIMIT	    3000	// 300.0v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1500	// 300.0v
    
    #define	HIGH_OP_CURRENT_LIMIT	250		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	250	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        1
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_OTP				70
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				100				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_30K
    #define POWER_RATING			300
    #define DETAIL_MODEL_TYPE		0    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		12778
    #define GAIN_WAT   		 		11996
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			19      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			17
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 0.1v
    
    #define	HIGH_OP_CURRENT_LIMIT	870		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	870	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)
    
#elif defined POWER_RATING_30K_S01
    #define POWER_RATING			300
    #define DETAIL_MODEL_TYPE		1    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		12778
    #define GAIN_WAT   		 		11996
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			19      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			17
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 0.1v
    
    #define OP_PROTECT_SLOW         1       //允许不同负载条件下的的超载时间保护
    #define	HIGH_OP_CURRENT_LIMIT	870		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	870	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_30K_S02
    #define POWER_RATING			300
    #define DETAIL_MODEL_TYPE		2    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		12778
    #define GAIN_WAT   		 		11996
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			19      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			17
    
    #define	SMALL_OP_VOLT_LIMIT		50		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    3000	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    3000	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	3000	// 0.1v
    
    #define OP_PROTECT_SLOW         1       //允许不同负载条件下的的超载时间保护
    #define	HIGH_OP_CURRENT_LIMIT	870		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	870	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_45K_S01
    #define POWER_RATING			450
    #define DETAIL_MODEL_TYPE		1    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    #define	LOW_OP_FREQ_LIMIT	        3000		// 300.0Hz
    #define	NORMAL_OP_FREQ_LIMIT	    4000		// 400.0Hz
    #define	HIGH_OP_FREQ_LIMIT	        5000		// 500.0Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		12778
    #define GAIN_WAT   		 		11996
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			19      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			17
    
    #define	SMALL_OP_VOLT_LIMIT		50		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1500	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1500	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1500	// 0.1v
    
    #define	HIGH_OP_CURRENT_LIMIT	1300		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	1300	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*105/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*105/3)

#elif defined POWER_RATING_60K_S01
    #define POWER_RATING			600
    #define DETAIL_MODEL_TYPE		1    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		12778
    #define GAIN_WAT   		 		11996
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			19      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			17
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 0.1v
    
    //#define OP_PROTECT_SLOW         1       //允许不同负载条件下的的超载时间保护
    #define	HIGH_OP_CURRENT_LIMIT	1800		//高电压档位最大限流值(0.1A)
    #define	LOW_OP_CURRENT_LIMIT	1800	    //低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        1
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)       
     
#elif defined POWER_RATING_90K
    #define POWER_RATING			900
    #define DETAIL_MODEL_TYPE		0    
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		18740
    #define GAIN_WAT   		 		17594
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			18      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			16
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 0.1v
    
    #define	HIGH_OP_CURRENT_LIMIT	2610	//高电压档位最大限流值(0.1A)  额定是260A 120% 持续 60分钟，150%持续60秒，200% 15秒
    #define	LOW_OP_CURRENT_LIMIT	2610	//低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)
    
#elif defined POWER_RATING_90K_S01
    #define POWER_RATING			900
    #define DETAIL_MODEL_TYPE		1    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		8545  
    #define GAIN_OPV   		 		8545
    #define GAIN_OPI   		 		18740
    #define GAIN_WAT   		 		17594
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			18      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			16
    
    #define	SMALL_OP_VOLT_LIMIT		100		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    1300	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	1300	// 0.1v
    
    #define OP_PROTECT_SLOW         1       //允许不同负载条件下的的超载时间保护
    #define	HIGH_OP_CURRENT_LIMIT	2610	//高电压档位最大限流值(0.1A)  额定是260A 120% 持续 60分钟，150%持续60秒，200% 15秒
    #define	LOW_OP_CURRENT_LIMIT	2610	//低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)

#elif defined POWER_RATING_100K_S01
    #define POWER_RATING			1000
    #define DETAIL_MODEL_TYPE		1    
    
    #define HIGH_FREQ_OUTPUT    		//if enable HIGH_FREQ_OUTPUT，400Hz 交流输出，精度0.1Hz	//else 45~65Hz交流输出，精度0.01Hz
    #define	LOW_OP_FREQ_LIMIT	    400			// 40.0Hz
    #define	NORMAL_OP_FREQ_LIMIT	500			// 50.0Hz
    #define	HIGH_OP_FREQ_LIMIT	    1200		// 120.0Hz
    
    #define GAIN_TEMP  		    	10
    #define GAIN_IPV   		  		12528
    #define GAIN_BUS   		  		15524
    #define GAIN_INV   		 		17090  
    #define GAIN_OPV   		 		17090
    #define GAIN_OPI   		 		18740
    #define GAIN_WAT   		 		17594
    #define GAIN_PWM   		 		7955
    #define GAIN_ANGLE 		    	0
    #define GAIN_S_ANGLE    		0
    #define GAIN_T_ANGLE    		0
    #define GAIN_CF					94
    #define	Q_VALUE_WATT			17      // WATT GAIN Q VALUE
    #define	Q_VALUE_CURR			16
    
    #define	SMALL_OP_VOLT_LIMIT		50		// 0.1v
    #define	LOW_OP_VOLT_LIMIT	    1500	// 0.1v
    #define	HIGH_OP_VOLT_LIMIT	    3000	// 0.1v
    #define	HIGH_OP_VOLT_PROG_LIMIT	3000	// 0.1v
    
    #define	HIGH_OP_CURRENT_LIMIT	1380	//高电压档位最大限流值(0.1A)  
    #define	LOW_OP_CURRENT_LIMIT	2770	//低电压档位最大限流值(0.1A)
    
    #define OUTPUT_TRS_RATIO        (130/300)
    
    #define	NO_VOLT_POINT			50
    #define	NO_CURR_POINT			5   //(0.1A)
    #define	NO_WATT_POINT			50
    #define	NO_VA_POINT				50
    
    #define	HSINK_PFC_OTP			70
    #define	HSINK_INV_OTP			80
    #define	AMB_OTP					60
    
    #define FAN_SPEED_UP            1
    
    //Power
    #define	OP_POWER_PF				80				// pf=1.0
    #define	OP_POWER_VA				(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_WATT			(Uint16)((Uint32)POWER_RATING*100/3)
    #define	OP_POWER_VA_102			(Uint16)((Uint32)POWER_RATING*102/3)
    #define	OP_POWER_WATT_102		(Uint16)((Uint32)POWER_RATING*102/3)
           
#endif

//*****************************************************************************
#ifdef HIGH_FREQ_OUTPUT
    #define SINEPOT                     180         // sine wave point    中频(350~450Hz)：每个AC周期均分成180个点,进行采样
    #define OP_FREQ_UNIT                10          // 100:0.01Hz; 10:0.1Hz
    
    #ifndef NORMAL_OP_FREQ_LIMIT					//没有自定义特规频率段
    #define	LOW_OP_FREQ_LIMIT	        3500		// 350.0Hz
    #define	NORMAL_OP_FREQ_LIMIT	    4000		// 400.0Hz
    #define	HIGH_OP_FREQ_LIMIT	        4500		// 450.0Hz
    #endif
    

    //#define	LOW_OP_FREQ_LIMIT	        400			// 40.0Hz
    //#define	NORMAL_OP_FREQ_LIMIT	    500			// 50.0Hz
    //#define	HIGH_OP_FREQ_LIMIT	        1200		// 120.0Hz
    
    //#define LOW_OP_FREQ_LIMIT	        100		    //  10.0Hz
    //#define NORMAL_OP_FREQ_LIMIT	    500		    //  50.0Hz
    //#define HIGH_OP_FREQ_LIMIT	        2000		// 200.0Hz
#else
    #define SINEPOT                     360         // sine wave point    低频(45~65Hz)：每个AC周期均分成360个点,进行采样
    #define OP_FREQ_UNIT                100         // 100:0.01Hz; 10:0.1Hz
    
    #ifndef NORMAL_OP_FREQ_LIMIT					//没有自定义特规频率段
    #define	LOW_OP_FREQ_LIMIT	        4500		// 45.00Hz
    #define	NORMAL_OP_FREQ_LIMIT	    5000		// 50.00Hz
    #define	HIGH_OP_FREQ_LIMIT	        6500		// 65.00Hz
    #endif
#endif

#define SEMI_SIN                        (SINEPOT/2) // Half sine wave
/*****************************************************************************
 I/O PORT Bit Set/Clear Macro Definition
*****************************************************************************/
#define WDG_CLK_TRG                 (GpioDataRegs.GPATOGGLE.bit.GPIO28 = 1)

#define CHK_EPO	      			    (GpioDataRegs.GPADAT.bit.GPIO29 == 1)

#define DC2DC_ON       	            (GpioDataRegs.GPACLEAR.bit.GPIO30 = 1)
#define DC2DC_OFF       	        (GpioDataRegs.GPASET.bit.GPIO30   = 1)
#define CHK_DC2DC_ON                (GpioDataRegs.GPADAT.bit.GPIO30 == 0)

//#define INPUT_RELAY_OFF		        (GpioDataRegs.GPACLEAR.bit.GPIO31 = 1)
//#define INPUT_RELAY_ON		        (GpioDataRegs.GPASET.bit.GPIO31   = 1)
//#define	CHK_INPUT_RELAY_ON		    (GpioDataRegs.GPADAT.bit.GPIO31 == 1)

#define	CHK_INV_I_PEAK_R	        (GpioDataRegs.GPBDAT.bit.GPIO34 == 1)
#define	CHK_INV_I_PEAK_S	        (GpioDataRegs.GPBDAT.bit.GPIO35 == 1)
#define	CHK_INV_I_PEAK_T	        (GpioDataRegs.GPBDAT.bit.GPIO36 == 1)
#define	CHK_INV_I_PEAK_PROTECT	    (CHK_INV_I_PEAK_R || CHK_INV_I_PEAK_S || CHK_INV_I_PEAK_T)

#define OUTPUT_RELAY_ON		        (GpioDataRegs.GPBCLEAR.bit.GPIO37 = 1)
#define OUTPUT_RELAY_OFF		    (GpioDataRegs.GPBSET.bit.GPIO37   = 1)
#define CHK_OUTPUT_RELAY_ON     	(GpioDataRegs.GPBDAT.bit.GPIO37 == 0)

#define	INVERTER_STEP_SINGEL_0	    (GpioDataRegs.GPBCLEAR.bit.GPIO44 = 1)
#define	INVERTER_STEP_SINGEL_1	    (GpioDataRegs.GPBSET.bit.GPIO44   = 1)
/*
#define DRY_OUTPUT1_0               (GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1)
#define DRY_OUTPUT1_1               (GpioDataRegs.GPBSET.bit.GPIO49   = 1)

#define DRY_OUTPUT2_0               (GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
#define DRY_OUTPUT2_1               (GpioDataRegs.GPBSET.bit.GPIO50   = 1)

#define DRY_OUTPUT3_0               (GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)
#define DRY_OUTPUT3_1               (GpioDataRegs.GPBSET.bit.GPIO51   = 1)

#define	CHK_DRY_INPUT1	            (GpioDataRegs.GPBDAT.bit.GPIO52 == 1)                                       
#define	CHK_DRY_INPUT2	            (GpioDataRegs.GPBDAT.bit.GPIO53 == 1)                                       
#define	CHK_DRY_INPUT3	            (GpioDataRegs.GPBDAT.bit.GPIO54 == 1)                                       
*/

#define DRY_OUTPUT1_0               (GpioDataRegs.GPBSET.bit.GPIO49   = 1)
#define DRY_OUTPUT1_1               (GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1)
#define	CHK_DRY_ONPUT1_1            (GpioDataRegs.GPBDAT.bit.GPIO49 == 0) 

#define DRY_OUTPUT2_0               (GpioDataRegs.GPBSET.bit.GPIO50   = 1)
#define DRY_OUTPUT2_1               (GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
#define	CHK_DRY_ONPUT2_1            (GpioDataRegs.GPBDAT.bit.GPIO50 == 0) 

#define DRY_OUTPUT3_0               (GpioDataRegs.GPBSET.bit.GPIO51   = 1)
#define DRY_OUTPUT3_1               (GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)
#define	CHK_DRY_ONPUT3_1            (GpioDataRegs.GPBDAT.bit.GPIO51 == 0) 

#define	CHK_DRY_INPUT1	            (GpioDataRegs.GPBDAT.bit.GPIO52 == 0)                                       
#define	CHK_DRY_INPUT2	            (GpioDataRegs.GPBDAT.bit.GPIO53 == 0)                                       
#define	CHK_DRY_INPUT3	            (GpioDataRegs.GPBDAT.bit.GPIO54 == 0) 

#define INVERTER_DISABLE_R      	(GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1)                                 
#define INVERTER_ENABLE_R       	(GpioDataRegs.GPBSET.bit.GPIO55   = 1)                                 
#define CHK_INVERTER_ENABLE_R		(GpioDataRegs.GPBDAT.bit.GPIO55 == 1)                              
                                                                                                                
#define INVERTER_DISABLE_S      	(GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1)                                 
#define INVERTER_ENABLE_S       	(GpioDataRegs.GPBSET.bit.GPIO56   = 1)                                 
#define CHK_INVERTER_ENABLE_S		(GpioDataRegs.GPBDAT.bit.GPIO56 == 1)                              
                                                                                                                
#define INVERTER_DISABLE_T          (GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1)                                      
#define INVERTER_ENABLE_T           (GpioDataRegs.GPBSET.bit.GPIO57   = 1)                                      
#define CHK_INVERTER_ENABLE_T	    (GpioDataRegs.GPBDAT.bit.GPIO57 == 1)                                  
                                                                                                                
#define CHK_INVERTER_ENABLE         (CHK_INVERTER_ENABLE_R && CHK_INVERTER_ENABLE_S && CHK_INVERTER_ENABLE_T)   
                                                                                                                
#define CHK_FAN1_LOCK	            (GpioDataRegs.GPBDAT.bit.GPIO58 == 1)

#define PFC_DISABLE                 (GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1)                                      
#define PFC_ENABLE                  (GpioDataRegs.GPBSET.bit.GPIO61   = 1)

#define CHK_KEY_X1				    (GpioDataRegs.GPCDAT.bit.GPIO64 == 1)
#define CHK_KEY_X2				    (GpioDataRegs.GPCDAT.bit.GPIO65 == 1)
#define CHK_KEY_X3				    (GpioDataRegs.GPCDAT.bit.GPIO66 == 1)
#define CHK_KEY_X4				    (GpioDataRegs.GPCDAT.bit.GPIO67 == 1)
#define CHK_KEY_X5				    (GpioDataRegs.GPCDAT.bit.GPIO68 == 1)

#define CHK_KEY_Y1				    (GpioDataRegs.GPCDAT.bit.GPIO69 == 1)
#define CHK_KEY_Y2				    (GpioDataRegs.GPCDAT.bit.GPIO70 == 1)
#define CHK_KEY_Y3				    (GpioDataRegs.GPCDAT.bit.GPIO71 == 1)
#define CHK_KEY_Y4				    (GpioDataRegs.GPBDAT.bit.GPIO59 == 1)
#define CHK_KEY_Y5				    (GpioDataRegs.GPBDAT.bit.GPIO60 == 1)


#define CHK_REPAIR_MODE   	        (GpioDataRegs.GPCDAT.bit.GPIO74 == 1)

#define CHK_RECT_OK_R   	        (GpioDataRegs.GPCDAT.bit.GPIO76 == 1)
#define CHK_RECT_OK_S   	        (GpioDataRegs.GPCDAT.bit.GPIO77 == 1)
#define CHK_RECT_OK_T   	        (GpioDataRegs.GPCDAT.bit.GPIO78 == 1)

#define CHK_INPUT_ZERO_R            (GpioDataRegs.GPCDAT.bit.GPIO79 == 1)        // check INPUT  ZERO SYNC
#define CHK_INPUT_ZERO_S            (GpioDataRegs.GPCDAT.bit.GPIO80 == 1)
#define CHK_INPUT_ZERO_T            (GpioDataRegs.GPCDAT.bit.GPIO81 == 1)

#define CHK_OP_TRS_TEMP             (GpioDataRegs.GPCDAT.bit.GPIO82 == 1)
//#define CHK_EPO                     (GpioDataRegs.GPCDAT.bit.GPIO83 == 1)


//#define SOFTSTART_RELAY_ON	        (GpioDataRegs.GPACLEAR.bit.GPIO15 = 1)
//#define SOFTSTART_RELAY_OFF	        (GpioDataRegs.GPASET.bit.GPIO15   = 1)

//#define BUS_SEL_PIN_0			    (GpioDataRegs.GPACLEAR.bit.GPIO22 = 1)
//#define BUS_SEL_PIN_1			    (GpioDataRegs.GPASET.bit.GPIO22   = 1)
//#define CHK_BUS_SEL_PIN_1			(GpioDataRegs.GPADAT.bit.GPIO22 == 1)

//#define PFC_ON       	            (GpioDataRegs.GPACLEAR.bit.GPIO23 = 1)
//#define PFC_OFF       	            (GpioDataRegs.GPASET.bit.GPIO23   = 1)
//#define CHK_PFC_ON                  (GpioDataRegs.GPADAT.bit.GPIO23 == 0)


//#define DC_OFFSET_R_0               (GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1)
//#define DC_OFFSET_R_1               (GpioDataRegs.GPBSET.bit.GPIO48   = 1)
//
//#define DC_OFFSET_S_0               (GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1)
//#define DC_OFFSET_S_1               (GpioDataRegs.GPBSET.bit.GPIO49   = 1)
//
//#define DC_OFFSET_T_0               (GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
//#define DC_OFFSET_T_1               (GpioDataRegs.GPBSET.bit.GPIO50   = 1)



#define LCD_WRITE_0				    (GpioDataRegs.GPCCLEAR...bit.GPIO82 = 1)      
#define LCD_WRITE_1				    (GpioDataRegs.GPCSET.biiit.GPIO82   = 1)      
                              		                                                     
#define LCD_RS_PIN_0			    (GpioDataRegs.GPBCLEAR...bit.GPIO83 = 1)      
#define LCD_RS_PIN_1			    (GpioDataRegs.GPBSET.biiit.GPIO83   = 1)      
                              		                                                     
#define LCD_E_PIN_0				    (GpioDataRegs.GPCCLEAR...bit.GPIO60 = 1)      
#define LCD_E_PIN_1				    (GpioDataRegs.GPCSET.biiit.GPIO60   = 1)      
                                                                                         
#define BUZZER_ON          		    (EPwm12Regs.AQCSFRC.bit.CSFB = 0)                     
#define BUZZER_OFF          		(EPwm12Regs.AQCSFRC.bit.CSFB = 1)                     
                                                                                         
#define LCD_BKLIT_0				    (GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1)               
#define LCD_BKLIT_1				    (GpioDataRegs.GPCSET.bit.GPIO85   = 1)               




#define LCD_DATA0_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1)      //故障灯号显示
#define LCD_DATA0_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO87   = 1)

#define LCD_DATA1_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO88 = 1)      //输出On/Off状态显示
#define LCD_DATA1_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO88   = 1)

//#define LCD_DATA2_PIN_0           (GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1)      
//#define LCD_DATA2_PIN_1           (GpioDataRegs.GPCSET.bit.GPIO89   = 1)
//#define SOFTSTART_RELAY_OFF         (GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1)      //输出软启动
//#define SOFTSTART_RELAY_ON          (GpioDataRegs.GPCSET.bit.GPIO89   = 1)
#define INPUT_RELAY_OFF		        (GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1)
#define INPUT_RELAY_ON		        (GpioDataRegs.GPCSET.bit.GPIO89   = 1)
#define	CHK_INPUT_RELAY_ON		    (GpioDataRegs.GPCDAT.bit.GPIO89 == 1)


#define LCD_DATA3_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1)
#define LCD_DATA3_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO90   = 1)

#define LCD_DATA4_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO91 = 1)		//高低档
#define LCD_DATA4_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO91   = 1)

//#define LCD_DATA5_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO92 = 1)
//#define LCD_DATA5_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO92   = 1)
#define CHK_316_SC_R                (GpioDataRegs.GPCDAT.bit.GPIO92 == 1)

//#define LCD_DATA6_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO93 = 1)
//#define LCD_DATA6_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO93   = 1)
#define CHK_316_SC_S                (GpioDataRegs.GPCDAT.bit.GPIO93 == 1)

//#define LCD_DATA7_PIN_0             (GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1)
//#define LCD_DATA7_PIN_1             (GpioDataRegs.GPCSET.bit.GPIO94   = 1)
#define CHK_316_SC_T                (GpioDataRegs.GPCDAT.bit.GPIO94 == 1)
/*****************************************************************************
   Constant Definition
*****************************************************************************/
// AC INPUT FREQUENCY
#define F_700HZ         700             // 70.0 Hz
#define F_695HZ         695             // 69.5 Hz
#define F_405HZ         405             // 40.5 Hz
#define F_400HZ         400             // 40.0 Hz

/******************************************************************************/

/******************************************************************************/

//Periodic Auto Test time
#define MIN_DAY         60*24           // 60 minute * 24 hour
#define MIN_WEEK        MIN_DAY*7       // 7  day, 1 week
#define MIN_BIWEEK      MIN_DAY*14      // 14 day, 2 week
#define MIN_MONTH       MIN_DAY*28      // 28 day, 4 week

#define SEC_DAY         (Uint32)MIN_DAY  *60     // 1  day,
#define SEC_WEEK        (Uint32)MIN_WEEK *60     // 7  day, 1 week
#define SEC_MONTH       (Uint32)MIN_MONTH*60     // 28 day, 1 month

#define SEC_10DAY       (Uint32)SEC_DAY  *10     // 10   day,
#define SEC_30DAY       (Uint32)SEC_DAY  *30     // 30   day,
#define SEC_60DAY       (Uint32)SEC_DAY  *60     // 60   day,
#define SEC_90DAY       (Uint32)SEC_DAY  *90     // 90   day,
#define SEC_100DAY      (Uint32)SEC_DAY  *100    // 100  day,
#define SEC_1000DAY     (Uint32)SEC_DAY  *1000   // 1000 day,

//Minute count
#define MIN_1HOUR       60              // 1  hours
#define MIN_2HOUR       60*2            // 2  hours
#define MIN_4HOUR       60*4            // 4  hours
#define MIN_8HOUR       60*8            // 8  hours
#define MIN_12HOUR      60*12           // 12 hours
#define MIN_24HOUR      60*24           // 24 hours

#define MIN_5DAY        MIN_DAY*5       // 5  day

//Second count
#define SEC_10       	10              // 10 sec
#define SEC_20       	20              // 20 sec
#define SEC_1MIN        60              // 1  minute
#define SEC_2MIN        60*2            // 2  minutes
#define SEC_3MIN        60*3            // 3  minutes
#define SEC_4MIN        60*4            // 4  minutes
#define SEC_5MIN        60*5            // 5  minutes

#define SEC_10MIN       (Uint32)SEC_1MIN *10     // 10   minute
#define SEC_100MIN      (Uint32)SEC_1MIN *100    // 100  minute
#define SEC_1000MIN     (Uint32)SEC_1MIN *1000   // 1000 minute

#define SEC_20MIN       1200            // 20 minute

#define SEC_1HOUR       3600            // 1  hour
#define SEC_1H30M       5400            // 1  hour 30 min
#define SEC_2HOUR       3600*2          // 2  hours

#define DCOFFSET        2048            // 1.65V DC level offset

#define COUNT_0         0               // Counter 0 times
#define COUNT_1         1               // Counter 1 times
#define COUNT_2         2               // Counter 2 times
#define COUNT_3         3               // Counter 3 times
#define COUNT_4         4               // Counter 4 times
#define COUNT_5         5               // Counter 5 times
#define COUNT_6         6               // Counter 6 times
#define COUNT_7         7               // Counter 7 times
#define COUNT_8         8               // Counter 8 times
#define COUNT_9         9               // Counter 9 times
#define COUNT_10        10              // Counter 10 times
#define COUNT_20        20              // Counter 20 times
#define COUNT_30        30              // Counter 30 times

#define RMSD_60S        60              // DRY CONTACT default 60S
#define MODE_10S        10              // 10S, AC MODE time
#define MODE_8S         8               // 8S, AC MODE time
#define MODE_4S         4               // 4S, AC MODE time
#define MODE_2S         2               // 2S, DC MODE time
#define BURN_2S         2               // 2S, Burn In delay time
#define MIN_4WEEK       40320           // Minute, 4 weeks

#define SEC_01S         1               // 0.1S,  1*100ms
#define SEC_02S         2               // 0.2S,  2*100ms
#define SEC_03S         3               // 0.3S,  3*100ms
#define SEC_04S         4               // 0.4S,  4*100ms
#define SEC_05S         5               // 0.5S,  5*100ms
#define SEC_06S         6               // 0.6S,  6*100ms
#define SEC_08S         8               // 0.8S,  6*100ms
#define SEC_1S          10              // 1S  , 10*100ms
#define SEC_2S          20              // 2S  , 20*100ms
#define SEC_3S          30              // 3S  , 30*100ms
#define SEC_4S          40              // 4S  , 40*100ms
#define SEC_5S          50              // 5S  , 50*100ms
#define SEC_6S          60              // 6S  , 60*100ms
#define SEC_10S         100             // 10S ,100*100ms
#define SEC_11S         110             // 11S ,110*100ms
#define SEC_15S         150             // 15S ,150*100ms
#define SEC_20S         200             // 20S ,200*100ms
#define SEC_30S         300             // 30S ,300*100ms
#define SEC_240S        2400            // 240S,2400*100ms

#define MINI_003S       6/2               // 0.1S,   10*10ms
#define MINI_01S        10/2              // 0.1S,   10*10ms
#define MINI_02S        20/2              // 0.2S,   20*10ms
#define MINI_03S        30/2              // 0.3S,   30*10ms
#define MINI_04S        40/2              // 0.4S,   40*10ms
#define MINI_05S        50/2              // 0.5S,   50*10ms
#define MINI_1S         100/2             // 1S  ,  100*10ms
#define MINI_2S         200/2             // 2S  ,  200*10ms
#define MINI_3S         300/2             // 3S  ,  300*10ms
#define MINI_4S         400/2             // 4S  ,  400*10ms
#define MINI_5S         500/2             // 5S  ,  500*10ms


/*****************************************************************************
   Variables
*****************************************************************************/
/*typedef struct
{
    unsigned B00         : 1;    // Bit  0:
    unsigned B01         : 1;    // Bit  1:
    unsigned B02         : 1;    // Bit  2:
    unsigned B03         : 1;    // Bit  3:
    unsigned B04         : 1;    // Bit  4:
    unsigned B05         : 1;    // Bit  5:
    unsigned B06         : 1;    // Bit  6:
    unsigned B07         : 1;    // Bit  7:
    unsigned B08         : 1;    // Bit  8:
    unsigned B09         : 1;    // Bit  9:
    unsigned B10         : 1;    // Bit 10:
    unsigned B11         : 1;    // Bit 11:
    unsigned B12         : 1;    // Bit 12:
    unsigned B13         : 1;    // Bit 13:
    unsigned B14         : 1;    // Bit 14:
    unsigned B15         : 1;    // Bit 15:
} WORD_sBit;

typedef struct
{
    Uint16  LowByte     : 8;    // Bit  0~7:
    Uint16  HighByte    : 8;    // Bit  8~15:
} WORD_BYTE;
*/


/*****************************************************************************/
extern  Uint16  u16MicroSec;        // Mico second
extern  Uint16  u16Mini1ms;                     // Mini second, 1 ms
extern  Uint16  u16Mini10ms;                    // Mini second, 10 ms
extern  Uint16  u16Mini50ms;                    // Mini second, 50 ms
extern  Uint16  u16Mini100ms;                  // Mini second, 100 ms
extern  Uint16  u16Second;                      // Second
extern  Uint16  u16Minute;                      // Minute
extern  Uint16  u16Hour;                        // Hour

extern  Uint16   u16DisplayTime;               // DISPLAY LCD time temp.

extern  Uint16   u16DcbusHighCnt1, u16DcbusHighCnt2, u16DcbusLowCnt1, u16DcbusLowCnt2;            // DC BUS + fail count

extern  Uint8    AC_FREQ_CNT;               // AC Frequency out of range count, backup

extern  Uint16   wGAIN_ADD_R,wGAIN_ADD_S,wGAIN_ADD_T;               // PWM gain add constant
extern  Uint16   u16CalPwmGain_R,u16CalPwmGain_S,u16CalPwmGain_T;
extern  Uint16   u16BusDelayTime;           // Check DCBUS delay time

extern	Uint16	 u16LcdBacklightOnTime;	    // LCD backlight on tiem

extern  Uint16   u16DelayOn;

extern  Uint32   dwOPI_PEAK_TMP;            // Output I A/D sum peak temp
extern  Uint32   dwOPI_PEAK_SUM;            // Output I A/D sum peak


typedef struct{
	Uint16	wTEMP;
	Uint16	wBUS;
	Uint16	wINV;
	Uint16	wOPV;
	Uint16	wOPI;
	Uint16	wWAT;
	Uint16	wPWM;
	Uint16	wAngle;
	Uint16  wIPV;
}_sGain;
extern _sGain sGain_R,sGain_S,sGain_T;

extern Uint16 u16IpeakProtect;
extern Uint16 u16GainDcOffset;
extern Uint16 u16PowerRecycle;
extern Uint16 u16PowerRecycleLevel;

extern	Uint32	dwPwmGainTemp_R;       
extern	Uint32	dwPwmGainTemp_S;  
extern	Uint32	dwPwmGainTemp_T;   
       
extern	Uint16  wBusTemp;    
extern	Uint16  u16BusOvpCnt;                  

typedef struct
{
    Uint16	u16Load;
    Uint16	u16Save;

    Uint16	u16Deep;
    Uint16	u16Start;
    Uint16	u16End;
    Uint16	u16Cycles;
    struct _sStep
    {
        Uint16  wR_StartVolt;
		Uint16  wR_EndVolt;
		Uint16  wS_StartVolt;
		Uint16  wS_EndVolt;
		Uint16  wT_StartVolt;
		Uint16  wT_EndVolt;
		Uint16  wFreq_Start;
		Uint16  wFreq_End;
        Uint16	u16TimeHigh;
        Uint16	u16TimeLow;
    } u16Step[20];
} _sProgram;

extern	_sProgram	sProgramming;


typedef struct{
	unsigned char 	u16Index;
	unsigned long 	dwTimePeriod;
	unsigned long 	u32TimeCnt;
	unsigned int  	u16CycleCnt;

	unsigned char 	bR_UpDown;
	unsigned int  	wR_DeltaVolt;
	unsigned int  	wR_StepVolt;
	unsigned long 	dwR_StepTime;
	unsigned long 	dwR_TotalStepTime;

	unsigned char 	bS_UpDown;
	unsigned int  	wS_DeltaVolt;
	unsigned int  	wS_StepVolt;
	unsigned long 	dwS_StepTime;
	unsigned long 	dwS_TotalStepTime;

	unsigned char 	bT_UpDown;
	unsigned int  	wT_DeltaVolt;
	unsigned int  	wT_StepVolt;
	unsigned long 	dwT_StepTime;
	unsigned long 	dwT_TotalStepTime;

	unsigned char 	bF_UpDown;
	unsigned int  	wF_Delta;
	unsigned int  	wF_Step;
	unsigned long 	dwF_StepTime;
	unsigned int 	wF_StepTimeDiv;
	unsigned int 	wF_TotalTimeDiv;
	unsigned long 	dwF_TotalStepTime;

	Uint16 u16InitWaitCnt;
	Uint16 u16StartPoint;
	Uint16 u16StepChangeAdjustDelay;
}_sProgRun;
extern	_sProgRun	sProgRun;

extern struct _sLVRT
{
    union   UN_FUNCTION_FLAG
    {
        Uint16    WORD;
        struct  ST_FUNCTION_FLAG1
        {
            Uint16 u1TestR      : 1;
            Uint16 u1TestS		: 1;
            Uint16 u1TestT		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;

            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
            Uint16         		: 1;
        } bit;
    } sFunction;
    Uint16      wTestTime;			//time base : 1ms
	Uint16 		wTimeCount;
	Uint16		wVoltage;		//0.1v
    Uint16 		wOldGain_R;
    Uint16 		wOldGain_S;
    Uint16 		wOldGain_T;
} sLVRT;

extern	Uint16	u16SineWaveIndex;
extern	Uint32	u32IsrTimeBase;
extern	Uint32	u32TimeBase;
extern	Uint32	u32TimeBaseRem;

extern struct _sSet
{
    Uint32	u32OpPeriod;
    Uint16	u16OutputVoltage_R;
    Uint16	u16OutputVoltage_S;
    Uint16	u16OutputVoltage_T;
    Uint16	u16OutputFrequency;
    Uint16	u16OutputOnAngle;
    Uint16	u16OutputOnAngleDo;
    Uint16	u16OutputCurrentLimit;
    Uint16	u16OutputRange;
    Uint16  u16SRPhaseAngle;
	Uint16  u16TRPhaseAngle;
    Uint16	u16RemainingRunTime;
    Uint16  u16LimitYear;
	Uint16  u16LimitMonthDay;

    Uint16  u16OldFrequency;
	Uint16  woldOutputVoltage;
	Uint16  wBeforePgSetOutputVoltage_R;
	Uint16  wBeforePgSetOutputVoltage_S;
	Uint16  wBeforePgSetOutputVoltage_T;
	Uint16  wBeforePgSetOutputFrequency;

    Uint16  u16OutputCurrent;
    
    nu16HiLoByte_t	wBeforePgStatus;

	nu16HiLoByte_t	u16OutputMode;
	nu16HiLoByte_t	wLoadSaveProgramMemoryIndex;
} sSet;

extern	nu16HiLoByte_t	wTempVaule;

extern	Uint16	wRGain,wSGain,wTGain;
extern  Uint16	wG1RGain,wG1SGain,wG1TGain;

extern	Uint16  u16IndexR,u16IndexS,u16IndexT;

extern	nu32HiLoWord_t iMathDoubleWord_G1R;

extern	Uint32 u32TempLoadEnergy_R,u32TempLoadEnergy_S,u32TempLoadEnergy_T;


extern struct _sPdRunTime
{
    Uint16  u16RunTimeDate;

    union   UN_RTHourMinute
    {
        Uint16  u16All;
        struct  ST_HM
        {
            Uint16  u8Minute        : 8;    // Bit  0~7:
            Uint16  u8Hour          : 8;    // Bit  8~15:
        } sByte;
    } sHourMinute;

    union   UN_SecondNone
    {
        Uint16  u16All;
        struct  ST_SN
        {
            Uint16  u8None          : 8;    // Bit  0~7:
            Uint16  u8Second        : 8;    // Bit  8~15:
        } sByte;
    } sSecondNone;
}sPdRunTime,sNowRunTime;
 

extern	Uint16	u16InvIPeakProtectCnt, u16InvErrorCnt_R, u16InvErrorDelayCnt,u16Inv316ScProtectCnt;
extern	Uint16	wInvOpVolError;
extern	Uint16	u16RelayDelayCnt, u16OverVoltageCnt_R, u16SetupModeCnt, u16RepairModeCnt;
extern  Uint16  u16SoftstartCnt;
extern	int16   i16TemperatureOffset;

extern Uint16 u16LastInvAdcVolt;
extern Uint16	u16ProgramErrorCode;

extern int32 i32Inv1RMSAmpVref_R,i32Inv1RMSAmpVref_S,i32Inv1RMSAmpVref_T;
extern int32 i32Inv1AmpVref_R,i32Inv1AmpVref_S,i32Inv1AmpVref_T;
extern Uint16 u16RmsErrorDelayCnt;

extern Uint32  u32EndIsrTimeCnt, u32StartIsrTimeCnt, u32CpuIsrTime,u32IsrTimeMax;       // Mico second

extern Uint32   u32timeisrStartIsrTimeCnt, u32timeisrEndIsrTimeCnt;
extern Uint32   u16timeisrLastCpuIsrTime;

extern int16 i16DebugIndex,i16DebugData[4][360];

//谐波叠加
//地址:0x0000 0800-0x0000 08FF(寄存器数量256，支持功能码03,10)
#define HARM_TOTAL_NUM  100
extern struct _sHarmSuperDataRegs
{
    Uint16 u16HarmConfig1;
    Uint16 u16Command;
	int16	i16Harmrms[HARM_TOTAL_NUM];        //xx	01-99	稳态谐波含量（按%显示）
	//int16	i16Harmphase[HARM_TOTAL_NUM-1];      //xxxx	0-360	    稳态谐波相位
}HarmSuperData;
extern Uint32 u32TotalHarmValue;

extern float32 f32SinewaveTable[360];
extern int16 i16SinewaveTable[360];
/*****************************************************************************/
extern  union   UN_PASSWORD_FLAG
        {
        Uint8    BYTE;
        struct  ST_PASSWORD_FLAG
        {
        Uint8	SET_PASSWORD	:1;
        Uint8	RESULT			:1;
        Uint8	CHANGE_PASSWORD	:1;
        Uint8    WriteTimeLimitEnable    :1;
        }       sBit;
        }       PASSWORD_FLAG;		// PASSWORD flag
        
extern  union   UN_ADC_FLAG
{
    Uint8    BYTE;
    struct  ST_ADC_FLAG
    {
        Uint8    SAMPLE_OK       : 1;    // sampling ok                 flag
        Uint8    OPI_SAMPLE_OK   : 1;    // OPI 1     cycle sampling ok flag
        Uint8    OPI_HALF_OK     : 1;    // OPI  half cycle sampling ok flag
        Uint8    WAT_HALF_OK     : 1;    // Watt half cycle sampling ok flag

        Uint8	RMS_CONTROL		: 1;
    }       sBit;
}       ADC_FLAG;               // ADC flag

extern  union   UN_CONFIG
{
    Uint16    WORD;
    struct  ST_CONFIG
    {
        Uint16    LcdEntryIntoFirmwareUpgrade 	: 1;    // Bit  0:
        Uint16    Reserved9     				: 1;    // Bit  1:
        Uint16    Reserved10 				    : 1;    // Bit  2:
        Uint16    Reserved11 				    : 1;    // Bit  3:
        Uint16    Reserved12 				    : 1;    // Bit  4:
        Uint16    Reserved13 				    : 1;    // Bit  5:
        Uint16    Reserved14 				    : 1;    // Bit  6:
        Uint16    Reserved15 				    : 1;    // Bit  7:
                                                              
        Uint16    Reserved0  				    : 1;    // Bit  8:
        Uint16    Reserved1  				    : 1;    // Bit  9:
        Uint16    LcdModifyProgramDataEnable	: 1;    // Bit 10:
        Uint16    ProgramSoftstartEnable  	    : 1;    // Bit 11:
        Uint16    ProgramRunEnable			    : 1;    // Bit 12:
        Uint16    AdjustPwmGainFastEnable		: 1;    // Bit 13:
        Uint16    InverterSoftstartEnable       : 1;    // Bit 14:
        Uint16    RunTimeLimitEnable     	    : 1;    // Bit 15:
    }       sBit;
}       CONFIG_FLAG;               // CONFIG flag

extern  union   UN_FAULT1_WARN
{
    Uint16    WORD;
    struct  ST_FAULT1_WARN
    {
        Uint16    OverCurrent       	: 1;    // Bit  0:              
        Uint16    ProgramParaError      : 1;    // Bit  1:              
        Uint16    OverWatt102      		: 1;    // Bit  2:              
        Uint16    OverVa102         	: 1;    // Bit  3:              
        Uint16    InvIPeakProtect       : 1;    // Bit  4:         
        Uint16    InvDcOffsetProtect 	: 1;    // Bit  5:              
        Uint16    InverterOverVoltage 	: 1;    // Bit  6:              
        Uint16    OverTemperature    	: 1;    // Bit  7:         
                                                                       
        Uint16    bInv316ScProtect    	: 1;    // Bit  8:         
        Uint16    InvOpDifferent      	: 1;    // Bit  9:          
        Uint16    IpTrOverTemperature   : 1;    // Bit 10:              
        Uint16    OpTrOverTemperature   : 1;    // Bit 11:         
        Uint16    DCBUS_FAIL	        : 1;    // Bit 12:              
        Uint16    u1EPO	                : 1;    // Bit 13:         
        Uint16    u1RecyclePowerFail    : 1;    // Bit 14:              
        Uint16    TimeOut	            : 1;    // Bit 15:         
    }sBit;
}FAULT1_FLAG;               // LCD flag

extern  union   UN_WARN_FLAG
{
    Uint8    BYTE;
    struct  ST_WARN_FLAG
    {
        Uint16    FAN_FAIL        : 1;    // Bit 0: FAN fail flag
        Uint16    CHARGER_FAIL    : 1;    // Bit 1: CHARGER fail flag
        Uint16    LN_FAIL         : 1;    // Bit 2: L,N LINE fail flag
        Uint16    SHORT_WARN      : 1;    // Bit 3: SHORT CKT fail flag, Beep warning only
        Uint16    SHORT_HOLD      : 1;    // Bit 4: SHORT CKT fail flag hold
        Uint16    SHORT_HOLD_ON   : 1;    // Bit 5: SHORT CKT fail flag hold on
        Uint16    SHORT_HOLD_OFF  : 1;    // Bit 6: SHORT CKT fail flag hold off
        Uint16    TEMP_LOW_FAIL   : 1;    // Bit 7:
    } sBit;
} WARN_FLAG;              // Warning flag


extern  union   UN_AC_FLAG
{
    Uint8    BYTE;
    struct  ST_AC_FLAG
    {
        Uint16    AC_NORM         : 1;    // AC normal      1:abnormal
        Uint16    AC_LOSS         : 1;    // AC loss
        Uint16    AC_LOW          : 1;    // AC too low
        Uint16    AC_HIGH         : 1;    // AC too high
        Uint16    AC_FAST         : 1;    // AC fast
        Uint16    AC_IN_LOW       : 1;    // AC input low flag
        Uint16    AC_WARN         : 1;    // AC high warning flag
        Uint16    ACHI_BP         : 1;    // AC too high beep flag
    } sBit;
} AC_FLAG;                // ACIN flag

extern  union   UN_BUS_FLAG
{
    Uint16    WORD;
    struct  ST_BUS_FLAG
    {
        Uint16    u1BusOvp_R 		    : 1;    // Bit  0:// DC BUS + too high flag
        Uint16    u1BusOvp_S       	    : 1;    // Bit  1:// DC BUS + too low  flag
        Uint16    u1BusOvp_T      	    : 1;    // Bit  2:// DC BUS - too high flag
        Uint16    NOUSE3            : 1;    // Bit  3:// DC BUS - too low  flag
        Uint16    NOUSE4            : 1;	// Bit  4:// 2008/11/14
        Uint16    NOUSE5 			: 1;    // Bit  5:
        Uint16    NOUSE6 			: 1; 	// Bit  6:
        Uint16    NOUSE7    		: 1;    // Bit  7:
                                                      
        Uint16    PLS_HI            : 1;    // Bit  8:
        Uint16    PLS_LO            : 1;    // Bit  9:
        Uint16    MUS_HI            : 1;    // Bit 10:
        Uint16    MUS_LO            : 1;    // Bit 11:
        Uint16    SPIKE_BUS_LOW     : 1;    // Bit 12:
        Uint16	  NOUSE13			: 1;    // Bit 13:
        Uint16    NOUSE14     	    : 1;    // Bit 14:
        Uint16    NOUSE15     	    : 1;    // Bit 15:
    }       sBit;
}       BUS_FLAG;               // DC BUS flag

extern  union   UN_WATT_FLAG
{
    Uint8    BYTE;
    struct  ST_WATT_FLAG
    {
        Uint16    WATT_I          : 1;    // I flag, 0:PLUS 1:MINUS
        Uint16    WATT_BI         : 1;    // I flag, 0:PLUS 1:MINUS FOR bypass
        Uint16    WATT_V          : 1;    // V flag, 0:PLUS 1:MINUS
        Uint16    WATT_NOLOAD     : 1;    // WATT no load flag
        Uint16    VA_NOLOAD       : 1;    // VA   no load flag
    }       sBit;
}       WATT_FLAG;              // Watt flag

extern  union   UN_FREQ_FLAG
{
    Uint8    BYTE;
    struct  ST_FREQ_FLAG
    {
        Uint16    ACFreqOutRange  : 1;  // AC IN Frequency Out of Range 40Hz -- 70Hz
    }       sBit;
}       FREQ_FLAG;                      // Frequency Flag

extern  union   UN_DryContactFlag
{
    Uint16    u16All;
    struct  ST_DryContactFlag
    {
        Uint16    ProgVoltFreqUpDown		:1;     // Bit 0:
        Uint16    ManualVoltFreqUpDown	    :1;     // Bit 1:
        Uint16    NOUSE2      			    :1;     // Bit 2:
        Uint16    NOUSE3         			:1;     // Bit 3:
        Uint16    NOUSE4         			:1;     // Bit 4:
        Uint16    NOUSE5 					:1;     // Bit 5:
        Uint16    NOUSE6 					:1;     // Bit 6:
        Uint16    NOUSE7					:1;     // Bit 7:
            
        Uint16  u1Input1            : 1;    // Bit  8:
        Uint16  u1Input2            : 1;    // Bit  9:
        Uint16  u1Input3            : 1;    // Bit 10:
        Uint16                      : 1;    // Bit 11:
        Uint16                      : 1;    // Bit 12:
        Uint16                      : 1;    // Bit 13:
        Uint16                      : 1;    // Bit 14:
        Uint16                      : 1;    // Bit 15:
    }       sBit;
}       DryContactFlag;                        // Rectifier Flag
        
extern  union   UN_SourceControlFlag
{
    Uint16    WORD;
    struct  ST_SourceControlFlag
    {
        Uint16  u1KeepOneCycle      : 1;    // Bit  0:
        Uint16  NOUSE1              : 1;    // Bit  1:
        Uint16  NOUSE2              : 1;    // Bit  2:
        Uint16  NOUSE3              : 1;    // Bit  3:
        Uint16  NOUSE4              : 1;    // Bit  4:
        Uint16  NOUSE5              : 1;    // Bit  5:
        Uint16  NOUSE6              : 1;    // Bit  6:
        Uint16  IP_RELAY_LOCK       : 1;    // Bit  7:
                                                      
        Uint16  OutputOn            : 1;    // Bit  8:
        Uint16  VSET                : 1;    // Bit  9:
        Uint16  FSET                : 1;    // Bit 10:
        Uint16	Program_Init_Wait   : 1;    // Bit 11:
        Uint16	Program_Running     : 1;    // Bit 12:
        Uint16	RemoteControlOn     : 1;    // Bit 13:
        Uint16  RepairOn            : 1;    // Bit 14:
        Uint16  LVRT_RUN            : 1;    // Bit 15:
    }       sBit;
}       SourceControlFlag, LastSourceControlFlag;                   	//


extern  union   UN_TIMES_FLAG
{
    Uint16  u16All;
    struct  ST_TIMES_FLAG
    {
        unsigned  u1EepWrOneTime      : 1;    // Bit  0:
        unsigned  NoUse01             : 1;    // Bit  1:
        unsigned  NoUse02             : 1;    // Bit  2:
        unsigned  NoUse03             : 1;    // Bit  3:
        unsigned  u1TimeBase100us     : 1;    // Bit  4:
        unsigned  u1TimeBase1ms       : 1;    // Bit  5:
        unsigned  u1TimeBase10ms      : 1;    // Bit  6:
        unsigned  u1TimeBase50ms      : 1;    // Bit  7:
                                                        
        unsigned  u1TurnOffToStandby  : 1;    // Bit  8:
        unsigned  u1DcModeSDA         : 1;    // Bit  9:DC Mode, Receive SDA Command
        unsigned  u1SDAAutoboot       : 1;    // Bit 10:DC Mode, Receive SDA Command and AC Recover when Count Down, UPS AUTOBOOT
        unsigned  u1ParallelSDR       : 1;    // Bit 11:
        unsigned  u1OpRelayKeepOnBps  : 1;    // Bit 12:
        unsigned  NoUse13             : 1;    // Bit 13:
        unsigned  NoUse14             : 1;    // Bit 14:
        unsigned  NoUse15             : 1;    // Bit 15:
    } sBit;
} sTimesFlag;

extern  union   UN_SYS_FLAG
{
    Uint16  u16All;
    struct  ST_SYS_FLAG
    {
        unsigned  u1SYS_50HZ                  : 1;    // Bit  0: 0:60Hz 1:50Hz from EEPROM
        unsigned  u1FRE_50HZ                  : 1;    // Bit  1: Output freq.   0:60Hz  1:50Hz
        unsigned  u1SyncLock                  : 1;    // Bit  2: 1:Phase lock
        unsigned  u1OutofBpsFreq              : 1;    // Bit  3: BYPASS Out of frequency, SYNC used
        unsigned  u1OutRange                  : 1;    // Bit  4: Out of freq. range, detect 50/60 used
        unsigned                              : 1;    // Bit  5:
        unsigned  u1AutoStartUps              : 1;    // Bit  6:
        unsigned                              : 1;    // Bit  7:

        unsigned  u1DftZeroCrossingForScrCtrl : 1;    // Bit  8:
        unsigned  u1ProgramMode               : 1;    // Bit  9:
        unsigned                              : 1;    // Bit 10:
        unsigned  NoUse11                     : 1;    // Bit 11:
        unsigned  NoUse12                     : 1;    // Bit 12:
        unsigned  NoUse13                     : 1;    // Bit 13:
        unsigned  NoUse14                     : 1;    // Bit 14:
        unsigned  u1Ready                     : 1;    // Bit 15:

    } sBit;
} sSystemFlag;               // System flag

/*****************************************************************************/

#define TEMPERATURE_HS_LEN 161      
#define TEMPERATURE_HS_OFFSET (int16)30        //-30℃    
extern const Uint16 u16NTC_HS_TABLE[TEMPERATURE_HS_LEN];

#define TEMPERATURE_AMB_LEN 121
#define TEMPERATURE_AMB_OFFSET (int16)20        //-20℃  
extern const Uint16 u16NTC_AMB_TABLE[TEMPERATURE_AMB_LEN];




#endif

