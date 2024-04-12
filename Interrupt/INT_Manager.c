#include "INT_Manager.h"

static volatile uint8 RB4_flag = 1;
static volatile uint8 RB5_flag = 1;
static volatile uint8 RB6_flag = 1;
static volatile uint8 RB7_flag = 1;

#if INTERRUPT_PRIORITY_LEVEL_ENABLE == INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh (void){
    if ( INTCONbits.INT0IE == INTERRUPT_ENABLE && INTCONbits.INT0IF == INTERRUPT_OCCUR){
        INT0_ISR();
    }
    else{}
    
    if ( (INTCON3bits.INT2IE == INTERRUPT_ENABLE) && (INTCON3bits.INT2IF == INTERRUPT_OCCUR) ){
        INT2_ISR();
    }
    else{}
}

void __interrupt(low_priority) InterruptManagerLow (void){
    if ( (INTCON3bits.INT1IE == INTERRUPT_ENABLE) && (INTCON3bits.INT1IF == INTERRUPT_OCCUR) ){
        INT1_ISR();
    }
    else{}
    
}

#else
void __interrupt() InterruptManager(void){
    if ( (INTCONbits.INT0IE == INTERRUPT_ENABLE) && (INTCONbits.INT0IF == INTERRUPT_OCCUR) ){
        INT0_ISR();
    }
    else{}
    
    if ( (INTCON3bits.INT1IE == INTERRUPT_ENABLE) && (INTCON3bits.INT1IF == INTERRUPT_OCCUR) ){
        INT1_ISR();
    }
    else{}
    
    if ( (INTCON3bits.INT2IE == INTERRUPT_ENABLE) && (INTCON3bits.INT2IF == INTERRUPT_OCCUR) ){
        INT2_ISR();
    }
    else{}
    
    /**************************On Change********************************************/
    /*ISR for changing from 0 to 5v*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4)==STD_HIGH && RB4_flag == 1){
        RB4_flag = 0;
        RB4_ISR(0);
    }
    else{}
    /*ISR for changing from 5v to 0*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4)==STD_LOW && RB4_flag == 0){
        RB4_flag = 1;
        RB4_ISR(1);
    }
    else{}

    /*ISR for changing from 0 to 5v*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5)==STD_HIGH && RB5_flag == 1){
        RB5_flag = 0;
        RB5_ISR(0);
    }
    else{}
    /*ISR for changing from 5v to 0*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5)==STD_LOW && RB5_flag == 0){
        RB5_flag = 1;
        RB5_ISR(1);
    }
    else{}
    
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6)==STD_HIGH && RB6_flag == 1){
        RB6_flag = 0;
        RB6_ISR(0);
    }
    else{}
    /*ISR for changing from 5v to 0*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6)==STD_LOW && RB6_flag == 0){
        RB6_flag = 1;
        RB6_ISR(1);
    }
    else{}

    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB7)==STD_HIGH && RB7_flag == 1){
        RB7_flag = 0;
        RB7_ISR(0);
    }
    else{}
    /*ISR for changing from 5v to 0*/
    if ( (INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB7)==STD_LOW && RB7_flag == 0){
        RB7_flag = 1;
        RB7_ISR(1);
    }
    else{}
    
    /*------------ADC Interrupt-----------------------*/
    #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.ADIE == INTERRUPT_ENABLE) && (PIR1bits.ADIF == INTERRUPT_OCCUR) ){
        ADC_ISR();
    }
    else{}
    #endif

    /*------------TIMER0 Interrupt-----------------------*/
    #if TIM0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (INTCONbits.TMR0IE == INTERRUPT_ENABLE) && (INTCONbits.TMR0IF == INTERRUPT_OCCUR) ){
        TIM0_ISR();
    }
    else{}
    #endif

    /*------------TIMER1 Interrupt-----------------------*/
    #if TIM1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.TMR1IE == INTERRUPT_ENABLE) && (PIR1bits.TMR1IF == INTERRUPT_OCCUR) ){
        TIM1_ISR();
    }
    else{}
    #endif

    /*------------TIMER2 Interrupt-----------------------*/
    #if TIM2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.TMR2IE == INTERRUPT_ENABLE) && (PIR1bits.TMR2IF == INTERRUPT_OCCUR) ){
        TIM2_ISR();
    }
    else{}
    #endif

    /*------------TIMER3 Interrupt-----------------------*/
    #if TIM3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE2bits.TMR3IE == INTERRUPT_ENABLE) && (PIR2bits.TMR3IF == INTERRUPT_OCCUR) ){
        TIM3_ISR();
    }
    else{}
    #endif

    /*------------CCP Interrupt-----------------------*/
    #if CCP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.CCP1IE == INTERRUPT_ENABLE) && (PIR1bits.CCP1IF == INTERRUPT_OCCUR) ){
        CCP1_ISR();
    }
    else if ( (PIE2bits.CCP2IE == INTERRUPT_ENABLE) && (PIR2bits.CCP2IF == INTERRUPT_OCCUR) ){
        CCP2_ISR();
    }
    else{}
    #endif

    /*------------USART Interrupt-----------------------*/
    #if USART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.TXIE == INTERRUPT_ENABLE) && (PIR1bits.TXIF == INTERRUPT_OCCUR) ){
        UART_TX_ISR();
    }
    else{}
    #endif

    #if USART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.RCIE == INTERRUPT_ENABLE) && (PIR1bits.RCIF == INTERRUPT_OCCUR) ){
        UART_RX_ISR();
    }
    else{}
    #endif

        /*------------I2C Interrupt-----------------------*/
    #if I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR) ){
        I2C_ISR();
    }
    else{}
    
    if ( (PIE2bits.BCLIE == INTERRUPT_ENABLE) && (PIR2bits.BCLIF == INTERRUPT_OCCUR) ){
        I2C_BC_ISR();
    }
    else{}
    #endif

        /*------------SPI Interrupt-----------------------*/
    #if SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ( (PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR) ){
        SPI_ISR();
    }
    else{}
    #endif

}

#endif