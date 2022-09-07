/**
 * System Driver Source File
 * 
 * @file system.c
 * 
 * @ingroup systemdriver
 * 
 * @brief This is the generated driver implementation file for the System Driver.
 *
 * @version Driver Version 1.0.1
*/
/*
? [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

 /**
   Section: Included Files
 */
#include "../system.h"

/**
 * Initializes MCU, drivers and middleware in the project
**/

int8_t SLPCTRL_Initialize();

void SYSTEM_Initialize(void)
{ 
//    CLOCK_Initialize();
//    PIN_MANAGER_Initialize();
//    ADC0_Initialize();
    I2C0_Client_Initialize();
//    RTC_Initialize();
//    SLPCTRL_Initialize();
//    SPI0_Initialize();
    TCA0_Initialize();
    TCB0_Initialize();
    TCB1_Initialize();
//    VREF_Initialize();
    CPUINT_Initialize();
}

void SYSTEM_PreInitialize(void)
{ 
    CLOCK_Initialize();           // need to do system clocka
    PIN_MANAGER_Initialize();     // need to control externals
    ADC0_Initialize();            // need to read battery status
    RTC_Initialize();             // best way to get nice timing for events
    SPI0_Initialize();            // needed to show status on LEDs
    VREF_Initialize();            // needed for properly reading V_sense
    SLPCTRL_Initialize();
}

int8_t SLPCTRL_Initialize()
{
    //SEN disabled; SMODE PDOWN; 
    ccp_write_io((void*)&(SLPCTRL.CTRLA),0x4);
    
    //PMODE AUTO; 
    ccp_write_io((void*)&(SLPCTRL.VREGCTRL),0x0);
    

    return 0;
}


