#include "io.h"
#include <stdint.h>

void io_init(void)
{
    // Configura como saída digital
    TRISDbits.TRISD0 = 0;  // LED1 (TAREFA_1)
    TRISDbits.TRISD1 = 0;  // LED2 (TAREFA_2)
    TRISDbits.TRISD2 = 0;  // LED3 (TAREFA_3)

    // Inicializa LEDs como desligados
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
}

void io_set_pin(uint8_t led, uint8_t state)
{
    switch (led)
    {
        case 1:
            LED1 = state;
            break;
        case 2:
            LED2 = state;
            break;
        case 3:
            LED3 = state;
            break;
        default:
            break;
    }
}

uint16_t adc_read(void) {
    ADCON0bits.GO = 1;
    while(ADCON0bits.GODONE);
    return ADRES;
}


void adc_config(void) {
    ADCON0bits.CHS = 0b0000;
    ADCON1bits.VCFG = 0b00;
    ADCON1bits.PCFG = 0b1110;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b101;
    ADCON2bits.ADCS = 0b101;
    ADCON0bits.ADON = 1;
}


void pwm_config(void) {
    TRISCbits.RC2 = 0;
    PR2 = 199;
    CCPR1L = 100;
    T2CONbits.T2CKPS = 0b00;
    CCP1CONbits.DC1B = 0b00;
    CCP1CONbits.CCP1M = 0b1100;
}


void activate_pwm(uint16_t dc) {
    if(!T2CONbits.TMR2ON) {
        TMR2 = 0;
        T2CONbits.TMR2ON = 1;
    }
    uint16_t pwm_value = ((uint32_t)(PR2 + 1) * dc) / 50;
    CCPR1L = (uint8_t)(pwm_value >> 2);
    CCP1CONbits.DC1B = pwm_value & 0x03;
}


void stop_pwm() {
    CCP1CONbits.CCP1M = 0b0000;
    T2CONbits.TMR2ON = 0;             
    TMR2 = 0;                         
    TRISCbits.TRISC2 = 0;
}

void config_interruption(void) {
    TRISBbits.TRISB0 = 1;             
    INTCON2bits.INTEDG0 = 1;
    INTCONbits.INT0F = 0;
    INTCONbits.INT0IE = 1;            
    INTCONbits.GIE = 1;               
    INTCONbits.PEIE = 1;
}

