#!/usr/bin/python3
import serial  # type: ignore
import time

# Configuración del puerto serie
serial_port = '/dev/ttyACM0'  
baud_rate = 115200  

# Inicializa la conexión serie
ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)  # Espera a que el puerto serie esté listo

try:
    while True:
        if ser.in_waiting > 0:  # Verifica si hay datos disponibles para leer
            line = ser.readline().decode('utf-8').rstrip()  # Lee y decodifica la línea
            print("Datos recibidos:", line)  # Imprime los datos en la consola

            # Intenta convertir el valor recibido a un flotante
            try:
                float_value = float(line)
                print("Valor flotante recibido:", float_value)
            except ValueError:
                print("No es un valor flotante válido.")

except KeyboardInterrupt:
    print("Finalizando...")

finally:
    ser.close()  # Cierra el puerto serie
