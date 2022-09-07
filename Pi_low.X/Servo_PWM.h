/* 
 * File:   Servo_PWM.h
 * Author: will
 *
 * Created on August 27, 2022, 10:02 PM
 */

#ifndef SERVO_PWM_H
#define	SERVO_PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*
       Defines for Servo signal generation
     */

    typedef enum {
        PWM_PWM0,
        PWM_PWM1,
        PWM_PWM2,
        PWM_PWM3,
        PWM_GPIO0,
        PWM_GPIO1,
        PWM_GPIO2,
        PWM_GPIO3,
        PWM_DEADTIME
    } PWM_Channel_Selector;
    
    void enable_PWM_channel  (PWM_Channel_Selector channel);
    void disable_PWM_channel (PWM_Channel_Selector channel);
    bool query_PWM_channel   (PWM_Channel_Selector channel);

    // note, the above channels are used as a bit mask, if more than 8
    // channels, you will need to add bits to the following datatype
    extern uint8_t PWM_Channel_Mask;
    extern uint16_t PWM_PWM0_Pulse_Len;
    extern uint16_t PWM_PWM1_Pulse_Len;
    extern uint16_t PWM_PWM2_Pulse_Len;
    extern uint16_t PWM_PWM3_Pulse_Len;
    extern uint16_t PWM_GPIO0_Pulse_Len;
    extern uint16_t PWM_GPIO1_Pulse_Len;
    extern uint16_t PWM_GPIO2_Pulse_Len;
    extern uint16_t PWM_GPIO3_Pulse_Len;

    // hard codec value for 30Mhz
#define PWM_DEADTIME_15m4 0xb21f
#define PWM_SERVO_CENTRAL 0x1193

    void PWM_Generator_callback(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERVO_PWM_H */

