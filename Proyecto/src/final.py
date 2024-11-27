import serial
import time
import paho.mqtt.client as mqtt
import requests
import base64
import json
import threading

# Configuración del puerto serial
arduino_port = '/dev/ttyUSB1'  
baud_rate = 9600
ser = serial.Serial(arduino_port, baud_rate, timeout=1)

# Espera a que se establezca la conexión serial
time.sleep(2)

# Configuración del servidor MQTT y HTTP
mqtt_server = "iot.eie.ucr.ac.cr"
mqtt_port = 1883
mqtt_topic_telemetry = "v1/devices/me/telemetry"
mqtt_username = 'yi1bw9v68qanrq8t7gyx'

# URL para enviar telemetría vía HTTP
http_url = "http://iot.eie.ucr.ac.cr:8080/api/v1/yi1bw9v68qanrq8t7gyx/telemetry"


estado_boton = False
imagen_enviada = False
notificacion = "Sistema Desactivado"
mqtt_client = None


def obtener_imagen_base64(filename):
    with open(filename, "rb") as image_file:
        base64_image = base64.b64encode(image_file.read()).decode('utf-8')
    html_content = f"""
    <!DOCTYPE html>
    <html>
        <body>
            <h2>Imagen de la Cámara</h2>
            <img src="data:image/jpeg;base64,{base64_image}" alt="Imagen de prueba" />
        </body>
    </html>
    """
    return html_content.strip()

def enviar_imagen_http(filename):
    payload = {"Imagen": obtener_imagen_base64(filename)}
    headers = {"Content-Type": "application/json"}
    response = requests.post(http_url, json=payload, headers=headers)
    if response.status_code == 200:
        print("Imagen enviada exitosamente.")
    else:
        print("Error al enviar la imagen:", response.status_code, response.text)


def capture_image():
    # Envía el comando para tomar una foto al Arduino
    ser.write(b'snapshot\n')
    
    # Espera la respuesta y guarda la imagen
    img_data = bytearray()
    jpg_size = None  # Inicializa la variable

    # Leer el tamaño de la imagen
    while True:
        size_line = ser.readline().decode().strip()
        if "Storing" in size_line:
            jpg_size = int(size_line.split()[1])
            print(f"Tamaño de la imagen: {jpg_size} bytes")
            break
        elif size_line == "":
            print("Esperando respuesta del Arduino...")
            time.sleep(0.5)
        else:
            print(size_line)  

    # Verifica si jpg_size se definió correctamente
    if jpg_size is None:
        print("Error: No se pudo obtener el tamaño de la imagen.")
        return None

    # Leer la imagen en bloques de 32 bytes
    bytes_remaining = jpg_size
    while bytes_remaining > 0:
        chunk_size = min(32, bytes_remaining)
        img_data.extend(ser.read(chunk_size))
        bytes_remaining -= chunk_size
        print(".", end="", flush=True)  

    print("\nImagen capturada correctamente.")
    
    # Guardar la imagen como archivo .jpg temporalmente
    with open("final.jpg", "wb") as img_file:
        img_file.write(img_data)
    print("Imagen guardada como 'final.jpg'.")

    return "final.jpg"
    
def monitor_motion():
    """Función para monitorear el movimiento detectado por el sensor ultrasónico."""
    global notificacion
    try:
        while True:
            # Lee mensajes del Arduino
            line = ser.readline().decode().strip()
            if line == "Sistema activado.":
                notificacion = "Sistema activado, esperando movimiento"
                enviar_telemetria()
            
            if line == "Sistema desactivado.":
                notificacion = "Sistema Desactivado"
                enviar_telemetria()
                enviar_telemetria()
            if line == "Movimiento detectado":
                print("Movimiento detectado por el sensor ultrasónico.")

                notificacion = "Se detectó un movimiento, por favor vea la imagen"
                enviar_telemetria()
                
                # Captura la imagen desde el Arduino
                imagen_filename = capture_image()   
                if imagen_filename:
                    enviar_imagen_http(imagen_filename)
                    print("Datos enviados a ThingsBoard (HTTP): Imagen")

            elif line:
                print(f"Arduino: {line}")  # Otros mensajes de Arduino
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario.")
    finally:
        ser.close()


def enviar_estado_boton():
    if estado_boton:
        ser.write(b'1')  # Enviar '1' cuando el botón esté ON
    else:
        ser.write(b'0')  # Enviar '0' cuando el botón esté OFF

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado al servidor MQTT.")
        client.subscribe("v1/devices/me/rpc/request/+")
    else:
        print("Error de conexión, código de resultado:", rc)

def on_message(client, userdata, msg):
    global estado_boton, imagen_enviada
    print(f"Mensaje recibido en el tema {msg.topic}: {msg.payload.decode('utf-8')}")

    data = json.loads(msg.payload.decode('utf-8'))
    method = data.get("method")
    params = data.get("params")

    if method == "getValue_1":
        if isinstance(params, bool):
            estado_boton = params
            print("Estado del botón:", "ON" if estado_boton else "OFF")
            if not estado_boton:
                imagen_enviada = False

            enviar_estado_boton()

            request_id = msg.topic.split("/")[-1]
            response_topic = f"v1/devices/me/rpc/response/{request_id}"
            response_payload = json.dumps({"result": f"Estado actualizado a {estado_boton}"})
            client.publish(response_topic, response_payload)
            print(f"Respuesta enviada a ThingsBoard: {response_payload}")
        else:
            print("El valor de params no es un booleano válido")

def main():
    # Configurar MQTT
    global mqtt_client
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(mqtt_username)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(mqtt_server, mqtt_port, 60)

    # Inicia el cliente MQTT en segundo plano
    mqtt_client.loop_start()

    enviar_telemetria()

    # Crea un hilo para monitor_motion
    motion_thread = threading.Thread(target=monitor_motion, daemon=True)
    motion_thread.start()

    try:
        print("Sistema en ejecución. Presiona Ctrl+C para salir.")
        while True:
            time.sleep(1)  # Mantiene el programa principal activo
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario.")
    finally:
        mqtt_client.loop_stop()
        mqtt_client.disconnect()
        ser.close()


def enviar_telemetria():
    """Función para enviar la telemetría al servidor MQTT."""
    global notificacion, mqtt_client
    payload = {
        "notificacion": notificacion
    }
    mqtt_client.publish(mqtt_topic_telemetry, json.dumps(payload))

if __name__ == "__main__":
    main()