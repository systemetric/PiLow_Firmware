
/* 
 ADC WORKERS
 */
#include "mcc_generated_files/system/system.h"
#include "ADC.h"
#include "SPI.h"
#include "I2C_PI_Interface.h" /* for debug only */

uint32_t enabled_ADC_channels = (uint32_t) 1 << ADC0_channel_V_SENSE
        // | (long)1 << ADC0_channel_GPIO0  
        // | (long)1 << ADC0_channel_GPIO1  
        // | (long)1 << ADC0_channel_GPIO2  
        // | (long)1 << ADC0_channel_GPIO3  
        ; //bitmask

ADC0_channel_t current_ADC_channel = ADC0_channel_V_SENSE;

uint16_t ADC_results[ADC0_channel_MAX];

uint8_t battery_ok = 2;

void roundrobbin_ADC_Callback(void) {
    // If there is a conversion underway, then something odd has gone on, just 
    // exit and deal with it when the next conversion comes in
    if (!(ADC0_COMMAND & ADC_STCONV_bm)) {
        uint16_t reading = ADC0_GetConversionResult();
        
        if (ADC0.MUXPOS == ADC0_channel_GND)
        {
            // last reading was for GND, start a new reading for the chanel we 
            // care about
           
            ADC0.COMMAND = ADC_SPCONV_bm;
            ADC0.MUXPOS = current_ADC_channel;
            ADC0.COMMAND = ADC_STCONV_bm;
        }
        else
        {
            ADC_results[current_ADC_channel] = reading;
            
            // start the flushing out of the capacitor
            ADC0.COMMAND = ADC_SPCONV_bm;
            ADC0.MUXPOS = ADC0_channel_GND;
            ADC0.COMMAND = ADC_STCONV_bm;
            
            // work out the next item
            // like this, but it's too slow with 16 bit values and glitches the 
            // I2C
            //uint16_t next = current_ADC_channel + 1;
            //while (!(enabled_ADC_channels & ((uint32_t)1 << next))) {
            //    next += 1;
            //    if (next >= ADC0_channel_MAX) {
            //        next = 0;
            //    }
            //}
            // current_ADC_channel = next;
            // if (current_ADC_channel == ADC0_channel_V_SENSE) {
            //    // special case for VSENSE, select the bandgap reference
            //    VREF_ADC0REF = VREF_REFSEL_4V096_gc | 0x80;
            //} else {
            //    // All others compare against VDD
            //    VREF_ADC0REF = VREF_REFSEL_VDD_gc | 0x80;
            //}
            
            
            // lets ignore the enabled channels and just scan everything in order
            switch (current_ADC_channel){
                case ADC0_channel_V_SENSE:
                    SPI_BatteryMeter(reading);
                    if (reading < V_LOW)
                        battery_ok = battery_ok ? battery_ok - 1 : 0;
                    else
                        battery_ok = 4; // ensure 4 consecutive readings before starting a shutdown
                    current_ADC_channel = ADC0_channel_GPIO0;
                    VREF_ADC0REF = VREF_REFSEL_VDD_gc | 0x80;
                    break;
                case ADC0_channel_GPIO0:
                    current_ADC_channel = ADC0_channel_GPIO1;
                    break;
                case ADC0_channel_GPIO1:
                    current_ADC_channel = ADC0_channel_GPIO2;
                    break;
                case ADC0_channel_GPIO2:
                    current_ADC_channel = ADC0_channel_GPIO3;
                    break;
                case ADC0_channel_GPIO3:
                    current_ADC_channel = ADC0_channel_PWM0;
                    break;
                case ADC0_channel_PWM0:
                    current_ADC_channel = ADC0_channel_PWM1;
                    break;
                case ADC0_channel_PWM1:
                    current_ADC_channel = ADC0_channel_PWM2;
                    break;
                case ADC0_channel_PWM2:   
                    current_ADC_channel = ADC0_channel_PWM3;
                    break;
                case ADC0_channel_PWM3:
                    current_ADC_channel = ADC0_channel_V_SENSE;
                    VREF_ADC0REF = VREF_REFSEL_4V096_gc | 0x80;
                //    break;
                default:
                    // error ?
                    current_ADC_channel = ADC0_channel_V_SENSE;
                    VREF_ADC0REF = VREF_REFSEL_4V096_gc | 0x80;
                    break;
            }
        }       
    }
}

void enable_ADC_Channel(ADC0_channel_t channel) {
    if (channel <= ADC0_channel_TEMPSENSE) {
        // not sure if required, can the ADC be left polling the GPIO when used as GPIO?
        enabled_ADC_channels = enabled_ADC_channels | ((uint32_t)1 << channel);
    }
}

bool query_ADC_Channel(ADC0_channel_t channel) {
    return enabled_ADC_channels & ((uint32_t)1 << channel);
}

void disable_ADC_Channel(ADC0_channel_t channel) {
    // not sure if required, can the ADC be left polling the GPIO when used as GPIO?
    if (channel <= ADC0_channel_TEMPSENSE) {
        enabled_ADC_channels = enabled_ADC_channels & ~((uint32_t)1 << channel);
        ADC_results[channel] = 0; // clear any old results
    }
}