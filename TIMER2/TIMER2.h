/* 
 * File:   TIMER2.h
 * Author: Karim Emad
 *
 * Created on October 6, 2023, 12:34 AM
 */

#ifndef TIMER2_H
#define	TIMER2_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define    TIM2_POSTSCALER_1    0
#define    TIM2_POSTSCALER_2    1   
#define    TIM2_POSTSCALER_3    2   
#define    TIM2_POSTSCALER_4    3
#define    TIM2_POSTSCALER_5    4
#define    TIM2_POSTSCALER_6    5
#define    TIM2_POSTSCALER_7    6
#define    TIM2_POSTSCALER_8    7
#define    TIM2_POSTSCALER_9    8
#define    TIM2_POSTSCALER_10   9
#define    TIM2_POSTSCALER_11   10
#define    TIM2_POSTSCALER_12   11
#define    TIM2_POSTSCALER_13   12
#define    TIM2_POSTSCALER_14   13
#define    TIM2_POSTSCALER_15   14
#define    TIM2_POSTSCALER_16   15

#define    TIM2_PRESCALER_1     0
#define    TIM2_PRESCALER_4     1
#define    TIM2_PRESCALER_16    2   

/*Function Like Macro Section*/
#define TIM2_ENABLE()    (T2CONbits.TMR2ON = 1)
#define TIM2_DISABLE()   (T2CONbits.TMR2ON = 0)

/*Data Type Declaration Section*/
typedef struct {
    #if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*TIM2_InterruptHandler) (void);
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority ;
    #endif
    #endif
    uint8 preload_value;
    uint8 postscaler_value ;
    uint8 prescaler_value ;
}TIM2_config_t;

/*Function Declaration Section*/
Std_ReturnType TIM2_Init         (const TIM2_config_t *timer);
Std_ReturnType TIM2_DeInit       (const TIM2_config_t *timer);
Std_ReturnType TIM2_Write_Value  (const TIM2_config_t *timer , uint8 value);
/**
 * 
 * @param timer : object of TIM2_config_t
 * @param value
 * @return 
 */
Std_ReturnType TIM2_Read_Value   (const TIM2_config_t *timer , uint8 *value);

#endif	/* TIMER2_H */

