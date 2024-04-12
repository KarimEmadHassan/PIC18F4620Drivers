/* 
 * File:   INT_Config_Gen.h
 * Author: Karim Emad
 *
 * Created on September 21, 2023, 9:32 PM
 */

#ifndef INT_CONFIG_GEN_H
#define	INT_CONFIG_GEN_H

#define INTERRUPT_FEATURE_ENABLE    1
#define INTERRUPT_FEATURE_DISABLE   0

#define INTERRUPT_PRIORITY_LEVEL_ENABLE    INTERRUPT_FEATURE_DISABLE
#define EXTERNAL_INTERRUPT_INTx_ENABLE     INTERRUPT_FEATURE_ENABLE
#define EXTERNAL_INTERRUPT_OnCange_ENABLE  INTERRUPT_FEATURE_ENABLE
#define ADC_INTERRUPT_FEATURE_ENABLE       INTERRUPT_FEATURE_DISABLE
#define TIM0_INTERRUPT_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE
#define TIM1_INTERRUPT_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE
#define TIM2_INTERRUPT_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE
#define TIM3_INTERRUPT_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE
#define CCP_INTERRUPT_FEATURE_ENABLE       INTERRUPT_FEATURE_ENABLE
#define USART_TX_INTERRUPT_FEATURE_ENABLE  INTERRUPT_FEATURE_ENABLE
#define USART_RX_INTERRUPT_FEATURE_ENABLE  INTERRUPT_FEATURE_ENABLE
#define I2C_INTERRUPT_FEATURE_ENABLE       INTERRUPT_FEATURE_ENABLE
#define SPI_INTERRUPT_FEATURE_ENABLE       INTERRUPT_FEATURE_ENABLE


#endif	/* INT_CONFIG_GEN_H */
