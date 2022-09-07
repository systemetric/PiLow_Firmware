/**
 * TCB1 Generated Driver API Header File
 *
 * @file tcb1.h
 *
 * defgroup tcb1 TCB1
 *
 * @brief This header file provides APIs for the TCB1 driver.
 *
 * @version TCB1 Driver Version 1.1.2
*/
/*
© [2022] Microchip Technology Inc. and its subsidiaries.

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

#ifndef TCB1_H_INCLUDED
#define TCB1_H_INCLUDED

#include <stdint.h>
#include "../system/utils/compiler.h"
#include "./timer_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const struct TMR_INTERFACE TCB1_Interface;

/**
 * @ingroup tcb1
 * @typedef uint8_t TCB1_pwm_register_t
 * @brief Datatype for the result of 8-bit PWM register
 */ 
typedef uint8_t TCB1_pwm_register_t;
/**
 * @ingroup tcb1
 * @brief PWM Enable TCB1. Enables the Timer/Counter B Peripheral.
 * @param none
 * @return none
 */
void TCB1_PWM_Enable(void);
/**
 * @ingroup tcb1
 * @brief PWM Disable TCB1. PWM disable for Timer/Counter.
 * @param none
 * @return none
 */
void TCB1_PWM_Disable(void);
/**
 * @ingroup tcb1
 * @brief Loading counter value for 8-bit PWM
 * @param TCB1_pwm_register_t counter_value - Counter value for PWM
 * @return none
 */
void TCB1_load_counter(TCB1_pwm_register_t counter_value);
/**
 * @ingroup tcb1
 * @brief Counter reaches top when it becomes equal to the highest value in the count sequence.
 * @param TCB1_pwm_register_t top_value - Loading the top value for PWM
 * @return none
 */
void TCB1_load_top(TCB1_pwm_register_t top_value);
/**
 * @ingroup tcb1
 * @brief Loading duty cycle value for 8-bit PWM
 * @param TCB1_pwm_register_t duty_value - Loading PWM duty value
 * @return none
 */
void TCB1_load_duty_cycle(TCB1_pwm_register_t duty_value);
TCB1_pwm_register_t TCB1_read_duty_cycle(void);

/**
 * @ingroup tcb1
 * @brief Initialize tcb interface 
 * @param none
 * @return none
 */
void TCB1_Initialize(void);
/**
 * @ingroup tcb1
 * @brief start tcb interface 
 * @param none
 * @return none
 */
void TCB1_Start(void);
/**
 * @ingroup tcb1
 * @brief stop tcb interface 
 * @param none
 * @return none
 */
void TCB1_Stop(void);
/**
 * @ingroup tcb1
 * @brief Enable the capture interrupt for tcb interface 
 * @param none
 * @return none
 */
void TCB1_EnableCaptInterrupt(void);
/**
 * @ingroup tcb1
 * @brief Disable the capture interrupt for tcb interface 
 * @param none
 * @return none
 */
void TCB1_DisableCaptInterrupt(void);
/**
 * @ingroup tcb1
 * @brief Enable the overflow interrupt for tcb interface 
 * @param none
 * @return none
 */
void TCB1_EnableOvfInterrupt(void);
/**
 * @ingroup tcb1
 * @brief Disable the overflow interrupt for tcb interface 
 * @param none
 * @return none
 */
void TCB1_DisableOvfInterrupt(void);
/**
 * @ingroup tcb1
 * @brief Read the 16bit timer value for tcb 
 * @param none
 * @return uint16_t
 */
uint16_t TCB1_Read(void);
/**
 * @ingroup tcb1
 * @brief Write the 16bit timer value for tcb 
 * @param uint16_t timerVal - 16-bit Timer value to write for tcb interface
 * @return none
 */
void TCB1_Write(uint16_t timerVal);
/**
 * @ingroup tcb1
 * @brief Clearing the capture interrupt flag after the capture overflow flag set.
 * @param none
 * @return none
 */
void TCB1_ClearCaptInterruptFlag(void);
/**
 * @ingroup tcb1
 * @brief Enable the capture interrupt to set the capture interrupt flag, when interrupt occur.
 * @param none
 * @return none
 */
bool TCB1_IsCaptInterruptEnabled(void);
/**
 * @ingroup tcb1
 * @brief Clearing the Overflow interrupt flag after the overflow flag set.
 * @param none
 * @return none
 */
void TCB1_ClearOvfInterruptFlag(void);
/**
 * @ingroup tcb1
 * @brief Enable the Overflow interrupt to set overflow flag, when overflow occur.
 * @param none
 * @return none
 */
bool TCB1_IsOvfInterruptEnabled(void);



#ifdef __cplusplus
}
#endif

#endif /* TCB1_H_INCLUDED */