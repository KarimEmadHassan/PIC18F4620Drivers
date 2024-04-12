/* 
 * File:   ADC.h
 * Author: Karim Emad
 *
 * Created on September 26, 2023, 12:40 PM
 */

#ifndef ADC_H
#define	ADC_H

/*Includes Section*/
#include "pic18f4620.h"
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"

/*Macros Section*/
#define ADC_ENABLED      1
#define ADC_DISABLED     0

#define ADC_CONVERSION_DONE           1
#define ADC_CONVERSION_INPROGRESS     0

/*if (ADC_AN6_ANALOG) was  choosed that means AN0,1,2,3,4,5,6 is enabled as ADC*/
#define ADC_AN0_ANALOG      0x0E
#define ADC_AN1_ANALOG      0x0D
#define ADC_AN2_ANALOG      0x0C
#define ADC_AN3_ANALOG      0x0B
#define ADC_AN4_ANALOG      0x0A
#define ADC_AN5_ANALOG      0x09
#define ADC_AN6_ANALOG      0x08
#define ADC_AN7_ANALOG      0x07
#define ADC_AN8_ANALOG      0x06
#define ADC_AN9_ANALOG      0x05
#define ADC_AN10_ANALOG     0x04
#define ADC_AN11_ANALOG     0x03
#define ADC_AN12_ANALOG     0x02
#define ADC_ALL_ANALOG      0x01
#define ADC_ALL_DIGITAL     0x0F

#define ADC_RIGHT_JUSTIFICATION      1
#define ADC_LEFT_JUSTIFICATION       0

#define ADC_VOLT_REF_ENABLE        1
#define ADC_VOLT_REF_DISABLE       0

/*Function Like Macro Section*/
#define ADC_ENABLE()      (ADCON0bits.ADON = 1)
#define ADC_DISABLE()     (ADCON0bits.ADON = 0)

#define ADC_CONVERSION_STATUS()     (ADCON0bits.GO_nDONE)   /*Determine The Conservison Status*/
#define ADC_START_CONVERSION()      (ADCON0bits.GO_nDONE = 1)   

#define ADC_INTERNAL_REF()      do {ADCON1bits.VCFG1 = 0;\
                                    ADCON1bits.VCFG1 = 0;\
                                    }while (0)

#define ADC_EXTERNAL_REF()      do {ADCON1bits.VCFG1 = 1;\
                                    ADCON1bits.VCFG1 = 1;\
                                    }while (0)

#define ADC_RIGHT_JUSTIFIED()      (ADCON2bits.ADFM = 1)
#define ADC_LEFT_JUSTIFIED()       (ADCON2bits.ADFM = 0)

#define ADC_PORT_CONFIG(_config)      (ADCON1bits.PCFG = _config)


/*Data Type Declaration Section*/
typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12
}ADC_Channel_t;

typedef enum {
    ADC_TAD_0 = 0,
    ADC_TAD_2,
    ADC_TAD_4,
    ADC_TAD_6,
    ADC_TAD_8,
    ADC_TAD_12,
    ADC_TAD_16,
    ADC_TAD_20,
}ADC_Acquisition_Time_t;

typedef enum {
    ADC_Fosc_2 = 0,
    ADC_Fosc_8,
    ADC_Fosc_32,
    ADC_Frc,
    ADC_Fosc_4,
    ADC_Fosc_16,
    ADC_Fosc_64,
}ADC_Conversion_Time_t;

typedef struct {
    #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*ADC_InterruptHandler) (void);    /*pointer to function for the call back*/
    #endif
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;
    #endif
    ADC_Channel_t cahnnel;                  /*to select one channel between the 13channel*/
    ADC_Acquisition_Time_t acq_time;        /*to select the Acquisition Time*/
    ADC_Conversion_Time_t conversion_clk;   /*to select the Conversion Clock*/
    uint8 volt_ref :1;                      /*Single bit to select the voltage ref. internal or external*/
    uint8 result_format :1;                 /*Single bit to select the o/p format right or left*/
}adc_config_t;

/*Function Declaration Section*/
Std_ReturnType ADC_Init                      (const adc_config_t *adc);
Std_ReturnType ADC_DeInit                    (const adc_config_t *adc);
Std_ReturnType ADC_Select_Channel            (const adc_config_t *adc , ADC_Channel_t channel);
Std_ReturnType ADC_Start_Conversion          (const adc_config_t *adc);
Std_ReturnType ADC_Conversion_Status         (const adc_config_t *adc , uint8 *state);
Std_ReturnType ADC_Conversion_Result         (const adc_config_t *adc , uint16 *result);
Std_ReturnType ADC_Get_Conversion_Blocking   (const adc_config_t *adc , ADC_Channel_t channel , uint16 *result);
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_config_t *adc , ADC_Channel_t channel );

#endif	/* ADC_H */