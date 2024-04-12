/* 
 * File:   INT_Config.h
 * Author: Karim Emad
 *
 * Created on September 21, 2023, 6:49 PM
 */

#ifndef INT_CONFIG_H
#define	INT_CONFIG_H

/*Includes Section*/
#include "pic18f4620.h"
#include "../MCAL_Std_Types.h"
#include "INT_Config_Gen.h"

/*Macros Section*/
#define INTERRUPT_DISABLE   0
#define INTERRUPT_ENABLE    1

#define INTERRUPT_NOT_OCCUR    0
#define INTERRUPT_OCCUR        1

#define INTERRUPT_PRIORITY_DISABLE   0
#define INTERRUPT_PRIORITY_ENABLE    1


/*Function Like Macro Section*/
#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_PRIORITY_FEATURE_ENABLE()        (RCONbits.IPEN = 1)
#define INTERRUPT_PRIORITY_FEATURE_DISABLE()       (RCONbits.IPEN = 0)

#define INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE()   (INTCONbits.GIEH = 1)
#define INTERRUPT_GLOBAL_INTERRUPT_HIGH_DISABLE()  (INTCONbits.GIEH = 0)

#define INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE()    (INTCONbits.GIEL = 1)
#define INTERRUPT_GLOBAL_INTERRUPT_LOW_DISABLE()   (INTCONbits.GIEL = 0)

#else
#define INTERRUPT_PERIPHERAL_ENABLE()              (INTCONbits.PEIE = 1)
#define INTERRUPT_PERIPHERAL_DISABLE()             (INTCONbits.PEIE = 0)

#define INTERRUPT_GLOBAL_INTERRUPT_ENABLE()        (INTCONbits.GIE = 1)
#define INTERRUPT_GLOBAL_INTERRUPT_DISABLE()       (INTCONbits.GIE = 0)
#endif

/*Data Type Declaration Section*/
typedef enum {
    Low_Priorty=0,
    High_Priorty        
}interrupt_priority;

/*Function Declaration Section*/

#endif	/* INT_CONFIG_H */

