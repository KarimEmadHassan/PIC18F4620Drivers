#include "INT_External.h"

static void (*INT0_InterruptHandler) (void) = NULL;
static void (*INT1_InterruptHandler) (void) = NULL;
static void (*INT2_InterruptHandler) (void) = NULL;

static void (*RB4_InterruptHandler_HIGH) (void) = NULL;
static void (*RB4_InterruptHandler_LOW) (void) = NULL;
static void (*RB5_InterruptHandler_HIGH) (void) = NULL;
static void (*RB5_InterruptHandler_LOW) (void) = NULL;
static void (*RB6_InterruptHandler_HIGH) (void) = NULL;
static void (*RB6_InterruptHandler_LOW) (void) = NULL;
static void (*RB7_InterruptHandler_HIGH) (void) = NULL;
static void (*RB7_InterruptHandler_LOW) (void) = NULL;

static Std_ReturnType INTx_Enable        ( const interrupt_intx_t  *intx );
static Std_ReturnType INTx_Disable       ( const interrupt_intx_t  *intx );
static Std_ReturnType INTx_Priority_Init ( const interrupt_intx_t  *intx );
static Std_ReturnType INTx_Edge_Init     ( const interrupt_intx_t  *intx );
static Std_ReturnType INTx_PIN_Init      ( const interrupt_intx_t  *intx );
static Std_ReturnType INTx_Flag_Clear    ( const interrupt_intx_t  *intx );
static Std_ReturnType INT0_Interrupt_Handler    ( void (*InterruptHandler) (void) );
static Std_ReturnType INT1_Interrupt_Handler    ( void (*InterruptHandler) (void) );
static Std_ReturnType INT2_Interrupt_Handler    ( void (*InterruptHandler) (void) );
static Std_ReturnType Interrupt_INTx_Handler    ( const interrupt_intx_t  *intx );

static Std_ReturnType RBx_Enable             ( const interrupt_rbx_t  *intx );
static Std_ReturnType RBx_Disable            ( const interrupt_rbx_t  *intx );
static Std_ReturnType RBx_Priority_Init      ( const interrupt_rbx_t  *intx );
static Std_ReturnType RBx_Flag_Clear         ( const interrupt_rbx_t  *intx );
static Std_ReturnType Interrupt_RBx_Handler  ( const interrupt_rbx_t  *intx );

/*----------------------------------------------------------------------------*/
/**
 * 
 * @param intx
 * @return 
 */
Std_ReturnType INTx_Init ( const interrupt_intx_t *intx){
    Std_ReturnType RET = E_OK;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        /*disable the interrupt*/
        RET = INTx_Disable(intx);
        /*Clear the interrupt flag*/
        RET = INTx_Flag_Clear(intx);
        /*Configure Edge & pin & Priority*/
        RET = INTx_Edge_Init(intx);
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE==INTERRUPT_FEATURE_ENABLE
        RET = INTx_Priority_Init(intx);
        #endif
        RET = INTx_PIN_Init(intx);
        /*Configure The interrupt Call back*/
        RET = Interrupt_INTx_Handler(intx);
        /*Enable the interrupt*/
        RET = INTx_Enable(intx);
    }
}

void INT0_ISR (void){
    /*Clear Flag*/
    INTERRUPT_INT0_CLEAR_FLAG();
    /*Code*/
    
    /*Callback function*/
    if (INT0_InterruptHandler){
        INT0_InterruptHandler();
    }
}

void INT1_ISR (void){
    /*Clear Flag*/
    INTERRUPT_INT1_CLEAR_FLAG();
    /*Code*/
    
    /*Callback function*/
    if (INT1_InterruptHandler){
        INT1_InterruptHandler();
    }
}

void INT2_ISR (void){
    /*Clear Flag*/
    INTERRUPT_INT2_CLEAR_FLAG();
    /*Code*/
    
    /*Callback function*/
    if (INT2_InterruptHandler){
        INT2_InterruptHandler();
    }
}


/**
 * 
 * @param intx
 * @return 
 */
Std_ReturnType INTx_DeInit ( const interrupt_intx_t *intx){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        /*disable the interrupt*/
        RET = INTx_Disable(intx);
        /*Clear the interrupt flag*/
        RET = INTx_Flag_Clear(intx);
    }
    return RET;

}

/**
 * 
 * @param intx
 * @return 
 */
Std_ReturnType RBx_Init ( const interrupt_rbx_t  *intx){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the interrupt*/
        RET = RBx_Disable(intx);
        /*Clear the flag*/
        RET = INTx_Flag_Clear(intx);
        /*Configure the pin*/
        RET = GPIO_PIN_Direction_Init(&(intx->pin));
        /*Configure the priority*/
        RET = RBx_Priority_Init(intx);
        /*Callback Function*/
        RET = Interrupt_RBx_Handler(intx);
        /*Enable the interrupt*/
        RET = RBx_Enable(intx);
    }
    return RET;

}

