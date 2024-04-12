/* 
 * File:   TIMER0.h
 * Author: Karim Emad
 *
 * Created on October 4, 2023, 4:56 PM
 */

#ifndef TIMER0_H
#define	TIMER0_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define TIM0_ENABLE_PRESCALER    0x01
#define TIM0_DISABLE_PRESCALER   0x00

#define TIM0_ENABLE_RISING_EDGE      0x01
#define TIM0_ENABLE_FALLING_EDGE     0x00

#define TIM0_ENABLE_TIMER_MODE       0x01
#define TIM0_ENABLE_COUNTER_MODE     0x00

#define TIM0_ENABLE_8BITS_MODE       0x01
#define TIM0_ENABLE_16BITS_MODE      0x00

/*Function Like Macro Section*/
#define TIM0_ENABLE()    (T0CONbits.TMR0ON = 1)
#define TIM0_DISABLE()   (T0CONbits.TMR0ON = 0)

#define TIM0_CONFIGURE_8BITS()     (T0CONbits.T08BIT = 1)
#define TIM0_CONFIGURE_16BITS()    (T0CONbits.T08BIT = 0)

#define TIM0_COUNTER_MODE()         (T0CONbits.T0CS = 1)
#define TIM0_TIMER_MODE()           (T0CONbits.T0CS = 0)

#define TIM0_FALLING_EDGE()     (T0CONbits.T0SE = 1)
#define TIM0_RISING_EDGE()      (T0CONbits.T0SE = 0)

#define TIM0_PRESCALER_DISABLE()     (T0CONbits.PSA = 1)
#define TIM0_PRESCALER_ENABLE()      (T0CONbits.PSA = 0)

/*Data Type Declaration Section*/
typedef enum {
    TIM0_PRESCALER_DIV_2 = 0,
    TIM0_PRESCALER_DIV_4,
    TIM0_PRESCALER_DIV_8,
    TIM0_PRESCALER_DIV_16,
    TIM0_PRESCALER_DIV_32,
    TIM0_PRESCALER_DIV_64,
    TIM0_PRESCALER_DIV_128,
    TIM0_PRESCALER_DIV_256        
}TIM0_prescaler_t;

typedef struct {
    #if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*TIM0_InterruptHandler) (void);
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority ;
    #endif
    #endif
    TIM0_prescaler_t prescaler_value;
    uint16 preload_value;
    uint8 prescaler_mode     :1;
    uint8 edge_mode          :1;
    uint8 timer0_mode        :1;
    uint8 register_size_mode :1;
}TIM0_config_t;

/*Function Declaration Section*/
Std_ReturnType TIM0_Init         (const TIM0_config_t *timer);
Std_ReturnType TIM0_DeInit       (const TIM0_config_t *timer);
Std_ReturnType TIM0_Write_Value  (const TIM0_config_t *timer , uint16 value);
Std_ReturnType TIM0_Read_Value   (const TIM0_config_t *timer , uint16 *value);


#endif	/* TIMER0_H */

