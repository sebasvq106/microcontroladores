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
#include "Modelos/edge-impulse-sdk/classifier/ei_run_classifier.h"

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
#define USART1_TX_PIN GPIO9

typedef struct Gyro {
  float x;
  float y;
  float z;
} gyro;

uint8_t spi_send_data(uint16_t reg, uint16_t val);
int16_t read_axis(uint8_t lsb_command, uint8_t msb_command);
gyro read_xyz(void); 
void display_data(gyro lectura, bool is_active);
void delay(void);  
void init_sytem(void);
void send_data_usart(gyro measurement);
void run_inference(void);

float input_data[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

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

static void spi_setup(void) {
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

int16_t read_axis(uint8_t lsb_reg, uint8_t msb_reg) {
    int16_t result = (spi_send_data(lsb_reg, 0) | (spi_send_data(msb_reg, 0) << 8));
    return result;                                     
}

gyro read_xyz(void) {
    gyro measurement;
    measurement.x = read_axis(GYR_OUT_X_L | REG_RNW, GYR_OUT_X_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.y = read_axis(GYR_OUT_Y_L | REG_RNW, GYR_OUT_Y_H | REG_RNW) * L3GD20_SENSIBILIDAD;
    measurement.z = read_axis(GYR_OUT_Z_L | REG_RNW, GYR_OUT_Z_H | REG_RNW) * L3GD20_SENSIBILIDAD;

    input_data[0] = measurement.x;
    input_data[1] = measurement.y;
    input_data[2] = measurement.z;

    return measurement;
}

void display_data(gyro measurement, bool is_active) {
    char display_str[50];
    gfx_fillScreen(LCD_WHITE);
    gfx_setTextSize(2);
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    gfx_setCursor(23, 10);
    gfx_puts(" Sismografo ");

    sprintf(display_str, "Eje X: %f", measurement.x);
    gfx_setCursor(20, 50);
    gfx_puts(display_str);

    sprintf(display_str, "Eje Y: %f", measurement.y);
    gfx_setCursor(20, 90);
    gfx_puts(display_str);

    sprintf(display_str, "Eje Z: %f", measurement.z);
    gfx_setCursor(20, 130);
    gfx_puts(display_str);

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

void delay(void) {
    volatile int count; 
    for (int i = 0; i < 900000; i++) {
        count++; 
    }
}

static void gpio_setup(void) {
	rcc_periph_clock_enable(RCC_GPIOG);
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

static void button_setup(void) {
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

static void usart_setup(void) {
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);
}

void send_data_usart(gyro measurement) {
    char buffer[100];
    sprintf(buffer, "X: %f, Y: %f, Z: %f \n", measurement.x, measurement.y, measurement.z);
    for (int i = 0; buffer[i] != '\0'; i++) {
        usart_send_blocking(USART1, buffer[i]); 
    }
}

void run_inference(void) {
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    signal.get_data = &get_signal_data;

    ei_impulse_result_t result;
    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        printf("Error ejecutando el modelo (%d)\n", res);
        return;
    }

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        char buffer[50];
        sprintf(buffer, "%s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
        for (int j = 0; buffer[j] != '\0'; j++) {
            usart_send_blocking(USART1, buffer[j]);
        }
    }
}

int get_signal_data(size_t offset, size_t length, float *out_ptr) {
    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = input_data[offset + i];
    }
    return 0;
}

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
}

int main(void) {
    gyro measurement;
    init_sytem();
    bool is_active = false;
    int i;

    while (1) {
        measurement = read_xyz();
        display_data(measurement, is_active);
        delay();

        if (gpio_get(GPIOA, GPIO0)) {
            is_active = !is_active;
            for (i = 0; i < 300000; i++) {
                __asm__("nop");
            }
        }

        if (is_active) {
            send_data_usart(measurement);
            run_inference();

            gpio_toggle(GPIOG, GPIO13);
            for (i = 0; i < 3000000; i++) {
                __asm__("nop");
            }
        } else {
            gpio_clear(GPIOG, GPIO13);
        }
    }
    return 0; 
}
