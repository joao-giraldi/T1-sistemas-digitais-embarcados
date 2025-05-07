#include "sync.h"
#include "config.h"
#include "types.h"
#include "kernel.h"
#include "scheduler.h"
#include <xc.h>

// Declara fila de aptos
extern ready_queue_t r_queue;

// API
void sem_init(sem_t *sem, int16_t value)
{
    sem->s_value            = value;
    sem->s_size             = 0;
    sem->s_pos_out          = 0;
}

void sem_wait(sem_t *sem)
{
    di();
    
    sem->s_value--;
    if (sem->s_value < 0) {
        // Bloqueia a tarefa
        sem->s_queue[sem->s_size] = r_queue.task_running;
        sem->s_size = (sem->s_size+1) % MAX_USER_TASKS;
        // For�a a preemp��o
        SAVE_CONTEXT(SEM_WAITING);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    ei();    
}

void sem_post(sem_t *sem)
{
    di();
    
    sem->s_value++;
    
    if (sem->s_value <= 0) {
        // Desbloqueia tarefa mais antiga
        r_queue.ready_queue[sem->s_queue[sem->s_pos_out]].task_state = READY;
        sem->s_pos_out = (sem->s_pos_out + 1) % MAX_USER_TASKS;        
    }
    
    ei();
}

//MUTEX
void mutex_init(mutex_t *m)
{
    m->flag = false; //Mutex começa inicialmente livre
    m->s_size = 0; //Fila de espera vazia
    m->s_pos_out = 0;
}

void mutex_lock(mutex_t *m)
{
    di();

    if (m->flag == false) { 
        //Mutex livre, pode pegar
        m->flag = true;
    } else { 
        //Mutex ocupado, bloquear a tarefa atual e enfileirar
        m->s_queue[m->s_size] = r_queue.task_running;
        m->s_size = (m->s_size + 1) % MAX_USER_TASKS;
        //r_queue.ready_queue[r_queue.task_running].task_state = MUTEX_WAITING;

        //Preempção para quando trocar a tarefa
        SAVE_CONTEXT(MUTEX_WAITING);
        scheduler();
        RESTORE_CONTEXT();

    }

    ei();
}

void mutex_unlock(mutex_t *m)
{
    di();

    
    if (m->s_size > 0) {
        // Desbloqueia a próxima tarefa esperando
        uint8_t task_id = m->s_queue[m->s_pos_out];
        r_queue.ready_queue[task_id].task_state = READY;
        m->s_pos_out = (m->s_pos_out + 1) % MAX_USER_TASKS;
        // O mutex continua ocupado - ele só será liberado quando essa nova tarefa também fizer unlock.
    } else {
        // Nenhuma esperando, libera o mutex
        m->flag = false;
    }
    
    ei();
}


