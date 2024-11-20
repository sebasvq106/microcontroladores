# Como ejecutar el Laboratorio

1. **Ubicación la carpeta:**  
   Los archivos de la carpeta src deben estar dentro de la siguiente ruta para poder utilizar el Makefile:
   ```
   ~/libopencm3/libopencm3-examples/examples/stm32/f4/stm32f429i-discovery
   ```

2. **Compilación:**  
   Utilice el make para compilar el archivo, esto generará el archivo main.elf

3. **Conversión a formato binario:**  
   Luego de la compilación, convierte el archivo `.elf` a formato `.bin` con:
   ```
   arm-none-eabi-objcopy -O binary main.elf firmware.bin
   ```

4. **Flasheo a la tarjeta:**  
   Finalmente, transfiere el archivo binario a tu dispositivo con el siguiente comando:
   ```
   st-flash --reset write firmware.bin 0x8000000
   ```
5. **Ejecución del script Python:**  
Para ejecutar el script de Pyhton basta con utilizar el comando:
   ```
   python3 datos.py
   ```
