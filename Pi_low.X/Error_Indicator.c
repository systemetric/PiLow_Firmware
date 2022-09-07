
#include "mcc_generated_files/system/system.h"
#include "Error_Indicator.h"
#include "SPI.h" 
#include "I2C_PI_Interface.h"
#include "ADC.h"

uint8_t error_code = 0;
uint8_t loop_count = 0;
uint8_t lockout = 0;

void Set_LED_Error(uint8_t new_error) {
    if (error_code < new_error) {
        error_code = new_error;
    }
}

void Clear_LED_Error(uint8_t new_error) {
    // This will clear the error code, but not also any lower codes
    // perhaps we could look at errors in the I2C bitmask?
    if (error_code == new_error) {
        error_code = 0;
    }
}

void Stabalization_lockout(void) {
    lockout = LOCKOUT_CYCLES;
}

bool Power_Stable(void)
{
    return lockout == 0;
}

void Peroidic_worker(void) {
    //flash the Error LED n times with m gap to indicate error state

    if (loop_count & 1) {
        // every even tick we turn off the Error Led
        LED_ERR_SetLow();
    } else if (error_code >= (loop_count >> 1))
    {
        LED_ERR_SetHigh();
    }
    SPI_Update();

    if (loop_count >= ((error_code + 1) << 1) ) // twice as many to account for on and off cycles
        loop_count = 1; // don't reset to zero, because zero means no error so doesn't need checking
    else 
        loop_count += 1;

    if (lockout > 0) { // perhaps we should have different lockouts, for now, one will do
        lockout--;
    } else {
        
        // check the error state of the system every full cycle unless we are 
        // waiting for the power rails to stabalize
        
        // should these workers be run more regurally?, or does the system self 
        // protect and all we are doing is detecting the self protection status?

        if (ACC5V_PWR_GetValue() && !ACC_5V_SENSE_GetValue()) {
            /* There is a overload on the 5v line, shut it down rather than 
             leaving the buck to struggle */
            I2C2_SystemState |= I2C2_System_5v_Fault;
            ACC5V_PWR_SetLow();
            if (error_code < ERR_LOW_VOLTAGE_5V) {
                error_code = ERR_LOW_VOLTAGE_5V;
            }
        }
        if (MOTOR_PWR_GetValue() && MOTOR_PWR_STATUS_GetValue()) {
            /* All error states are indicated by a high */
            I2C2_SystemState |= I2C2_System_MotorPower_Fault;
            MOTOR_PWR_SetLow();
            if (error_code < ERR_CURRENT_TRIP_A) {
                error_code = ERR_CURRENT_TRIP_A;
            }
        }
        if (ACC12V_PWR_GetValue() && ACC12V_STATUS_GetValue()) {
            /* All error states are indicated by a high */
            I2C2_SystemState |= I2C2_System_12v_Fault;
            ACC12V_PWR_SetLow();
            if (error_code < ERR_CURRENT_TRIP_B) {
                error_code = ERR_CURRENT_TRIP_B;
            }
        }
        if ((ADC_results[ADC0_channel_V_SENSE] <= V_LOW) && (ACC12V_PWR_GetValue() || ACC5V_PWR_GetValue())) {
            // how do we detect if we are in a round - can we define that End of Round makes these low?
            if (error_code < ERR_LOW_POWER_LOCKOUT) {
                error_code = ERR_LOW_POWER_LOCKOUT;
            }
        }
    }
}

