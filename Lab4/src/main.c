#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "rcc.h"
#include "spi.h"
#include "gfx.h"
#include "gpio.h"


#define REG_WHO_AM_I		0x0F   // Registro identificador del giroscopio 
#define REG_RNW			(1 << 7)   // Escribe sobre un cero

// Registros de control
#define REG_CTRL_REG1		0x20    // Registro de control 1
#define REG_CTRL_REG2       0x21    // Registro de control 2
#define REG_CTRL_REG4		0x23    // Registro de control 4

// Registros del giroscopio
#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29
#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B
#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D

#define L3GD20_SENSIBILIDAD  (0.004375F)  // Sensibilidad del giroscopio

// Estructura para almacenar las lecturas de los ejes X, Y, y Z del giroscopio
typedef struct Gyro {
  int16_t x;
  int16_t y;
  int16_t z;
} gyro;

// Declaracion de funciones
uint8_t spi_send_data(uint16_t reg, uint16_t val);
int16_t read_axis(uint8_t lsb_command, uint8_t msb_command);
gyro read_xyz(void); 
void display_data(gyro lectura);
void delay(void);  
void init_sytem(void);

// Funcion para comunicarse por SPI
uint8_t spi_send_data(uint16_t reg, uint16_t val) {
    uint8_t result;
    gpio_clear(GPIOC, GPIO1);           // Bajar CS (Chip Select) para comenzar transaccion
    spi_send(SPI5, reg);                // Enviar registro al giroscopio
    spi_read(SPI5);                     // Leer respuesta del giroscopio
    spi_send(SPI5, val);                // Enviar valor que se desea escribir
    result = spi_read(SPI5);            // Leer respuesta del giroscopio
    gpio_set(GPIOC, GPIO1);             // Subir CS para finalizar transaccion
    return result; 
}

// Funcion para configurar el modulo SPI
static void spi_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOC | RCC_GPIOF); 

    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN,
			GPIO7 | GPIO8 | GPIO9);
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);
	gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ,
				GPIO7 | GPIO9);

	/* Chip select line */
	gpio_set(GPIOC, GPIO1);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

    rcc_periph_clock_enable(RCC_SPI5); 

    // Configuracion del Master
    spi_set_master_mode(SPI5);   
    spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);  
    spi_set_clock_polarity_0(SPI5);   
    spi_set_clock_phase_0(SPI5);      
    spi_set_full_duplex_mode(SPI5);   
    spi_set_unidirectional_mode(SPI5); 
    spi_enable_software_slave_management(SPI5); 
    spi_send_msb_first(SPI5);   
    spi_set_nss_high(SPI5);    
    spi_enable(SPI5);   

    // Configuracion del Slave
    spi_send_data(REG_CTRL_REG1, 0x3F);       // Habilitar PD, ZEN, YEN, XEN, y configurar el ancho de banda
    spi_send_data(REG_CTRL_REG4, (1 << 4));   // 500 dps
}

// Funcion para leer un eje del giroscopio
int16_t read_axis(uint8_t lsb_reg, uint8_t msb_reg) {
    int16_t result = (spi_send_data(lsb_reg, 0) | (spi_send_data(msb_reg, 0) << 8));
    return result;                                     
}

// Funcion para obtener la lectura completa
gyro read_xyz(void) {
    gyro measurement;

    spi_send_data(REG_WHO_AM_I | 0x80, 0);                 

    // Lee y escala los valores de los ejes
    measurement.x = read_axis(GYR_OUT_X_L | REG_RNW, GYR_OUT_X_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.y = read_axis(GYR_OUT_Y_L | REG_RNW, GYR_OUT_Y_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.z = read_axis(GYR_OUT_Z_L | REG_RNW, GYR_OUT_Z_H | REG_RNW) * L3GD20_SENSIBILIDAD;

    return measurement; 
}


// Funcion para mostrar los datos en la pantalla LCD
void display_data(gyro measurement) {
    char display_str[50];
    
    // Limpiar la pantalla y configurar el texto
    gfx_fillScreen(LCD_WHITE);
    gfx_setTextSize(2);


    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    gfx_setCursor(23, 10);
    gfx_puts(" Sismografo ");		

    // Eje X 
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    sprintf(display_str, "Eje X: %d", measurement.x);
    gfx_setCursor(20, 50);
    gfx_puts(display_str);

    // Eje Y 
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    sprintf(display_str, "Eje Y: %d", measurement.y);
    gfx_setCursor(20, 90);
    gfx_puts(display_str);

    // Eje Z 
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    sprintf(display_str, "Eje Z: %d", measurement.z);
    gfx_setCursor(20, 130);
    gfx_puts(display_str);

    lcd_show_frame();
}

// Funcion delay
void delay(void) {
    volatile int count; 
    for (int i = 0; i < 900000; i++) {
        count++; 
    }
}

// Funcion para inicializar todo el sistema
void init_sytem(void) {
    console_setup(115200);
    clock_setup();
    sdram_init();
    spi_setup();
    lcd_spi_init();
    gfx_init(lcd_draw_pixel, 240, 320);
}

// Funcion main
int main(void) {
    gyro measurement;
    init_sytem();

    while (1) {
        measurement = read_xyz();  
        display_data(measurement);  
        delay(); 
    }
    return 0;  
}