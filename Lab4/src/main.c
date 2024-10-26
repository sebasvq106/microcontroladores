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
#include "usart.h"
#include "adc.h"


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
#define USART1_TX_PIN GPIO9

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
void display_data(gyro lectura, bool is_active, float bateria);
void delay(void);  
void init_sytem(void);
void send_data_usart(gyro measurement, float bateria);

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
void display_data(gyro measurement, bool is_active, float bateria) {
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

    // Bateria
    if (bateria > 7.0f) {
        gfx_setTextColor(LCD_GREEN, LCD_WHITE); // Color verde para batería > 7V
    } else {
        gfx_setTextColor(LCD_RED, LCD_WHITE); // Color rojo para batería <= 7V
    }

    sprintf(display_str, "Bateria:%.1f V", bateria);
    gfx_setCursor(20, 200);
    gfx_puts(display_str);

    // Transmision
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    if (is_active) {
        gfx_setCursor(20, 270);  
        gfx_puts("Env Datos: Si");
    } else {
        gfx_setCursor(20, 270);  
        gfx_puts("Env Datos: No ");
    }

    lcd_show_frame();
}

// Funcion delay
void delay(void) {
    volatile int count; 
    for (int i = 0; i < 900000; i++) {
        count++; 
    }
}

// Funcion de gpio setup 
static void gpio_setup(void)
{
	/* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Set GPIO13 (in GPIO port G) to 'output push-pull'. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO13);

    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

// Funcion del button setup
static void button_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

// Funcion de configuracion del uart
static void usart_setup(void)
{
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

// Funcion para enviar los datos por UART
void send_data_usart(gyro measurement, float bateria) {
    char buffer[100];
    // Formato de los datos a enviar
    sprintf(buffer, "X: %d, Y: %d, Z: %d, Bateria: %.1f V\n", measurement.x, measurement.y, measurement.z, bateria);
    for (int i = 0; buffer[i] != '\0'; i++) {
        usart_send_blocking(USART1, buffer[i]); 
    }
}

// Configuracion del ADC
static void adc_setup(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);
	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);
	adc_power_on(ADC1);

}

// Lectura del adc
static uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

// Funcion para inicializar todo el sistema
void init_sytem(void) {
    console_setup(115200);
    clock_setup();
    rcc_periph_clock_enable(RCC_ADC1);
    sdram_init();
    spi_setup();
    lcd_spi_init();
    gfx_init(lcd_draw_pixel, 240, 320);
    button_setup();
	gpio_setup();
    usart_setup();
    adc_setup();
}

// Funcion main
int main(void) {
    gyro measurement;
    init_sytem();
    int i;
    bool is_active = false;
    uint16_t input_adc;
    float bateria;

    while (1) {
        measurement = read_xyz();
        input_adc = read_adc_naiive(3);
        bateria =  (input_adc * 9.0f) / 4095.0f; 
        display_data(measurement, is_active, bateria);  
        delay();

        if (gpio_get(GPIOA, GPIO0)) {
            
            is_active = !is_active;

            for (i = 0; i < 300000; i++) {
                __asm__("nop");
            }
        }

        if (is_active) {
            send_data_usart(measurement, bateria); // Enviar mediciones

            // Control del LED: Parpadeo
            gpio_toggle(GPIOG, GPIO13); // Alternar el estado del LED
            for (i = 0; i < 3000000; i++) {		/* Wait a bit. */
			    __asm__("nop");
		    }
        } else {
            gpio_clear(GPIOG, GPIO13); // Asegúrate de que el LED esté apagado si no está activo
        } 

        if (bateria < 7.0f) {
            gpio_toggle(GPIOG, GPIO14); // Alternar el estado del LED en PG14
            for (i = 0; i < 1000000; i++) { /* Esperar un tiempo para el parpadeo */
                __asm__("nop");
            }
        } else {
            gpio_clear(GPIOG, GPIO14);
        }
    }
    return 0; 
}
