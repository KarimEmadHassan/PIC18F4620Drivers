#include "TIMER2.h"

#if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*TIM2_InterruptHandler) (void) = NULL ;
#endif

static uint8 timer_preload_Val = 0 ;
/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM2_Init (const TIM2_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM2_DISABLE();
        /*Set the prescaler value*/
        (T2CONbits.T2CKPS) = timer->prescaler_value;
        /*Set the postscaler value*/
        (T2CONbits.TOUTPS) = timer->postscaler_value;
        /*Set the preload value*/
        TMR2 = timer->preload_value ;
        timer_preload_Val = timer->preload_value ;
        /*Handle the interrupt if enabled*/
        #if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM2_InterruptHandler = timer->TIM2_InterruptHandler;
            TIM2_INTERRUPT_ENABLE();
            TIM2_INTERRUPT_CLEAR_FLAG();
            #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PRIORITY_FEATURE_ENABLE();
                if ( High_Priorty == timer->priority) {
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    TIM2_HIGH_PRIORITY();
                }
                else if ( Low_Priorty == timer->priority){
                    INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    TIM2_LOW_PRIORITY();
                }
                else{}
            #else
                INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                INTERRUPT_PERIPHERAL_ENABLE();
            #endif
        #endif
        /*Enable the module*/
        TIM2_ENABLE();
    }
    return RET;
}

/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM2_DeInit (const TIM2_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM2_DISABLE();
        /*Disable the interrupt if enabled*/
        #if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM2_INTERRUPT_DISABLE();
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
Std_ReturnType TIM2_Write_Value  (const TIM2_config_t *timer , uint8 value){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        TMR2 = value;
    }
    return RET;
}

/**
 * 
 * @param timer
 * @param value
 * @return 
 */
Std_ReturnType TIM2_Read_Value   (const TIM2_config_t *timer , uint8 *value){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        *value = TMR2;
    }
    return RET;
}


void TIM2_ISR (void){
    TIM2_INTERRUPT_CLEAR_FLAG();
    TMR2 = timer_preload_Val;
    if (TIM2_InterruptHandler){
        TIM2_InterruptHandler();
    }
}
