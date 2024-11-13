import pandas as pd

# Cargar cada archivo CSV y asignarles etiquetas
mov1 = pd.read_csv('movimiento_arriba_abajo.csv')
mov2 = pd.read_csv('movimiento_derecha_izquierda.csv')
mov3 = pd.read_csv('movimiento_circulo.csv')

# Unir los archivos en un solo DataFrame
data = pd.concat([mov1, mov2, mov3], ignore_index=True)

# Guardar el conjunto de datos combinado
data.to_csv('movimientos_combinados.csv', index=False)
