// Encoder KY-040 en Arduino UNO R4 WiFi.
// VCC -> 5V, GND -> GND, CLK -> D2, DT -> D3, SW -> D4.
#include "Perilla.h"

EncoderRotativo encoder(2, 3, 4);
int32_t posicionImpresa = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Encoder KY-040 listo");
}

void loop()
{
  // Sondear en cada vuelta y evitar demoras largas para no perder transiciones.
  if (encoder.actualizar())
  {
    const int32_t posicion = encoder.getPosicion();
    if (posicion != posicionImpresa)
    {
      posicionImpresa = posicion;
      Serial.print("Posicion: ");
      Serial.println(posicion);
    }
  }
  if (encoder.fuePresionado()) Serial.println("Boton presionado");
  if (encoder.fueLiberado()) Serial.println("Boton liberado");
}
