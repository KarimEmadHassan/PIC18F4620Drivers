#include "SPI.h"

#if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
    static void (*SPI_InterruptHandler) (void) = NULL;
#endif

/**
 * 
 * @param spi_obj
 * @return 
 */
Std_ReturnType SPI_Init (const spi_config_t *spi_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == spi_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the moduele*/
        SPI_SERIAL_PINS_DISABLE_CFG();
        /*Configure the spi mode*/
        SPI_MODE_CFG(spi_obj->spi_mode);
        /*Configure the spi pins mode*/
        pin_config_t SPI_SDO = {.PORT = PORTC_INDEX, .PIN_NUMBER = PIN5, .DIRECTION = OUTPUT};
        pin_config_t SPI_SDI = {.PORT = PORTC_INDEX, .PIN_NUMBER = PIN4, .DIRECTION = INPUT};
        if ( (spi_obj->spi_mode != SPI_SLAVE_MODE_SS_ENABLED) || (spi_obj->spi_mode != SPI_SLAVE_MODE_SS_DISABLED) ){
            pin_config_t SPI_Master_CLK = {.PORT = PORTC_INDEX, .PIN_NUMBER = PIN3, .DIRECTION = OUTPUT};
            RET = GPIO_PIN_Init(&SPI_SDO); 
            RET = GPIO_PIN_Init(&SPI_SDI);
            RET = GPIO_PIN_Init(&SPI_Master_CLK);
        }
        else {
            pin_config_t SPI_Slave_CLK = {.PORT = PORTC_INDEX, .PIN_NUMBER = PIN3, .DIRECTION = INPUT};
            pin_config_t SPI_SS = {.PORT = PORTA_INDEX, .PIN_NUMBER = PIN5, .DIRECTION = INPUT};
            RET = GPIO_PIN_Init(&SPI_SDO); 
            RET = GPIO_PIN_Init(&SPI_SDI);
            RET = GPIO_PIN_Init(&SPI_Slave_CLK);
            if ( spi_obj->spi_mode == SPI_SLAVE_MODE_SS_ENABLED){
                RET = GPIO_PIN_Init(&SPI_SS);
            }
            else { }
        }
        /*Configure the sample bit*/
        if ( (spi_obj->spi_mode != SPI_SLAVE_MODE_SS_ENABLED) || (spi_obj->spi_mode != SPI_SLAVE_MODE_SS_DISABLED) ){
            SPI_SAMPLE_BIT_CFG(spi_obj->master_sample_bit);
        }
        else {
            RET = E_NOT_OK;
        }
        /*Configure the clock select bit*/
        SPI_CLK_SELECT_BIT_CFG(spi_obj->clk_transmission);
        /*Configure the clock polarity*/
        SPI_CLK_POLARITY_CFG(spi_obj->clk_polarity);
        /*Configure the interrupt*/
        #if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            SPI_InterruptHandler = spi_obj->I2C_DefaultInterruptHandler;   /*save the address of the ISR to the global pointer to function*/
            SPI_INTERRUPT_ENABLE();
            SPI_INTERRUPT_CLEAR_FLAG();
        #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
            if ( spi_obj-> priority == High_Priorty ){
                INTERRUPT_GLOBAL_INTERRUPT_HIGH_ENABLE();
                SPI_HIGH_PRIORITY();
            }
            else if ( spi_obj-> priority == Low_Priorty ){
                INTERRUPT_GLOBAL_INTERRUPT_LOW_ENABLE();
                SPI_LOW_PRIORITY();
            }
            else {}
        #else
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERIPHERAL_ENABLE();
        #endif
        #endif
        /*Enable the moduele*/
        SPI_SERIAL_PINS_ENABLE_CFG();
    }
    return RET;
}

/**
 * 
 * @param spi_obj
 * @return 
 */
Std_ReturnType SPI_Deinit (const spi_config_t *spi_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == spi_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable the moduele*/
        SPI_SERIAL_PINS_DISABLE_CFG();
        /*Disable Interrupt*/
        #if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            SPI_INTERRUPT_DISABLE();
        #endif
    }
    return RET;
}

/**
 * 
 * @param spi_obj
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Read_Data (const spi_config_t *spi_obj , uint8 *_data){
    Std_ReturnType RET = E_OK;
    if ( NULL == spi_obj || NULL == _data){
        RET = E_NOT_OK;
    }
    else {
        /*Wait for Buffer flag*/
        while ( !(SSPSTATbits.BF) );
        /*Read data*/
        *_data = SSPBUF;
        
    }
    return RET;
}

/**
 * 
 * @param spi_obj
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Write_Data (const spi_config_t *spi_obj , uint8 _data ){
    Std_ReturnType RET = E_OK;
    if ( NULL == spi_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Write data to the mssp buffer*/
        SSPBUF = _data;
        /*Wait for the flag*/
        while(SSPSTATbits.BF);
        /*Clear the Flag*/
        PIR1bits.SSPIF = 0;
    }
    return RET;
}

#if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
void SPI_ISR (void){
    SPI_INTERRUPT_CLEAR_FLAG();
    if ( SPI_InterruptHandler ){
        SPI_InterruptHandler();
    }
}
#endif