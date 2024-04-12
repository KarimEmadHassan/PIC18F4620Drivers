/* 
 * File:   INT_External.h
 * Author: Karim Emad
 *
 * Created on September 21, 2023, 6:50 PM
 */

#ifndef INT_EXTERNAL_H
#define	INT_EXTERNAL_H

/*Includes Section*/
#include "INT_Config.h"
#include "../GPIO/HAL_GPIO.h"

/*Macros Section*/
#if EXTERNAL_INTERRUPT_INTx_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_INT0_ENABLE()             (INTCONbits.INT0IE = 1)
#define INTERRUPT_INT0_DISABLE()            (INTCONbits.INT0IE = 0)
#define INTERRUPT_INT0_CLEAR_FLAG()         (INTCONbits.INT0IF = 0)
#define INTERRUPT_INT0_RISING_EDGE()        (INTCON2bits.INTEDG0 = 1)
#define INTERRUPT_INT0_FALLING_EDGE()       (INTCON2bits.INTEDG0 = 0)

#define INTERRUPT_INT1_ENABLE()             (INTCON3bits.INT1IE = 1)
#define INTERRUPT_INT1_DISABLE()            (INTCON3bits.INT1IE = 0)
#define INTERRUPT_INT1_CLEAR_FLAG()         (INTCON3bits.INT1IF = 0)
#define INTERRUPT_INT1_RISING_EDGE()        (INTCON2bits.INTEDG1 = 1)
#define INTERRUPT_INT1_FALLING_EDGE()       (INTCON2bits.INTEDG1 = 0)

#define INTERRUPT_INT2_ENABLE()             (INTCON3bits.INT2IE = 1)
#define INTERRUPT_INT2_DISABLE()            (INTCON3bits.INT2IE = 0)
#define INTERRUPT_INT2_CLEAR_FLAG()         (INTCON3bits.INT2IF = 0)
#define INTERRUPT_INT2_RISING_EDGE()        (INTCON2bits.INTEDG2 = 1)
#define INTERRUPT_INT2_FALLING_EDGE()       (INTCON2bits.INTEDG2 = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_INT1_HIGH_PRIORITY()      (INTCON3bits.INT1IP = 1)
#define INTERRUPT_INT1_LOW_PRIORITY()       (INTCON3bits.INT1IP = 0)

#define INTERRUPT_INT2_HIGH_PRIORITY()      (INTCON3bits.INT2IP = 1)
#define INTERRUPT_INT2_LOW_PRIORITY()       (INTCON3bits.INT2IP = 0)
#endif

#endif

#if EXTERNAL_INTERRUPT_OnCange_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_RBx_ENABLE()             (INTCONbits.RBIE = 1)
#define INTERRUPT_RBx_DISABLE()            (INTCONbits.RBIE = 0)
#define INTERRUPT_RBx_CLEAR_FLAG()         (INTCONbits.RBIF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_RBx_HIGH_PRIORITY()      (INTCON2bits.RBIP = 1)
#define INTERRUPT_RBx_LOW_PRIORITY()       (INTCON2bits.RBIP = 0)
#endif

#endif

/*Function Like Macro Section*/

/*Data Type Declaration Section*/
typedef enum {
    Interrupt_Falling_Edge=0,
    Interrupt_raising_Edge
}interrupt_intx_edge;
typedef enum {
    Interrupt_INT0=0,
    Interrupt_INT1,
    Interrupt_INT2        
}interrupt_intx_source;

typedef struct {
    void (*External_Interrupt_Handler) (void);
    interrupt_intx_edge edge;
    interrupt_intx_source source;
    interrupt_priority priority;
    pin_config_t pin;
}interrupt_intx_t;

typedef struct {
    void (*External_Interrupt_Handler_High) (void);
    void (*External_Interrupt_Handler_Low) (void);
    interrupt_priority priority;
    pin_config_t pin;
}interrupt_rbx_t;

/*Function Declaration Section*/
Std_ReturnType INTx_Init   ( const interrupt_intx_t *intx);
Std_ReturnType INTx_DeInit ( const interrupt_intx_t *intx);
Std_ReturnType RBx_Init    ( const interrupt_rbx_t  *intx);
Std_ReturnType RBx_DeInit  ( const interrupt_rbx_t  *intx);

#endif	/* INT_EXTERNAL_H */

