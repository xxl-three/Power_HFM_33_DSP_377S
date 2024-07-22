
MEMORY
{
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */
          /* BEGIN is used for the "boot to Flash" bootloader mode   */

   RAMM0           	: origin = 0x000122, length = 0x0002DE
   RAMD0           	: origin = 0x00B000, length = 0x000800
   RAMLS0          	: origin = 0x008000, length = 0x000600
   RAM_BOOT       	: origin = 0x008600, length = 0x000200
   RAMLS1          	: origin = 0x008800, length = 0x000800
   RAMLS2      		: origin = 0x009000, length = 0x000800
   RAMLS3      		: origin = 0x009800, length = 0x000800
   RAMLS4      		: origin = 0x00A000, length = 0x000800
    
   RAMGS1      : origin = 0x00D000, length = 0x001000
   //RAMGS12     		: origin = 0x018000, length = 0x001000
   //RAMGS13     		: origin = 0x019000, length = 0x001000
   //RAMGS14     		: origin = 0x01A000, length = 0x001000
   //RAMGS15     		: origin = 0x01B000, length = 0x001000
   RESET           	: origin = 0x3FFFC0, length = 0x000002
    
   USER_RAM_FUNC    : origin = 0x018000, length = 0x004000
   
   /* Flash sectors */
   /* Bootloader section */
   BEGIN           	: origin = 0x080000, length = 0x000010                                
   BOOT_RAM_FUNC    : origin = 0x080010, length = 0x000200                                
   BOOT_MAIN        : origin = 0x080210, length = 0x0001F0                                
   BOOT_APP_ENTRY   : origin = 0x080400, length = 0x000200                                
   BOOT_APP         : origin = 0x080600, length = 0x002A00                                
   BOOT_CINIT       : origin = 0x083000, length = 0x000400                                
   BOOT_FAPI_FUNC   : origin = 0x083400, length = 0x000C00  /* FLASH code run on RAM */   
   //FLASHA         : origin = 0x080002, length = 0x0017FE  /* on-chip Flaash */        
   //FLASHB         : origin = 0x082000, length = 0x002000  /* on-chip Flashh */          
                                                                                          
   FLASHC           : origin = 0x084000, length = 0x002000  /* on-chip Flash */           
   FLASHD           : origin = 0x086000, length = 0x002000  /* on-chip Flashh */      
   
   /* User Application section */ 
   //
   PASSWORD         : origin = 0x088000, length = 0x000010  /* on-chip FLASH */ /* Password Sector */
   MODEL            : origin = 0x088010, length = 0x000010  /* on-chip FLASH */ /* Model    Sector */
   VERSION          : origin = 0x088020, length = 0x000010  /* on-chip FLASH */ /* Version  Sector */
   MAIN_UPS         : origin = 0x088030, length = 0x0003D0  /* on-chip FLASH */ /* Main UPS sector */    
   USER_APP_ROM     : origin = 0x088400, length = 0x02DC00	/* on-chip Flash */ 
   //FLASHE         : origin = 0x088400, length = 0x007C00	/* on-chip Flash */ 
   //FLASHF         : origin = 0x090000, length = 0x008000	/* on-chip Flash */
   //FLASHG         : origin = 0x098000, length = 0x008000	/* on-chip Flash */
   //FLASHH         : origin = 0x0A0000, length = 0x008000	/* on-chip Flash */
   //FLASHI         : origin = 0x0A8000, length = 0x008000	/* on-chip Flash */
   //FLASHJ         : origin = 0x0B0000, length = 0x008000	/* on-chip Flash */
   FLASHKL          : origin = 0x0B6000, length = 0x006000	/* on-chip Flash */
   //FLASHK         : origin = 0x0B8000, length = 0x002000	/* on-chip Flash */
   //FLASHL         : origin = 0x0BB000, length = 0x002000	/* on-chip Flash */   
   //FLASHMN        : origin = 0x0BC000, length = 0x004000	/* on-chip Flash */
   FLASHM           : origin = 0x0BC000, length = 0x002000	/* on-chip Flash */
   FLASHN           : origin = 0x0BE000, length = 0x002000	/* on-chip Flash */
   
   //FLASHOS        : origin = 0x0C0000, length = 0x010000	/* on-chip Flash */
   //FLASHO         : origin = 0x0C0000, length = 0x002000	/* on-chip Flash */
   //FLASHP         : origin = 0x0C2000, length = 0x002000	/* on-chip Flash */
   //FLASHQ         : origin = 0x0C4000, length = 0x002000	/* on-chip Flash */   
   //FLASHR         : origin = 0x0C6000, length = 0x002000	/* on-chip Flash */
   //FLASHS         : origin = 0x0C8000, length = 0x008000	/* on-chip Flash */
   //FLASHT         : origin = 0x0D0000, length = 0x008000	/* on-chip Flash */   
   //FLASHU         : origin = 0x0D8000, length = 0x008000	/* on-chip Flash */   
   //FLASHV         : origin = 0x0E0000, length = 0x008000	/* on-chip Flash */   
   //FLASHW         : origin = 0x0E8000, length = 0x008000	/* on-chip Flash */   
   //FLASHX         : origin = 0x0F0000, length = 0x008000	/* on-chip Flash */
   //FLASHY         : origin = 0x0F8000, length = 0x002000	/* on-chip Flash */
   //FLASHZ         : origin = 0x0FA000, length = 0x002000	/* on-chip Flash */
   //FLASHAA        : origin = 0x0FC000, length = 0x002000	/* on-chip Flash */   
   //FLASHAB        : origin = 0x0FE000, length = 0x002000	/* on-chip Flash */   

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation */

   BOOT_RSVD       : origin = 0x000002, length = 0x000120     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   RAMD1           : origin = 0x00B800, length = 0x000800

   RAMLS5          : origin = 0x00A800, length = 0x000800

   RAMGS0          : origin = 0x00C000, length = 0x001000
   //RAMGS1        : origin = 0x00D000, length = 0x001000
   //RAMGS2        : origin = 0x00E000, length = 0x001000
   //RAMGS3        : origin = 0x00F000, length = 0x001000
   //RAMGS4        : origin = 0x010000, length = 0x001000
   //RAMGS5        : origin = 0x011000, length = 0x001000
   //RAMGS6        : origin = 0x012000, length = 0x001000
   //RAMGS7        : origin = 0x013000, length = 0x001000
   //RAMGS8        : origin = 0x014000, length = 0x001000
   //RAMGS9      : origin = 0x015000, length = 0x001000
   //RAMGS10     : origin = 0x016000, length = 0x001000
   //RAMGS11     : origin = 0x017000, length = 0x001000
   RAMGS12     : origin = 0x018000, length = 0x001000
   RAMGS13     : origin = 0x019000, length = 0x001000
    
   /* RAMGS2 | RAMGS3 | RAMGS4 | RAMGS5 | RAMGS6 | RAMGS7 */                                                  
   VARIABLE    : origin = 0x00E000, length = 0x007000  /* Far global/static variables */
   
   /* RAMGS9 | RAMGS10 | RAMGS11 */      
   DEBUG_RAM_P : origin = 0x015000, length = 0x003000
}


