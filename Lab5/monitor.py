import serial

# Configurar el puerto serial
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Ajusta el puerto según tu configuración

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print(line)  # Muestra los resultados en la consola
        # Lógica adicional para interpretar los resultados
