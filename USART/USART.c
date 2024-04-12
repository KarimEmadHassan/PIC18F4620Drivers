#include "USART.h"

static void Baudrate_Calculations (const usart_config_t *_usart);
static void UART_Tx_Init (const usart_config_t *_usart);
static void UART_Rx_Init (const usart_config_t *_usart);

#if USART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*USART_TX_InterruptHandler) (void) = NULL;
#endif

#if USART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*USART_RX_InterruptHandler) (void) = NULL;
    static void (*USART_Frame_Error_InterruptHandler) (void) = NULL;
    static void (*USART_Overrun_Error_InterruptHandler) (void) = NULL;
#endif

/**
 * 
 * @param _usart
 * @return Std_ReturnType
 */
Std_ReturnType USART_Async_Init (const usart_config_t *_usart){
    Std_ReturnType ret = E_OK;
    if( NULL == _usart ){
        ret = E_NOT_OK;
    }
    else{
        /*Disable EUSART*/
        RCSTAbits.SPEN = EUSART_DISABLE;
        /*Set the pin as input*/
        TRISCbits.RC7 = 1;
        /*Set the pin as input*/
        TRISCbits.RC6 = 1;
        /*Set the baudrate*/
        Baudrate_Calculations(_usart);
        /*configure the transmitter*/
        UART_Tx_Init(_usart);
        /*configure the receiver*/
        UART_Rx_Init(_usart);
        /*Enable EUSART*/
        RCSTAbits.SPEN = EUSART_ENABLE;
    }
    return ret;
}

/**
 * 
 * @param _usart
 * @return 
 */
Std_ReturnType USART_Async_Deinit (const usart_config_t *_usart){
    Std_ReturnType ret = E_OK;
    if( NULL == _usart ){
        ret = E_NOT_OK;
    }
    else{
        /*Disable EUSART*/
        RCSTAbits.SPEN = EUSART_DISABLE;
    }
    return ret;
}

/**
 * 
 * @param _usart
 * @param _data
 * @return 
 */
Std_ReturnType USART_Async_Read_Byte_Blocking ( uint8 *_data){
    Std_ReturnType ret = E_OK;
    while ( !(PIR1bits.RCIF) );
    *_data = RCREG;
    return ret;
}

/**
 * 
 * @param _data
 * @return 
 */
Std_ReturnType USART_Async_Read_Byte_Non_Blocking ( uint8 *_data){
    Std_ReturnType ret = E_OK;
    if ( 1 == PIR1bits.RCIF ) {
        *_data = RCREG;
    }
    else {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @return 
 */
Std_ReturnType USART_Async_Restart  (void){
    Std_ReturnType ret = E_OK;
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
    return ret;
}

/**
 * 
 * @param _usart
 * @param _data
 * @return 
 */
Std_ReturnType USART_Async_Write_Byte_Blocking (uint8 _data){
    Std_ReturnType ret = E_OK;
    while( !(TXSTAbits.TRMT));
#if USART_TX_INTERRUPT_FEATURE_ENABLE  == INTERRUPT_FEATURE_ENABLE
    USART_TX_INTERRUPT_ENABLE();
#endif
    TXREG = _data;
    return ret;
}

/**
 * 
 * @param _data
 * @return 
 */
Std_ReturnType USART_Async_Write_Byte_Non_Blocking ( uint8 _data){
    Std_ReturnType ret = E_OK;
    if( 1 == (TXSTAbits.TRMT)){
        #if USART_TX_INTERRUPT_FEATURE_ENABLE  == INTERRUPT_FEATURE_ENABLE
            USART_TX_INTERRUPT_ENABLE();
        #endif
    TXREG = _data;
    }
    return ret;
}

/**
 * 
 * @param _data
 * @param len
 * @return 
 */
Std_ReturnType USART_Async_Write_String_Blocking ( uint8 *_data , uint8 len){
    Std_ReturnType ret = E_OK; 
    if ( NULL == _data){
        ret = E_NOT_OK;
    }
    else {
        for ( uint8 counter=0 ; counter < len ; counter++){
            ret = USART_Async_Write_Byte_Blocking(_data[counter]);
        }
    }
}

/**
 * 
 * @param _data
 * @param len
 * @return 
 */
Std_ReturnType USART_Async_Write_String_Non_Blocking ( uint8 *_data , uint8 len){
    Std_ReturnType ret = E_OK; 
    if ( NULL == _data){
        ret = E_NOT_OK;
    }
    else {
        for ( uint8 counter=0 ; counter < len ; counter++){
            ret = USART_Async_Write_Byte_Non_Blocking(_data[counter]);
        }
    }
}

static void Baudrate_Calculations (const usart_config_t *_usart){
    float Baudrate_Temp_Value = 0;
    switch ( _usart->baudrate_config ){
        case (BAUDRATE_ASYN_8BITS_LOW_SPEED):
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_8BITS_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/64) - 1;
            break;
        case (BAUDRATE_ASYN_8BITS_HIGH_SPEED):
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_8BITS_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/16) - 1;
            break;    
        case (BAUDRATE_ASYN_16BITS_LOW_SPEED):
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BITS_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/16) - 1;
            break;
        case (BAUDRATE_ASYN_16BITS_HIGH_SPEED):
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BITS_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/4) - 1;
            break;
        case (BAUDRATE_SYN_8BITS):
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_8BITS_BAUDRATE_GEN;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/4) - 1;
            break;
        case (BAUDRATE_SYN_16BITS):
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BITS_BAUDRATE_GEN;
            Baudrate_Temp_Value = ((_XTAL_FREQ / (float)(_usart->baudrate_value))/4) - 1;
            break;
        default:;
    }
    SPBRG = (uint8)((uint32)Baudrate_Temp_Value);
    SPBRGH = (uint8)(((uint32)Baudrate_Temp_Value) >> 8);
}