void RB4_ISR (uint8 level){
    INTERRUPT_RBx_CLEAR_FLAG();
    if (0 == level){
        RB4_InterruptHandler_LOW();
    }
    else if ( 1 == level){
        RB4_InterruptHandler_HIGH();
    }
    else {}
}

void RB5_ISR (uint8 level){
    INTERRUPT_RBx_CLEAR_FLAG();
    if (0 == level){
        RB5_InterruptHandler_LOW();
    }
    else if ( 1 == level){
        RB5_InterruptHandler_HIGH();
    }
    else {}
}

void RB6_ISR (uint8 level){
    INTERRUPT_RBx_CLEAR_FLAG();
    if (0 == level){
        RB6_InterruptHandler_LOW();
    }
    else if ( 1 == level){
        RB6_InterruptHandler_HIGH();
    }
    else {}
}

void RB7_ISR (uint8 level){
    INTERRUPT_RBx_CLEAR_FLAG();
    if (0 == level){
        RB7_InterruptHandler_LOW();
    }
    else if ( 1 == level){
        RB7_InterruptHandler_HIGH();
    }
    else {}
}

/**
 * 
 * @param intx
 * @return 
 */
Std_ReturnType RBx_DeInit  ( const interrupt_rbx_t  *intx){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the interrupt*/
        INTERRUPT_RBx_DISABLE();
        /*Clear the flag*/
        INTERRUPT_RBx_CLEAR_FLAG();

    }
    return RET;
}

