/* 
 * File:   Error_Indicator.h
 * Author: will
 *
 * Created on August 27, 2022, 11:20 PM
 */

#ifndef ERROR_INDICATOR_H
#define	ERROR_INDICATOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* 
     * Led Blink code for error light
     * 
     * Not yet implemented
     *  
     */

    typedef enum {
        // increasing severity
        ERR_LOW_POWER_LOCKOUT = 1, // when the voltage is low, we do not allow 
        // the motors or ACC ports to be enabled
        // but if they are already enabled, we will 
        // allow running till critical or new 
        // round detected
        ERR_LOW_VOLTAGE_5V = 2, // No power detected when expected on 5vACC
        ERR_CURRENT_TRIP_A = 3, // Current Limit or Overload on A (motors?))
        ERR_CURRENT_TRIP_B = 4, // Current Limit or overload on 12Vacc
        ERR_MOTOR_TRIP = 5,     // Some kind of motor trip
        ERR_OVERHEAT = 6,
        ERR_MAX_LEN 
    } LED_ERROR_STATES;


    void Set_LED_Error(uint8_t);
    void Clear_LED_Error(uint8_t);
    void Peroidic_worker(void);
    void Stabalization_lockout(void);
    bool Power_Stable(void);

#define LOCKOUT_CYCLES 4
    
    
extern uint8_t error_code;
extern uint8_t loop_count;
    

#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_INDICATOR_H */

