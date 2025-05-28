#ifndef IO_H
#define IO_H

#include <xc.h>
#include <stdint.h>

// Definições dos pinos dos LEDs
#define LED1 LATDbits.LATD0  // TAREFA_1
#define LED2 LATDbits.LATD1  // TAREFA_2
#define LED3 LATDbits.LATD2  // TAREFA_3

#define FREIO_LED LATDbits.LATD7

// Estados
#define LED_ON  1
#define LED_OFF 0

// API
void io_init(void);
void io_set_pin(uint8_t led, uint8_t state);

void adc_config(void);
uint16_t adc_read(void);

void pwm_config(void);
void activate_pwm(uint16_t dc);
void stop_pwm(void);

void config_interruption(void);

#endif /* IO_H */
