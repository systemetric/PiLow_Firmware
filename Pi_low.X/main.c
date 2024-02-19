/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
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
// define this to prevent the AVR signalling the pi
#undef NOSHUTDOWN

#include <avr/sleep.h>

#include "mcc_generated_files/system/system.h"
#include "main.h"

#include "I2C_PI_Interface.h"
#include "Servo_PWM.h"
#include "SPI.h"
#include "ADC.h"
#include "Error_Indicator.h"
#include "mcc_generated_files/reset/rstctrl.h"


uint16_t gpioCounter0,gpioCounter2 = 0;
void GPIOCounter0_Update(void) {
    gpioCounter0 += GPIO1_GetValue()?-1:1;

}
void GPIOCounter2_Update(void) {
    gpioCounter2 += GPIO3_GetValue()?-1:1;

}


int main(void) {

    uint8_t state = SSM_Reset;

    while (1) {
        switch (state) {
            case SSM_Reset:
                SYSTEM_PreInitialize(); // only start up the minimum of the system, no interrupts

                VREF_ADC0REF = VREF_REFSEL_4V096_gc | 0x80;
                // Boot only when battey is good
                if (ADC0_GetConversion(ADC0_channel_V_SENSE) < V_LOW)
                {
                    // it feels like we should not go into this cul-de-sac if 
                    // the PI is self powered but if we "trap" in here for "no 
                    // battery" then the PI can be safely used for recovery with 
                    // USB power and we don't really need the Start button 
                    // workaround
                    
                    // The only issue with this is when the PI has applied a 
                    // firmware patch on battery power and the battery falls 
                    // to low for us to boot the AVR the pi remains on and 
                    // cannot be cleanly shut down. 
                    
                    // Perhaps we should lower our standards for a pre-booted
                    // system, or only alow patches from the PI end when well 
                    // charged. 
                    // Perhaps we should do both!
                    
                    // The battery is low use the helper to set the LED and 
                    // motor control
                    SPI_boot_low_voltage();
                    // shut down PWM and GPIO??
                    // should all have been done by PreInit?
                    // Just go to sleep and never wake up
                    // This consumes a lot more power than expected. 
                    // The cover is still running, but now using an LDO, perhaps 
                    // future versions should power the motor driver logic and 
                    // Power LED from another source?
                    MAIN_PWR_SetLow();
                    MAIN_PWR_SetDigitalOutput();
                    sleep_mode();
                } else {
                    // time to bring up the PI 
                    SYSTEM_Initialize();
                    MAIN_PWR_SetHigh();
                    MAIN_PWR_SetDigitalOutput();
                    RTC_WriteCounter(0); // start timer used to check boot happens
                    state = SSM_BootPI;
                }
                break;
            case SSM_BootPI:
            {
                // by now the PI should be booting and all perhiphals are running
                if (nPI_RUNNING_GetValue()) {
                    if (RTC_ReadCounter() > PI_BOOT_TIME) {
                        // error, the PI has not booted in a reasonable time
                        RTC_WriteCounter(0);
                        state = SSM_ShutDownPI;
                    } else {
                        switch ((RTC_ReadCounter()>>2) & 0x3) {
                            case 0:
                                PROGRESS_0();
                                break;
                            case 1:
                                PROGRESS_1();
                                break;
                            case 2:
                                PROGRESS_2();
                                break;
                            case 3:
                                PROGRESS_3();
                                break;
                        }
                        SPI_ShiftReg_transfer();
                    }
                } else {
                    // Pi Detected running, load up all the helpers
                    TCB0_load_top(0xff);
                    TCB1_load_top(0xff);
                    TCB0_load_duty_cycle(0x00);
                    TCB1_load_duty_cycle(0x00);
                    MOTOR_PWR_SetHigh(); // debug
                    //MOTORS_ENABLE();    // debug
                    //Stabalization_lockout();

                    /* TCA used for Servo PWM control */
                    TCA0_OverflowCallbackRegister(PWM_Generator_callback);

                    /* ADC */
                    ADC0_RegisterResrdyCallback(roundrobbin_ADC_Callback);
                    ADC0_StartConversion(ADC0_channel_V_SENSE);

                    /* PI communication emulates an EEPROM interface for ease of use*/
                    TWI0_CallbackRegister(I2C_EEPROM_Callback);

                    // use the RTC-PI interrupt to flash the error state
                    // use the RTC-PI interrupt to trigger SPI reload instead of running continious?
                    RTC_SetPITIsrCallback(Peroidic_worker);
                    
                    GPIO0_HANDLER(GPIOCounter0_Update);
                    GPIO2_HANDLER(GPIOCounter2_Update);
                    
                    LED_USER_SetLow();
                    LED_ERR_SetLow();

                    state = SSM_FullPower;
                }
                break;
            }
            case SSM_FullPower:
            {
#ifndef NO_SHUTDOWN
                if (nPI_RUNNING_GetValue() || !battery_ok) {
                    //if the PI is shut down (odd?) or the battery is critically low
                    RTC_WriteCounter(0);
                    state = SSM_ShutDownPI;
                }
#endif
                SPI_worker();
                break;
            }
            case SSM_ShutDownPI:

                nSHUTDOWN_SetLow();
                nSHUTDOWN_SetDigitalOutput();
                ACC12V_PWR_SetLow();
                MOTOR_PWR_SetLow();
                ACC5V_PWR_SetLow();
                DISABLE_INTERRUPTS();

                if (nPI_RUNNING_GetValue() || (RTC_ReadCounter() > PI_SD_TIME)) {
                    // If the PI has shut down, or we have to give up waiting
                    MAIN_PWR_SetLow();
                    // If a power switch event caused this then this event will 
                    // turn off all power unless toggled on again. In which case
                    RTC_WriteCounter(0);
                    while ((RTC_ReadCounter() <= 4));

                    RSTCTRL_reset(); // do a software reset
                } else {

                    switch ((RTC_ReadCounter()>>2) & 0x3) {
                        case 0:
                            PROGRESS_3();
                            break;
                        case 1:
                            PROGRESS_2();
                            break;
                        case 2:
                            PROGRESS_1();
                            break;
                        case 3:
                            PROGRESS_0();
                            break;
                    }
                    SPI_ShiftReg_transfer();
                }
                break; 
        }
    }
}