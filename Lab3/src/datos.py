#!/usr/bin/python3
import serial
import csv

# Configuración del puerto serial
ser = serial.Serial(
    port='/tmp/ttyS1',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0
)

# Abre el archivo en modo escritura
f = open('datos_voltajes.csv', 'w')

print(f"Conectado a: {ser.portstr}")

try:
    while True:
        for c in ser.read():
            c = chr(c)              # Convierte el byte a carácter.
            print(c, end="")        # Imprime el carácter en la consola.
            f.write(c)              # Escribe el carácter en el archivo.

except KeyboardInterrupt:
        print("Interrumpido por el usuario.")


finally:
    ser.close()
    f.close()

