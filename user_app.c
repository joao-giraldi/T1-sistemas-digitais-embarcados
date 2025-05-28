#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "pipe.h"
#include "config.h"
#include "io.h"
#include "types.h"

#if APP_1 == ON

TASK tarefa_1()
{
    while (1) {
        LATDbits.LD0 = ~PORTDbits.RD0;
    }    
}

TASK tarefa_2()
{
    while (1) {
        LATDbits.LD1 = ~PORTDbits.RD1;
    }    
}

TASK tarefa_3()
{
    while (1) {
        LATDbits.LD2 = ~PORTDbits.RD2;
   }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    Define as tarefas como fun��es globais para
    // evitar que o compilador as retire na fase
    // de gera��o de otimiza��o.
    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_2 == ON

sem_t s;

TASK tarefa_1()
{
    while (1) {
        LATDbits.LATD7 = ~PORTDbits.RD7;
        //sem_wait(&s);
        LATDbits.LD0 = ~PORTDbits.RD0;
        //change_state(WAITING);
        //delay(50);
        
    }    
}

TASK tarefa_2()
{
    while (1) {
        //sem_wait(&s);
        LATDbits.LD1 = ~PORTDbits.RD1;
        //change_state(WAITING);
    }    
}

TASK tarefa_3()
{
    while (1) {
        //sem_wait(&s);
        LATDbits.LD2 = ~PORTDbits.RD2;
        //change_state(WAITING);
    }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
   
    // Inicializar o sem�foro
    sem_init(&s, 0);
    
    // Define as tarefas como fun��es globais para
    // evitar que o compilador as retire na fase
    // de gera��o de otimiza��o.
    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_3 == ON

pipe_t pipe;

TASK produtor()
{
    uint8_t dados[3] = {'L', 'D', 'L'};
    uint8_t i = 0;

    while (1) {
        write_pipe(&pipe, dados[i]);
        LED1 = ~LED1;  // Pisca LED1 a cada envio para indicar produção
        i = (i + 1) % 3;
        delay(2);
    }
}

TASK consumidor()
{
    uint8_t dado_pipe;

    while (1) {
        read_pipe(&pipe, &dado_pipe);

        if (dado_pipe == 'L')
            LED2 = LED_ON;  // Liga LED2 quando lê 'L'
        else if (dado_pipe == 'D')
            LED2 = LED_OFF; // Desliga LED2 quando lê 'D'

        LED3 = ~LED3;  // Pisca LED3 indicando consumo
        delay(2);
    }
}


TASK task_controle_estabilidade(void)
{
    aciona_freio();
    r_queue.ready_queue[TID_ESTABILIDADE].task_state = WAITING;
}


void user_config()
{
    io_init();

    create_pipe(&pipe);

    create_task(1, 1, produtor);
    create_task(2, 1, consumidor);
    create_task(TID_ESTABILIDADE, 4, task_controle_estabilidade);
    r_queue.ready_queue[TID_ESTABILIDADE].task_state = WAITING;

    config_ext_int();
    asm("global _produtor, _consumidor, _task_controle_estabilidade");
}


#elif APP_4 == ON

mutex_t test_mutex;

TASK tarefa_1()
{
    while (1) {
        mutex_lock(&test_mutex);
        LED1 = LED_ON;
        delay(1);
        LED1 = LED_OFF;
        mutex_unlock(&test_mutex);
        delay(1);
    }
}

TASK tarefa_2()
{
    while (1) {
        mutex_lock(&test_mutex);
        LED2 = LED_ON;
        delay(1);
        LED2 = LED_OFF;
        mutex_unlock(&test_mutex);
        delay(1);
    }
}

void user_config()
{
    io_init();
    mutex_init(&test_mutex);

    create_task(1, 3, tarefa_1);
    create_task(2, 3, tarefa_2);

    asm("global _tarefa_1, _tarefa_2");
}

#elif APP_5 == ON

uint8_t contador_baixa = 0;
uint8_t contador_media = 0;
uint8_t contador_alta  = 0;

TASK tarefa_baixa()
{
    while (1) {
        if (contador_baixa < 3) {
            LED1 = LED_ON;
            delay(1);
            LED1 = LED_OFF;
            delay(1);
            contador_baixa++;
            change_state(READY);  // Cede o controle após cada execução
        } else {
            contador_baixa = 0;
        }
    }
}

TASK tarefa_media()
{
    while (1) {
        if (contador_media < 3) {
            LED2 = LED_ON;
            delay(1);
            LED2 = LED_OFF;
            delay(1);
            contador_media++;
            change_state(READY);  // Cede o controle após cada execução
        } else {
            contador_media = 0;
        }
    }
}

TASK tarefa_alta()
{
    while (1) {
        if (contador_alta < 3) {
            LED3 = LED_ON;
            delay(1);
            LED3 = LED_OFF;
            delay(1);
            contador_alta++;
            change_state(READY);  // Cede o controle após cada execução
        } else {
            contador_alta = 0;
        }
    }
}

void user_config()
{
    io_init();

    create_task(1, 1, tarefa_baixa);
    create_task(2, 2, tarefa_media);
    create_task(3, 3, tarefa_alta);

    // Garante que o linker não elimine as funções
    asm("global _tarefa_baixa");
    asm("global _tarefa_media");
    asm("global _tarefa_alta");
}


#elif APP_6 == ON

pipe_t pipe_instancia;
pipe_t *pipe_acelerador = &pipe_instancia;
mutex_t mutex_pwm;
uint16_t valor_adc = 0;

TASK task_acelerador(void)
{
    uint16_t pedal;
    while (1) {
        pedal = adc_read();
        write_pipe(pipe_acelerador, pedal);
        delay(1);     
    }
}

TASK task_controle_central(void)
{
    while (1) {
        mutex_lock(&mutex_pwm);
        read_pipe(pipe_acelerador, &valor_adc);
        mutex_unlock(&mutex_pwm);
        delay(1);
    }    
}

TASK task_injecao()
{
    uint16_t duty_cicle;
    while (1) {
        mutex_lock(&mutex_pwm);
        duty_cicle = (uint16_t)((uint32_t)valor_adc * 195 / 1000);
        mutex_unlock(&mutex_pwm);

        if(duty_cicle > 20) {
            duty_cicle = 200;
        } 
        else if(duty_cicle < 20) {
            duty_cicle = 0;
        }
        activate_pwm(duty_cicle);
        delay(1);
   }
}




void user_config()
{
    io_init();
    adc_config();
    pwm_config();

    // Importante: o PWM é no pino RC2
    TRISCbits.RC2 = 0;

    create_pipe(pipe_acelerador);
    mutex_init(&mutex_pwm);

    create_task(1, 3, task_acelerador);
    create_task(2, 2, task_controle_central);
    create_task(3, 4, task_injecao);

    asm("global _task_acelerador, _task_controle_central, _task_injecao");
}

#endif

