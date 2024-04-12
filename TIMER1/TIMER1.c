#include "TIMER1.h"

#if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*TIM1_InterruptHandler) (void) = NULL;
#endif

static uint16 timer1_preload_val = 0;    

/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM1_Init (const TIM1_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM1_DISABLE();
        /*set the prescaler value to T1CKPS bits*/
        TIMER1_PRESCALER_SELECT(timer->prescaler_value);
        /*Set the timer mode*/
        if ( timer->timer1_mode == TIM1_ENABLE_TIMER_MODE){
            TIM1_TIMER_MODE();
        }
        else if ( timer->timer1_mode == TIM1_ENABLE_COUNTER_MODE){
            TIM1_COUNTER_MODE();
            if ( timer->counter_mode == TIM1_ENABLE_ASYNCHRONOUS_MODE){
                TIM1_ASYNCHRONOUS_MODE();
            }
            else if ( timer->counter_mode == TIM1_ENABLE_SYNCHRONOUS_MODE){
                TIM1_SYNCHRONOUS_MODE();
            }
            else{}
        }
        else {}
        TMR1H = ((timer->preload_value) >> 8);
        TMR1L = (uint8)(timer->preload_value);
        timer1_preload_val = timer->preload_value; 
        /*Set the register size*/
        if ( timer->register_mode == TIM1_ENABLE_16BITS_MODE){
            TIM1_CONFIGURE_16BITS();
        }
        else if ( timer->register_mode == TIM1_ENABLE_8BITS_MODE){
            TIM1_CONFIGURE_8BITS();
        }
        else {}
        
        /*Handle the interrupt if enabled*/
        #if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM1_InterruptHandler = timer->TIM1_InterruptHandler;
            TIM1_INTERRUPT_ENABLE();
            TIM1_INTERRUPT_CLEAR_FLAG();
            #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PRIORITY_FEATURE_ENABLE();
                if ( High_Priorty == timer->priority) {
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    TIM1_HIGH_PRIORITY();
                }
                else if ( Low_Priorty == timer->priority){
                    INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    TIM1_LOW_PRIORITY();
                }
                else{}
            #else
                INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                INTERRUPT_PERIPHERAL_ENABLE();
            #endif
        #endif
        /*Enable the module*/
        TIM1_ENABLE();  
    }
    return RET; 
}

/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM1_DeInit (const TIM1_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM1_DISABLE();
        /*Disable the interrupt if enabled*/
        #if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM1_INTERRUPT_DISABLE();
        #endif
    }
    return RET; 
}

/**
 * 
 * @param timer
 * @param value
 * @return 
 */
Std_ReturnType TIM1_Write_Value (const TIM1_config_t *timer , uint16 value){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        TMR1H = (value) >> 8;
        TMR1L = (uint8)(value);
    }
    return RET; 
}

/**
 * 
 * @param timer
 * @param value
 * @return 
 */
Std_ReturnType TIM1_Read_Value (const TIM1_config_t *timer , uint16 *value){
    Std_ReturnType RET = E_OK;
    uint8 tim1_L = 0 , tim1_H = 0 ;
    if ( NULL == timer || NULL == value ){
        RET = E_NOT_OK;
    }
    else {
        tim1_L = TMR1L;
        tim1_H = TMR1H;
        *value = (uint16)((tim1_H <<8) + tim1_L);
    }
    return RET;
}

void TIM1_ISR (void){
    TIM1_INTERRUPT_CLEAR_FLAG();
    TMR1H = timer1_preload_val >> 8;
    TMR1L = (uint8)timer1_preload_val;
    if (TIM1_InterruptHandler){
        TIM1_InterruptHandler();
    }
}