#include "CCP1.h"

#if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*CCP1_InterruptHandler) (void) = NULL;
    static void (*CCP2_InterruptHandler) (void) = NULL;
#endif

static void CCP_Capture_Mode_Set ( const CCP_Config_t *ccp_obj );
static void CCP_Compare_Mode_Set ( const CCP_Config_t *ccp_obj );
static void CCP_PWM_Mode_Set ( const CCP_Config_t *ccp_obj );
static void CCP_Capture_Mode_Timer_Set ( const CCP_Config_t *ccp_obj );
    

Std_ReturnType CCP_Init (const CCP_Config_t *ccp_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == ccp_obj ){
        RET = E_NOT_OK;
    }
    else {
        /**Disable the Moduele**/
        if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
            CCP1_SET_MODE(CCP_DISABLED); 
        }
        else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
            CCP2_SET_MODE(CCP_DISABLED); 
        }
        else{ }
        /**Configure the CCP Mode**/
        switch ( ccp_obj->ccp_mode ){
        /******Capture Mode*******/
            #if ((CCP1_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE))
            case (CCP_Capture_Mode): 
                CCP_Capture_Mode_Set(ccp_obj); 
                CCP_Capture_Mode_Timer_Set(ccp_obj);
                break;
            #endif
        /******Compare Mode*******/
            #if ((CCP1_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE))
            case (CCP_Compare_Mode):
                CCP_Compare_Mode_Set(ccp_obj);
                break;
            #endif
        /******PWM Mode*******/
            #if ((CCP1_SELECTED_MODE == CCP_CONFIG_PWM_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_PWM_MODE) )
            case (CCP_PWM_Mode):   
                CCP_PWM_Mode_Set(ccp_obj);
                break;
            #endif
            default: RET = E_NOT_OK;
        }
        /**Configure the CCP Pin**/
        RET = GPIO_PIN_Init( &(ccp_obj->ccp_pin) );
        /**Configure the CCP Interrupt**/
        #if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
                CCP1_INTERRUPT_ENABLE();
                CCP1_INTERRUPT_CLEAR_FLAG();
                CCP1_InterruptHandler = ccp_obj->CCP1_InterruptHandler;
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
                CCP2_INTERRUPT_ENABLE();
                CCP2_INTERRUPT_CLEAR_FLAG();
                CCP2_InterruptHandler = ccp_obj->CCP2_InterruptHandler;
            }
            else{ }
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITY_FEATURE_ENABLE();
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
                if ( ccp_obj->ccp1_priority == High_Priorty){
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    CCP1_HIGH_PRIORITY();
                }
                else if (ccp_obj->ccp1_priority == Low_Priorty){
                    INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    CCP1_LOW_PRIORITY();
                }
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
                else if (ccp_obj->ccp2_priority == High_Priorty){
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    CCP2_HIGH_PRIORITY();
                }
                else if (ccp_obj->ccp2_priority == Low_Priorty){
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    CCP2_LOW_PRIORITY();
                }
            }
            else{ }
        #else    
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
        #endif    
        #endif
    }
    return RET;
}

Std_ReturnType CCP_DeInit (const CCP_Config_t *ccp_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == ccp_obj ){
        RET = E_NOT_OK;
    }
    else {
        if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
            CCP1_SET_MODE(CCP_DISABLED);
            #if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP1_INTERRUPT_DISABLE();
            #endif
        }
        else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
            CCP2_SET_MODE(CCP_DISABLED); 
            #if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP2_INTERRUPT_DISABLE(); 
            #endif
        }
        else{ }
    }
    return RET;
}

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE) )
    Std_ReturnType CCP_Check_Capture_Ready (const CCP_Config_t *ccp_obj , uint8 *capture_state){
        Std_ReturnType RET = E_OK;
        if ( NULL == capture_state ){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL ){
                if ( CCP_CAPTURE_READY == PIE1bits.CCP1IE){
                    *capture_state = CCP_CAPTURE_READY;
                    CCP1_INTERRUPT_CLEAR_FLAG();
                }
                else {
                    *capture_state = CCP_CAPTURE_NOT_READY;
                }
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL ){
                if ( CCP_CAPTURE_READY == PIE2bits.CCP2IE){
                    *capture_state = CCP_CAPTURE_READY;
                    CCP2_INTERRUPT_CLEAR_FLAG();
                }
                else {
                    *capture_state = CCP_CAPTURE_NOT_READY;
                }
            }
            else {  }
        }
        return RET;
    }
    
    Std_ReturnType CCP_Get_Capture_Value (const CCP_Config_t *ccp_obj , uint16 *capture_value){
        Std_ReturnType RET = E_OK;
        CCP_Register capture_temp_value = {.ccpr_low = 0 , .ccpr_high = 0};
        if ( NULL == capture_value ){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL ){
                capture_temp_value.ccpr_low = CCPR1L;
                capture_temp_value.ccpr_high = CCPR1H; 
                *capture_value = capture_temp_value.ccpr_16bits;
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL ){
                capture_temp_value.ccpr_low = CCPR2L;
                capture_temp_value.ccpr_high = CCPR2H; 
                *capture_value = capture_temp_value.ccpr_16bits;
            }
            else {  }
        }
        return RET;
        }