/*---------------------------------------------------------------------*/

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType INTx_Enable ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT0):  
                #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                    INTERRUPT_PRIORITY_FEATURE_ENABLE();
                    INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                #else
                    INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                    INTERRUPT_PERIPHERAL_ENABLE();
                #endif
                INTERRUPT_INT0_ENABLE(); 
                break;
                
            case (Interrupt_INT1):  
                #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                    INTERRUPT_PRIORITY_FEATURE_ENABLE();
                    if ( intx->priority == High_Priorty ){
                        INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    }
                    else if ( intx->priority == Low_Priorty ){
                        INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    }
                    else {}
                #else
                    INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                    INTERRUPT_PERIPHERAL_ENABLE();
                #endif
                INTERRUPT_INT1_ENABLE(); 
                break;    
                
            case (Interrupt_INT2):  
                #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
                    INTERRUPT_PRIORITY_FEATURE_ENABLE();
                    if ( intx->priority == High_Priorty ){
                        INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                    }
                    else if ( intx->priority == Low_Priorty ){
                        INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                    }
                    else {}
                #else
                    INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
                    INTERRUPT_PERIPHERAL_ENABLE();
                #endif
                INTERRUPT_INT2_ENABLE(); 
                break;    
                
            default:                RET = E_NOT_OK;
        }
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType INTx_Disable ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT0):  INTERRUPT_INT0_DISABLE(); break;
            case (Interrupt_INT1):  INTERRUPT_INT1_DISABLE(); break;    
            case (Interrupt_INT2):  INTERRUPT_INT2_DISABLE(); break;    
            default:                RET = E_NOT_OK;
        }
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
static Std_ReturnType INTx_Priority_Init ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT1): 
                if ( intx->priority == Low_Priorty ){
                    INTERRUPT_INT1_LOW_PRIORITY();
                }
                else if ( intx->priority == High_Priorty){
                    INTERRUPT_INT1_HIGH_PRIORITY();
                }
                else {}
                break;    
                
            case (Interrupt_INT2): 
                if ( intx->priority == Low_Priorty ){
                    INTERRUPT_INT2_LOW_PRIORITY();
                }
                else if ( intx->priority == High_Priorty){
                    INTERRUPT_INT2_HIGH_PRIORITY();
                }
                else {}
                break;    
            default:                
                RET = E_NOT_OK;
        }
    }
    return RET;

}
#endif

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType INTx_Edge_Init ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT0):  
                if ( intx->edge == Interrupt_Falling_Edge){
                    INTERRUPT_INT0_FALLING_EDGE();
                }
                else if ( intx->edge == Interrupt_raising_Edge){
                    INTERRUPT_INT0_RISING_EDGE();
                }
                else {}
                break;
            case (Interrupt_INT1):  
                if ( intx->edge == Interrupt_Falling_Edge){
                    INTERRUPT_INT1_FALLING_EDGE();
                }
                else if ( intx->edge == Interrupt_raising_Edge){
                    INTERRUPT_INT1_RISING_EDGE();
                }
                else {}
                break;    
            case (Interrupt_INT2):  
                if ( intx->edge == Interrupt_Falling_Edge){
                    INTERRUPT_INT2_FALLING_EDGE();
                }
                else if ( intx->edge == Interrupt_raising_Edge){
                    INTERRUPT_INT2_RISING_EDGE();
                }
                else {}
                break;    
            default:                
                RET = E_NOT_OK;
        }
    }
    return RET;

}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType INTx_PIN_Init ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        RET = GPIO_PIN_Direction_Init(&(intx->pin));
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType INTx_Flag_Clear    ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT0):  
                INTERRUPT_INT0_CLEAR_FLAG();
                break;
            case (Interrupt_INT1):  
                INTERRUPT_INT1_CLEAR_FLAG();
                break;    
            case (Interrupt_INT2):  
                INTERRUPT_INT2_CLEAR_FLAG();
                break;    
            default:                
                RET = E_NOT_OK;
        }
    }
    return RET;

}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT0_Interrupt_Handler ( void (*InterruptHandler) (void) ){
    Std_ReturnType RET = E_OK;
    if ( NULL == InterruptHandler ){
        RET = E_NOT_OK;
    }
    else {
        INT0_InterruptHandler = InterruptHandler;
    }
    return RET;
}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT1_Interrupt_Handler ( void (*InterruptHandler) (void) ){
    Std_ReturnType RET = E_OK;
    if ( NULL == InterruptHandler ){
        RET = E_NOT_OK;
    }
    else {
        INT1_InterruptHandler = InterruptHandler;
    }
    return RET;
}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT2_Interrupt_Handler ( void (*InterruptHandler) (void) ){
    Std_ReturnType RET = E_OK;
    if ( NULL == InterruptHandler ){
        RET = E_NOT_OK;
    }
    else {
        INT2_InterruptHandler = InterruptHandler;
    }
    return RET;

}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Handler ( const interrupt_intx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch ( intx -> source ){
            case (Interrupt_INT0): 
                RET = INT0_Interrupt_Handler ( intx->External_Interrupt_Handler );
                break;    
                
            case (Interrupt_INT1): 
                RET = INT1_Interrupt_Handler ( intx->External_Interrupt_Handler );
                break;    
                
            case (Interrupt_INT2): 
                RET = INT2_Interrupt_Handler ( intx->External_Interrupt_Handler );
                break; 
                
            default:                
                RET = E_NOT_OK;
        }
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType RBx_Enable ( const interrupt_rbx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        INTERRUPT_RBx_ENABLE();
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType RBx_Disable ( const interrupt_rbx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        INTERRUPT_RBx_DISABLE();
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType RBx_Flag_Clear    ( const interrupt_rbx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        INTERRUPT_RBx_CLEAR_FLAG();
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType RBx_Priority_Init ( const interrupt_rbx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITY_FEATURE_ENABLE();
            if ( intx->priority == High_Priorty ){
                INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                INTERRUPT_RBx_HIGH_PRIORITY();
            }
            else if ( intx->priority == Low_Priorty ){
                INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                INTERRUPT_RBx_LOW_PRIORITY();
            }
            else {}
        #else
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
        #endif
    }
    return RET;
}

/**
 * 
 * @param intx
 * @return 
 */
static Std_ReturnType Interrupt_RBx_Handler  ( const interrupt_rbx_t  *intx ){
    Std_ReturnType RET = E_OK ;
    if ( NULL == intx ){
        RET = E_NOT_OK;
    }
    else {
        switch (intx->pin.PIN_NUMBER){
            case (PIN4): 
                RB4_InterruptHandler_HIGH = intx->External_Interrupt_Handler_High;
                RB4_InterruptHandler_LOW = intx->External_Interrupt_Handler_Low;
                break;
            case (PIN5): 
                RB5_InterruptHandler_HIGH = intx->External_Interrupt_Handler_High;
                RB5_InterruptHandler_LOW = intx->External_Interrupt_Handler_Low;
                break;
            case (PIN6): 
                RB6_InterruptHandler_HIGH = intx->External_Interrupt_Handler_High;
                RB6_InterruptHandler_LOW = intx->External_Interrupt_Handler_Low;
                break;
            case (PIN7): 
                RB7_InterruptHandler_HIGH = intx->External_Interrupt_Handler_High;
                RB7_InterruptHandler_LOW = intx->External_Interrupt_Handler_Low;
                break;
            default : RET = E_NOT_OK;
        }
    }
    return RET;
}