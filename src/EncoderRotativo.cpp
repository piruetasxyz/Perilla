#include "Perilla.h"
#include "Hardware.h"

static const int8_t deltaCuadratura[16] = {
    0, -1, +1, 0,
    +1, 0, 0, -1,
    -1, 0, 0, +1,
    0, +1, -1, 0};

EncoderRotativo::EncoderRotativo(uint8_t clk, uint8_t dt, int16_t sw)
    : pinCLK(clk), pinDT(dt), pinSW(sw), estadoAnterior(0), avance(0), posicion(0),
      transicionesPorPaso(4), direccion(QUIETA), invertido(false), antirreboteMs(25),
      botonLectura(false), botonEstable(false), cambioBotonMs(0),
      eventoPresionado(false), eventoLiberado(false)
{
    PerillaHardware::configurarEntradaPullup(pinCLK);
    PerillaHardware::configurarEntradaPullup(pinDT);
    estadoAnterior = (PerillaHardware::leerPatitaDigital(pinCLK) << 1) |
                     PerillaHardware::leerPatitaDigital(pinDT);
    if (pinSW >= 0)
    {
        PerillaHardware::configurarEntradaPullup((uint8_t)pinSW);
        botonLectura = !PerillaHardware::leerPatitaDigital((uint8_t)pinSW);
        botonEstable = botonLectura;
        cambioBotonMs = PerillaHardware::tiempoMilisegundos();
    }
}

bool EncoderRotativo::actualizar()
{
    bool cambio = false;
    direccion = QUIETA;
    const uint8_t estado = (PerillaHardware::leerPatitaDigital(pinCLK) << 1) |
                           PerillaHardware::leerPatitaDigital(pinDT);
    if (estado != estadoAnterior)
    {
        const uint8_t cambioEstado = (estadoAnterior << 2) | estado;
        const int8_t delta = deltaCuadratura[cambioEstado];
        estadoAnterior = estado;
        if (delta == 0)
        {
            // Un salto de dos bits es invalido: descartar el ciclo parcial.
            avance = 0;
        }
        else
        {
            avance += delta;
            if (avance >= (int8_t)transicionesPorPaso)
            {
                const int8_t signo = invertido ? -1 : 1;
                posicion += signo;
                direccion = signo > 0 ? HORARIO : ANTIHORARIO;
                avance = 0;
                cambio = true;
            }
            else if (avance <= -(int8_t)transicionesPorPaso)
            {
                const int8_t signo = invertido ? 1 : -1;
                posicion += signo;
                direccion = signo > 0 ? HORARIO : ANTIHORARIO;
                avance = 0;
                cambio = true;
            }
        }
    }

    if (pinSW >= 0)
    {
        const uint32_t ahora = PerillaHardware::tiempoMilisegundos();
        const bool lectura = !PerillaHardware::leerPatitaDigital((uint8_t)pinSW);
        if (lectura != botonLectura)
        {
            botonLectura = lectura;
            cambioBotonMs = ahora;
        }
        else if (botonEstable != botonLectura && (uint32_t)(ahora - cambioBotonMs) >= antirreboteMs)
        {
            botonEstable = botonLectura;
            if (botonEstable)
                eventoPresionado = true;
            else
                eventoLiberado = true;
        }
    }
    return cambio;
}

int32_t EncoderRotativo::getPosicion() const { return posicion; }
EncoderRotativo::Direccion EncoderRotativo::getDireccion() const { return direccion; }
void EncoderRotativo::setPosicion(int32_t nuevaPosicion) { posicion = nuevaPosicion; }
void EncoderRotativo::setTransicionesPorPaso(uint8_t n)
{
    if (n < 1) n = 1;
    if (n > 4) n = 4;
    transicionesPorPaso = n;
    avance = 0;
}
void EncoderRotativo::setInvertido(bool valor) { invertido = valor; }
void EncoderRotativo::setAntirrebote(uint16_t ms) { antirreboteMs = ms; }
bool EncoderRotativo::getBotonPresionado() const { return botonEstable; }
bool EncoderRotativo::fuePresionado() { bool e = eventoPresionado; eventoPresionado = false; return e; }
bool EncoderRotativo::fueLiberado() { bool e = eventoLiberado; eventoLiberado = false; return e; }
