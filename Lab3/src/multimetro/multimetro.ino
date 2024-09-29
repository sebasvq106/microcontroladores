#include <PCD8544.h>  // Incluir la librería PCD8544
#include <math.h>     // Para usar funciones matemáticas como sqrt()

// Definiciones de los pines para leer tensiones y controlar componentes
static const byte VOLTAJE = 3;   // Pin donde se mide el voltaje
static const byte SWITCH = 5;    // Pin para cambiar entre AC y DC
const float R_1 = 86150;         // Resistencia R1 en ohmios
const float R_2 = 10000;         // Resistencia R2 en ohmios
const float V_ref = 5.0;         // Voltaje de referencia del Arduino

static PCD8544 lcd;  // Crear el objeto LCD para la pantalla Nokia 5110

void setup() {
  lcd.begin();           // Inicializar la pantalla LCD
  lcd.clear();           // Limpiar la pantalla para comenzar
  pinMode(SWITCH, INPUT); // Configurar el pin SWITCH como entrada
}

void loop() {
  int modo = analogRead(SWITCH);  // Leer si es AC o DC

  if (modo == LOW) {
      lecturaVoltaje(false);  // Modo DC
  } else {
      lecturaVoltaje(true);   // Modo AC
  }
}

// Implementación de la función lecturaVoltaje
void lecturaVoltaje(bool esAC) {
  float adc_value = analogRead(VOLTAJE);   // Leer valor ADC del voltaje
  float v_out = (adc_value / 1023.0) * V_ref;  // Convertir el valor ADC a voltaje
  float v_in = v_out * (R_1 + R_2) / R_2;  // Aplicar la fórmula del divisor de tensión
  float v_real = v_in - 24.0;              // Ajustar el rango del voltaje

  if (esAC) {
    v_real = v_real / sqrt(2);  // Convertir el valor de pico a RMS si es AC
  }

  // Mostrar el valor en la pantalla LCD
  lcd.clear();        // Limpiar la pantalla
  lcd.setCursor(0, 0);
  lcd.print("Voltaje: ");
  lcd.setCursor(0, 1);
  lcd.print(v_real, 2);  // Mostrar el valor con 2 decimales
  if (esAC) {
    lcd.print(" V RMS");
  } else {
    lcd.print(" V");
  }

  delay(1000);  // Esperar un segundo antes de la siguiente lectura
}
