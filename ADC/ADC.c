#include "ADC.h"

static void ADC_Channel_Config (const ADC_Channel_t channel);
static void ADC_Format_Config (const adc_config_t *adc);
static void ADC_Reference_Config (const adc_config_t *adc);

#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*ADC_InterruptHandler) (void) = NULL;
#endif
    
/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_Init (const adc_config_t *adc){
    Std_ReturnType RET = E_OK;
    if ( NULL == adc ){
        RET = E_NOT_OK;
    }
    else {
        /*disable the module*/
        ADC_DISABLE();
        /*Set Acq time*/
        (ADCON2bits.ACQT) = adc->acq_time;
        /*Set conversion time*/
        (ADCON2bits.ADCS) = adc->conversion_clk;
        /*configure the channel*/
        (ADCON0bits.CHS) = adc->cahnnel;
        ADC_Channel_Config ((adc->cahnnel));
        /*configure the interrupt*/
        #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        ADC_InterruptHandler = adc->ADC_InterruptHandler;   /*save the address of the ISR to the global pointer to function*/
        ADC_INTERRUPT_ENABLE();
        ADC_INTERRUPT_CLEAR_FLAG();
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
        if ( adc->priority == High_Priorty ){
            INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
            ADC_HIGH_PRIORITY();
        }
        else if ( adc->priority == Low_Priorty ){
            INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
            ADC_LOW_PRIORITY();
        }
        else {}
        #else
        INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
        INTERRUPT_PERIPHERAL_ENABLE();

        #endif
        #endif
        /*Set the format*/
        ADC_Format_Config(adc);
        /*configure the voltage ref.*/
        ADC_Reference_Config(adc);
        /*Enable the module*/
        ADC_ENABLE();
    }
    return RET;

}


/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_DeInit (const adc_config_t *adc){
    Std_ReturnType RET = E_OK;
    if ( NULL == adc ){
        RET = E_NOT_OK;
    }
    else {
        ADC_DISABLE();
        #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        ADC_INTERRUPT_DISABLE();
        #endif
    }
    return RET;
}

/**
 * 
 * @param adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_Select_Channel (const adc_config_t *adc , ADC_Channel_t channel){
    Std_ReturnType RET = E_OK;
    (ADCON0bits.CHS) = channel;
    ADC_Channel_Config (channel);
    return RET;
}

/**
 * 
 * @param adc
 * @return 
 */
Std_ReturnType ADC_Start_Conversion (const adc_config_t *adc){
    Std_ReturnType RET = E_OK;
    if ( NULL == adc ){
        RET = E_NOT_OK;
    }
    else {
        ADC_START_CONVERSION();
    }
    return RET;

}

/**
 * 
 * @param adc
 * @param state
 * @return 
 */
Std_ReturnType ADC_Conversion_Status (const adc_config_t *adc , uint8 *state){
    Std_ReturnType RET = E_OK;
    if ( NULL == adc || NULL == state){
        RET = E_NOT_OK;
    }
    else {
        *state = !(ADC_CONVERSION_STATUS());
    }
    return RET;
}

/**
 * 
 * @param adc
 * @param result
 * @return 
 */
Std_ReturnType ADC_Conversion_Result (const adc_config_t *adc , uint16 *result){
    Std_ReturnType RET = E_OK;
    if ( NULL == adc || NULL == result ){
        RET = E_NOT_OK;
    }
    else {
        if ( adc->result_format == ADC_RIGHT_JUSTIFICATION){
            *result = (uint16)((ADRESH << 8) + ADRESL);
        }
        else if ( adc->result_format == ADC_LEFT_JUSTIFICATION){
            *result = (uint16)(((ADRESH << 8) + ADRESL)>>6);
        }
        else {
            *result = (uint16)((ADRESH << 8) + ADRESL);
        }
    }
    return RET;
}

/**
 * 
 * @param adc
 * @param channel
 * @param result
 * @return 
 */
