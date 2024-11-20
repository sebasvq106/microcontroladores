#!/usr/bin/python3
import serial  # type: ignore
import time

# Configuración del puerto serie
serial_port = '/dev/ttyACM0'  
baud_rate = 9600  # Configura la velocidad de baudios

# Inicializa la conexión serie
ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)  # Espera a que el puerto serie esté listo

try:
    while True:
        if ser.in_waiting > 0:  # Verifica si hay datos disponibles para leer
            # Lee y decodifica la línea, ignorando los errores de decodificación
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()  # Ignora los caracteres no válidos
            print("Datos recibidos:", line)  # Imprime los datos en la consola

except KeyboardInterrupt:
    print("Finalizando...")

finally:
    ser.close()  # Cierra el puerto serie
