# Como ejecutar el Laboratorio

1. **Ubicación la carpeta:**  
   Se tiene que clonar el repositorio de MicroML y mover los archivos de src a una carpeta dentro de projects, como se muestra en la siguiente dirección: 
   ```
   ~/MicroML/projects
   ```

2. **Compilación:**  
   Regrese a la caperta MicroML y ejecute el siguiente comando: 
   ```
   make -f archive_makefile PROJECT=<name of our project>
   ```
   Luego, regresa a la carpeta de src y utiliza el comando make para crear el archivo binario.

3. **Flasheo a la tarjeta:**  
   Dirigete a la carpeta generada build y utiliza el siguiente comando:
   ```
   st-flash --reset write firmware.bin 0x8000000
   ```
4. **Ejecución del script Python:**  
Para ejecutar el script de Pyhton basta con utilizar el comando:
   ```
   python3 monitor.py
   ```
