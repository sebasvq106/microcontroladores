#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "rcc.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
#include "main_functions.h"



// Definiciones del giroscopio
#define REG_WHO_AM_I        0x0F
#define REG_RNW             (1 << 7)
#define REG_CTRL_REG1       0x20
#define REG_CTRL_REG4       0x23
#define GYR_OUT_X_L         0x28
#define GYR_OUT_X_H         0x29
#define GYR_OUT_Y_L         0x2A
#define GYR_OUT_Y_H         0x2B
#define GYR_OUT_Z_L         0x2C
#define GYR_OUT_Z_H         0x2D
#define L3GD20_SENSIBILIDAD (0.004375F)


// Estructura para almacenar datos del giroscopio
typedef struct Gyro {
    float x;
    float y;
    float z;
} gyro;

// Funciones para el giroscopio
uint8_t spi_send_data(uint16_t reg, uint16_t val);
int16_t read_axis(uint8_t lsb_command, uint8_t msb_command);
gyro read_xyz(void);
void init_sytem(void);
void spi_setup();

// Funcion para comunicarse por SPI
uint8_t spi_send_data(uint16_t reg, uint16_t val) {
    uint8_t result;
    gpio_clear(GPIOC, GPIO1);
    spi_send(SPI5, reg);
    spi_read(SPI5);
    spi_send(SPI5, val);
    result = spi_read(SPI5);
    gpio_set(GPIOC, GPIO1);
    return result;
}

// Configuración SPI para el giroscopio
void spi_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC | RCC_GPIOF);
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO7 | GPIO8 | GPIO9);
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);
    gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO7 | GPIO9);

    gpio_set(GPIOC, GPIO1);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

    rcc_periph_clock_enable(RCC_SPI5);
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

    spi_send_data(REG_CTRL_REG1, 0x3F);
    spi_send_data(REG_CTRL_REG4, (1 << 4));
}

// Funcion para leer un eje del giroscopio
int16_t read_axis(uint8_t lsb_reg, uint8_t msb_reg) {
    return (spi_send_data(lsb_reg, 0) | (spi_send_data(msb_reg, 0) << 8));
}

// Lectura completa
gyro read_xyz(void) {
    gyro measurement;
    measurement.x = read_axis(GYR_OUT_X_L | REG_RNW, GYR_OUT_X_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.y = read_axis(GYR_OUT_Y_L | REG_RNW, GYR_OUT_Y_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.z = read_axis(GYR_OUT_Z_L | REG_RNW, GYR_OUT_Z_H | REG_RNW) * L3GD20_SENSIBILIDAD;

    return measurement;
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


// Configuración del sistema
void init_system(void) {
    console_setup(115200);
    clock_setup();
    rcc_periph_clock_enable(RCC_ADC1);
    sdram_init();
    spi_setup();
    usart_setup();
    tensor_setup();
}


// Main
int main(void) {
    init_system();
    int numSamples = 9;
    int samplesRead = 0;
    gyro measurement;

    while (1) {
       measurement = read_xyz();
       detectar(samplesRead,measurement.x,measurement.y,measurement.z);
       if (samplesRead == numSamples){
       	  samplesRead = 0;
       }
       samplesRead++;
    }

    return 0;
}
