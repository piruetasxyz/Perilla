// KY-040 en Pico: GND -> GND, VCC -> 3V3 (nunca 5V),
// CLK -> GPIO 14, DT -> GPIO 15, SW -> GPIO 16.
#include "Perilla.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main()
{
    stdio_init_all();
    EncoderRotativo encoder(14, 15, 16);
    int32_t posicionImpresa = 0;
    while (true)
    {
        if (encoder.actualizar())
        {
            const int32_t posicion = encoder.getPosicion();
            if (posicion != posicionImpresa)
            {
                posicionImpresa = posicion;
                printf("Posicion: %ld\n", (long)posicion);
            }
        }
        if (encoder.fuePresionado()) printf("Boton presionado\n");
        if (encoder.fueLiberado()) printf("Boton liberado\n");
        // Sin sleep_ms: mantener el sondeo frecuente.
    }
}
