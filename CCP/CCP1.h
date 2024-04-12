/* 
 * File:   CCP.h
 * Author: Karim Emad
 *
 * Created on January 22, 2024, 1:28 AM
 */

#ifndef CCP1_H
#define	CCP1_H

/*Includes Section*/
#include <pic18f4620.h>
#include "CCP1_Config.h"
#include "../MCAL_Std_Types.h"
#include "../GPIO/HAL_GPIO.h"
#include "../Interrupt/INT_Internal.h"
#include "../TIMER2/TIMER2.h"

/*Macros Section*/
#define CCP_DISABLED                               0x00

#define CCP_CAPTURE_MODE_FALLING_EDGE              0x04
#define CCP_CAPTURE_MODE_RISING_EDGE               0x05
#define CCP_CAPTURE_MODE_4TH_RISING_EDGE           0x06
#define CCP_CAPTURE_MODE_16TH_RISING_EDGE          0x07

#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH           0x04
#define CCP_COMPARE_MODE_SET_PIN_LOW               0x08
#define CCP_COMPARE_MODE_SET_PIN_HIGH              0x09
#define CCP_COMPARE_MODE_GENERATE_SW_INTERRUPT     0x0A
#define CCP_COMPARE_MODE_GENERATE_SPECIAL_EVENT    0x0B

#define CCP_PWM_MODE                               0x0C

#define CCP_CAPTURE_NOT_READY                      0x00
#define CCP_CAPTURE_READY                          0x01

#define CCP_COMPARE_NOT_READY                      0x00
#define CCP_COMPARE_READY                          0x01

#define    CCP_POSTSCALER_1    1
#define    CCP_POSTSCALER_2    2   
#define    CCP_POSTSCALER_3    3   
#define    CCP_POSTSCALER_4    4
#define    CCP_POSTSCALER_5    5
#define    CCP_POSTSCALER_6    6
#define    CCP_POSTSCALER_7    7
#define    CCP_POSTSCALER_8    8
#define    CCP_POSTSCALER_9    9
#define    CCP_POSTSCALER_10   10
#define    CCP_POSTSCALER_11   11
#define    CCP_POSTSCALER_12   12
#define    CCP_POSTSCALER_13   13
#define    CCP_POSTSCALER_14   14
#define    CCP_POSTSCALER_15   15
#define    CCP_POSTSCALER_16   16

#define    CCP_PRESCALER_1     1
#define    CCP_PRESCALER_4     4
#define    CCP_PRESCALER_16    16   

/*Function Like Macro Section*/
#define CCP1_SET_MODE(Mode)                         (CCP1CONbits.CCP1M = Mode)
#define CCP2_SET_MODE(Mode)                         (CCP2CONbits.CCP2M = Mode)

/*Data Type Declaration Section*/
typedef enum {
    CCP1_CHANNEL = 0,
    CCP2_CHANNEL
}CCP_Channel;

typedef enum {
    CCP_Capture_Mode =0,
    CCP_Compare_Mode,
    CCP_PWM_Mode
}CCP_Mode;

typedef enum {
    CCP1_CCP2_TIM3 =0,
    CCP1_TIM1_CCP2_TIM3,
    CCP1_CCP2_TIM1
}CCP_Capture_Timer;

typedef union {
    struct{
        uint8 ccpr_low;
        uint8 ccpr_high;
    };
    struct{
        uint16 ccpr_16bits;
    };
}CCP_Register;


typedef struct {
    CCP_Channel ccp_channel;
    CCP_Mode ccp_mode;
    uint8 ccp_mode_varient;
    pin_config_t ccp_pin;
    CCP_Capture_Timer ccp_capture_timer;
#if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (*CCP1_InterruptHandler) (void);
    void (*CCP2_InterruptHandler) (void);
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority ccp1_priority ; 
    interrupt_priority ccp2_priority ; 
    #endif
#endif
 
#if ((CCP1_SELECTED_MODE == CCP_CONFIG_PWM_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_PWM_MODE) )
    uint32 pwm_frequency;
    uint8 postscaler_value ;
    uint8 prescaler_value ;
#endif
}CCP_Config_t;


/*Function Declaration Section*/
Std_ReturnType CCP_Init   (const CCP_Config_t *ccp_obj);
Std_ReturnType CCP_DeInit (const CCP_Config_t *ccp_obj);

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_CAPTURE_MODE) )
    Std_ReturnType CCP_Check_Capture_Ready (const CCP_Config_t *ccp_obj , uint8 *capture_state);
    Std_ReturnType CCP_Get_Capture_Value   (const CCP_Config_t *ccp_obj , uint16 *capture_value);
#endif

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_COMPARE_MODE) )
    Std_ReturnType CCP_Check_Compare_Ready (const CCP_Config_t *ccp_obj , uint8 *compare_state);
    Std_ReturnType CCP_Set_Compare_Value   (const CCP_Config_t *ccp_obj , uint16 compare_value);

#endif

#if ((CCP1_SELECTED_MODE == CCP_CONFIG_PWM_MODE) || (CCP2_SELECTED_MODE == CCP_CONFIG_PWM_MODE))
    Std_ReturnType CCP_PWM_Start    (const CCP_Config_t *ccp_obj);
    Std_ReturnType CCP_PWM_Stop     (const CCP_Config_t *ccp_obj);
    Std_ReturnType CCP_PWM_Set_Duty (const CCP_Config_t *ccp_obj , uint8 pwm_duty);

#endif

#endif	/* CCP_H */

