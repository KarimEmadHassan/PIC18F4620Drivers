/* 
 * File:   newfile.h
 * Author: Karim Emad
 *
 * Created on October 5, 2023, 11:42 AM
 */

#ifndef TIMER1_H
#define	TIMER1_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define TIM1_ENABLE_COUNTER_MODE       0x01
#define TIM1_ENABLE_TIMER_MODE         0x00

#define TIM1_ENABLE_ASYNCHRONOUS_MODE       0x01
#define TIM1_ENABLE_SYNCHRONOUS_MODE        0x00

#define TIM1_ENABLE_OSCILLATOR         0x01
#define TIM1_DISABLE_OSCILLATOR        0x00

#define TIM1_PRESCALER_DIV_1    0
#define TIM1_PRESCALER_DIV_2    1
#define TIM1_PRESCALER_DIV_4    2
#define TIM1_PRESCALER_DIV_8    3

#define TIM1_ENABLE_16BITS_MODE       0x01
#define TIM1_ENABLE_8BITS_MODE        0x00

/*Function Like Macro Section*/
#define TIM1_ENABLE()               (T1CONbits.TMR1ON = 1)
#define TIM1_DISABLE()              (T1CONbits.TMR1ON = 0)

#define TIM1_COUNTER_MODE()         (T1CONbits.TMR1CS = 1)
#define TIM1_TIMER_MODE()           (T1CONbits.TMR1CS = 0)

#define TIM1_ASYNCHRONOUS_MODE()    (T1CONbits.T1SYNC = 1)
#define TIM1_SYNCHRONOUS_MODE()     (T1CONbits.T1SYNC = 0)


#define TIM1_OSCILLATOR_ENABLE()    (T1CONbits.T1OSCEN = 1)
#define TIM1_OSCILLATOR_DISABLE()   (T1CONbits.T1OSCEN = 0)

#define TIMER1_PRESCALER_SELECT(_PRESCALER_) (T1CONbits.T1CKPS = _PRESCALER_)

#define TIM1_CLK_STATUS()           (T1CONbits.T1RUN)

#define TIM1_CONFIGURE_16BITS()     (T1CONbits.RD16 = 1)
#define TIM1_CONFIGURE_8BITS()      (T1CONbits.RD16 = 0)

/*Data Type Declaration Section*/
typedef struct {
    #if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*TIM1_InterruptHandler) (void);
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority ;
    #endif
    #endif
    uint16 preload_value;
    uint8 prescaler_value :2;
    uint8 timer1_mode     :1;
    uint8 counter_mode    :1;
    uint8 oscillator_mode :1;
    uint8 register_mode   :1;
}TIM1_config_t;

/*Function Declaration Section*/
Std_ReturnType TIM1_Init         (const TIM1_config_t *timer);
Std_ReturnType TIM1_DeInit       (const TIM1_config_t *timer);
Std_ReturnType TIM1_Write_Value  (const TIM1_config_t *timer , uint16 value);
Std_ReturnType TIM1_Read_Value   (const TIM1_config_t *timer , uint16 *value);

#endif	/* TIMER1_H */

