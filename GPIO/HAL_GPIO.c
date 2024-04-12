#include "HAL_GPIO.h"

volatile uint8 *TRIS_REG[] = { &TRISA , &TRISB , &TRISC , &TRISD , &TRISE };
volatile uint8 *PORT_REG[] = { &PORTA , &PORTB , &PORTC , &PORTD , &PORTE };
volatile uint8 *LAT_REG[] = { &LATA , &LATB , &LATC , &LATD , &LATE };

/**
 * 
 * @param _pin_config
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Direction_Init (const pin_config_t *_pin_config){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config || (_pin_config->PIN_NUMBER)>MAX_PINS-1 ){
        RET = E_NOT_OK;
    }
    
    else{
        switch(_pin_config->DIRECTION){
            case (OUTPUT):
                CLEAR_BIT( *TRIS_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER );   
                break;
            case (INPUT):
                SET_BIT( *TRIS_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER );  
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
 * @param _pin_config
 * @param direction_status
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Direction_Status (const pin_config_t *_pin_config , direction_t *direction_status){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config || NULL== direction_status ||(_pin_config->PIN_NUMBER)>MAX_PINS-1 || (_pin_config->PORT)>MAX_PORTS-1){
        RET = E_NOT_OK;
    }
    
    else{
        *direction_status = READ_BIT ( *TRIS_REG[_pin_config->PORT] , _pin_config->DIRECTION);
    }
    return RET;
}
#endif

/**
 * 
 * @param _pin_config
 * @param logic_status
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Write_Logic (const pin_config_t *_pin_config , logic_t logic_status){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config ||(_pin_config->PIN_NUMBER)>MAX_PINS-1 || (_pin_config->PORT)>MAX_PORTS-1){
        RET = E_NOT_OK;
    }
    
    else{
        switch (logic_status){
            case (HIGH):
                SET_BIT ( *LAT_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER );
                break;
            case (LOW):
                CLEAR_BIT ( *LAT_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER );
                break;
            default :
                RET = E_NOT_OK;
        }
    }
    return RET;
}
#endif

/**
 * 
 * @param _pin_config
 * @param logic_status
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Read_Logic (const pin_config_t *_pin_config , logic_t *logic_status){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config || NULL == logic_status || (_pin_config->PIN_NUMBER)>MAX_PINS-1 || (_pin_config->PORT)>MAX_PORTS-1){
        RET = E_NOT_OK;
    }
    
    else{
        *logic_status = READ_BIT ( *PORT_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER);
    }
    return RET;
}
#endif

/**
 * 
 * @param _pin_config
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Toggle_Logic (const pin_config_t *_pin_config){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config || (_pin_config->PIN_NUMBER)>MAX_PINS-1 || (_pin_config->PORT)>MAX_PORTS-1){
        RET = E_NOT_OK;
    }
    
    else{
        TOGGLE_BIT ( *LAT_REG[_pin_config->PORT] , _pin_config->PIN_NUMBER);
    }
    return RET;
}
#endif

/**
 * 
 * @param _pin_config
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PIN_Init (const pin_config_t *_pin_config){
    Std_ReturnType RET = E_OK;
    if ( NULL == _pin_config ||(_pin_config->PIN_NUMBER)>MAX_PINS-1 || (_pin_config->PORT)>MAX_PORTS-1){
        RET = E_NOT_OK;
    }
    else{
        RET = GPIO_PIN_Direction_Init(_pin_config);
        RET = GPIO_PIN_Write_Logic(_pin_config , _pin_config->LOGIC);
    }
    return RET;
}
#endif

/**
 * 
 * @param port
 * @param direction
 * @return 
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PORT_Direction_Init (port_index_t port , uint8 direction){
    Std_ReturnType RET = E_OK;
    if ( (port)>MAX_PORTS-1 ){
        RET = E_NOT_OK;
    }
    
    else{
        *TRIS_REG[port] = direction;
    }
    return RET;
}
#endif

/**
 * 
 * @param port
 * @param direction_status
 * @return 
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PORT_Direction_Status (port_index_t port , uint8 *direction_status){
    Std_ReturnType RET = E_OK;
    if ( (port)> MAX_PORTS-1 || NULL == direction_status ){
        RET = E_NOT_OK;
    }
    
    else{
        *direction_status = *TRIS_REG[port] ;
    }
    return RET;
}
#endif

/**
 * 
 * @param port
 * @param logic
 * @return 
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PORT_Write_Logic (port_index_t port , uint8 logic){
    Std_ReturnType RET = E_OK;
    if ( (port)> MAX_PORTS-1 ){
        RET = E_NOT_OK;
    }
    
    else{
        *LAT_REG[port] = logic;
    }
    return RET;
}
#endif

/**
 * 
 * @param port
 * @param logic
 * @return 
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PORT_Read_Logic (port_index_t port , uint8 *logic){
    Std_ReturnType RET = E_OK;
    if ( (port)> MAX_PORTS-1 || NULL == logic ){
        RET = E_NOT_OK;
    }
    
    else{
        *logic = *LAT_REG[port] ;
    }
    return RET;
}
#endif

/**
 * 
 * @param port
 * @return 
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType GPIO_PORT_Toggle_Logic (port_index_t port){
    Std_ReturnType RET = E_OK;
    if ( (port)> MAX_PORTS-1 ){
        RET = E_NOT_OK;
    }
    
    else{
        *LAT_REG[port] ^= 0xFF ;
    }
    return RET;
}
#endif