/* 
 * File:   INT_Internal.h
 * Author: Karim Emad
 *
 * Created on September 21, 2023, 6:49 PM
 */

#ifndef INT_INTERNAL_H
#define	INT_INTERNAL_H

/*Includes Section*/
#include "INT_Config.h"
#include "INT_Config_Gen.h"

/*Macros Section*/

/*Function Like Macro Section*/
    /*--ADC Section--*/
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define ADC_INTERRUPT_DISABLE()            (PIE1bits.ADIE = 0)
#define ADC_INTERRUPT_ENABLE()             (PIE1bits.ADIE = 1)
#define ADC_INTERRUPT_CLEAR_FLAG()         (PIR1bits.ADIF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define ADC_HIGH_PRIORITY()      (IPR1bits.ADIP = 1)
#define ADC_LOW_PRIORITY()       (IPR1bits.ADIP = 0)
#endif

#endif

    /*--TIMER0 Section--*/
#if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM0_INTERRUPT_DISABLE()            (INTCONbits.TMR0IE = 0)
#define TIM0_INTERRUPT_ENABLE()             (INTCONbits.TMR0IE = 1)
#define TIM0_INTERRUPT_CLEAR_FLAG()         (INTCONbits.TMR0IF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM0_HIGH_PRIORITY()      (INTCON2bits.TMR0IP = 1)
#define TIM0_LOW_PRIORITY()       (INTCON2bits.TMR0IP = 0)
#endif

#endif

    /*--TIMER1 Section--*/
#if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM1_INTERRUPT_DISABLE()            (PIE1bits.TMR1IE = 0)
#define TIM1_INTERRUPT_ENABLE()             (PIE1bits.TMR1IE = 1)
#define TIM1_INTERRUPT_CLEAR_FLAG()         (PIR1bits.TMR1IF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM1_HIGH_PRIORITY()      (IPR1bits.TMR1IP = 1)
#define TIM1_LOW_PRIORITY()       (IPR1bits.TMR1IP = 0)
#endif

#endif

    /*--TIMER2 Section--*/
#if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM2_INTERRUPT_DISABLE()            (PIE1bits.TMR2IE = 0)
#define TIM2_INTERRUPT_ENABLE()             (PIE1bits.TMR2IE = 1)
#define TIM2_INTERRUPT_CLEAR_FLAG()         (PIR1bits.TMR2IF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM2_HIGH_PRIORITY()      (IPR1bits.TMR2IP = 1)
#define TIM2_LOW_PRIORITY()       (IPR1bits.TMR2IP = 0)
#endif

#endif

    /*--TIMER3 Section--*/
#if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM3_INTERRUPT_DISABLE()            (PIE2bits.TMR3IE = 0)
#define TIM3_INTERRUPT_ENABLE()             (PIE2bits.TMR3IE = 1)
#define TIM3_INTERRUPT_CLEAR_FLAG()         (PIR2bits.TMR3IF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define TIM3_HIGH_PRIORITY()      (IPR2bits.TMR3IP = 1)
#define TIM3_LOW_PRIORITY()       (IPR2bits.TMR3IP = 0)
#endif
#endif

    /*--CCP1&2 Section--*/
#if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define CCP1_INTERRUPT_DISABLE()            (PIE1bits.CCP1IE = 0)
#define CCP2_INTERRUPT_DISABLE()            (PIE2bits.CCP2IE = 0)
#define CCP1_INTERRUPT_ENABLE()             (PIE1bits.CCP1IE = 1)
#define CCP2_INTERRUPT_ENABLE()             (PIE2bits.CCP2IE = 1)
#define CCP1_INTERRUPT_CLEAR_FLAG()         (PIR1bits.CCP1IF = 0)
#define CCP2_INTERRUPT_CLEAR_FLAG()         (PIR2bits.CCP2IF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define CCP1_HIGH_PRIORITY()      (IPR1bits.CCP1IP = 1)
#define CCP1_LOW_PRIORITY()       (IPR1bits.CCP1IP = 0)
#define CCP2_HIGH_PRIORITY()      (IPR2bits.CCP2IP = 1)
#define CCP2_LOW_PRIORITY()       (IPR2bits.CCP2IP = 0)
#endif
#endif

    /*--EUSART TX Section--*/
#if USART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define USART_TX_INTERRUPT_DISABLE()            (PIE1bits.TXIE = 0)
#define USART_TX_INTERRUPT_ENABLE()             (PIE1bits.TXIE = 1)
/*The flag will be cleared after data is written*/

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define USART_TX_HIGH_PRIORITY()      (IPR1bits.TXIP = 1)
#define USART_TX_LOW_PRIORITY()       (IPR1bits.TXIP = 0)
#endif
#endif

    /*--EUSART RX Section--*/
#if USART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define USART_RX_INTERRUPT_DISABLE()            (PIE1bits.RCIE = 0)
#define USART_RX_INTERRUPT_ENABLE()             (PIE1bits.RCIE = 1)
/*The flag will be cleared after data is read*/

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define USART_RX_HIGH_PRIORITY()      (IPR1bits.RCIP = 1)
#define USART_RX_LOW_PRIORITY()       (IPR1bits.RCIP = 0)
#endif
#endif

    /*--I2C Section--*/
#if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define I2C_INTERRUPT_DISABLE()                 (PIE1bits.SSPIE = 0)
#define I2C_BUS_COLLISION_INTERRUPT_DISABLE()   (PIE2bits.BCLIE = 0)

#define I2C_INTERRUPT_ENABLE()                 (PIE1bits.SSPIE = 1)
#define I2C_BUS_COLLISION_INTERRUPT_ENABLE()   (PIE2bits.BCLIE = 1)

#define I2C_INTERRUPT_CLEAR_FLAG()         (PIR1bits.SSPIF = 0)
#define I2C_BUS_COLLISION_CLEAR_FLAG()     (PIR2bits.BCLIF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define I2C_HIGH_PRIORITY()                   (IPR1bits.SSPIP = 1)
#define I2C_BUS_COLLISION_HIGH_PRIORITY()     (IPR2bits.BCLIP = 1)

#define I2C_LOW_PRIORITY()                    (IPR1bits.SSPIP = 0)
#define I2C_BUS_COLLISION_LOW_PRIORITY()      (IPR2bits.BCLIP = 0)
#endif
#endif

    /*--SPI Section--*/
#if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define SPI_INTERRUPT_DISABLE()            (PIE1bits.SSPIE = 0)
#define SPI_INTERRUPT_ENABLE()             (PIE1bits.SSPIE = 1)

#define SPI_INTERRUPT_CLEAR_FLAG()         (PIR1bits.SSPIF = 0)

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define SPI_HIGH_PRIORITY()                   (IPR1bits.SSPIP = 1)
#define SPI_LOW_PRIORITY()                    (IPR1bits.SSPIP = 0)

#endif
#endif

/*Data Type Declaration Section*/

/*Function Declaration Section*/

#endif	/* INT_INTERNAL_H */