Std_ReturnType ADC_Get_Conversion_Blocking (const adc_config_t *adc , ADC_Channel_t channel , uint16 *result){
    Std_ReturnType RET = E_OK;
    uint8 conversion_status=0;
    if ( NULL == adc || NULL == result){
        RET = E_NOT_OK;
    }
    else {
        /*select channel*/
        RET = ADC_Select_Channel(adc , channel);
        /*start conversion*/
        RET = ADC_Start_Conversion (adc);
        /*check conversion state*/
        while (ADC_CONVERSION_STATUS());
        RET = ADC_Conversion_Result(adc , result);
    }
    return RET;
}

/**
 * 
 * @param adc
 * @param channel
 * @param result
 * @return 
 */
Std_ReturnType ADC_Start_Conversion_Interrupt (const adc_config_t *adc , ADC_Channel_t channel){
    Std_ReturnType RET = E_OK;
    uint8 conversion_status=0;
    if ( NULL == adc ){
        RET = E_NOT_OK;
    }
    else {
        /*select channel*/
        RET = ADC_Select_Channel(adc , channel);
        /*start conversion*/
        RET = ADC_Start_Conversion (adc);
        /*after conversion is done the flag will be set and the interrupt will execute the ISR*/
    }
    return RET;
}


/*----------------------------------------------------------------------------*/

/**
 * 
 * @param channel
 */
static void ADC_Channel_Config (const ADC_Channel_t channel){
    switch (channel){
        case (ADC_CHANNEL_0): SET_BIT  (TRISA , _TRISA_RA0_POSN); break;
        case (ADC_CHANNEL_1): SET_BIT  (TRISA , _TRISA_RA1_POSN); break;
        case (ADC_CHANNEL_2): SET_BIT  (TRISA , _TRISA_RA2_POSN); break;
        case (ADC_CHANNEL_3): SET_BIT  (TRISA , _TRISA_RA3_POSN); break;
        case (ADC_CHANNEL_4): SET_BIT  (TRISA , _TRISA_RA5_POSN); break;
        case (ADC_CHANNEL_5): SET_BIT  (TRISE , _TRISE_RE0_POSN); break;
        case (ADC_CHANNEL_6): SET_BIT  (TRISE , _TRISE_RE1_POSN); break;
        case (ADC_CHANNEL_7): SET_BIT  (TRISE , _TRISE_RE2_POSN); break;
        case (ADC_CHANNEL_8): SET_BIT  (TRISB , _TRISB_RB2_POSN); break;
        case (ADC_CHANNEL_9): SET_BIT  (TRISB , _TRISB_RB3_POSN); break;
        case (ADC_CHANNEL_10): SET_BIT (TRISB , _TRISB_RB1_POSN); break;
        case (ADC_CHANNEL_11): SET_BIT (TRISB , _TRISB_RB4_POSN); break;
        case (ADC_CHANNEL_12): SET_BIT (TRISB , _TRISB_RB0_POSN); break;
        default : ;
    }
}

/**
 * 
 * @param adc
 */
static void ADC_Format_Config (const adc_config_t *adc){
    if ( adc->result_format == ADC_RIGHT_JUSTIFICATION){
        ADC_RIGHT_JUSTIFIED();
    }
    else if ( adc->result_format == ADC_LEFT_JUSTIFICATION){
        ADC_LEFT_JUSTIFIED();
    }
    else {
        ADC_RIGHT_JUSTIFIED();
    }
}

/**
 * 
 * @param adc
 */
static void ADC_Reference_Config (const adc_config_t *adc){
    if ( adc->volt_ref == ADC_VOLT_REF_ENABLE){
        ADC_EXTERNAL_REF();
    }
    else if ( adc->volt_ref == ADC_VOLT_REF_DISABLE){
        ADC_INTERNAL_REF();
    }
    else {
        ADC_INTERNAL_REF();
    }
}


#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
void ADC_ISR (void){
    ADC_INTERRUPT_CLEAR_FLAG();
    if (ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
}
#endif