# MarchONs

En este repositorio se planea poner la información y códigos correspondientes al proyecto final de Electrónica Digital 3.

## Descripción del Proyecto

Un reloj inteligente que permita tomar diferentes bio estadísticas, enfocadas al deporte y a la actividad física, mediante el uso de sensores.

\image html Diag_bloques.png width=700

Se usará el micro-controlador RP2040 para procesar todas las señales. El sensor de acelerómetro y giroscopio se utiliza para medir los pasos por día viene conectado a un level shifter debido a que su voltaje típico de alimentación es de 1.8V y el RP2040 maneja una lógica de 3.3V. El sensor de ritmo cardíaco se usa para dar el pulso, conectado a I2C. La señal del RTC (*"Real"* time clock) externo para mostrar la hora en un momento dado y guardar datos es conectada mediante SPI.

Estos datos se mostrarán en una pantalla LCD circular que posee un controlador conectado por SPI, y sus funcionalidades son controladas por pulsadores para presentar el dato pedido.

Adicionalmente, se leerá la lectura del valor análogo de la batería mediante un divisor de voltaje conectado a un ADC.

Para la estructura principal del proyecto se basa en el flujo de programa de polling e interrupciones:

\image html Diag_flujo.png width=350



## Requisitos funcionales

- **Procesamiento de datos:** Con diferentes sensores se busca calcular diferentes medidas como se muestra:
  - *Giroscopio y Acelerómetro:* Cálculo de pasos mediante una conexión I2C.
  - *Sensor de ritmo cardíaco:* Se trabaja con una señal análoga pasada por I2C, el muestreo depende de la frecuencia máxima de latidos por minuto, que son 220 latidos por minuto, que serían casi 4 latidos por segundo, por lo que se debe muestrear a un valor similar.
  - *RTC:* Con este módulo se plantea seguir la fecha y hora, este maneja una conexión serial, como se cambia cada segundo no se necesita una transmisión de datos de alta velocidad.

- **Alimentación:** Se utilizará una batería que en caso de que se esté acabando se podrá recargar, se espera que se tenga una protección contra sobrecargas.

- **Interfaz de Usuario:** Se tendrá una pantalla LCD conectada mediante el protocolo SPI para mostrar los datos encontrados. Se debe poder configurar para empezar el uso del reloj normal.

## Requisitos NO funcionales

- **Tiempo de respuesta:** Se requiere que el tiempo de respuesta sea consecuente con la acción, esto es, que se permita el guardado y despliegue en un tiempo que no sea tan lejano al tiempo de reacción de un humano.
  
- **Usabilidad:** Si bien se espera que el reloj sea intuitivo, se planea tener un manual de uso para su correcto funcionamiento y así conocer sus capacidades completas y evitar accidentes.
  
- **Autonomía de la batería:** Se espera que con la batería planeada se tenga por lo menos media hora de funcionamiento completo (En un modo completo de rendimiento).

## Escenario de Pruebas

En el momento de presentar este proyecto, se plantean las siguientes pruebas para mostrar su correcto funcionamiento:

- Se empieza configurando el reloj (Dando datos del usuario y fecha actual).
- Se muestra el correcto funcionamiento de:
  - El medidor de pasos caminando o corriendo y mirando si se da el conteo correcto de los pasos (esperando estar en el 20% del valor correcto).
  - El sensor de ritmo cardíaco empezando por el valor inicial, que sería el estado "normal" (de notar que puede ser un poco elevado por los nervios de la sustentación) y hacer una actividad física demandante como saltar o correr y mirar este valor.
  - La fecha y hora configuradas a través del display de LCD circular, comparando con otro dispositivo que tenga la hora.
- Se apaga para ver si sigue el conteo de la fecha y hora de manera normal y si se guardan los pasos.
- Se muestra el correcto funcionamiento de la carga de la batería, llegando hasta el límite, después, se deja encendido hasta que pasen 30 minutos.
## Cronograma
Se esperaba terminar el proyecto en cuatro semanas siguiendo el cronograma:

\image html Cronograma.png width=70%

Esto no fue cumplido debido a diferentes situaciones personales y dificultades encontradas en el desarrollo del proyecto, pero se espera que la fecha final de este sea el 13/12/2024.

## Presupuesto

Se anexa en la siguiente tabla un presupuesto tentativo incluyendo los componentes principales para el funcionamiento del proyecto final, se tiene un valor comprensible para un grupo de tres personas.

<div style="justify-self:center;">
| Elemento                                                               | Valor (\$COP)   |
|:----------------------------------------------------------------------:|:---------------:|
| Placa MCU RP2040 con LCD redondo y sensor de acelerómetro y giroscopio | 93.415          |
| RTC DS1302 Mini                                                        | 5.593           |
| Sensor de ritmo cardíaco MAX30102                                      | 19.000          |
| Batería Litio-Polímero 3.7v 500mAh                                     | 16.800          |
| Headers 1.27mm x4                                                      | 16.123          |
| **Total**                                                              | 150.931         |
</div>