#include <xc.h>
#include "timer.h"
#include "kernel.h"
#include "scheduler.h"
#include "io.h"
#include <stdbool.h>
#include "user_app.h"


void config_timer0()
{
    // Habilita interrup��es de perif�ricos
    INTCONbits.PEIE     = 1;
    // Habilita interrup��o do timer 0
    INTCONbits.TMR0IE   = 1;
    // Seta o flag do timer em zero
    INTCONbits.TMR0IF   = 0;
    // Transi��o do timer por refer�ncia interna
    T0CONbits.T0CS      = 0;
    // Ativa preescaler para o timer zero
    T0CONbits.PSA       = 0;
    // Preesclaer 1:64
    T0CONbits.T0PS      = 0b101;
    // Valor inicial do timer
    TMR0 = 0;
}

void start_timer0()
{
    T0CONbits.TMR0ON = 1;
}

// Tratador de interrup��o do timer
void __interrupt(high_priority) isr(void)
{
    // Tratamento do Timer0
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        decrease_time();
    }

    // Tratamento da interrupção externa INT0 (RB0)
    if (INTCONbits.INT0IF) {
        static bool estabilidade_criada = false;

        if (!estabilidade_criada) {
            create_task(TID_ESTABILIDADE, 255, task_controle_estabilidade); // Prioridade 255 = máxima
            estabilidade_criada = true;
        }

        INTCONbits.INT0IF = 0;
    }
}