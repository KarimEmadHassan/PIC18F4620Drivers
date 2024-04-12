/* 
 * File:   SPI.h
 * Author: Karim Emad
 *
 * Created on April 9, 2024, 4:56 AM
 */

#ifndef SPI_H
#define	SPI_H

/*Includes Section*/
#include <pic18f4620.h>
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define SPI_SAMPLE_DATA_AT_END                  1
#define SPI_SAMPLE_DATA_AT_MIDDLE               0

#define SPI_TRANSMISSION_FROM_ACTIVE_TO_IDLE    1
#define SPI_TRANSMISSION_FROM_IDLE_TO_ACTIVE    0

#define SPI_BUFFER_IS_FULL                      1
#define SPI_BUFFER_IS_EMPTY                     0

#define SPI_BUS_COLLISION_DETECTED              1
#define SPI_BUS_COLLISION_NOT_DETECTED          0

#define SPI_OVERFLOW_DETECTED                   1
#define SPI_OVERFLOW_NOT_DETECTED               0

#define SPI_CLK_HIGH_LEVEL                      1
#define SPI_CLK_LOW_LEVEL                       0

#define SPI_MASTER_MODE_CLK_FOSC_BY4            0x00
#define SPI_MASTER_MODE_CLK_FOSC_BY16           0x01
#define SPI_MASTER_MODE_CLK_FOSC_BY64           0x02
#define SPI_MASTER_MODE_CLK_TMR2_BY2            0x03
#define SPI_SLAVE_MODE_SS_ENABLED               0x04
#define SPI_SLAVE_MODE_SS_DISABLED              0x05

/*Function Like Macro Section*/
#define SPI_SERIAL_PINS_ENABLE_CFG()   (SSPCON1bits.SSPEN = 1)
#define SPI_SERIAL_PINS_DISABLE_CFG()  (SSPCON1bits.SSPEN = 0)

#define SPI_CLK_HIGH_LEVEL_CFG()       (SSPCON1bits.CKP = 1)
#define SPI_CLK_LOW_LEVEL_CFG()        (SSPCON1bits.CKP = 0)

#define SPI_MODE_CFG(_mode)            (SSPCON1bits.SSPM = _mode)
#define SPI_SAMPLE_BIT_CFG(_mode)      (SSPSTATbits.SMP = _mode)
#define SPI_CLK_SELECT_BIT_CFG(_mode)  (SSPSTATbits.CKE = _mode)
#define SPI_CLK_POLARITY_CFG(_mode)    (SSPCON1bits.CKP = _mode)

/*Data Type Declaration Section*/
typedef struct {
    uint8 master_sample_bit :1;
    uint8 clk_transmission :1;
    uint8 clk_polarity :1;
    uint8 spi_mode :4;
    uint8 spi_reserved :1;
    #if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        void (*I2C_DefaultInterruptHandler) (void) ;
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
        interrupt_priority priority ;
    #endif
    #endif    
}spi_config_t;

/*Function Declaration Section*/
Std_ReturnType SPI_Init (const spi_config_t *spi_obj);
Std_ReturnType SPI_Deinit (const spi_config_t *spi_obj);
Std_ReturnType SPI_Read_Data (const spi_config_t *spi_obj , uint8 *_data);
Std_ReturnType SPI_Write_Data (const spi_config_t *spi_obj , uint8 _data );

#endif	/* SPI_H */

