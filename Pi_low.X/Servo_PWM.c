


#include "mcc_generated_files/system/system.h"
#include "Servo_PWM.h"
#include "SPI.h"

uint8_t PWM_Channel_Mask = 0xf; // should be zero if we let users treat these like IO?
uint8_t PWM_Current_Channel;

uint16_t PWM_PWM0_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_PWM1_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_PWM2_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_PWM3_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_GPIO0_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_GPIO1_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_GPIO2_Pulse_Len = PWM_SERVO_CENTRAL;
uint16_t PWM_GPIO3_Pulse_Len = PWM_SERVO_CENTRAL;

void enable_PWM_channel (PWM_Channel_Selector channel){
    PWM_Channel_Mask |= 1 << channel;
}

void disable_PWM_channel (PWM_Channel_Selector channel){
    PWM_Channel_Mask &= ~(1 << channel);
}

bool query_PWM_channel (PWM_Channel_Selector channel){
    return PWM_Channel_Mask & (1 << channel);
}

void PWM_Generator_callback(void) {
    // this function is called on an overflow from TCA
    // that means that we need to reset the previous output to zero
    // and restart the timer for the next output
    // CPUINT.LVL1VEC = TCA0_OVF_vect_num; in interrupt.c to make TCA 
    // higher priority and remove servo jitter

    if ((1 << PWM_Current_Channel) & PWM_Channel_Mask) {
        // the channel was enabled, so set it low
        switch (PWM_Current_Channel) {
            case PWM_PWM0:
                PWM0_SetLow();
                break;
            case PWM_PWM1:
                PWM1_SetLow();
                break;
            case PWM_PWM2:
                PWM2_SetLow();
                break;
            case PWM_PWM3:
                PWM3_SetLow();
                break;
            case PWM_GPIO0:
                GPIO0_SetLow();
                break;
            case PWM_GPIO1:
                GPIO1_SetLow();
                break;
            case PWM_GPIO2:
                GPIO2_SetLow();
                break;
            case PWM_GPIO3:
                GPIO3_SetLow();
                break;
            default:
                // Error?
                break;
        }
    }
    // note, there should be at least 20ms between pulses on each line
    // assuming the shortest pulse of 0.6ms on all channels we need to 
    // add 15.2ms of "dead time"

    PWM_Current_Channel = (PWM_Current_Channel == PWM_DEADTIME) ? PWM_PWM0 : PWM_Current_Channel + 1;

    if (1 << PWM_Current_Channel & PWM_Channel_Mask) {
        switch (PWM_Current_Channel) {
            case PWM_PWM0:
                PWM0_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_PWM0_Pulse_Len;
                break;
            case PWM_PWM1:
                PWM1_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_PWM1_Pulse_Len;
                break;
            case PWM_PWM2:
                PWM2_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_PWM2_Pulse_Len;
                break;
            case PWM_PWM3:
                PWM3_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_PWM3_Pulse_Len;
                break;
            case PWM_GPIO0:
                GPIO0_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_GPIO0_Pulse_Len;
                break;
            case PWM_GPIO1:
                GPIO1_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_GPIO1_Pulse_Len;
                break;
            case PWM_GPIO2:
                GPIO2_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_GPIO2_Pulse_Len;
                break;
            case PWM_GPIO3:
                GPIO3_SetHigh();
                TCA0.SINGLE.CNT = 0;
                TCA0.SINGLE.PER = PWM_GPIO3_Pulse_Len;
                break;
                // NOTE: DEADTIME will not get here as it will shift out of 
                // the uint8_t - handle it as a special case
        }
    }
    if (PWM_Current_Channel == PWM_DEADTIME) {
        TCA0.SINGLE.CNT = 0;
        TCA0.SINGLE.PER = PWM_DEADTIME_15m4;
    }
    TCA0_ClearOverflowInterruptFlag();
}

