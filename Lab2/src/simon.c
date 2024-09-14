#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

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
uint16_t LED_DELAY = 2000;

Estado estado_actual = IDLE;

// Array que contiene los pines de los LEDs (PB3, PB4, PB5, PB6)
uint8_t led_pins[4] = { PB3, PB4, PB5, PB6 };

// Variables para los tres tipos de delays
volatile uint16_t delay_counter_blink = 0;     // Para el parpadeo
volatile uint16_t delay_counter_sequence = 0;   // Para la secuencia dinámica
volatile uint16_t delay_counter_button = 0;     // Para la espera tras presionar el botón

// Funcion que enciende un Led
void turn_on_leds(uint8_t led_mask) {
    PORTB = led_mask;
}

// Delay para el parpadeo de LEDs
void start_blink_delay(uint16_t delay_ms) {
    delay_counter_blink = delay_ms / 10;
}

uint8_t is_blink_delay_done() {
    return delay_counter_blink == 0;
}

// Delay dinámico para la secuencia de LEDs
void start_sequence_delay() {
    delay_counter_sequence = LED_DELAY / 10;
}

uint8_t is_sequence_delay_done() {
    return delay_counter_sequence == 0;
}

// Delay tras presionar un botón
void start_button_delay(uint16_t delay_ms) {
    delay_counter_button = delay_ms / 10;
}

uint8_t is_button_delay_done() {
    return delay_counter_button == 0;
}

// Configurar el Timer1 en modo CTC
void setup_timer1() {
    TCCR1B |= (1 << WGM12);          		// Configurar Timer1 en modo CTC
    TCCR1B |= (1 << CS12) | (1 << CS10); 	// Prescaler de 1024
    OCR1A = 156;                     		// Valor de comparación para generar una interrupción cada ~10ms
    TIMSK |= (1 << OCIE1A);          		// Habilitar interrupción por comparación
}

// Configurar interrupciones externas para INT0 e INT1 en flanco ascendente
void setup_external_interrupts() {
    MCUCR |= (1 << ISC00) | (1 << ISC10);  // Configurar INT0 e INT1 para detectar flanco ascendente
    GIMSK |= (1 << INT0) | (1 << INT1);    // Habilitar interrupciones INT0 e INT1
}


// ISR para manejar los contadores de los delays
ISR(TIMER1_COMPA_vect) {
    if (delay_counter_blink > 0) {
        delay_counter_blink--;
    }

    if (delay_counter_sequence > 0) {
        delay_counter_sequence--;
    }

    if (delay_counter_button > 0) {
        delay_counter_button--;
    }
}

// Mostrar la secuencia aleatoria de LEDs con delays dinámicos
void show_random_sequence() {
    if (NUM_LEDS == 4) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            uint8_t random_index = rand() % 4;
            secuencia_leds[i] = random_index;
            turn_on_leds(1 << led_pins[random_index]);

            start_sequence_delay(); 
            while (!is_sequence_delay_done());

            PORTB = 0x00; // Apagar LEDs
            start_blink_delay(1000); // Tiempo entre LEDs
            while (!is_blink_delay_done()); // Esperar el tiempo de apagado
        }
    } else {
        secuencia_leds[NUM_LEDS - 1] = rand() % 4;

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            turn_on_leds(1 << led_pins[secuencia_leds[i]]);

            start_sequence_delay(); // Iniciar delay dinámico
            while (!is_sequence_delay_done()); // Esperar a que termine el delay

            PORTB = 0x00;  // Apagar todos los LEDs
            start_blink_delay(1000); // Tiempo de espera entre LEDs
            while (!is_blink_delay_done()); // Esperar el tiempo de apagado
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

    setup_timer1(); // Configurar el Timer1
    sei(); // Habilitar interrupciones globales

    while (1) {
        switch (estado_actual) {
            case IDLE:
                // Verificar si se ha presionado cualquier botón
                if ((PIND & ((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5)))) {
                    // Parpadear todos los LEDs 2 veces
                    for (uint8_t i = 0; i < 2; i++) {
                        turn_on_leds((1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6)); // Encender todos los LEDs
                        start_blink_delay(1000); // Iniciar delay de 1 segundo
                        while (!is_blink_delay_done()); // Esperar a que el delay termine

                        PORTB = 0x00; // Apagar todos los LEDs
                        start_blink_delay(1000); // Iniciar delay de 1 segundo
                        while (!is_blink_delay_done()); // Esperar a que el delay termine
                    }
                    estado_actual = SECUENCIA;
                }
                break;

            case SECUENCIA:
                show_random_sequence();
                estado_actual = INPUT;
                break;

            case INPUT:
                while (indice_input < NUM_LEDS) {
                    if ((PIND & (1 << PD5))) {
                        secuencia_botones[indice_input] = 3;
                        start_button_delay(500); // Iniciar delay de 500 ms tras pulsar el botón
                        while (!is_button_delay_done()); // Esperar
                        indice_input++;
                    }

                    if ((PIND & (1 << PD4))) {
                        secuencia_botones[indice_input] = 2;
                        start_button_delay(500); // Iniciar delay de 500 ms tras pulsar el botón
                        while (!is_button_delay_done()); // Esperar
                        indice_input++;
                    }

                    if ((PIND & (1 << PD3))) {
                        secuencia_botones[indice_input] = 1;
                        start_button_delay(500); // Iniciar delay de 500 ms tras pulsar el botón
                        while (!is_button_delay_done()); // Esperar
                        indice_input++;
                    }

                    if ((PIND & (1 << PD2))) {
                        secuencia_botones[indice_input] = 0;
                        start_button_delay(500); // Iniciar delay de 500 ms tras pulsar el botón
                        while (!is_button_delay_done()); // Esperar
                        indice_input++;
                    }
                }

                // Comparar secuencias
                if (comparar_secuencias()) {
                    indice_input = 0;
                    NUM_LEDS++;
                    LED_DELAY -= 200; // Disminuir el delay dinámico
                    estado_actual = SECUENCIA;
                } else {
                    for (uint8_t i = 0; i < 3; i++) {
                        turn_on_leds((1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6)); // Encender todos los LEDs
                        start_blink_delay(1000); // Iniciar delay de 1 segundo
                        while (!is_blink_delay_done()); // Esperar a que el delay termine

                        PORTB = 0x00; // Apagar todos los LEDs
                        start_blink_delay(1000); // Iniciar delay de 1 segundo
                        while (!is_blink_delay_done()); // Esperar a que el delay termine
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

