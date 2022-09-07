#include "mcc_generated_files/system/system.h"
#include "I2C_PI_Interface.h"
#include "Servo_PWM.h"
#include "ADC.h"
#include "SPI.h"
#include "Error_Indicator.h"

#define REVISION 10


/* Status variables */
//uint8_t Motor_A_Mag = 0;
//uint8_t Motor_B_Mag = 0;
//bool Motor_A_Forward = true;
//bool Motor_B_Forward = true;

uint8_t I2C2_MotorState = 0;
uint8_t I2C2_SystemState = 0;
uint8_t debug40 [10] = {0};

// preprocessor functions because of the pin name issues

#define WRITE_D(PIN, DATA) { \
if (!DATA) PIN ## _SetLow(); \
else       PIN ## _SetHigh();\
} 

#define REPORT_IO_STATE(PIN) { \
if (query_ADC_Channel(ADC0_channel_ ## PIN)) TWI0_WriteByte(GPIO_INPUT_ANL); \
else if (query_PWM_channel(PWM_ ## PIN))     TWI0_WriteByte(GPIO_PWM_SERVO); \
else if (PIN ## _GetOutputDir())             TWI0_WriteByte(GPIO_OUT); \
else if (PIN ## _GetPullUp())                TWI0_WriteByte(GPIO_INPUT_PULL); \
else                                         TWI0_WriteByte(GPIO_INPUT); \
}

#define SET_IO_STATE(PIN, STATE) { \
switch (STATE) { \
    case GPIO_OUT: \
        disable_ADC_Channel(ADC0_channel_ ## PIN);\
        disable_PWM_channel(PWM_ ## PIN);\
        PIN ## _SetDigitalOutput();\
        PIN ## _ResetPullUp();\
        break;\
    case GPIO_INPUT:\
        disable_ADC_Channel(ADC0_channel_ ## PIN);\
        disable_PWM_channel(PWM_ ## PIN);\
        PIN ## _SetDigitalInput();\
        PIN ## _ResetPullUp();\
        break;\
    case GPIO_INPUT_ANL:\
        enable_ADC_Channel(ADC0_channel_ ## PIN);\
        disable_PWM_channel(PWM_ ## PIN);\
        PIN ## _SetDigitalInput();\
        PIN ## _ResetPullUp();\
        break;\
    case GPIO_INPUT_PULL:\
        disable_ADC_Channel(ADC0_channel_ ## PIN);\
        disable_PWM_channel(PWM_ ## PIN);\
        PIN ## _SetDigitalInput();\
        PIN ## _SetPullUp();\
        break;\
    case GPIO_PWM_SERVO:\
        disable_ADC_Channel(ADC0_channel_ ## PIN);\
        enable_PWM_channel(PWM_ ## PIN);\
        PIN ## _SetDigitalOutput();\
        PIN ## _ResetPullUp();\
    } \
}

bool I2C_EEPROM_Callback(i2c_client_transfer_event_t event) {
    // behaviour observed: Address match always followed by a RX means the device 
    // address has matched and the following data is the "internal" address
    // consecutive writes are writes to this address and the following addresses 
    // in ascending order
    // consecutive reads are reads to this address and the following addresses in 
    // ascending order
    static char i2cstring [64] = "Brainbox3.0 beta -  Robocon (c) Will Munns";
    //  STATUS_SetHigh(); 
    static uint8_t addr = 0;
    static bool next_rx_is_address = false;
    static uint8_t storedLowValue;
    uint8_t data;
    switch (event) {
        case I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH:
            // if you're happy and you know it clap your hands
            next_rx_is_address = true;
            break;
        case I2C_CLIENT_TRANSFER_EVENT_TX_READY:
            /* In the callback, client must write to transmit register by calling I2Cx_WriteByte() */
            switch (addr++) {
                case I2C2_PWM0_H:
                    TWI0_WriteByte(PWM_PWM0_Pulse_Len >> 8);
                    break;
                case I2C2_PWM1_H:
                    TWI0_WriteByte(PWM_PWM1_Pulse_Len >> 8);
                    break;
                case I2C2_PWM2_H:
                    TWI0_WriteByte(PWM_PWM2_Pulse_Len >> 8);
                    break;
                case I2C2_PWM3_H:
                    TWI0_WriteByte(PWM_PWM3_Pulse_Len >> 8);
                    break;

                case I2C2_PWM0_L:
                    TWI0_WriteByte(PWM_PWM0_Pulse_Len);
                    break;
                case I2C2_PWM1_L:
                    TWI0_WriteByte(PWM_PWM1_Pulse_Len);
                    break;
                case I2C2_PWM2_L:
                    TWI0_WriteByte(PWM_PWM2_Pulse_Len);
                    break;
                case I2C2_PWM3_L:
                    TWI0_WriteByte(PWM_PWM3_Pulse_Len);
                    break;

                case I2C2_GPIO0_C:
                    // extract state of GPIO, precidence analog, Out, Pullup, in
                    REPORT_IO_STATE(GPIO0);
                    break;
                case I2C2_GPIO1_C:
                    REPORT_IO_STATE(GPIO1);
                    break;
                case I2C2_GPIO2_C:
                    REPORT_IO_STATE(GPIO2);
                    break;
                case I2C2_GPIO3_C:
                    REPORT_IO_STATE(GPIO3);
                    break;
                case I2C2_PWM0_C:
                    REPORT_IO_STATE(PWM0);
                    break;
                case I2C2_PWM1_C:
                    REPORT_IO_STATE(PWM1);
                    break;
                case I2C2_PWM2_C:
                    REPORT_IO_STATE(PWM2);
                    break;
                case I2C2_PWM3_C:
                    REPORT_IO_STATE(PWM3);
                    break;

                case I2C2_GPIO0_D:
                    TWI0_WriteByte(GPIO0_GetValue());
                    break;
                case I2C2_GPIO1_D:
                    TWI0_WriteByte(GPIO1_GetValue());
                    break;
                case I2C2_GPIO2_D:
                    TWI0_WriteByte(GPIO2_GetValue());
                    break;
                case I2C2_GPIO3_D:
                    TWI0_WriteByte(GPIO3_GetValue());
                    break;
                case I2C2_PWM0_D:
                    TWI0_WriteByte(PWM0_GetValue());
                    break;
                case I2C2_PWM1_D:
                    TWI0_WriteByte(PWM1_GetValue());
                    break;
                case I2C2_PWM2_D:
                    TWI0_WriteByte(PWM2_GetValue());
                    break;
                case I2C2_PWM3_D:
                    TWI0_WriteByte(PWM3_GetValue());
                    break;

                    //               case I2C2_STATUS:
                    //TWI0_WriteByte(STATUS_GetValue());
                    //                  break;
                    /*        
                        case I2C2_UNSAFE_MODE:
                        case I2C2_ENABLE_12V:
                     */
                    // Analog In handle    
                    // GPIO 0
                case I2C2_GPIO0_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO0] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_GPIO0]);
                    break;

                case I2C2_GPIO1_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO1] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_GPIO1]);
                    break;

                case I2C2_GPIO2_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO2] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_GPIO2]);
                    break;

                case I2C2_GPIO3_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO3] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_GPIO3]);
                    break;

                case I2C2_BATTV_H:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_V_SENSE] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_V_SENSE]);
                    break;

                case I2C2_PWM0_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_PWM0] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_PWM0]);
                    break;

                case I2C2_PWM1_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_PWM1] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_PWM1]);
                    break;

                case I2C2_PWM2_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO2] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_PWM2]);
                    break;

                case I2C2_PWM3_AH:
                    TWI0_WriteByte((uint8_t) ((uint16_t) ADC_results[ADC0_channel_GPIO3] / (uint16_t) 256));
                    storedLowValue = (uint8_t) (ADC_results[ADC0_channel_PWM3]);
                    break;

                case I2C2_GPIO0_AL:
                case I2C2_GPIO1_AL:
                case I2C2_GPIO2_AL:
                case I2C2_GPIO3_AL:
                case I2C2_PWM0_AL:
                case I2C2_PWM1_AL:
                case I2C2_PWM2_AL:
                case I2C2_PWM3_AL:
                case I2C2_BATTV_L:
                    TWI0_WriteByte(storedLowValue);
                    break;
                    // no FVR

                    /*
                        case I2C2_FVR_L:
                        case I2C2_FVR_H:
                     */
                case I2C2_VERSION:
                    TWI0_WriteByte(REVISION);
                    break;

                    //case I2C2_ENABLE_12V_ACC:
                    //case I2C2_ENABLE_5V_ACC:
                case I2C2_ENABLE_5V_ACC:
                    TWI0_WriteByte(ACC5V_PWR_GetValue());
                    break;

                case I2C2_MOTOR_1_MAG:
                    TWI0_WriteByte(TCB0_read_duty_cycle());
                    break;
                case I2C2_MOTOR_0_MAG:
                    TWI0_WriteByte(TCB1_read_duty_cycle());
                    break;
                case I2C2_MOTOR_1_DIR:
                    TWI0_WriteByte(TCB0_Read());
                    break;
                case I2C2_MOTOR_0_DIR:
                    TWI0_WriteByte(TCB1_Read());
                    break;

                case I2C2_MOTOR_ERROR_STATE:
                    TWI0_WriteByte(I2C2_MotorState);
                    break;

                case I2C2_SYSTEM_ERROR_STATE:
                    TWI0_WriteByte(I2C2_SystemState);
                    break;


                case I2C2_DEBUG50:
                    TWI0_WriteByte(debug40[0]);
                    break;

                case I2C2_DEBUG51:
                    TWI0_WriteByte(debug40[1]);
                    break;

                case I2C2_DEBUG52:
                    TWI0_WriteByte(debug40[2]);
                    break;

                case I2C2_DEBUG53:
                    TWI0_WriteByte(debug40[3]);
                    break;

                case I2C2_DEBUG54:
                    TWI0_WriteByte(debug40[4]);
                    break;

                case I2C2_DEBUG55:
                    TWI0_WriteByte(debug40[5]);
                    break;

                case I2C2_DEBUG56:
                    TWI0_WriteByte(debug40[6]);
                    break;

                case I2C2_DEBUG57:
                    TWI0_WriteByte(debug40[7]);
                    break;

                case I2C2_DEBUG58:
                    TWI0_WriteByte(debug40[8]);
                    break;

                case I2C2_DEBUG59:
                    TWI0_WriteByte(debug40[9]);
                    break;

                case I2C2_STRING:
                default:
                    if (addr - 1 > I2C2_STRING && addr - 64 < I2C2_STRING)
                        TWI0_WriteByte(i2cstring[(addr - 2) - I2C2_STRING]);
                    else
                        if (addr > 0x80) {
                        TWI0_WriteByte(ADC_results[addr - 0x81] / 256);
                    } else
                        TWI0_WriteByte(0xff);
                    break;
            }
            break;
        case I2C_CLIENT_TRANSFER_EVENT_RX_READY:
            /* TWI Host wants to write. In the callback, client must read data by calling I2Cx_ReadByte()  */
            data = TWI0_ReadByte();
            if (next_rx_is_address) {
                addr = data;
                next_rx_is_address = false;
            } else {
                // temp variable to store high (software writes high then low over I2C when sending 16 bits)
                static uint16_t HighValStore = 0;
                switch (addr++) {
                    case I2C2_PWM0_H:
                    case I2C2_PWM1_H:
                    case I2C2_PWM2_H:
                    case I2C2_PWM3_H:
                        HighValStore = data << 8;
                        break;
                    case I2C2_PWM0_L:
                        PWM_Channel_Mask |= PWM_PWM0;
                        PWM_PWM0_Pulse_Len = HighValStore + data;
                        break;
                    case I2C2_PWM1_L:
                        PWM_Channel_Mask |= PWM_PWM1;
                        PWM_PWM1_Pulse_Len = HighValStore + data;
                        break;
                    case I2C2_PWM2_L:
                        PWM_Channel_Mask |= PWM_PWM2;
                        PWM_PWM2_Pulse_Len = HighValStore + data;
                        break;
                    case I2C2_PWM3_L:
                        PWM_Channel_Mask |= PWM_PWM3;
                        PWM_PWM3_Pulse_Len = HighValStore + data;
                        break;

                    case I2C2_GPIO0_C:
                        SET_IO_STATE(GPIO0, data);
                        break;
                    case I2C2_GPIO1_C:
                        SET_IO_STATE(GPIO1, data);
                        break;
                    case I2C2_GPIO2_C:
                        SET_IO_STATE(GPIO2, data);
                        break;
                    case I2C2_GPIO3_C:
                        SET_IO_STATE(GPIO3, data);
                        break;
                        
                    case I2C2_PWM0_C:
                        SET_IO_STATE(PWM0, data);
                        break;
                    case I2C2_PWM1_C:
                        SET_IO_STATE(PWM1, data);
                        break;
                    case I2C2_PWM2_C:
                        SET_IO_STATE(PWM2, data);
                        break;
                    case I2C2_PWM3_C:
                        SET_IO_STATE(PWM3, data);
                        break;                     
         

                    case I2C2_GPIO0_D:
                        WRITE_D(GPIO0, data);
                        break;
                    case I2C2_GPIO1_D:
                        WRITE_D(GPIO1, data);
                        break;                    
                    case I2C2_GPIO2_D:
                        WRITE_D(GPIO2, data);
                        break;                    
                    case I2C2_GPIO3_D:
                        WRITE_D(GPIO3, data);
                        break;
                    
                    case I2C2_PWM0_D:
                        WRITE_D(PWM0, data);
                        break;
                    case I2C2_PWM1_D:
                        WRITE_D(PWM1, data);
                        break;
                    case I2C2_PWM2_D:
                        WRITE_D(PWM2, data);
                        break;
                    case I2C2_PWM3_D:
                        WRITE_D(PWM3, data);
                        break;
                      

                   
                    case I2C2_UNSAFE_MODE:
                        // unsafe mode stops driving the Power control and leaves it tristate 
                        // needed prior to programming ??
                        MAIN_PWR_SetDigitalInput();
                        break;
                    case I2C2_ENABLE_MOTORS:
                        if (data == 0) {
                            MOTORS_DISABLE();
                            I2C2_MotorState = 0;
                            Clear_LED_Error(ERR_MOTOR_TRIP);
                        } else {
                            Stabalization_lockout();
                            // RTC PIT is used to hold off error checking for 0.25s to allow power to settle
                            if (!(I2C2_SystemState & (I2C2_Motor_A_Overheat | I2C2_Motor_B_Overheat))) {
                                // overheat locks out motor enable til system reboot
                                MOTORS_ENABLE();
                            }
                            I2C2_MotorState = 0;
                        }
                        SPI_Update();
                        break;
                    case I2C2_ENABLE_MOTOR_PWR:
                        if (data == 0) {
                            MOTOR_PWR_SetLow();
                            I2C2_SystemState &= ~I2C2_System_MotorPower_Fault;
                            Clear_LED_Error(ERR_CURRENT_TRIP_A);
                        } else {
                            Stabalization_lockout();
                            // RTC PIT is used to hold off error checking for 0.25s to allow power to settle
                            MOTOR_PWR_SetHigh();
                            I2C2_MotorState = 0;
                        }
                        break;
                    case I2C2_ENABLE_12V_ACC:
                        if (data == 0) {
                            ACC12V_PWR_SetLow();
                            I2C2_SystemState &= ~I2C2_System_12v_Fault;
                            Clear_LED_Error(ERR_CURRENT_TRIP_B);
                        } else {
                            Stabalization_lockout();
                            ACC12V_PWR_SetHigh();
                        }
                        break;
                    case I2C2_ENABLE_5V_ACC:
                        if (data == 0) {
                            ACC5V_PWR_SetLow();
                            I2C2_SystemState &= ~I2C2_System_5v_Fault;
                            Clear_LED_Error(ERR_LOW_VOLTAGE_5V);
                        } else {
                            Stabalization_lockout();
                            ACC5V_PWR_SetHigh();
                        }
                        break;
                    case I2C2_MOTOR_1_MAG:
                        // note, PWM counts down, so we need to invert this,
                        // but we don't because the NAND logic that routes
                        // signals already inverts it
                        TCB0_load_duty_cycle(data);
                        break;
                    case I2C2_MOTOR_0_MAG:
                        TCB1_load_duty_cycle(data);
                        break;
                    case I2C2_MOTOR_1_DIR:
                        if (data)
                            MOTOR_A_FORWARD();
                        else
                            MOTOR_A_BACKWARD();
                        break;

                    case I2C2_MOTOR_0_DIR:
                        if (data)
                            MOTOR_B_FORWARD();
                        else
                            MOTOR_B_BACKWARD();
                        SPI_Update();
                        break;
                    case I2C2_STATUS:
                        if (data)
                            LED_USER_SetHigh();
                        else
                            LED_USER_SetLow();
                        SPI_Update();
                        break;

                        //default: // If not one of above cases, do nothing
                }
            }
            break;
        default:;
    }
    return true;
}
