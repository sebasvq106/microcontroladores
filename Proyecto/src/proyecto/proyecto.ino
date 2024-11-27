#include <Keypad.h>
#include <EEPROM.h>
#include <Adafruit_VC0706.h>
#include <SPI.h>

// Configuración del buzzer y el pin
const int buzzerPin = A4;
const int trigPin = 4;
const int echoPin = 5;
bool activo = false;
bool detection = false;
bool estadoActivado = false;
bool foto_tomada = false;

// Configuración de la cámara
#if defined(__AVR__) || defined(ESP8266)
#include <SoftwareSerial.h>         
SoftwareSerial cameraconnection(2, 3); // Conexión de la cámara: TX en pin 2, RX en pin 3
#else
#define cameraconnection Serial1
#endif

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Configuración del teclado 
const byte filas = 4;
const byte columnas = 4;
char keys[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte colPins[filas] = {9, 8, 7, 6};    
byte rowPins[columnas] = {13, 12, 11, 10}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, filas, columnas);

char codigoIngresado[5];
int indice = 0;
int codigoGuardado; 

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  codigoGuardado = leerCodigo(); 
  // Inicializa la cámara
  if (cam.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }

  // Configura el tamaño de la imagen 
  cam.setImageSize(VC0706_160x120); 
  uint8_t imgsize = cam.getImageSize();
  Serial.print("Image size: ");
  if (imgsize == VC0706_640x480) Serial.println("640x480");
  if (imgsize == VC0706_320x240) Serial.println("320x240");
  if (imgsize == VC0706_160x120) Serial.println("160x120");

  Serial.println("Ingrese el código en el keypad o envíe '1'/'0' desde Serial para activar el sistema de seguridad.");
}

void loop() {
  if (!activo) {
    detection = false;
    foto_tomada = false; 
  }

  if (detection) {
    digitalWrite(buzzerPin, HIGH); 
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Verificar si hay datos en el puerto serial
  if (Serial.available() > 0) {
    char entrada = Serial.read();
    if (entrada == '1') {
      activo = true;
      estadoActivado = true; // Marca que estado fue activado una vez
      Serial.println("Sistema activado.");
    } else if (entrada == '0' && estadoActivado) {
      activo = false; 
      estadoActivado = false; 
      Serial.println("Sistema desactivado.");
    }
  }

  // Verificar el código ingresado en el Keypad
  char tecla = keypad.getKey();
  if (tecla) {
    codigoIngresado[indice] = tecla;
    indice++;

    // Si se han ingresado 4 dígitos
    if (indice == 4) { 
      codigoIngresado[indice] = '\0'; 
      int codigoIngresadoInt = atoi(codigoIngresado);

      Serial.print("Código ingresado: ");
      Serial.println(codigoIngresadoInt);

      if (codigoIngresadoInt == codigoGuardado) {
        activo = !activo; // Alterna el estado de "activo"
        Serial.print("Sistema ");
        Serial.println(activo ? "activado." : "desactivado.");
      } else {
        Serial.println("Código incorrecto.");
      }

      indice = 0; 
    }
  }

  // Toma de mediciones del ultrasonico
  if (activo && !detection){
    float distance = measureDistance();

    // Si la distancia es menor a 6 cm, envía un mensaje a Python
    if (distance > 0 && distance < 6) {
    detection = true;
    Serial.println("Movimiento detectado");
    delay(1000); 
    }
  }

  if (activo && detection && !foto_tomada){
    // Espera el comando 'snapshot' desde Python
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();

      if (command == "snapshot") {
        Serial.println("Taking picture...");

        if (!cam.takePicture()) {
          Serial.println("Failed to snap!");
          return;
        }
        Serial.println("Picture taken!");

        // Envía el tamaño de la imagen a Python
        uint32_t jpglen = cam.frameLength();
        Serial.print("Storing ");
        Serial.print(jpglen);
        Serial.println(" byte image.");

        // Envía la imagen en bloques de 32 bytes
        while (jpglen > 0) {
          uint8_t bytesToRead = min((uint32_t)32, jpglen);
          uint8_t *buffer = cam.readPicture(bytesToRead);
          Serial.write(buffer, bytesToRead);  // Enviar el bloque de datos
          jpglen -= bytesToRead;
        }

        Serial.println("done!");  

        cam.resumeVideo();

        foto_tomada = true;
      }
    }
  }

}

int leerCodigo() {
  int parteAlta = EEPROM.read(0);  // Leer la parte alta del número
  int parteBaja = EEPROM.read(1);  // Leer la parte baja del número
  return (parteAlta << 8) | parteBaja;  // Reconstruir el número
}

// Función para medir la distancia con el sensor ultrasónico
float measureDistance() {
  // Enviar un pulso de disparo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer el tiempo de retorno del eco
  long duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en cm
  float distance = duration * 0.034 / 2;

  return distance;
}