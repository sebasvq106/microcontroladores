#!/usr/bin/python3
import serial  # type: ignore
import time
import csv

# Configuración del puerto serie
serial_port = '/dev/ttyACM0'
baud_rate = 115200

# Inicializa la conexión serie
ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)

# Crear el archivo CSV y escribir los encabezados
csv_file = 'movimiento_circulo.csv'
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['x', 'y', 'z', 'label'])  # Encabezados del archivo CSV con la etiqueta

    try:
        while True:
            if ser.in_waiting > 0:  # Verifica si hay datos disponibles para leer
                line = ser.readline().decode('utf-8').rstrip()  # Lee y decodifica la línea

                # Procesa los datos
                try:
                    parts = line.split(', ')
                    if len(parts) < 3:
                        continue  # Omite si los datos están incompletos

                    x_value = int(parts[0].split(': ')[1])
                    y_value = int(parts[1].split(': ')[1])
                    z_value = int(parts[2].split(': ')[1])

                    # Escribe los datos en el archivo CSV con la etiqueta de movimiento
                    writer.writerow([x_value, y_value, z_value, 'Circulo'])

                except ValueError:
                    continue  # Omite si hay un error en el procesamiento de los datos

    except KeyboardInterrupt:
        print("Finalizando...")
    finally:
        ser.close()  # Cierra el puerto serie
