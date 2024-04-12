/* 
 * File:   I2C.h
 * Author: Karim Emad
 *
 * Created on February 20, 2024, 11:44 PM
 */

#ifndef I2C_H
#define	I2C_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define I2C_ENABLE                    1
#define I2C_DISABLE                   0

#define I2C_MASTER_MODE               1
#define I2C_SLAVE_MODE                0

#define I2C_SLEW_RATE_DISABLE         1
#define I2C_SLEW_RATE_ENABLE          0

#define I2C_SMBUS_ENABLE              1
#define I2C_SMBUS_DISABLE             0

#define I2C_LAST_BYTE_DATA            1
#define I2C_LAST_BYTE_ADDRESS         0

#define I2C_STOP_BIT_DETECTED         1
#define I2C_STOP_BIT_NOT_DETECTED     0

#define I2C_START_BIT_DETECTED        1
#define I2C_START_BIT_NOT_DETECTED    0

#define I2C_SLAVE_MODE_7BITS_ADDRESS                0x06U
#define I2C_SLAVE_MODE_10BITS_ADDRESS               0x07U
#define I2C_MASTER_MODE_DEFINED_CLOCK               0x08U       /*clock = FOSC/(4 * (SSPADD + 1))*/
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED         0x0BU
#define I2C_SLAVE_MODE_7BITS_ADDRESS_SS_INTERRUPT   0x0EU
#define I2C_SLAVE_MODE_10BITS_ADDRESS_SS_INTERRUPT  0x0FU

#define I2C_GENERAL_CALL_ENABLE        1
#define I2C_GENERAL_CALL_DISABLE       0

#define I2C_MASTER_RECEIVER_ENABLE     1
#define I2C_MASTER_RECEIVER_DISABLE    0

#define I2C_ACK_NOT_RESEVED_FROM_SALVE 1
#define I2C_ACK_RESEVED_FROM_SALVE     0

#define I2C_MASTER_SEND_NACK           1
#define I2C_MASTER_SEND_ACK            0

/*Function Like Macro Section*/
/* Slew Rate Enable/Disable */
#define I2C_SLEW_RATE_DISABLE_CFG()         (SSPSTATbits.SMP = 1)
#define I2C_SLEW_RATE_ENABLE_CFG()          (SSPSTATbits.SMP = 0)
/* SMBus Enable/Disable */
#define I2C_SMBus_DISABLE_CFG()             (SSPSTATbits.CKE = 0)
#define I2C_SMBus_ENABLE_CFG()              (SSPSTATbits.CKE = 1)
/* General Call Enable */
#define I2C_GENERAL_CALL_DISABLE_CFG()      (SSPCON2bits.GCEN = 0)
#define I2C_GENERAL_CALL_ENABLE_CFG()       (SSPCON2bits.GCEN = 1)
/* Master Mode Receive Enable */
#define I2C_MASTER_RECEIVE_DISABLE_CFG()    (SSPCON2bits.RCEN = 0)
#define I2C_MASTER_RECEIVE_ENABLE_CFG()     (SSPCON2bits.RCEN = 1)
/* Master Synchronous Serial Port Enable */
#define MSSP_MODULE_ENABLE_CFG()            (SSPCON1bits.SSPEN = 1)
#define MSSP_MODULE_DISABLE_CFG()           (SSPCON1bits.SSPEN = 0)
/* Clock Stretch */
#define I2C_CLOCK_STRETCH_ENABLE()          (SSPCON1bits.CKP = 0)
#define I2C_CLOCK_STRETCH_DISABLE()         (SSPCON1bits.CKP = 1)

/*Data Type Declaration Section*/
typedef struct {
    uint32 i2c_master_clock;
    uint8 mssp_mode_select;
    uint8 i2c_slave_address;
    uint8 i2c_mode :1;
    uint8 i2c_slew_rate :1;
    uint8 i2c_SMbus :1;
    uint8 i2c_general_call :1;
    uint8 i2c_receiver :1;
    uint8 i2c_reserved :3;
    #if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*I2C_Report_Bus_Collision) (void) ;
    void (*I2C_DefaultInterruptHandler) (void) ;
    void (*I2C_Report_Receiver_Overflow) (void) ;
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority i2c_priority;
    interrupt_priority bus_collision_priority;
    #endif
    #endif
}i2c_config_t;    

/*Function Declaration Section*/
Std_ReturnType I2C_Init (const i2c_config_t *i2c_obj);
Std_ReturnType I2C_Deinit (const i2c_config_t *i2c_obj);
Std_ReturnType I2C_Send_Start (const i2c_config_t *i2c_obj);
Std_ReturnType I2C_Send_Repeated_Start (const i2c_config_t *i2c_obj);
Std_ReturnType I2C_Send_Stop (const i2c_config_t *i2c_obj);
Std_ReturnType I2C_Read_Data (const i2c_config_t *i2c_obj , uint8 Ack , uint8 *_data);
Std_ReturnType I2C_Write_Data (const i2c_config_t *i2c_obj , uint8 _data , uint8 *_ack);

#endif	/* I2C_H */

