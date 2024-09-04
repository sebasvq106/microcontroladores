#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Configurar los pines B3, B4, B5, y B6 como salidas
    DDRB = (1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6);
    
    // Configurar los pines D2, D3, D4, y D5 como entradas
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));

    // Activar resistencias pull-up para los botones
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);

    while (1) {
        // Botón 1 (D5) enciende LED B6
        if (PIND & (1 << PD5)) {
            PORTB |= (1 << PB6);   // Encender LED B6
        } else {
            PORTB &= ~(1 << PB6);  // Apagar LED B6
        }

        // Botón 2 (D4) enciende LED B5
        if (PIND & (1 << PD4)) {
            PORTB |= (1 << PB5);   // Encender LED B5
        } else {
            PORTB &= ~(1 << PB5);  // Apagar LED B5
        }

        // Botón 3 (D3) enciende LED B4
        if (PIND & (1 << PD3)) {
            PORTB |= (1 << PB4);   // Encender LED B4
        } else {
            PORTB &= ~(1 << PB4);  // Apagar LED B4
        }

        // Botón 4 (D2) enciende LED B3
        if (PIND & (1 << PD2)) {
            PORTB |= (1 << PB3);   // Encender LED B3
        } else {
            PORTB &= ~(1 << PB3);  // Apagar LED B3
        }
    }
}

