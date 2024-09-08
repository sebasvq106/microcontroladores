#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> 


#define BLINK_DELAY 10000 

typedef enum {
    IDLE,
    SECUENCIA,
    INPUT
} Estado;

// Variables globales para los contadores

uint8_t secuencia_leds[14];
uint8_t secuencia_botones[14];
uint8_t indice_input = 0;
uint8_t NUM_LEDS = 4;
uint16_t LED_DELAY = 20000;
Estado estado_actual = IDLE;

// Array que contiene los pines de los LEDs (PB3, PB4, PB5, PB6)
uint8_t led_pins[4] = { PB3, PB4, PB5, PB6 };

void turn_on_leds(uint8_t led_mask) {
    PORTB = led_mask;
}

void delay_ms_variable(uint16_t delay) {
    while (delay--) {
        _delay_ms(1);  // Esperar 1 ms en cada iteración
    }
}

void show_random_sequence() {

    if (NUM_LEDS == 4){
	    for (uint8_t i = 0; i < NUM_LEDS; i++) {
		uint8_t random_index = rand() % 4;
		uint8_t led = (1 << led_pins[random_index]);
		secuencia_leds[i] = random_index;
		turn_on_leds(led);
		delay_ms_variable(LED_DELAY);
		PORTB = 0x00; // Apagar todos los LEDs
		_delay_ms(10000); // Tiempo de espera entre LEDs
	    }
    } else {
    
    uint8_t random_index = rand() % 4;
    secuencia_leds[NUM_LEDS - 1] = random_index;
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        uint8_t led = (1 << led_pins[secuencia_leds[i]]);
        turn_on_leds(led);
        delay_ms_variable(LED_DELAY);
        PORTB = 0x00;  // Apagar todos los LEDs
        _delay_ms(10000);  // Tiempo de espera entre LEDs
    }
   }
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
                estado_actual = INPUT; 
                break;
                
           case INPUT: 
           	while(indice_input < NUM_LEDS) {
           		if ((PIND & (1 << PD5))) {
           		secuencia_botones[indice_input] = 3;
           		_delay_ms(5000);
           		indice_input++;
           		}
           		
           		if ((PIND & (1 << PD4))){
           		secuencia_botones[indice_input] = 2;
           		_delay_ms(5000);
           		indice_input++;
           		}
           		
           		if ((PIND & (1 << PD3))){
           		secuencia_botones[indice_input] = 1;
           		_delay_ms(5000);
           		indice_input++;
           		}
           		
           		if ((PIND & (1 << PD2))){
           		secuencia_botones[indice_input] = 0;
           		_delay_ms(5000);
           		indice_input++;
           		}
           		estado_actual = INPUT;
           	}
           	
           	if (comparar_secuencias()){
           		indice_input = 0;
           		NUM_LEDS++;
           		LED_DELAY = LED_DELAY - 2000;
           		estado_actual = SECUENCIA;
           	} else {
           	
           		for (uint8_t i = 0; i < 3; i++) {
                        turn_on_leds((1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6)); // Encender todos los LEDs
                        _delay_ms(10000); // Esperar 10000 ms (10 segundos)
                        PORTB = 0x00; // Apagar todos los LEDs
                        _delay_ms(10000); // Esperar 10000 ms (10 segundos)
                    	}
                    	indice_input = 0;
                    	NUM_LEDS = 4;
                    	LED_DELAY = 20000;
		   	estado_actual = IDLE;
           	}
                break;
        }
    }
}

