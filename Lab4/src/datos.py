#!/usr/bin/python3
import serial  # type: ignore
import time
import paho.mqtt.client as mqtt  # type: ignore # Importa la biblioteca MQTT

# Configuracion del puerto serie
serial_port = '/dev/ttyACM0'  
baud_rate = 115200  

# Configuracion del cliente MQTT
mqtt_broker = 'iot.eie.ucr.ac.cr'  
mqtt_port = 1883  
mqtt_topic = 'v1/devices/me/telemetry'  
mqtt_username = 'nDYJOGSJOo87jGx7QfYV'  

# Inicializa la conexion serie
ser = serial.Serial(serial_port, baud_rate, timeout=1)
time.sleep(2)  

# Inicializa el cliente MQTT
mqtt_client = mqtt.Client()
mqtt_client.username_pw_set(mqtt_username)  
mqtt_client.connect(mqtt_broker, mqtt_port, 60)  

try:
    while True:
        if ser.in_waiting > 0:    # Verifica si hay datos disponibles para leer
            line = ser.readline().decode('utf-8').rstrip()  # Lee y decodifica la linea
            print("Datos recibidos:", line)  # Imprime los datos en la consola

            # Procesa los datos
            try:
                parts = line.split(', ')
                if len(parts) < 4:
                    print("Datos incompletos:", line)
                    continue

                x_value = int(parts[0].split(': ')[1])
                y_value = int(parts[1].split(': ')[1])
                z_value = int(parts[2].split(': ')[1])
                battery_value = float(parts[3].split(': ')[1].replace(' V', ''))

                # Imprime los valores
                print("Eje X:", x_value)
                print("Eje Y:", y_value)
                print("Eje Z:", z_value)
                print("Valor de la batería:", battery_value)

                # Determina el mensaje de estado de la batería
                battery_message = "batería baja" if battery_value < 7.0 else "batería normal"

                # Prepara el payload para enviar al broker MQTT
                payload = {
                    'x': x_value,
                    'y': y_value,
                    'z': z_value,
                    'battery': battery_value,
                    'battery_message': battery_message
                }

                # Publica los datos en el broker MQTT
                mqtt_client.publish(mqtt_topic, str(payload))

            except ValueError:
                print("Error al procesar los datos.")

except KeyboardInterrupt:
    print("Finalizando...")
finally:
    ser.close()  # Cierra el puerto serie
    mqtt_client.disconnect()  # Desconecta del broker MQTT
