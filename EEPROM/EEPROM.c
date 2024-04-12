#include "EEPROM.h"

/**
 * 
 * @param address
 * @param data
 * @return 
 */
Std_ReturnType EEPROM_Write_Byte (uint16 address , uint8 data){
    Std_ReturnType RET = E_OK;
    /*Read the interrupt Status*/
    uint8 interrupt_status = INTCONbits.GIE ;
    /*Update the address Register*/
    EEADR = (uint8) (address & 0xff);
    EEADRH = (uint8)((address >>8) & 0x03);
    /*update the data register*/
    EEDATA = data ;
    /*Access EEPROM memory*/
    EECON1bits.EEPGD = Access_EEPROM_Memory;
    EECON1bits.CFGS  = Access_Flash_EEPROM_Memory;
    /*Allow flash cycles*/
    EECON1bits.WREN  = Allow_Write_Cycles;
    /*Disable all general interrupt*/
    INTCONbits.GIE = 0;
    /*Write sequence 0x55 then 0xAA*/
    EECON2 = 0x55;
    EECON2 = 0xAA;
    /*Enable write control bit*/
    EECON1bits.WR  = Write_Enable;
    /*wait for writing complete*/
    while ( EECON1bits.WR );
    /*Inhibits write cycles*/
    EECON1bits.WREN  = Inhibits_Write_Cycles;
    /*Return the interrupt status*/
    INTCONbits.GIE = interrupt_status;
    return RET;
}

/**
 * 
 * @param address
 * @param data
 * @return 
 */
Std_ReturnType EEPROM_Read_Byte  (uint16 address , uint8 *data){
    Std_ReturnType RET = E_OK;
    if ( NULL == data ){
        RET = E_NOT_OK;
    }
    else {
        /*Update the address Register*/
        EEADR = (uint8) (address & 0xff);
        EEADRH = (uint8)((address >>8) & 0x03);
        /*Access EEPROM memory*/
        EECON1bits.EEPGD = Access_EEPROM_Memory;
        EECON1bits.CFGS  = Access_Flash_EEPROM_Memory;
        /*Enable read control bit*/
        EECON1bits.RD  = Read_Enable;
        NOP();
        NOP();
        /*Return data*/
        *data = EEDATA;
    }
    
    return RET;
}
