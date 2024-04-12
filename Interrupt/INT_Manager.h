/* 
 * File:   INT_Manager.h
 * Author: Karim Emad
 *
 * Created on September 21, 2023, 6:50 PM
 */

#ifndef INT_MANAGER_H
#define	INT_MANAGER_H

/*Includes Section*/
#include "INT_Config.h"

/*Macros Section*/

/*Function Like Macro Section*/

/*Data Type Declaration Section*/

/*Function Declaration Section*/
void INT0_ISR (void);
void INT1_ISR (void);
void INT2_ISR (void);
void RB4_ISR (uint8 level);
void RB5_ISR (uint8 level);
void RB6_ISR (uint8 level);
void RB7_ISR (uint8 level);
void ADC_ISR (void);
void TIM0_ISR (void);
void TIM1_ISR (void);
void TIM2_ISR (void);
void TIM3_ISR (void);
void CCP1_ISR (void);
void CCP2_ISR (void);
void UART_TX_ISR (void);
void UART_RX_ISR (void);
void I2C_ISR (void);
void I2C_BC_ISR (void);
void SPI_ISR (void);

#endif	/* INT_MANAGER_H */