#endif

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE) )
    Std_ReturnType CCP_Check_Compare_Ready (const CCP_Config_t *ccp_obj , uint8 *compare_state){
        Std_ReturnType RET = E_OK;
        if ( NULL == compare_state || NULL == ccp_obj){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL ){
                if ( CCP_COMPARE_READY == PIE1bits.CCP1IE){
                    *compare_state = CCP_COMPARE_READY;
                    CCP1_INTERRUPT_CLEAR_FLAG();
                }
                else {
                    *compare_state = CCP_COMPARE_NOT_READY;
                }
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL ){
                if ( CCP_COMPARE_READY == PIE2bits.CCP2IE){
                    *compare_state = CCP_COMPARE_READY;
                    CCP2_INTERRUPT_CLEAR_FLAG();
                }
                else {
                    *compare_state = CCP_COMPARE_NOT_READY;
                }
            }
            else {  }
        }
        return RET;
    }
    
    Std_ReturnType CCP_Set_Compare_Value (const CCP_Config_t *ccp_obj , uint16 compare_value){
        Std_ReturnType RET = E_OK;
        CCP_Register compare_temp_value = {.ccpr_low = 0 , .ccpr_high = 0};
        if ( ccp_obj == NULL ){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL ){
                compare_temp_value.ccpr_16bits = compare_value;
                CCPR1L = compare_temp_value.ccpr_low;
                CCPR1H = compare_temp_value.ccpr_high;
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL ){
                compare_temp_value.ccpr_16bits = compare_value;
                CCPR2L = compare_temp_value.ccpr_low;
                CCPR2H = compare_temp_value.ccpr_high;
            }
            else {  }
            
        }
        return RET;
    }

#endif

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_PWM_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_PWM_MODE))
    Std_ReturnType CCP_PWM_Start (const CCP_Config_t *ccp_obj){
        Std_ReturnType RET = E_NOT_OK;
        if ( NULL == ccp_obj){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
                CCP1_SET_MODE(CCP_PWM_MODE);
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
                CCP2_SET_MODE(CCP_PWM_MODE);
        }
        else{ }
        RET = E_OK;
        }
        return RET;
    }
    
    Std_ReturnType CCP_PWM_Stop (const CCP_Config_t *ccp_obj){
        Std_ReturnType RET = E_NOT_OK;
        if ( NULL == ccp_obj ){
            RET = E_NOT_OK;
        }
        else {
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
                CCP1_SET_MODE(CCP_DISABLED);
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
                CCP2_SET_MODE(CCP_DISABLED);
            }
            else{ }
            RET = E_OK;
        }
        return RET;
    }
    
    Std_ReturnType CCP_PWM_Set_Duty (const CCP_Config_t *ccp_obj , uint8 pwm_duty){
        Std_ReturnType RET = E_NOT_OK;
        if ( NULL == ccp_obj ){
            RET = E_NOT_OK;
        }
        else {
            uint16 duty_temp = (uint16)( 4.0 * ( PR2 + 1 ) * ( pwm_duty/100.0));
            if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
                CCP1CONbits.DC1B = (uint8)(duty_temp & 0x0003);
                CCPR1L = (uint8) (duty_temp >> 2);
            }
            else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
                CCP2CONbits.DC2B = (uint8)(duty_temp & 0x0003);
                CCPR2L = (uint8) (duty_temp >> 2);
            }
            else{ }
            RET = E_OK;
        }
        return RET;
    }
