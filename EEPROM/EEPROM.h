/* 
 * File:   EEPROM.h
 * Author: Karim Emad
 *
 * Created on September 23, 2023, 7:39 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

/*Includes Section*/
#include "pic18f4620.h"
#include "../MCAL_Std_Types.h"

/*Macros Section*/
#define Access_Flash_Memory     1
#define Access_EEPROM_Memory    0

#define Access_Configration_Registers     1
#define Access_Flash_EEPROM_Memory        0

#define Allow_Write_Cycles           1
#define Inhibits_Write_Cycles        0

#define Read_Enable           1
#define Read_Disable          0

#define Interrupt_Enable           1
#define Interrupt_Disable          0

#define Write_Enable           1
#define Write_Disable          0

/*Function Like Macro Section*/

/*Data Type Declaration Section*/

/*Function Declaration Section*/
Std_ReturnType EEPROM_Write_Byte (uint16 address , uint8 data);
Std_ReturnType EEPROM_Read_Byte  (uint16 address , uint8 *data);

#endif	/* EEPROM_H */

