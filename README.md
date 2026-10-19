# Perilla

Biblioteca para leer perillas con microcontroladores, y mapear su valor a otro rango.

`Perilla` lee potenciómetros y conserva su API actual. `EncoderRotativo` es una clase separada para encoders incrementales KY-040.

La biblioteca usa la API estándar de Arduino y declara compatibilidad con todas las arquitecturas Arduino (`architectures=*`). Para Raspberry Pi Pico se incluye una implementación propia para Pico SDK, separada de Arduino.

## Conexiones

Un potenciómetro tiene tres patitas:

- una de las patitas de los extremos va a voltaje (5V o 3.3V, según la placa).
- la otra patita de los extremos va a tierra (GND).
- la patita del medio va a una entrada análoga.

En Raspberry Pi Pico usa 3.3V (patita `3V3`, nunca 5V) y una de las patitas con conversor análogo-digital: GPIO 26, 27 o 28.

## Uso con Arduino

Instala la biblioteca copiando esta carpeta en tu carpeta de bibliotecas de Arduino (por ejemplo `~/Documents/Arduino/libraries/Perilla`), o desde Arduino IDE con *Sketch → Include Library → Add .ZIP Library...*.

```cpp
#include "Perilla.h"

// perilla conectada a la patita A0
Perilla perilla(A0);

void setup()
{
  Serial.begin(9600);

  // Arduino Uno lee de 0 a 1023
  perilla.setRangoLeido(0, 1023);
  // convertir a porcentaje
  perilla.setRangoMapeado(0, 100);
}

void loop()
{
  perilla.leer();

  Serial.println(perilla.getValorMapeado());
}
```

Ejemplo en [examples/ej00_leerPerilla/](./examples/ej00_leerPerilla/).

Si la lectura tiembla por el ruido, `perilla.setFiltro(20)` incorpora un 20% de cada lectura nueva. `perilla.quitarFiltro()` lo desactiva. Si no llamas a `setFiltro()`, la perilla se comporta igual que antes.

## Uso con Raspberry Pi Pico SDK

