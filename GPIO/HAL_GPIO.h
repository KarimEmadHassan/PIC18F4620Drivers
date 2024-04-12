/* 
 * File:   HAL_GPIO.h
 * Author: Karim Emad
 *
 * Created on September 12, 2023, 8:49 PM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/*Includes Section*/
#include "pic18f4620.h"
#include "../MCAL_Std_Types.h"
#include "../Device_Config.h"

/*Macros Section*/
#define MAX_PORTS    5
#define MAX_PINS     8

/*Function Like Macro Section*/
#define HWREG8(_X)             (*((volatile uint8 *)(_X)))
#define SET_BIT(REG , BIT)     (REG |= (1<<BIT))
#define CLEAR_BIT(REG , BIT)   (REG &= ~(1<<BIT))
#define TOGGLE_BIT(REG , BIT)  (REG ^= (1<<BIT))
#define READ_BIT(REG , BIT)    ((REG >> BIT)&1)


/*Data Type Declaration Section*/
typedef enum {
    LOW=0,
    HIGH
}logic_t;

typedef enum {
    OUTPUT=0,
    INPUT
}direction_t;

typedef enum {
    PIN0=0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7      
}pin_index_t;

typedef enum {
    PORTA_INDEX=0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX
}port_index_t;

typedef struct{
    uint8 PORT :3 ;
    uint8 PIN_NUMBER :3 ;
    uint8 DIRECTION :1 ;
    uint8 LOGIC :1 ;
}pin_config_t;

/*Function Declaration Section*/
Std_ReturnType GPIO_PIN_Direction_Init   (const pin_config_t *_pin_config);
Std_ReturnType GPIO_PIN_Direction_Status (const pin_config_t *_pin_config , direction_t *direction_status);
Std_ReturnType GPIO_PIN_Write_Logic      (const pin_config_t *_pin_config , logic_t logic_status);
Std_ReturnType GPIO_PIN_Read_Logic       (const pin_config_t *_pin_config , logic_t *logic_status);
Std_ReturnType GPIO_PIN_Toggle_Logic     (const pin_config_t *_pin_config);
Std_ReturnType GPIO_PIN_Init             (const pin_config_t *_pin_config);

Std_ReturnType GPIO_PORT_Direction_Init    (port_index_t port , uint8 direction);
Std_ReturnType GPIO_PORT_Direction_Status  (port_index_t port , uint8 *direction_status);
Std_ReturnType GPIO_PORT_Write_Logic       (port_index_t port , uint8 logic);
Std_ReturnType GPIO_PORT_Read_Logic        (port_index_t port , uint8 *logic);
Std_ReturnType GPIO_PORT_Toggle_Logic      (port_index_t port);


#endif	/* HAL_GPIO_H */