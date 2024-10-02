#include <PCD8544.h>  
#include <math.h>     

// Definiciones de los pines 
static const byte VOLTAJES[] = {3, 2, 1, 0};
static const byte SWITCH = 5;    // Pin para cambiar entre AC y DC
const float R_1 = 86150;         // Resistencia R1 en ohmios
const float R_2 = 10000;         // Resistencia R2 en ohmios
const float V_ref = 5.0;         // Voltaje de referencia del Arduino
static const byte ledPin = 8;    // Pin del LED
static const byte SERIAL_PIN = 9; // Switch para comunicacion serial

static PCD8544 lcd;            

void setup() {
  lcd.begin();                // Inicializar la pantalla LCD
  lcd.clear();                // Limpiar la pantalla para comenzar
  pinMode(SWITCH, INPUT);     // Configurar el pin SWITCH como entrada
  pinMode(ledPin, OUTPUT);     // Configurar el pin ledPin como salida
  pinMode(SERIAL_PIN, INPUT);  // Configurar el pin SERIAL como entrada
  Serial.begin(9600);         // Inicializar la comunicacion serial
}

void loop() {
  int modo = analogRead(SWITCH);            // Leer si es AC o DC
  int sendSerial = digitalRead(SERIAL_PIN);  // Leer si SERIAL está activado

  if (modo == LOW) {
      lecturaVoltaje(false, sendSerial);  // Modo DC
  } else {
      lecturaVoltaje(true, sendSerial);   // Modo AC
  }
}

// Implementación de la función lecturaVoltaje
void lecturaVoltaje(bool esAC, bool sendSerial) {
  lcd.clear();           // Limpiar la pantalla
  lcd.setCursor(0, 0);
  lcd.print(esAC ? "Modo: AC" : "Modo: DC"); // Mostrar AC o DC
  bool ledOn = false;

  for (int i = 0; i < 4; i++) {

    float adc_value = analogRead(VOLTAJES[i]);      // Leer valor ADC del voltaje
    float v_out = (adc_value / 1023.0) * V_ref;     // Convertir el valor ADC a voltaje
    float v_in = v_out * (R_1 + R_2) / R_2;         // Aplicar la fórmula del divisor de tensión
    float v_real = v_in - 24.0;                     // Ajustar el rango del voltaje
    if (esAC) {
      v_real = v_real / sqrt(2);                    // Convertir el valor de pico a RMS si es AC
    }

    // Mostrar los valores en la pantalla LCD
    lcd.setCursor(0, i + 1);
    lcd.print("V");
    lcd.print(i + 1);
    lcd.print(": ");
    lcd.print(v_real, 2);  
    lcd.print(esAC ? " V RMS" : " V");

    // Verificar la alarma
    if ((esAC && abs(v_real) >= 14.1421) || (!esAC && abs(v_real) >= 20)) {
      ledOn = true;
    }

    if (sendSerial == HIGH) {
      Serial.print("V");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(v_real, 2);
      Serial.print(esAC ? " V RMS" : " V");
      Serial.println();
    }
  }

  digitalWrite(ledPin, ledOn ? HIGH : LOW);
  delay(500);  // Esperar entre medidas
}