La carpeta [pico/](./pico/) tiene un proyecto CMake para placas Raspberry Pi Pico con Pico SDK. Selecciona el modelo con `-DPICO_BOARD=pico`, `pico_w`, `pico2` o `pico2_w` (según la placa y la versión del SDK). Necesitas el [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado, o la extensión Raspberry Pi Pico de VS Code.

```bash
cd pico
mkdir build
cd build
PICO_SDK_PATH=/ruta/a/pico-sdk cmake ..
cmake --build .
```

Esto genera un archivo `.uf2` por ejemplo, como `ej00_leerPerilla.uf2`. Para cargarlo, conecta la Pico manteniendo presionado el botón BOOTSEL y copia el archivo a la unidad que aparece.

En Pico, la perilla se lee de 0 a 4095 (12 bits), no de 0 a 1023 como en Arduino Uno.

## Encoder incremental KY-040

Un potenciómetro entrega una tensión proporcional a una posición absoluta y se lee con `Perilla`. Un encoder incremental entrega pulsos al girar: la posición se acumula desde el encendido (puede ser negativa) y no representa un ángulo absoluto.

`EncoderRotativo` está declarado en el mismo `Perilla.h` que la clase `Perilla`; ambas clases conservan comportamientos independientes. Conecta CLK, DT y opcionalmente SW a GPIO; los tres usan pull-up interno y el común del encoder va a GND. El botón es activo en bajo. En Arduino UNO R4 WiFi conecta VCC → 5V, GND → GND, CLK → D2, DT → D3 y SW → D4. El ejemplo existente [ej05_encoder](./examples/ej05_encoder/ej05_encoder.ino) imprime posición y eventos del botón a 115200 baudios.

En Raspberry Pi Pico conecta VCC → 3V3 (nunca 5V), GND → GND, CLK → GPIO 14, DT → GPIO 15 y SW → GPIO 16. La variante Pico SDK del ejemplo existente está en [pico/ej05_encoder/](./pico/ej05_encoder/main.cpp).

```cpp
#include "Perilla.h"

EncoderRotativo encoder(2, 3, 4); // CLK, DT, SW; omite SW con -1
encoder.setTransicionesPorPaso(4); // 1, 2 o 4 transiciones por paso
encoder.setInvertido(true);         // opcional
encoder.setPosicion(-10);

void loop() {
  if (encoder.actualizar()) {
    // encoder.getPosicion() cambia en pasos válidos
    // encoder.getDireccion() devuelve HORARIO o ANTIHORARIO
  }
  if (encoder.fuePresionado()) { /* evento de pulsación */ }
  if (encoder.fueLiberado()) { /* evento de liberación */ }
}
```

El valor predeterminado son cuatro transiciones de cuadratura por paso, apropiado para un clic completo habitual del KY-040. Si el módulo produce otro número de transiciones por clic, configura `setTransicionesPorPaso(1)`, `(2)`, `(3)` o `(4)` según corresponda. Saltos de dos bits se descartan y los rebotes vecinos que regresan al estado anterior se cancelan. El antirrebote del botón es temporal, no bloqueante, y por defecto requiere 25 ms de estabilidad.

La clase sondea los pines: llama `actualizar()` en cada vuelta del `loop` y evita demoras largas o escrituras seriales lentas. Si el programa no sondea con suficiente frecuencia puede perder transiciones y pasos; un encoder mecánico rápido puede requerir interrupciones, que esta API no implementa. La lectura del botón tampoco bloquea.

Para otra placa, cambia `PICO_BOARD` en [pico/CMakeLists.txt](./pico/CMakeLists.txt) (por ejemplo `pico` para la Pico original).

Ejemplo completo en [pico/ej00_leerPerilla/main.cpp](./pico/ej00_leerPerilla/main.cpp).

## Ejemplos

Cada ejemplo existe para Arduino, en [examples/](./examples/), y para Pico SDK, en [pico/](./pico/). En Arduino la perilla va en A0, y en Pico en GPIO 26.

| Ejemplo | Qué hace |
| --- | --- |
| `ej00_leerPerilla` | imprime el valor leído y el valor mapeado |
| `ej01_brilloLed` | controla el brillo de un led (en Arduino un led externo en la patita 9, en Pico el led de la placa) |
| `ej02_umbral` | enciende el led de la placa cuando la perilla pasa de la mitad |
| `ej03_dosPerillas` | lee dos perillas a la vez, en formato para el plotter serial |
| `ej04_filtro` | suaviza la lectura con un filtro opcional |
| `ej05_encoder` | lee un encoder KY-040: posición firmada, dirección y botón |

## Referencia

| Método | Descripción |
| --- | --- |
| `Perilla(uint8_t patita)` | crea un potenciómetro, el tipo por defecto, y configura la patita como entrada análoga |
| `Perilla(uint8_t patitaA, uint8_t patitaB, Tipo tipo)` | encoder en dos patitas digitales, con el común a tierra |
| `void setRangoLeido(uint16_t min, uint16_t max)` | rango que entrega la placa al leer un potenciómetro, por ejemplo `0, 1023` |
| `void setRangoMapeado(uint16_t min, uint16_t max)` | rango del valor mapeado, por ejemplo `0, 100`. En un encoder el valor parte en el mínimo y se mueve con los clics |
| `void setSensibilidad(uint8_t pasosPorClic)` | cuántos pasos vale cada clic del encoder, en los pasos y en el valor. El valor inicial es 1. Un 0 se toma como 1 |
| `void setFiltro(uint8_t porcentaje)` | suaviza la lectura. `20` incorpora un 20% de cada lectura nueva. Un `0` se toma como `1`, y un valor mayor que 100 se toma como 100 |
| `void quitarFiltro()` | desactiva el filtro. La próxima `leer()` vuelve a la lectura directa |
| `void leer()` | lee la perilla. En un encoder hay que llamarla en cada vuelta del loop |
| `uint16_t getValor()` | última lectura del potenciómetro, sin mapear (filtrada si el filtro está activo). En un encoder es igual a `getValorMapeado()` |
| `uint16_t getValorMapeado()` | valor de la perilla dentro del rango mapeado, sea potenciómetro o encoder |
| `Direccion getDireccion()` | `HORARIO`, `ANTIHORARIO` o `QUIETA`, según la última `leer()` |
| `int32_t getPasos()` | pasos acumulados del encoder. Puede ser negativo |
| `void setPatita(uint8_t patita)` | cambia la patita guardada (no la vuelve a configurar) |

Si no configuras los rangos, ambos parten en `0, 1023`, así que `getValorMapeado()` entrega lo mismo que `getValor()`.

## Encoder

Una perilla también puede ser un encoder. Se usa igual que un potenciómetro: cambia solo la línea que la crea.

```cpp
// Perilla perilla(A0);                    // potenciómetro
Perilla perilla(2, 3, Perilla::ENCODER);   // encoder

void setup()
{
  // en los dos casos, el valor va de 0 a 100
  perilla.setRangoMapeado(0, 100);
  // en un encoder, cada clic mueve el valor en 2
  perilla.setSensibilidad(2);
}

void loop()
{
  perilla.leer();

  // getValorMapeado() funciona igual con los dos tipos
  // getDireccion() es HORARIO, ANTIHORARIO o QUIETA
  // getPasos() es el total de clics, sin topes, y puede ser negativo
}
```

En un encoder, el valor parte en el mínimo del rango mapeado y se detiene en los extremos, como un potenciómetro en sus topes. `setSensibilidad(2)` hace que cada clic sume o reste 2. Si no la llamas, cada clic vale 1. El pulsador del eje, si el encoder tiene uno, se lee como un botón en otra patita.

Ejemplo en [examples/ej05_encoder/](./examples/ej05_encoder/).

### API de `EncoderRotativo`

| Método | Descripción |
| --- | --- |
| `EncoderRotativo(uint8_t clk, uint8_t dt, int16_t sw = -1)` | configura CLK, DT y botón opcional con pull-up interno |
| `bool actualizar()` | sondea entradas, devuelve true si cambió la posición; debe llamarse frecuentemente |
| `getDireccion() const` | sentido del último paso detectado (`HORARIO`, `ANTIHORARIO` o `QUIETA`) |
| `int32_t getPosicion() const` / `setPosicion(int32_t)` | consulta o establece posición firmada sin topes |
| `setTransicionesPorPaso(uint8_t)` | selecciona 1, 2 o 4 transiciones por paso |
| `setInvertido(bool)` | invierte el signo del giro |
| `setAntirrebote(uint16_t ms)` | intervalo de estabilidad del botón; predeterminado 25 ms |
| `getBotonPresionado() const` | estado estable actual; true significa presionado |
| `fuePresionado()` / `fueLiberado()` | consume el evento correspondiente una sola vez |

## Documentación

La documentación se genera con [Doxygen](https://www.doxygen.nl/) a partir de los comentarios en [src/](./src/), en español y en inglés, y se publica en <https://piruetasxyz.github.io/Perilla/>.

Para generarla en tu computador, en español:

```bash
(cat Doxyfile; echo "OUTPUT_LANGUAGE = Spanish"; echo "HTML_OUTPUT = es") | doxygen -
```

Queda en `build/docs/es/index.html`. Para inglés, usa `English` y `en`.

Cada comentario tiene una sección `\~spanish` y una sección `\~english`. Si agregas algo público sin documentar, la generación falla.

## Cómo está organizado

El código de [src/Perilla.cpp](./src/Perilla.cpp) no depende de ninguna plataforma. Todo lo que toca el hardware pasa por [src/Hardware.h](./src/Hardware.h), que tiene una implementación por plataforma:

- [src/arduino/ArduinoHardware.cpp](./src/arduino/ArduinoHardware.cpp): usa `analogRead`.
- [pico/PicoHardware.cpp](./pico/PicoHardware.cpp): usa las funciones `adc_*` del Pico SDK.

Para agregar otra plataforma, basta con escribir otra implementación de `Hardware.h`.

## Contenido

- [.github/](./.github): flujos de trabajo que compilan los ejemplos de Arduino y de Pico, revisan la biblioteca con Arduino Lint, y publican la documentación.
- [docs/](./docs/): página principal de la documentación y página para elegir idioma.
- [examples/](./examples/): ejemplos para Arduino.
- [Doxyfile](./Doxyfile): configuración de Doxygen.
- [pico/](./pico/): proyecto CMake, implementación y ejemplo para Pico SDK.
- [src/](./src/): código de la biblioteca.
- [keywords.txt](./keywords.txt): colores de sintaxis para Arduino IDE.
- [library.properties](./library.properties): metadatos para Arduino.
- [LICENSE](./LICENSE): licencia MIT.
- [README.md](./README.md): este documento.

## Versiones

- v0.3.0: octubre 2026, clase EncoderRotativo con cuadratura por sondeo, posición firmada y botón opcional antirrebote.
- v0.2.0: octubre 2026, encoder como tipo de perilla, con el valor dentro del rango mapeado, y filtro opcional para el ruido.
- v0.1.0: octubre 2026, compatibilidad con Raspberry Pi Pico SDK, y valores por defecto para los rangos.
- v0.0.1: septiembre 2025, primera versión para Arduino.

## Licencia

MIT, ver [LICENSE](./LICENSE).
