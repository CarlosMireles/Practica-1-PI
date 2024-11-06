# Práctica Lab1: Interfaces Paralelos

Este proyecto es una práctica de laboratorio diseñada para controlar un display de 7 segmentos y recibir entradas de un teclado matricial. Utiliza un microcontrolador para interactuar con un display de cuatro dígitos y responde a las pulsaciones de botones y teclado.

## Descripción

El programa realiza varias funciones:
1. **Control de Display de 7 segmentos**: Parpadea todos los segmentos de los dígitos extremos y muestra el número en el contador.
2. **Lectura de Pulsadores y Teclado**: Responde a las entradas de un teclado matricial de 3x4 y permite la entrada de números a través de los botones.
3. **Modos de Visualización**: Cuenta con tres modos de visualización que permiten modificar la manera en que los dígitos son mostrados en el display.

## Estructura de Archivos

- **24-25_plab1_cmr.pdsprj**: Proyecto base de Proteus que contiene el esquema y configuraciones iniciales del sistema.
- **Código Principal (esquema arriba)**: Contiene las definiciones, inicialización de puertos y funciones de control.

## Configuración de Pines

### Definiciones de Pines para el Teclado y el Display
- **Teclado**: Conectado al puerto `PORTL` para interactuar con el teclado matricial y controlar el encendido de los cátodos del display.
- **Pulsadores**: Configurados en el puerto `PORTC`, se usan para navegar e ingresar comandos.
- **Speaker**: Conectado al pin PC0 del puerto `PORTC`.

### Pines del Teclado y Display
- **PORTL (Pins 42-49)**: Utilizado tanto para las filas del teclado como para el control de los cátodos del display.
  - R1-R4: Filas del teclado.
  - D1-D4: Dígitos del display de 7 segmentos.

## Modos de Visualización

El programa ofrece tres modos de visualización para mostrar los dígitos en el display:

1. **Modo 1 - Visualización Completa**: Tres dígitos activos, visualizando centenas, decenas y unidades.
2. **Modo 2 - Visualización Reducida Inferior**: Dos dígitos activos (decenas y unidades).
3. **Modo 3 - Visualización Reducida Superior**: Dos dígitos activos (centenas y decenas).

Para cambiar el modo, se debe ingresar un número (1-3) a través del monitor serie.

## Funcionalidad del Código

### Inicialización
La función `setup()` configura los puertos de entrada y salida, inicializa la comunicación serie, y establece las condiciones iniciales del display y el teclado.

### Loop Principal
En `loop()`, se monitorean los botones y se llama a funciones para actualizar el contador, el cual se muestra en el display.

### Control del Display y Teclado
Cada dígito del display se controla en una interrupción `ISR(INT3_vect)`, la cual cicla entre los dígitos del display y utiliza `comprobarTeclado()` para capturar entradas del teclado.

## Controles

- **Botón Up (PC3)**: Incrementa el contador.
- **Botón Down (PC6)**: Decrementa el contador.
- **Botón Enter (PC4)**: Resetea el contador.
- **Botón Right (PC7)**: Cambia el incremento a 2.
- **Botón Left (PC5)**: Cambia el incremento a 1.
## Autor

Carlos Mireles Rodríguez  
