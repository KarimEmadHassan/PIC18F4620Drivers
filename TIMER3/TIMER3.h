/* 
 * File:   TIMER3.h
 * Author: Karim Emad
 *
 * Created on October 6, 2023, 5:35 PM
 */

#ifndef TIMER3_H
#define	TIMER3_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define TIM3_COUNTER    0x01
#define TIM3_TIMER      0x00

#define TIM3_COUNTER_ASYNC  0x01
#define TIM3_COUNTER_SYNC   0x00

#define TIM3_PRESCALER_1    0
#define TIM3_PRESCALER_2    1
#define TIM3_PRESCALER_4    2
#define TIM3_PRESCALER_8    3

#define TIM3_16BITS     0x01
#define TIM3_8BITS      0x00

/*Function Like Macro Section*/
#define TIM3_ENABLE()    (T3CONbits.TMR3ON = 1)
#define TIM3_DISABLE()   (T3CONbits.TMR3ON = 0)

#define TIM3_COUNTER_MODE()     (T3CONbits.TMR3CS = 1)
#define TIM3_TIMER_MODE()       (T3CONbits.TMR3CS = 0)

#define TIM3_ASYNCHRONOUS_MODE()      (T3CONbits.T3SYNC = 1)
#define TIM3_SYNCHRONOUS_MODE()       (T3CONbits.TMR3CS = 0)

#define TIM3_16BITS_READ_WRITE()      (T3CONbits.RD16 = 1)
#define TIM3_8BITS_READ_WRITE()       (T3CONbits.RD16 = 0)

#define TIM3_SET_PRESCALER(_prescaler)  (T3CONbits.T3CKPS = _prescaler)

/*Data Type Declaration Section*/
typedef struct {
    #if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*TIM3_InterruptHandler) (void);
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority ;
    #endif
    #endif
    uint16 preload_value;
    uint8 prescaler_value :2;
    uint8 module_mode  :1;
    uint8 counter_mode :1;
    uint8 register_size :1;
}TIM3_config_t;

/*Function Declaration Section*/
Std_ReturnType TIM3_Init         (const TIM3_config_t *timer);
Std_ReturnType TIM3_DeInit       (const TIM3_config_t *timer);
Std_ReturnType TIM3_Write_Value  (const TIM3_config_t *timer , uint16 value);
Std_ReturnType TIM3_Read_Value   (const TIM3_config_t *timer , uint16 *value);

#endif	/* TIMER3_H */
