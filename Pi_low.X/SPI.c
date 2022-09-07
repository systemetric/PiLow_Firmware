#include "mcc_generated_files/system/system.h"
#include "SPI.h"
#include "I2C_PI_Interface.h"
#include "Error_Indicator.h"
#include "ADC.h"

uint16_t spidata = 0;
volatile bool spi_needs_update = false;

void SPI_Update(void) {
    /* Ensure we are operating in SPI mode 0 */
    //SPI0_CTRLB &= ~SPI_MODE_3_gc;

    //nSS_ShiftReg_SetHigh();
    //SPI0_BufferWrite(&spidata, sizeof(spidata));
    //nSS_ShiftReg_SetLow();
    //nSS_ShiftReg_SetHigh();
    spi_needs_update = true;
}

void SPI_ShiftReg_transfer(void) {
    spi_needs_update = false;
    /* Ensure we are operating in SPI mode 0 */
    SPI0_CTRLB &= ~SPI_MODE_3_gc;

    nSS_ShiftReg_SetHigh();
    SPI0_BufferWrite(&spidata, sizeof (spidata));
    nSS_ShiftReg_SetLow();
    nSS_ShiftReg_SetHigh();
}

void SPI_boot_low_voltage()
{
    
        LED_ERR_SetLow();
        LED_USER_SetLow();
        MOTORS_DISABLE();
        BAT_CRITICAL();
        SPI_ShiftReg_transfer();
}

uint16_t SPI_Motor_transfer(uint8_t command) {
    uint8_t spiData [2];
    spiData[0] = command;

    /* reconfigure SPI for Mode 1 */
    SPI0_CTRLB |= SPI_MODE_1_gc;

    SS_Motor_SetHigh();
    SPI0_BufferExchange(spiData, sizeof spiData);
    SS_Motor_SetLow();
    return (spiData[0] << 8) +spiData[1]; //high (early) bits are ACK, result is in low bits
}

void SPI_BatteryMeter(uint16_t val) {

    if (val >= V_FULL)
        BAT_FULL();
    else if (val > V_GOOD)
        BAT_GOOD();
    else if (val > V_OK)
        BAT_OK();
    else if (val > V_LOW)
        BAT_LOW();
    else 
        BAT_CRITICAL();
    SPI_Update();
}

void SPI_worker(void) {
    if (spi_needs_update && Power_Stable()) {
        // there is a race condition here, if the power is stable and an interrupt
        // changes that we could be in a transient mode because this is running in 
        // user state. This wouldn't happen if this code was running in IRQ state
        uint16_t motorstate;
        
        /* Select Motor A */
        MOTOR_A_SPI_ENABLE();
        SPI_ShiftReg_transfer();
        /* read from Motor*/
        motorstate = SPI_Motor_transfer(9);
        if ((motorstate & 0x3f00) == 0x2a00 && Power_Stable()) {
            // valid reading, and lockout hasn't happened during the transfers
            if (motorstate & TLE7209_Diag_Enable)
            {
                // motors enabled, check in order
                if ((motorstate & (TLE7209_Diag_Over_Temp | TLE7209_Diag_Limit_Temp))
                        != (TLE7209_Diag_Over_Temp | TLE7209_Diag_Limit_Temp)) {
                    I2C2_MotorState |= I2C2_Motor_A_Overheat;
                    Set_LED_Error(ERR_OVERHEAT);
                }
                switch (motorstate & TLE7209_Diag_Mask){
                    case TLE7209_Short:
                        Set_LED_Error(ERR_MOTOR_TRIP);
                        I2C2_MotorState |= I2C2_Motor_A_Short;
                        break;
                    case TLE7209_Short_12_P:
                    case TLE7209_Short_2_P:
                    case TLE7209_Short_1_P:
                    case TLE7209_Short_12_N:
                    case TLE7209_Short_2_N:
                    case TLE7209_Short_1_N:
                        Set_LED_Error(ERR_MOTOR_TRIP);
                        I2C2_MotorState |= I2C2_Motor_A_Short_To_Rail;
                        break;
                }
            }
            else
            if ((motorstate & TLE7209_Diag_Mask) == TLE7209_Open) {
                    // motor is off, may not be "error" but log it
                    I2C2_MotorState |= I2C2_Motor_A_Open_Load;
            }
        }
        
        /* Select Motor B */
        MOTOR_B_SPI_ENABLE();
        SPI_ShiftReg_transfer();
        /* read from Motor*/
        motorstate = SPI_Motor_transfer(9);
        if ((motorstate & 0x3f00) == 0x2a00 && Power_Stable()) {
            // valid reading, and lockout hasn't happened during the transfers
            if (motorstate & TLE7209_Diag_Enable)
            {
                // motors enabled, check in order
                if ((motorstate & (TLE7209_Diag_Over_Temp | TLE7209_Diag_Limit_Temp))
                        != (TLE7209_Diag_Over_Temp | TLE7209_Diag_Limit_Temp)) {
                    I2C2_MotorState |= I2C2_Motor_B_Overheat;
                    Set_LED_Error(ERR_OVERHEAT);
                }
                switch (motorstate & TLE7209_Diag_Mask){
                    case TLE7209_Short:
                        Set_LED_Error(ERR_MOTOR_TRIP);
                        I2C2_MotorState |= I2C2_Motor_B_Short;
                        break;
                    case TLE7209_Short_12_P:
                    case TLE7209_Short_2_P:
                    case TLE7209_Short_1_P:
                    case TLE7209_Short_12_N:
                    case TLE7209_Short_2_N:
                    case TLE7209_Short_1_N:
                        Set_LED_Error(ERR_MOTOR_TRIP);
                        I2C2_MotorState |= I2C2_Motor_B_Short_To_Rail;
                        break;
                }
            }
            else
            if ((motorstate & TLE7209_Diag_Mask) == TLE7209_Open) {
                    // motor is off, may not be "error" but log it
                    I2C2_MotorState |= I2C2_Motor_B_Open_Load;
            }
        }
        
        
        
        
        // are there any errors that mean we should shut down the motor outputs
        if (I2C2_MotorState & ( I2C2_Motor_A_Short | I2C2_Motor_A_Overheat | I2C2_Motor_A_Short_To_Rail 
                              | I2C2_Motor_B_Short | I2C2_Motor_B_Overheat | I2C2_Motor_B_Short_To_Rail))
        {
            MOTORS_DISABLE();
            SPI_ShiftReg_transfer();
        }
    }
}