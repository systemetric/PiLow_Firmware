/* 
 * File:   main.h
 * Author: will
 *
 * Created on August 4, 2022, 9:27 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        SSM_Reset,
        SSM_LowPowerSleep,
                SSM_BootPI,
                SSM_FullPower,
                SSM_ShutDownPI,
                SSM_FullShutdown

    } SystemStateMachine;

#define PI_BOOT_TIME 600*4 /* Maximum time we allow the PI to boot in 0.25s increments */
#define PI_SD_TIME 600*4

#ifdef	__cplusplus
}
#endif 

#endif	/* MAIN_H */