#endif

    
static void CCP_Capture_Mode_Set ( const CCP_Config_t *ccp_obj ){
    if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
        switch(ccp_obj->ccp_mode_varient){
            case (CCP_CAPTURE_MODE_RISING_EDGE): 
                CCP1_SET_MODE(CCP_CAPTURE_MODE_RISING_EDGE); 
                break;
            case (CCP_CAPTURE_MODE_4TH_RISING_EDGE): 
                CCP1_SET_MODE(CCP_CAPTURE_MODE_4TH_RISING_EDGE); 
                break;  
            case (CCP_CAPTURE_MODE_16TH_RISING_EDGE): 
                CCP1_SET_MODE(CCP_CAPTURE_MODE_16TH_RISING_EDGE); 
                break; 
            case (CCP_CAPTURE_MODE_FALLING_EDGE): 
                CCP1_SET_MODE(CCP_CAPTURE_MODE_FALLING_EDGE); 
                break; 
        }
    }
    
    else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
         switch(ccp_obj->ccp_mode_varient){
            case (CCP_CAPTURE_MODE_RISING_EDGE): 
                CCP2_SET_MODE(CCP_CAPTURE_MODE_RISING_EDGE); 
                break;
            case (CCP_CAPTURE_MODE_4TH_RISING_EDGE): 
                CCP2_SET_MODE(CCP_CAPTURE_MODE_4TH_RISING_EDGE); 
                break;  
            case (CCP_CAPTURE_MODE_16TH_RISING_EDGE): 
                CCP2_SET_MODE(CCP_CAPTURE_MODE_16TH_RISING_EDGE); 
                break; 
            case (CCP_CAPTURE_MODE_FALLING_EDGE): 
                CCP2_SET_MODE(CCP_CAPTURE_MODE_FALLING_EDGE); 
                break; 
        }
    }
    
    else {}
}
    
static void CCP_Compare_Mode_Set ( const CCP_Config_t *ccp_obj ){
    if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
        switch(ccp_obj->ccp_mode_varient){
            case (CCP_COMPARE_MODE_TOGGLE_ON_MATCH): 
                CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); 
                break;
            case (CCP_COMPARE_MODE_SET_PIN_LOW): 
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); 
                break;  
            case (CCP_COMPARE_MODE_SET_PIN_HIGH): 
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); 
                break; 
            case (CCP_COMPARE_MODE_GENERATE_SW_INTERRUPT): 
                CCP1_SET_MODE(CCP_COMPARE_MODE_GENERATE_SW_INTERRUPT); 
                break;
            case (CCP_COMPARE_MODE_GENERATE_SPECIAL_EVENT): 
                CCP1_SET_MODE(CCP_COMPARE_MODE_GENERATE_SPECIAL_EVENT); 
            break;
        }
    }
    if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
        switch(ccp_obj->ccp_mode_varient){
            case (CCP_COMPARE_MODE_TOGGLE_ON_MATCH): 
                CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); 
                break;
            case (CCP_COMPARE_MODE_SET_PIN_LOW): 
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); 
                break;  
            case (CCP_COMPARE_MODE_SET_PIN_HIGH): 
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); 
                break; 
            case (CCP_COMPARE_MODE_GENERATE_SW_INTERRUPT): 
                CCP2_SET_MODE(CCP_COMPARE_MODE_GENERATE_SW_INTERRUPT); 
                break;
            case (CCP_COMPARE_MODE_GENERATE_SPECIAL_EVENT): 
                CCP2_SET_MODE(CCP_COMPARE_MODE_GENERATE_SPECIAL_EVENT); 
            break;
        }
    }
    
}

static void CCP_PWM_Mode_Set ( const CCP_Config_t *ccp_obj ){
    if ( ccp_obj->ccp_channel == CCP1_CHANNEL){
        if ( CCP_PWM_MODE == ccp_obj->ccp_mode_varient ){
            CCP1_SET_MODE(CCP_PWM_MODE); 
        }
        else { } 
    }
    else if ( ccp_obj->ccp_channel == CCP2_CHANNEL){
        if ( CCP_PWM_MODE == ccp_obj->ccp_mode_varient ){
            CCP2_SET_MODE(CCP_PWM_MODE); 
        }
        else { } 
    }
    else{ }
    PR2 = (uint8)((_XTAL_FREQ / (ccp_obj->pwm_frequency * 4.0 * ccp_obj->postscaler_value * ccp_obj->prescaler_value)) - 1);
}

static void CCP_Capture_Mode_Timer_Set ( const CCP_Config_t *ccp_obj ){
    if ( ccp_obj->ccp_capture_timer == CCP1_CCP2_TIM3){
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if ( ccp_obj->ccp_capture_timer == CCP1_TIM1_CCP2_TIM3){
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if ( ccp_obj->ccp_capture_timer == CCP1_CCP2_TIM1){
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{}
}
void CCP1_ISR (void){
    CCP1_INTERRUPT_CLEAR_FLAG();
    if (CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
}

void CCP2_ISR (void){
    CCP2_INTERRUPT_CLEAR_FLAG();
    if (CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
}
