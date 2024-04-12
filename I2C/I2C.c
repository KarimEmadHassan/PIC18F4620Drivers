#include "I2C.h"

static void I2C_Interrupt_Configuration (const i2c_config_t *i2c_obj);

#if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
static void (*I2C_InterruptHandler) (void) = NULL;
static void (*I2C_BusCollision_InterruptHandler) (void) = NULL;
static void (*I2C_ReportReceiveOverflow_InterruptHandler) (void) = NULL;
#endif


/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Init (const i2c_config_t *i2c_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable I2C*/
        MSSP_MODULE_DISABLE_CFG();
        /*Configure I2C mode*/
        TRISCbits.TRISC3 = 1;     /*Set SCL as input*/
        TRISCbits.TRISC4 = 1;     /*Set SCL as output*/
        if ( i2c_obj->i2c_mode == I2C_MASTER_MODE ){
            SSPCON1bits.SSPM = i2c_obj->mssp_mode_select;
            SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c_obj->i2c_master_clock) - 1);
        }
        else if ( i2c_obj->i2c_mode == I2C_SLAVE_MODE ){
            /*General Call Configuration*/
            if ( i2c_obj->i2c_general_call == I2C_GENERAL_CALL_ENABLE ){
                I2C_GENERAL_CALL_ENABLE_CFG();
            }
            else if ( i2c_obj->i2c_general_call == I2C_GENERAL_CALL_DISABLE ){
                I2C_GENERAL_CALL_DISABLE_CFG();
            }
            else { }
            /* Clear the Write Collision Detect */
            SSPCON1bits.WCOL = 0; /* No Collision */
            /* Clear the Receive Overflow Indicator */
            SSPCON1bits.SSPOV = 0; /* No Overflow */
            /* Release the clock */
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave Address */
            SSPADD = i2c_obj->i2c_slave_address;
            /* I2C Slave Mode Configurations */
            SSPCON1bits.SSPM = i2c_obj->mssp_mode_select;
        }    
        else { }
        /*Configure slew rete control*/
        if ( i2c_obj->i2c_slew_rate == I2C_SLEW_RATE_ENABLE ){
            I2C_SLEW_RATE_ENABLE_CFG();
        }
        else if ( i2c_obj->i2c_slew_rate == I2C_SLEW_RATE_DISABLE ){
            I2C_SLEW_RATE_DISABLE_CFG();
        }    
        else { }
        /*Configure SMbus*/
        if ( i2c_obj->i2c_SMbus == I2C_SMBUS_ENABLE ){
            I2C_SMBus_ENABLE_CFG();
        }
        else if ( i2c_obj->i2c_SMbus == I2C_SMBUS_DISABLE ){
            I2C_SMBus_DISABLE_CFG();
        }    
        else { }
        /*Interrupt Config*/
        #if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            I2C_Interrupt_Configuration (i2c_obj);
        #endif
        /*Enable I2C*/
        MSSP_MODULE_ENABLE_CFG();
    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Deinit (const i2c_config_t *i2c_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Disable I2C*/
        MSSP_MODULE_DISABLE_CFG();
        /*Disable Interrupt*/
        #if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            I2C_INTERRUPT_DISABLE();
            I2C_BUS_COLLISION_INTERRUPT_DISABLE();
        #endif
    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Send_Start (const i2c_config_t *i2c_obj){
        Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.*/
        SSPCON2bits.SEN = 1;
        /*Wait until it's cleared*/
        while ( SSPCON2bits.SEN );
        /*Clear the flag*/
        PIR1bits.SSPIF = 0;
        /*Check if the start condition has been detected*/
        if ( I2C_START_BIT_DETECTED == SSPSTATbits.S ){
            RET = E_OK;
        }
        else if ( I2C_START_BIT_NOT_DETECTED == SSPSTATbits.S ){
            RET = E_NOT_OK;
        }
        else { }
    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Send_Repeated_Start (const i2c_config_t *i2c_obj){
    Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware.*/
        SSPCON2bits.RSEN = 1;
        /*Wait until it's cleared*/
        while ( SSPCON2bits.RSEN );
        /*Clear the flag*/
        PIR1bits.SSPIF = 0;
    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Send_Stop (const i2c_config_t *i2c_obj){
        Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj ){
        RET = E_NOT_OK;
    }
    else {
        /*Initiate Stop condition on SDA and SCL pins. Automatically cleared by hardware.*/
        SSPCON2bits.PEN = 1;
        /*Wait until it's cleared*/
        while ( SSPCON2bits.PEN );
        /*Clear the flag*/
        PIR1bits.SSPIF = 0;
        /*Check if the start condition has been detected*/
        if ( I2C_STOP_BIT_DETECTED == SSPSTATbits.P ){
            RET = E_OK;
        }
        else if ( I2C_STOP_BIT_DETECTED == SSPSTATbits.P ){
            RET = E_NOT_OK;
        }
        else { }

    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @param Ack
 * @param _data
 * @return 
 */
Std_ReturnType I2C_Read_Data (const i2c_config_t *i2c_obj , uint8 Ack , uint8 *_data){
        Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj || NULL == _data){
        RET = E_NOT_OK;
    }
    else {
        /*Enable master Receiver*/
        I2C_MASTER_RECEIVE_ENABLE_CFG();
        /*Wait for Buffer flag*/
        while ( !(SSPSTATbits.BF) );
        /*Read data*/
        *_data = SSPBUF;
        /*Configure the ACK*/
        if ( Ack == I2C_MASTER_SEND_ACK ){
            SSPCON2bits.ACKDT = 0; /* Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1; 
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
        else if ( Ack == I2C_MASTER_SEND_NACK ){
            SSPCON2bits.ACKDT = 1; /* Not Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1;
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
        else {  }
        SSPCON2bits.ACKEN = 1;
    }
    return RET;
}

/**
 * 
 * @param i2c_obj
 * @param _data
 * @return 
 */
Std_ReturnType I2C_Write_Data (const i2c_config_t *i2c_obj , uint8 _data , uint8 *_ack){
        Std_ReturnType RET = E_OK;
    if ( NULL == i2c_obj || NULL == _ack ){
        RET = E_NOT_OK;
    }
    else {
        /*Write data to the mssp buffer*/
        SSPBUF = _data;
        /*Wait for the flag*/
        while(SSPSTATbits.BF);
        /*Clear the Flag*/
        PIR1bits.SSPIF = 0;
        /*Check the ACK bit*/
        if (I2C_ACK_RESEVED_FROM_SALVE == SSPCON2bits.ACKSTAT ){
            *_ack = I2C_ACK_RESEVED_FROM_SALVE;
        }
        else if (I2C_ACK_NOT_RESEVED_FROM_SALVE == SSPCON2bits.ACKSTAT ){
            *_ack = I2C_ACK_NOT_RESEVED_FROM_SALVE;
        }
        else {  }
    }
    return RET;
}   

#if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
void I2C_ISR (void){
    I2C_INTERRUPT_CLEAR_FLAG();
    if ( I2C_InterruptHandler ){
        I2C_InterruptHandler();
    }
}

void I2C_BC_ISR (void){
    I2C_BUS_COLLISION_CLEAR_FLAG();
    if ( I2C_BusCollision_InterruptHandler ){
        I2C_BusCollision_InterruptHandler();
    }

}
#endif

/**
 * 
 * @param i2c_obj
 */
#if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void I2C_Interrupt_Configuration (const i2c_config_t *i2c_obj){
    I2C_INTERRUPT_ENABLE();
    I2C_BUS_COLLISION_INTERRUPT_ENABLE();
    I2C_INTERRUPT_CLEAR_FLAG();
    I2C_BUS_COLLISION_CLEAR_FLAG();
    I2C_InterruptHandler = i2c_obj->I2C_DefaultInterruptHandler;
    I2C_BusCollision_InterruptHandler = i2c_obj->I2C_Report_Bus_Collision;
    I2C_ReportReceiveOverflow_InterruptHandler = i2c_obj->I2C_Report_Receiver_Overflow;
    #if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PRIORITY_FEATURE_ENABLE();
        if ( i2c_obj->i2c_priority == High_Priorty ){
            I2C_HIGH_PRIORITY();
        } 
        else if (i2c_obj->i2c_priority == Low_Priorty) {
            I2C_LOW_PRIORITY();
        } 
        else {}
        if (i2c_obj->bus_collision_priority == High_Priorty) {
            I2C_BUS_COLLISION_HIGH_PRIORITY();
        } 
        else if (i2c_obj->bus_collision_priority == Low_Priorty) {
            I2C_BUS_COLLISION_LOW_PRIORITY();
        }
        else {}
    #else
        INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
        INTERRUPT_PERIPHERAL_ENABLE();
    #endif
}
#endif