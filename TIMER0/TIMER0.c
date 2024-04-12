#include "TIMER0.h"

static void Prescaler_Config (const TIM0_config_t *timer);
static void Timer_Mode_Config (const TIM0_config_t *timer);
static void Register_Size_Config (const TIM0_config_t *timer);

#if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*TIM0_InterruptHandler) (void) = NULL;
#endif

static uint16 preload_val = 0;    
/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM0_Init (const TIM0_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM0_DISABLE();
        /*Check the prescaler status and set the value to T0PS bits*/
        Prescaler_Config (timer);
        /*Set the timer mode*/
        Timer_Mode_Config(timer);
        /*Set the register size*/
        Register_Size_Config(timer);
        preload_val = timer->preload_value ; 
        TMR0H = (timer->preload_value) >> 8;
        TMR0L = (uint8) (timer->preload_value);
        /*Handle the interrupt if enabled*/
        #if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM0_InterruptHandler = timer->TIM0_InterruptHandler;
            TIM0_INTERRUPT_ENABLE();
            TIM0_INTERRUPT_CLEAR_FLAG();
            #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PRIORITY_FEATURE_ENABLE();
                if ( High_Priorty == timer->priority) {
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    TIM0_HIGH_PRIORITY();
                }
                else if ( Low_Priorty == timer->priority){
                    INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    TIM0_LOW_PRIORITY();
                }
                else{}
            #else
                INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                INTERRUPT_PERIPHERAL_ENABLE();
            #endif
        #endif
        /*Enable the module*/
        TIM0_ENABLE();    
    }
    return RET; 
}

/**
 * 
 * @param timer
 * @return 
 */
Std_ReturnType TIM0_DeInit (const TIM0_config_t *timer){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the module*/
        TIM0_DISABLE();
        /*Disable the interrupt if enabled*/
        #if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            TIM0_INTERRUPT_DISABLE();
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
Std_ReturnType TIM0_Write_Value (const TIM0_config_t *timer , uint16 value){
    Std_ReturnType RET = E_OK;
    if ( NULL == timer ){
        RET = E_NOT_OK;
    }
    else {
        TMR0H = (value) >> 8;
        TMR0L = (uint8)(value);
    }
    return RET; 
}

/**
 * 
 * @param timer
 * @param value
 * @return 
 */
Std_ReturnType TIM0_Read_Value (const TIM0_config_t *timer , uint16 *value){
    Std_ReturnType RET = E_OK;
    uint8 tim0_L = 0 , tim0_H = 0 ;
    if ( NULL == timer || NULL == value){
        RET = E_NOT_OK;
    }
    else {
        tim0_L = TMR0L ;
        tim0_H = TMR0H ;
        *value = (uint16)((tim0_H<<8) + tim0_L);
    }
    return RET; 
}

#if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
void TIM0_ISR (void){
    TIM0_INTERRUPT_CLEAR_FLAG();
    TMR0H = (preload_val) >> 8;
    TMR0L = (uint8) (preload_val);
    if ( TIM0_InterruptHandler){
        TIM0_InterruptHandler(); 
    }
}
#endif

static void Prescaler_Config (const TIM0_config_t *timer){
    if (timer->prescaler_mode == TIM0_ENABLE_PRESCALER){
        TIM0_PRESCALER_ENABLE();
        T0CONbits.T0PS = timer->prescaler_value ;
    }
    else if (timer->prescaler_mode == TIM0_DISABLE_PRESCALER){
        TIM0_PRESCALER_DISABLE();
    }
    else {}
}
static void Timer_Mode_Config (const TIM0_config_t *timer){
    if (timer->timer0_mode == TIM0_ENABLE_TIMER_MODE){
        TIM0_TIMER_MODE();
    }
    else if (timer->timer0_mode == TIM0_ENABLE_COUNTER_MODE){
        TIM0_COUNTER_MODE();
        if ( timer->edge_mode == TIM0_ENABLE_RISING_EDGE){
            TIM0_RISING_EDGE();
        }
        else if ( timer->edge_mode == TIM0_ENABLE_FALLING_EDGE){
            TIM0_FALLING_EDGE();
        }
        else{}
    }
    else {}
}
static void Register_Size_Config (const TIM0_config_t *timer){
    if (timer->register_size_mode == TIM0_ENABLE_8BITS_MODE){
        TIM0_CONFIGURE_8BITS();
    }
    else if (timer->register_size_mode == TIM0_ENABLE_16BITS_MODE){
        TIM0_CONFIGURE_16BITS();
    }
    else {}
}
