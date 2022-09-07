/* 
 * File:   SPI.h
 * Author: will
 *
 * Created on August 27, 2022, 10:23 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern uint16_t spidata;
    void SPI_Update(void);
    uint16_t SPI_Motor_transfer(uint8_t command); // todo remove
    void SPI_ShiftReg_transfer(void); // todo remove
    void SPI_worker(void);
    void SPI_boot_low_voltage();
    void SPI_BatteryMeter(uint16_t val);

    typedef enum SPI_SHIFT_REG {
        //  NC            1<<0
        LED_ERR = 1 << 1,
        LED_USER = 1 << 2,
        BAT_FULL_GREEN = 1 << 3,
        BAT_MID_RED = 1 << 4,
        BAT_MID_GREEN = 1 << 5,
        BAT_LOW_RED = 1 << 6,
        BAT_LOW_GREEN = 1 << 7,
        BAT_ALL = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7),
        //  NC            1<<8
        MOTOR_A_ENABLE = 1 << 9,
        SS_A = 1 << 10,
        MOTOR_A_DIR = 1 << 11,
        MOTOR_B_DIR = 1 << 12,
        SS_B = 1 << 13,
        MOTOR_B_ENABLE = 1 << 14,
        //  NC            1<<15            
    } SPI_SHIFT_REG_BIT_POSITIONS;

    typedef enum {
        TLE7209_Diag_Enable = 1 << 7,
        TLE7209_Diag_Over_Temp = 1 << 6,
        TLE7209_Diag_Limit_Temp = 1 << 5,
        TLE7209_Diag_Limit_Current = 1 << 4, // not sure this is an error ?

        TLE7209_Diag_All_Good = 0x6f, // ignore current limit error?

        TLE7209_Diag_Mask = 0x8f,
// valid only when Disabled (bit 8 = 0)
        TLE7209_Open = 0x3,
// valid only when Enabled (bit 8 = 1)
        TLE7209_Short_12_P = 0x85,
        TLE7209_Short_2_P = 0x87,
        TLE7209_Short_12_N = 0x8a,
        TLE7209_Short_2_N = 0x8b,
        TLE7209_Short = 0x8c,
        TLE7209_Short_1_P = 0x8d,
        TLE7209_Short_1_N = 0x8e,
    } TLE7209_DIAG;


#define LED_ERR_SetHigh() do { spidata |= LED_ERR; } while(0)
#define LED_ERR_SetLow() do { spidata &= ~LED_ERR; } while(0)
#define LED_ERR_Toggle() do { spidata ^= LED_ERR; } while(0)
#define LED_ERR_GetValue() (spidata & LED_ERR)

#define LED_USER_SetHigh() do { spidata |= LED_USER; } while(0)
#define LED_USER_SetLow() do { spidata &= ~LED_USER; } while(0)
#define LED_USER_Toggle() do { spidata ^= LED_USER; } while(0)
#define LED_USER_GetValue() (spidata & LED_USER)

#define MOTORS_ENABLE() do { spidata |= MOTOR_A_ENABLE + MOTOR_B_ENABLE; } while(0)
#define MOTORS_DISABLE() do { spidata &= ~(MOTOR_A_ENABLE + MOTOR_B_ENABLE); } while(0)
#define MOTOR_A_FORWARD() do { spidata &= ~MOTOR_A_DIR; } while(0)
#define MOTOR_B_FORWARD() do { spidata &= ~MOTOR_B_DIR; } while(0)
#define MOTOR_A_BACKWARD() do { spidata |= MOTOR_A_DIR; } while(0)
#define MOTOR_B_BACKWARD() do { spidata |= MOTOR_B_DIR; } while(0)
    
#define MOTOR_A_SPI_ENABLE() do { spidata &= ~SS_B; spidata |= SS_A; } while(0)    
#define MOTOR_B_SPI_ENABLE() do { spidata &= ~SS_A; spidata |= SS_B; } while(0)
    
#define BAT_FULL() do { spidata &= ~BAT_ALL; spidata |= BAT_FULL_GREEN | BAT_MID_GREEN | BAT_LOW_GREEN; } while(0)
#define BAT_GOOD() do { spidata &= ~BAT_ALL; spidata |= BAT_MID_GREEN | BAT_LOW_GREEN; } while(0)
#define BAT_OK() do { spidata &= ~BAT_ALL; spidata |=  BAT_MID_GREEN | BAT_LOW_GREEN | BAT_MID_RED | BAT_LOW_RED; } while(0)
#define BAT_LOW() do { spidata &= ~BAT_ALL; spidata |=  BAT_LOW_GREEN | BAT_LOW_RED; } while(0)
#define BAT_CRITICAL() do { spidata &= ~BAT_ALL; spidata |=  BAT_LOW_RED; } while(0)

#define PROGRESS_0() do { spidata &= ~BAT_ALL;  } while(0)
#define PROGRESS_1() do { spidata &= ~BAT_ALL; spidata |= BAT_LOW_GREEN; } while(0)
#define PROGRESS_2() do { spidata &= ~BAT_ALL; spidata |= BAT_MID_GREEN; } while(0)
#define PROGRESS_3() do { spidata &= ~BAT_ALL; spidata |= BAT_FULL_GREEN; } while(0)


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

