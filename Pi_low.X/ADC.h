/* 
 * File:   ADC.h
 * Author: will
 *
 * Created on August 27, 2022, 10:16 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Battery voltage is divided by 3:1, then compared against 4.096v
    // i.e. 0xfff0 = 4.096*3 = 12.288
    // or working the other way
    // 1v => (1 * (65535/(3*4.096)) = 1 * 5333
    // 12 v = 12 * 5333
#define OneV 5333.25    


#define V_LOW  (uint16_t)(10*OneV)
#define V_OK   (uint16_t)(11*OneV)
#define V_GOOD (uint16_t)(12*OneV)
#define V_FULL (uint16_t) 0xfff0

    typedef enum {
        ADC0_channel_PWM0 = ADC_MUXPOS_AIN0_gc, /** AIN0 = PD0*/
        ADC0_channel_PWM1 = ADC_MUXPOS_AIN1_gc, /** AIN1 = PD1*/
        ADC0_channel_PWM2 = ADC_MUXPOS_AIN2_gc, /** AIN2 = PD2*/
        ADC0_channel_PWM3 = ADC_MUXPOS_AIN3_gc, /** AIN3 = PD3*/
        ADC0_channel_V_SENSE = ADC_MUXPOS_AIN4_gc, /** AIN4 = PD4*/
        ADC0_channel_GPIO0 = ADC_MUXPOS_AIN5_gc, /** AIN5 = PD5*/
        ADC0_channel_GPIO1 = ADC_MUXPOS_AIN6_gc, /** AIN6 = PD6*/
        ADC0_channel_GPIO2 = ADC_MUXPOS_AIN7_gc, /** AIN7 = PD7*/
        ADC0_channel_GPIO3 = ADC_MUXPOS_AIN16_gc, /** AIN16 = PF0*/
        ADC0_channel_ACC5V_SENSE = ADC_MUXPOS_AIN17_gc, /** AIN17 = PF1*/

        ADC0_channel_TEMPSENSE = ADC_MUXPOS_TEMPSENSE_gc,
        ADC0_channel_GND = ADC_MUXPOS_GND_gc,
        ADC0_channel_MAX
    } ADC0_channel_t;

    void roundrobbin_ADC_Callback(void);
    void enable_ADC_Channel(ADC0_channel_t channel);
    bool query_ADC_Channel(ADC0_channel_t channel);
    void disable_ADC_Channel(ADC0_channel_t channel);
    extern uint16_t ADC_results[ADC0_channel_MAX];
    extern uint8_t battery_ok;

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

