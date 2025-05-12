#include "io.h"

//Basicamente vou poder simplificar a definição de tarefas para "io_set_pin(1, ON);", por exemplo

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

