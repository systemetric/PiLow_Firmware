/* 
 * File:   I2C_PI_Interface.h
 * Author: will
 *
 * Created on August 27, 2022, 9:27 PM
 */

#ifndef I2C_PI_INTERFACE_H
#define	I2C_PI_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    extern uint8_t debug40 [10];
    

    bool I2C_EEPROM_Callback(i2c_client_transfer_event_t event);

    typedef enum {
        /* common */
        I2C2_PWM0_H = 1,
        I2C2_PWM0_L = 2,
        I2C2_PWM1_H = 3,
        I2C2_PWM1_L = 4,
        I2C2_PWM2_H = 5,
        I2C2_PWM2_L = 6,
        I2C2_PWM3_H = 7,
        I2C2_PWM3_L = 8,

        I2C2_GPIO0_AH = 9,
        I2C2_GPIO0_AL = 10,
        I2C2_GPIO1_AH = 11,
        I2C2_GPIO1_AL = 12,
        I2C2_GPIO2_AH = 13,
        I2C2_GPIO2_AL = 14,
        I2C2_GPIO3_AH = 15,
        I2C2_GPIO3_AL = 16,

        I2C2_GPIO0_C = 17,
        I2C2_GPIO1_C = 18,
        I2C2_GPIO2_C = 19,
        I2C2_GPIO3_C = 20,

        I2C2_GPIO0_D = 21,
        I2C2_GPIO1_D = 22,
        I2C2_GPIO2_D = 23,
        I2C2_GPIO3_D = 24,

        I2C2_STATUS = 25,

        I2C2_UNSAFE_MODE = 26,

        I2C2_ENABLE_MOTORS = 27,

        I2C2_BATTV_L = 29,
        I2C2_BATTV_H = 28,

        I2C2_FVR_L = 31,
        I2C2_FVR_H = 30,

        I2C2_VERSION = 32,


        /* Version 3+ below this line */
        I2C2_ENABLE_12V_ACC = 33,
        I2C2_ENABLE_5V_ACC = 34,
        I2C2_ENABLE_MOTOR_PWR = 35,

        I2C2_MOTOR_0_MAG = 36,
        I2C2_MOTOR_1_MAG = 37,

        I2C2_MOTOR_0_DIR = 38,
        I2C2_MOTOR_1_DIR = 39,

        I2C2_MOTOR_ERROR_STATE = 40, // a write to this register will clear detected errors
        I2C2_SYSTEM_ERROR_STATE = 41, // system errors are cleared by cycling the error source

        // PWM ports are just GPIO but with -+S plug , maybe we should treat them as such?    

        
        I2C2_GPIO0_PWM_H = 48,
        I2C2_GPIO0_PWM_L = 49,
        I2C2_GPIO1_PWM_H = 50,
        I2C2_GPIO1_PWM_L = 51,
        I2C2_GPIO2_PWM_H = 52,
        I2C2_GPIO2_PWM_L = 53,
        I2C2_GPIO3_PWM_H = 54,
        I2C2_GPIO4_PWM_L = 55,
                
        I2C2_PWM0_AH = 56,
        I2C2_PWM0_AL = 57,
        I2C2_PWM1_AH = 58,
        I2C2_PWM1_AL = 59,
        I2C2_PWM2_AH = 60,
        I2C2_PWM2_AL = 61,
        I2C2_PWM3_AH = 62,
        I2C2_PWM3_AL = 63,
                
        I2C2_PWM0_C = 64, 
        I2C2_PWM1_C = 65,
        I2C2_PWM2_C = 66,
        I2C2_PWM3_C = 67,

        I2C2_PWM0_D = 68,   
        I2C2_PWM1_D = 69,   
        I2C2_PWM2_D = 70,   
        I2C2_PWM3_D = 71,   
                
        // allow setting of GPIO as PWM ports  

        // these are useful for probing SFR's during developemnt, 
        // beware of side effects like clearing IRQs        

        I2C2_DEBUG50 = 0x50,
        I2C2_DEBUG51 = 0x51,
        I2C2_DEBUG52 = 0x52,
        I2C2_DEBUG53 = 0x53,
        I2C2_DEBUG54 = 0x54,
        I2C2_DEBUG55 = 0x55,
        I2C2_DEBUG56 = 0x56,
        I2C2_DEBUG57 = 0x57,
        I2C2_DEBUG58 = 0x58,
        I2C2_DEBUG59 = 0x59,
        I2C2_DEBUG5a = 0x5a,
        I2C2_DEBUG5b = 0x5b,
        I2C2_DEBUG5c = 0x5c,
        I2C2_DEBUG5d = 0x5d,
        I2C2_DEBUG5e = 0x5e,
        I2C2_DEBUG5f = 0x5f,

        I2C2_STRING = 0x80

    } I2C2_eepromAdress;

    typedef enum {
        GPIO_OUT = 0,
        GPIO_INPUT = 1,
        GPIO_INPUT_ANL = 2,
        GPIO_INPUT_PULL = 3,
        GPIO_PWM_SERVO = 4,
        // GPIO_PWM_MARK_SPACE =5,
        // GPIO_ULTRASONIC (needs two GPIO, how to force this?) = 6
    } I2C2_GPIO_STATE;

    typedef enum {
        I2C2_Motor_A_Open_Load = 1 << 0, // not always an error, may be weedy motors
        I2C2_Motor_A_Short = 1 << 1, // dead short or motor too power hungry for driver      
        I2C2_Motor_A_Overheat = 1 << 2, // In excess of 165 Degrees at Junction - beware of the hot case
        I2C2_Motor_A_Short_To_Rail = 1 << 3, // connection detected between GND and motor or 12v and motor
        I2C2_Motor_B_Open_Load = 1 << 4,
        I2C2_Motor_B_Short = 1 << 5,
        I2C2_Motor_B_Overheat = 1 << 6,
        I2C2_Motor_B_Short_To_Rail = 1 << 7,
    } I2C2_Motor_Error_State;

    typedef enum {
        I2C2_System_5v_Fault = 1 << 0, // Short on GPIO or Servo overload
        I2C2_System_MotorPower_Fault = 1 << 1, // Should not happen? 
        I2C2_System_12v_Fault = 1 << 2, // Short or overload on 12v Acc port
    } I2C2_System_Error_State;



    extern uint8_t I2C2_MotorState;
    extern uint8_t I2C2_SystemState;

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_PI_INTERFACE_H */

