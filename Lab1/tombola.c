#include <pic14/pic12f675.h>

#define push_button GP5   // Botón de entrada en GP5
#define DS GP0            
#define STC GP1           
#define SHC GP2          

// Tabla de números para el display de 7 segmentos
const unsigned char segment_map[10] = {
    0x7E, // 0
    0x0C, // 1
    0xB6, // 2
    0x9E, // 3
    0xCC, // 4
    0xDA, // 5
    0xFA, // 6
    0x0E, // 7
    0xFF, // 8
    0xCE  // 9
};

// Función delay
void delay(unsigned int time) {
    while (time--) {
        for (unsigned int j = 0; j < 1275; j++);
    }
}

// Función para enviar datos a los dos 74HC595
void shiftOutTwo(unsigned char data1, unsigned char data2) {
    for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
        DS = (data1 >> i) & 0x01; // Enviar cada bit del primer número
        SHC = 1;
        delay(5);
        SHC = 0;
        delay(5);
    }

    for (unsigned char i = 7; i < 0xFF; i--) { // Usar < 0xFF para evitar problemas de comparación
        DS = (data2 >> i) & 0x01; // Enviar cada bit del segundo número
        SHC = 1;
        delay(5);
        SHC = 0;
        delay(5);
    }

    STC = 1;
    delay(5);
    STC = 0;
}

// Función para debounce del botón
unsigned char debounce(void) {
    if (push_button == 1) {
        delay(10);
        if (push_button == 1) {
            return 1;
        }
    }
    return 0;
}

void main(void) {
    TRISIO = 0b00100000; // Todos los pines como salida excepto GP5 que es entrada
    GPIO = 0x00; // Inicializar todos los pines en bajo

    unsigned char r1 = 0; // Contador para el primer número
    unsigned char r2 = 0; // Contador para el segundo número
    unsigned char speed1 = 1; // Velocidad de cambio del primer contador
    unsigned char speed2 = 3; // Velocidad de cambio del segundo contador
    unsigned char index = 0; // Contador de pulsaciones del botón

    while (1) {
        // Incrementar los contadores rápidamente mientras el botón no está presionado
        if (push_button == 0) {
            r1 = (r1 + speed1) % 10; // Incremento rápido y ciclado entre 0 y 9
            r2 = (r2 + speed2) % 10;
            delay(10); // Pequeño retardo para que los números cambien rápidamente
        }

        if (debounce()) { // Verificar el botón con debounce
        
            shiftOutTwo(segment_map[r1], segment_map[r2]); // Enviar los valores a los 74HC595
            index++;

            // Esperar hasta que el botón se libere
            while (push_button == 1);

            // Retardo para evitar múltiples conteos
            delay(100);
        }

        // Si se ha presionado el botón 10 veces, mostrar 99 tres veces y reiniciar
        if (index >= 10) {
            for (unsigned char blink = 0; blink < 3; blink++) {
                delay(250); // Pausa
                shiftOutTwo(segment_map[9], segment_map[9]); // Mostrar 99
                delay(250); // Pausa
                shiftOutTwo(0x00, 0x00); // Apagar display
            }

            index = 0; // Reiniciar el contador para los próximos números
        }
    }
}

