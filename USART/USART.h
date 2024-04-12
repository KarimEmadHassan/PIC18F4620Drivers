/* 
 * File:   USART.h
 * Author: Karim Emad
 *
 * Created on February 12, 2024, 1:07 PM
 */

#ifndef USART_H
#define	USART_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define EUSART_DISABLE    0
#define EUSART_ENABLE     1

#define EUSART_ASYNCHRONOUS_MODE    0
#define EUSART_SYNCHRONOUS_MODE     1

#define EUSART_ASYNCHRONOUS_LOW_SPEED     0
#define EUSART_ASYNCHRONOUS_HIGH_SPEED    1

#define EUSART_8BITS_BAUDRATE_GEN     0
#define EUSART_16BITS_BAUDRATE_GEN     1

#define EUSART_ASYNCHRONOUS_TX_DISABLE    0
#define EUSART_ASYNCHRONOUS_TX_ENABLE     1

#define EUSART_ASYNCHRONOUS_TX_INTERRUPT_DISABLE    0
#define EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE     1

#define EUSART_ASYNCHRONOUS_TX_9BITS_DISABLE    0
#define EUSART_ASYNCHRONOUS_TX_9BITS_ENABLE     1

#define EUSART_ASYNCHRONOUS_RX_DISABLE    0
#define EUSART_ASYNCHRONOUS_RX_ENABLE     1

#define EUSART_ASYNCHRONOUS_RX_INTERRUPT_DISABLE    0
#define EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE     1

#define EUSART_ASYNCHRONOUS_RX_9BITS_DISABLE    0
#define EUSART_ASYNCHRONOUS_RX_9BITS_ENABLE     1

#define EUSART_FRAME_ERROR_CLEARED      0
#define EUSART_FRAME_ERROR_DETECTED     1

#define EUSART_OVERRUN_ERROR_CLEARED      0
#define EUSART_OVERRUN_ERROR_DETECTED     1

/*Function Like Macro Section*/

/*Data Type Declaration Section*/
typedef enum {
    BAUDRATE_ASYN_8BITS_LOW_SPEED = 0,
    BAUDRATE_ASYN_8BITS_HIGH_SPEED ,
    BAUDRATE_ASYN_16BITS_LOW_SPEED,
    BAUDRATE_ASYN_16BITS_HIGH_SPEED,
    BAUDRATE_SYN_8BITS,
    BAUDRATE_SYN_16BITS        
}Baudrate_formula_t;

typedef struct {
    uint8 usart_tx_enable :1 ;
    uint8 usart_tx_9bits_enable :1 ;
    uint8 usart_tx_interrupt_enable :1 ;
    uint8 usart_tx_reserved :5 ;
    interrupt_priority usart_tx_priority;
}usart_tx_cfg_t;

typedef struct {
    uint8 usart_rx_enable :1 ;
    uint8 usart_rx_9bits_enable :1 ;
    uint8 usart_rx_interrupt_enable :1 ;
    uint8 usart_rx_reserved :5 ;
    interrupt_priority usart_rx_priority;
}usart_rx_cfg_t;

typedef union {
    struct {
        uint8 usart_frame_error :1;
        uint8 usart_overrun_error :1;
        uint8 usart_error_reserved :6;
    };
    uint8 usart_error_status;
}usart_error_t;

typedef struct {
    usart_tx_cfg_t transmit_config;
    usart_rx_cfg_t receive_config;
    usart_error_t error_config;
    Baudrate_formula_t baudrate_config;
    uint32 baudrate_value;
    void (*USART_TXDefaultInterruptHandler) (void);
    void (*USART_RXDefaultInterruptHandler) (void);
    void (*USART_FrameErrorDefaultInterruptHandler) (void);
    void (*USART_OverrunErrorDefaultInterruptHandler) (void);
}usart_config_t;

/*Function Declaration Section*/
Std_ReturnType USART_Async_Init   (const usart_config_t *_usart);
Std_ReturnType USART_Async_Deinit (const usart_config_t *_usart);
Std_ReturnType USART_Async_Read_Byte_Blocking  ( uint8 *_data);
Std_ReturnType USART_Async_Read_Byte_Non_Blocking  ( uint8 *_data);
Std_ReturnType USART_Async_Restart  (void);
Std_ReturnType USART_Async_Write_Byte_Blocking ( uint8 _data);
Std_ReturnType USART_Async_Write_Byte_Non_Blocking ( uint8 _data);
Std_ReturnType USART_Async_Write_String_Blocking ( uint8 *_data , uint8 len);
Std_ReturnType USART_Async_Write_String_Non_Blocking ( uint8 *_data , uint8 len);

#endif	/* USART_H */