static void UART_Tx_Init (const usart_config_t *_usart){
    if (EUSART_ASYNCHRONOUS_TX_ENABLE == _usart->transmit_config.usart_tx_enable){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUS_TX_ENABLE;
        if (EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE == _usart->transmit_config.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE;
#if USART_TX_INTERRUPT_FEATURE_ENABLE  == INTERRUPT_FEATURE_ENABLE
            USART_TX_INTERRUPT_ENABLE();
            USART_TX_InterruptHandler = _usart->USART_TXDefaultInterruptHandler;
#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITY_FEATURE_ENABLE();
            if ( _usart->transmit_config.usart_tx_priority == High_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                USART_TX_HIGH_PRIORITY();
            }
            else if (_usart->receive_config.usart_rx_priority == Low_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                USART_TX_LOW_PRIORITY();
            }
            else{}
        #else    
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
#endif
#endif
        }
        else if (EUSART_ASYNCHRONOUS_TX_INTERRUPT_DISABLE == _usart->transmit_config.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_TX_INTERRUPT_DISABLE;
        }
        else {  }
    
        if (EUSART_ASYNCHRONOUS_TX_9BITS_ENABLE == _usart->transmit_config.usart_tx_9bits_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_TX_9BITS_ENABLE;
        }
        else if (EUSART_ASYNCHRONOUS_TX_9BITS_DISABLE == _usart->transmit_config.usart_tx_9bits_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_TX_9BITS_DISABLE;
        }
        else {  }   
    }
    
    else {   }
}

static void UART_Rx_Init (const usart_config_t *_usart){
    if (EUSART_ASYNCHRONOUS_RX_ENABLE == _usart->receive_config.usart_rx_enable){
        RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_ENABLE;
        if (EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE == _usart->receive_config.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE;
            USART_RX_InterruptHandler = _usart->USART_RXDefaultInterruptHandler;
            USART_Frame_Error_InterruptHandler =  _usart->USART_FrameErrorDefaultInterruptHandler;
            USART_Overrun_Error_InterruptHandler = _usart->USART_OverrunErrorDefaultInterruptHandler;       
#if USART_RX_INTERRUPT_FEATURE_ENABLE  == INTERRUPT_FEATURE_ENABLE
            USART_RX_INTERRUPT_ENABLE();
#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PRIORITY_FEATURE_ENABLE();
            if ( _usart->receive_config.usart_rx_priority == High_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                USART_RX_HIGH_PRIORITY();
            }
            else if (_usart->receive_config.usart_rx_priority == Low_Priorty){
                INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                USART_RX_LOW_PRIORITY();
            }
            else{}
        #else    
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
#endif
#endif
        }
        else if (EUSART_ASYNCHRONOUS_RX_INTERRUPT_DISABLE == _usart->receive_config.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_RX_INTERRUPT_DISABLE;
        }
        else {  }
    
        if (EUSART_ASYNCHRONOUS_RX_9BITS_ENABLE == _usart->receive_config.usart_rx_9bits_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_RX_9BITS_ENABLE;
        }
        else if (EUSART_ASYNCHRONOUS_RX_9BITS_DISABLE == _usart->receive_config.usart_rx_9bits_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_RX_9BITS_DISABLE;
        }
        else {  }   
    }
    
    else {   }
}

void UART_TX_ISR (void){
    USART_TX_INTERRUPT_DISABLE();
    if (USART_TX_InterruptHandler){
        USART_TX_InterruptHandler();
    }
    else {}
}

void UART_RX_ISR (void){
    if (USART_RX_InterruptHandler){
        uint8 rx_data = 0;
        USART_RX_InterruptHandler();
        rx_data = RCREG;
    }
    else {}
    
    if (USART_Frame_Error_InterruptHandler){
        USART_Frame_Error_InterruptHandler();
    }
    else {}
    
    if (USART_Overrun_Error_InterruptHandler){
        USART_Overrun_Error_InterruptHandler();
    }
    else {}
}
