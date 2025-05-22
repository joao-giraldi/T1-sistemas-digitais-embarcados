#include "io.h"
#include "kernel.h"

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

void aciona_freio(void)
{
    FREIO_LED = LED_ON;
    delay(5); // Simula frenagem
    FREIO_LED = LED_OFF;
}

void config_ext_int(void)
{
    TRISBbits.TRISB0 = 1;          // RB0 como entrada
    INTCONbits.INT0IF = 0;         // Limpa flag
    INTCONbits.INT0IE = 1;         // Habilita INT0
    INTCON2bits.INTEDG0 = 0;       // Interrupção na borda de descida
    RCONbits.IPEN = 1;             // Prioridade de interrupções
    INTCONbits.GIEH = 1;           // Interrupção global alta prioridade
}

void config_adc(void)
{
    TRISAbits.TRISA0 = 1;            // RA0 como entrada (AN0)
    ADCON1 = 0x0E;                   // RA0 analógico, os demais digitais
    ADCON2 = 0xA9;                   // Tacq + right justify
    ADCON0 = 0x01;                   // Canal 0 (AN0), ADC ligado
}

uint16_t read_adc(void)
{
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO);          // Espera terminar a conversão
    return ((ADRESH << 8) | ADRESL); // 10 bits
}

void config_pwm(void)
{
    TRISCbits.TRISC2 = 0;           // RC2 como saída (CCP1)
    CCP1CON = 0b00001100;           // Modo PWM
    T2CON = 0b00000100;             // Timer2 ligado, prescaler 1:1
    PR2 = 255;                      // Período do PWM
    CCPR1L = 0;                     // Duty cycle inicial
}

void set_pwm_duty(uint8_t duty)
{
    if (duty > 255) duty = 255;
    CCPR1L = duty;
}

