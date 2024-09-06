#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> 

#define NUM_LEDS 4
#define LED_DELAY 20000 
#define BLINK_DELAY 10000 

typedef enum {
    IDLE,
    SECUENCIA,
    INPUT
} Estado;

// Variables globales para los contadores

uint8_t secuencia_leds[10];
uint8_t secuencia_botones[10];
uint8_t indice_input = 0;
Estado estado_actual = IDLE;

// Array que contiene los pines de los LEDs (PB3, PB4, PB5, PB6)
uint8_t led_pins[NUM_LEDS] = { PB3, PB4, PB5, PB6 };

void turn_on_leds(uint8_t led_mask) {
    PORTB = led_mask;
}

void show_random_sequence() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        uint8_t random_index = rand() % NUM_LEDS;
        uint8_t led = (1 << led_pins[random_index]);
        turn_on_leds(led);
        _delay_ms(LED_DELAY);
        PORTB = 0x00; // Apagar todos los LEDs
        _delay_ms(10000); // Tiempo de espera entre LEDs
    }
    
    estado_actual = INPUT;
}

uint8_t comparar_secuencias() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        if (secuencia_botones[i] != secuencia_leds[i]) {
            return 0; // Las secuencias no coinciden
        }
    }
    return 1; // Las secuencias coinciden
}

int main(void) {
    // Configurar los pines PB3, PB4, PB5, y PB6 como salidas para los LEDs
    DDRB = (1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6);

    // Configurar los pines PD2, PD3, PD4 y PD5 como entradas para los botones
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));
    // Habilitar resistencias pull-up internas
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
	
    // Estado estado_actual = IDLE;

    while (1) {
        switch (estado_actual) {
            case IDLE:
                // Verificar si se ha presionado cualquier botón
                if ((PIND & ((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5)))) {
                    // Parpadear todos los LEDs 2 veces
                    for (uint8_t i = 0; i < 2; i++) {
                        turn_on_leds((1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6)); // Encender todos los LEDs
                        _delay_ms(10000); // Esperar 10000 ms (10 segundos)
                        PORTB = 0x00; // Apagar todos los LEDs
                        _delay_ms(10000); // Esperar 10000 ms (10 segundos)
                    }
                    estado_actual = SECUENCIA;
                }
                break;

            case SECUENCIA:
                show_random_sequence();
                // estado_actual = INPUT; 
                break;
                
           case INPUT: 
           	 if (indice_input < NUM_LEDS) {
                    if ((PIND & (1 << PD5))) {
                        secuencia_botones[indice_input] = 0;
                        indice_input++;
                        _delay_ms(5000); // Evitar rebotes
                    }
                    if ((PIND & (1 << PD4))) {
                        secuencia_botones[indice_input] = 1;
                        indice_input++;
                        _delay_ms(5000); // Evitar rebotes
                    }
                    if ((PIND & (1 << PD3))) {
                        secuencia_botones[indice_input] = 2;
                        indice_input++;
                        _delay_ms(5000); // Evitar rebotes
                    }
                    if ((PIND & (1 << PD2))) {
                        secuencia_botones[indice_input] = 3;
                        indice_input++;
                        _delay_ms(5000); // Evitar rebotes
                    }
                } else {
                    if (comparar_secuencias()) {
                        // Si la secuencia es correcta, encender el LED PB4
                        PORTB |= (1 << PB5);
                    }
                    estado_actual = IDLE;
                    indice_input = 0; // Reiniciar índice de entrada
                }
                break;
        }
    }
}

