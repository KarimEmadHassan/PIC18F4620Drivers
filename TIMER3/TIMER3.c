#include "TIMER3.h"


#if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*TIM3_InterruptHandler) (void) = NULL;
#endif

static uint16 timer3_preload_val = 0 ;    
/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM3_Init (const TIM3_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the Module*/
        TIM3_DISABLE();
        /*Set the module mode*/
        if ( timer->module_mode == TIM3_TIMER){
            TIM3_TIMER_MODE();
        }
        else if ( timer->module_mode == TIM3_COUNTER){
            TIM3_COUNTER_MODE();
            if ( timer->counter_mode == TIM3_COUNTER_ASYNC){
                TIM3_ASYNCHRONOUS_MODE();
            }
            else if ( timer->counter_mode == TIM3_COUNTER_SYNC){
                TIM3_SYNCHRONOUS_MODE();
            }
            else{}
        }
        else{}
        /*Set the prescaler value*/
        TIM3_SET_PRESCALER(timer->prescaler_value);
        timer3_preload_val = timer->preload_value;
        TMR3H = (timer->preload_value) >> 8;
        TMR3L = (uint8) timer->preload_value;
        /*Set the register size*/
        if ( timer->register_size == TIM3_16BITS){
            TIM3_16BITS_READ_WRITE();
        }
        else if ( timer->register_size == TIM3_8BITS){
            TIM3_8BITS_READ_WRITE();
        }
        else{}
        /*Handle the interrupt if enabled*/
        #if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM3_InterruptHandler = timer->TIM3_InterruptHandler;
            TIM3_INTERRUPT_ENABLE();
            TIM3_INTERRUPT_CLEAR_FLAG();
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITY_FEATURE_ENABLE();
            if ( timer->priority == High_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                TIM3_HIGH_PRIORITY();
            }
            else if (timer->priority == Low_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                TIM3_LOW_PRIORITY();
            }
            else{}
        #else    
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
        #endif    
        #endif       
        /*enable the Module*/
        TIM3_ENABLE();    
    }
    return RET;
}

/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM3_DeInit (const TIM3_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the Module*/
        TIM3_DISABLE();
        /*Disable the interrupt if enabled*/
        #if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM3_INTERRUPT_DISABLE();
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
Std_ReturnType TIM3_Write_Value (const TIM3_config_t *timer , uint16 value){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        TMR3H = value >> 8;
        TMR3L = (uint8)value;
    }
    return RET;
}

/**
 * 
 * @param timer
 * @param value
 * @return 
 */
Std_ReturnType TIM3_Read_Value (const TIM3_config_t *timer , uint16 *value){
    Std_ReturnType ret = E_OK;
    uint8 l_tmr3l = 0, l_tmr3h = 0;
    if((NULL == timer) || (NULL == value)){
        ret = E_NOT_OK;
    }
    else{
        l_tmr3l = TMR3L;
        l_tmr3h = TMR3H;
        *value = (uint16)((l_tmr3h << 8) + l_tmr3l);
    }
    return ret;
}

void TIM3_ISR (void){
    TIM3_INTERRUPT_CLEAR_FLAG();
    TMR3H = timer3_preload_val >> 8;
    TMR3L = (uint8) timer3_preload_val;
    if (TIM3_InterruptHandler){
        TIM3_InterruptHandler();
    }
}