SECTIONS
{
/* Allocate program areas: */
   .cinit              : > FLASHD      PAGE = 0
   .pinit              : > USER_APP_ROM      PAGE = 0
   .text               : > USER_APP_ROM      PAGE = 0
   codestart           : > BEGIN             PAGE = 0
       
    boot > FLASHC
    {
        -l rts2800_fpu32.lib(.text)
    }

//#ifdef __TI_COMPILER_VERSION__
//   #if __TI_COMPILER_VERSION__ >= 15009000
//    .TI.ramfunc : {} LOAD = FLASHKL,
//                         RUN = RAMGS12 | RAMGS13 | RAMGS14 |RAMGS15,
//                         LOAD_START(_RamfuncsLoadStart),
//                         LOAD_SIZE(_RamfuncsLoadSize),
//                         LOAD_END(_RamfuncsLoadEnd),
//                         RUN_START(_RamfuncsRunStart),
//                         RUN_SIZE(_RamfuncsRunSize),
//                         RUN_END(_RamfuncsRunEnd),
//                         PAGE = 0
//   #else
   ramfuncs            : LOAD = FLASHKL,
                         RUN = USER_RAM_FUNC,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_SIZE(_RamfuncsLoadSize),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         RUN_SIZE(_RamfuncsRunSize),
                         RUN_END(_RamfuncsRunEnd),
                         PAGE = 0
//   #endif
//#endif
                         
   Boot_ramfuncs       : LOAD = BOOT_RAM_FUNC,
                         RUN = RAM_BOOT,
                         LOAD_START(_BootRamfunsLoadStart),
                         LOAD_SIZE(_BootRamfunsLoadSize),
                         LOAD_END(_BootRamfunsLoadEnd),
                         RUN_START(_BootRamfunsRunStart),
                         RUN_SIZE(_BootRamfunsRunSize),
                         RUN_END(_BootRamfunsRunEnd),
                         PAGE = 0               
   
               
   
   GROUP
   {
       fAPIramfuncs
       { -l F021_API_F2837xS_FPU32.lib}
    
   } LOAD = BOOT_FAPI_FUNC,
     RUN  = RAMGS1, 
     LOAD_START(_FlashApiLoadStart),
     LOAD_END(_FlashApiLoadEnd),
     LOAD_SIZE(_FlashApiLoadSize),
     RUN_START(_FlashApiRunStart),
     RUN_END(_FlashApiRunEnd),
     RUN_SIZE(_FlashApiRunSize),
     PAGE = 0 
    
   
    
   BOOTLOAD_MAIN       : > BOOT_MAIN    PAGE = 0
   BOOTLOAD_APP_ENTRY  : > BOOT_APP_ENTRY  PAGE = 0
   BOOTLOAD_APP        : > BOOT_APP    PAGE = 0
    
   PASSWORD_ROM        : > PASSWORD    PAGE = 0
   MODEL_ROM           : > MODEL       PAGE = 0
   VERSION_ROM         : > VERSION     PAGE = 0
   USER_APP_ENTRY      : > MAIN_UPS    PAGE = 0
         
//   EEPROM_RAM		   : > RAMGS8	   PAGE = 1
   DEBUG_RAM           : > DEBUG_RAM_P      PAGE = 1
	
   
   BOOTER_RAM          : > RAMLS0      PAGE = 0
    					 
   /* Allocate uninitalized data sections: */
   .stack              : > RAMGS0      PAGE = 1
   .esysmem             : > RAMLS5      PAGE = 1
   .ebss               : RUN = VARIABLE,
                         RUN_START(_ebssStart),
   			 			 RUN_END(_ebssEnd),
   			 			 PAGE = 1
   			 			 
   /* Initalized sections go in Flash */
   .econst             : > FLASHM      PAGE = 0
   .switch             : > FLASHN      PAGE = 0
   
   .reset              : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */
   
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
