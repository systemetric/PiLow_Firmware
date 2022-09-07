/**
  * TCB0 Generated Driver File
  *
  * @file tcb0.c
  *
  * @ingroup tcb0
  * @brief This file contains the driver code for TCB0 module.
  * version TCB0 Driver Version 1.1.2
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

#include "../tcb0.h"

const struct TMR_INTERFACE TCB0_Interface = {
    .Initialize = TCB0_Initialize,
    .Start = TCB0_Start,
    .Stop = TCB0_Stop,
    .PeriodCountSet = TCB0_Write,
    .TimeoutCallbackRegister = NULL,
    .Tasks = NULL
};




void TCB0_Initialize(void)
{
    //Compare or Capture
    TCB1.CCMP = 0xFFFF;

    //Count
    TCB0.CNT = 0x0;

    //ASYNC disabled; CCMPEN enabled; CCMPINIT disabled; CNTMODE PWM8; 
    TCB0.CTRLB = 0x17;
    
    //DBGRUN disabled; 
    TCB0.DBGCTRL = 0x0;

    //CAPTEI disabled; EDGE disabled; FILTER disabled; 
    TCB0.EVCTRL = 0x0;

    //CAPT disabled; OVF disabled; 
    TCB0.INTCTRL = 0x0;

    //CAPT disabled; OVF disabled; 
    TCB0.INTFLAGS = 0x0;

    //Temporary Value
    TCB0.TEMP = 0x0;

    //CASCADE disabled; CLKSEL DIV1; ENABLE enabled; RUNSTDBY disabled; SYNCUPD disabled; 
    TCB0.CTRLA = 0x1;

}

void TCB0_Start(void)
{
    TCB0.CTRLA |= TCB_ENABLE_bm; /* Start Timer */
}

void TCB0_Stop(void)
{
    TCB0.CTRLA &= ~TCB_ENABLE_bm; /* Stop Timer */
}

void TCB0_Write(uint16_t timerVal)
{
    TCB0.CNT = timerVal;
}

uint16_t TCB0_Read(void)
{
    uint16_t readVal;

    readVal = TCB0.CNT;

    return readVal;
}

void TCB0_EnableCaptInterrupt(void)
{
    TCB0.INTCTRL |= TCB_CAPT_bm; /* Capture or Timeout: enabled */
}

void TCB0_EnableOvfInterrupt(void)
{
	TCB0.INTCTRL |= TCB_OVF_bm; /* Overflow Interrupt: enabled */
}

void TCB0_DisableCaptInterrupt(void)
{
    TCB0.INTCTRL &= ~TCB_CAPT_bm; /* Capture or Timeout: disabled */
}

void TCB0_DisableOvfInterrupt(void)
{
	TCB0.INTCTRL &= ~TCB_OVF_bm; /* Overflow Interrupt: disabled */

}

inline void TCB0_ClearCaptInterruptFlag(void)
{
    TCB0.INTFLAGS &= ~TCB_CAPT_bm;
}

inline void TCB0_ClearOvfInterruptFlag(void)
{
	TCB0.INTFLAGS &= ~TCB_OVF_bm;

}

inline bool TCB0_IsCaptInterruptEnabled(void)
{
    return ((TCB0.INTCTRL & TCB_CAPT_bm) > 0);
}

inline bool TCB0_IsOvfInterruptEnabled(void)
{
        return ((TCB0.INTCTRL & TCB_OVF_bm) > 0);
}

/* 8-bit PWM API's */      
void TCB0_PWM_Enable(void)
{
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

void TCB0_PWM_Disable(void)
{
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void TCB0_load_counter(TCB0_pwm_register_t counter_value)
{
    TCB0.CNT = counter_value;
}

void TCB0_load_top(TCB0_pwm_register_t top_value)
{
    TCB0.CCMPL = top_value;
}

void TCB0_load_duty_cycle(TCB0_pwm_register_t duty_value)
{
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
    TCB0.CCMPL = 0xff;       // workaround hardware bug
	TCB0.CCMPH = duty_value;
	TCB0.CTRLA |= TCB_ENABLE_bm;
}

TCB0_pwm_register_t TCB0_read_duty_cycle(void)
{
	return TCB0.CCMPH;
}
