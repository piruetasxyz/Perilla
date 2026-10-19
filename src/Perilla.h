#ifndef PERILLA_H
#define PERILLA_H

#include <stdint.h>

/**
 * \~spanish
 * @brief Lee una perilla y entrega su valor.
 *
 * Puede ser un potenciometro en una patita analoga, o un encoder
 * en dos patitas digitales. El tipo se elige al crearla.
 *
 * En un potenciometro mapea la lectura a otro rango. Por defecto ambos
 * rangos son de 0 a 1023, asi que getValorMapeado() entrega lo mismo
 * que getValor() hasta que se configuren.
 *
 * En un encoder, cada clic mueve el valor dentro del rango mapeado,
 * asi que getValorMapeado() funciona igual con los dos tipos. Ademas
 * leer() actualiza la direccion y los pasos.
 *
 * El filtro para reducir el ruido esta desactivado hasta que se llame
 * a setFiltro().
 *
 * \~english
 * @brief Reads a knob and returns its value.
 *
 * It can be a potentiometer on one analog pin, or an encoder on two
 * digital pins. The kind is chosen when it is created.
 *
 * On a potentiometer it maps the reading to another range. By default
 * both ranges are 0 to 1023, so getValorMapeado() returns the same as
 * getValor() until they are configured.
 *
 * On an encoder, each click moves the value within the mapped range,
 * so getValorMapeado() works the same with both kinds. leer() also
 * updates the direction and the steps.
 *
 * The noise filter stays disabled until setFiltro() is called.
 */
class Perilla
{
public:
    /**
     * \~spanish
     * @brief Tipo de perilla que se elige al crearla.
     *
     * \~english
     * @brief Kind of knob chosen when it is created.
     */
    enum Tipo
    {
        /**
         * \~spanish
         * Potenciometro en una patita analoga.
         *
         * \~english
         * Potentiometer on one analog pin.
         */
        POTENCIOMETRO,

        /**
         * \~spanish
         * Encoder en dos patitas digitales.
         *
         * \~english
         * Encoder on two digital pins.
         */
        ENCODER
    };

    /**
     * \~spanish
     * @brief Direccion del ultimo giro visto por leer().
     *
     * \~english
     * @brief Direction of the last turn seen by leer().
     */
    enum Direccion
    {
        /**
         * \~spanish
         * En esa lectura la perilla no se movio.
         *
         * \~english
         * The knob did not move on that reading.
         */
        QUIETA,

        /**
         * \~spanish
         * Giro en el sentido del reloj.
         *
         * \~english
         * Clockwise turn.
         */
        HORARIO,

        /**
         * \~spanish
         * Giro en el sentido contrario al reloj.
         *
         * \~english
         * Counterclockwise turn.
         */
        ANTIHORARIO
    };

    /**
     * \~spanish
     * @brief Crea un potenciometro y configura la patita como entrada analoga.
     *
     * Una perilla es un potenciometro por defecto. Para un encoder, que usa
     * dos patitas, se crea con Perilla(patitaA, patitaB, Perilla::ENCODER).
     *
     * @param nuevaPatita patita donde esta conectada la patita del medio de la perilla.
     *
     * \~english
     * @brief Creates a potentiometer and configures the pin as an analog input.
     *
     * A knob is a potentiometer by default. For an encoder, which uses
     * two pins, create it with Perilla(patitaA, patitaB, Perilla::ENCODER).
     *
     * @param nuevaPatita pin the knob's middle leg is connected to.
     */
    Perilla(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Crea un encoder en dos patitas, o un potenciometro si el tipo no es ENCODER.
     *
     * El comun del encoder va a tierra. Las dos patitas quedan como
     * entrada con pull-up. Si el tipo es POTENCIOMETRO, solo se usa
     * la primera patita.
     *
     * @param nuevaPatitaA patita A del encoder, o patita analoga del potenciometro.
     * @param nuevaPatitaB patita B del encoder.
     * @param nuevoTipo POTENCIOMETRO o ENCODER.
     *
     * \~english
     * @brief Creates an encoder on two pins, or a potentiometer if the kind is not ENCODER.
     *
     * The encoder common goes to ground. Both pins are configured as
     * pull-up inputs. If the kind is POTENCIOMETRO, only the first pin
     * is used.
     *
     * @param nuevaPatitaA encoder pin A, or the potentiometer analog pin.
     * @param nuevaPatitaB encoder pin B.
     * @param nuevoTipo POTENCIOMETRO or ENCODER.
     */
    Perilla(uint8_t nuevaPatitaA, uint8_t nuevaPatitaB, Tipo nuevoTipo);

    /**
     * \~spanish
     * @brief Cambia la patita guardada, sin volver a configurarla.
     * @param nuevaPatita nueva patita.
     *
     * \~english
     * @brief Changes the stored pin, without configuring it again.
     * @param nuevaPatita new pin.
     */
    void setPatita(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Configura el rango que entrega la placa al leer.
     *
     * Por ejemplo de 0 a 1023 en Arduino Uno, o de 0 a 4095 en Raspberry Pi Pico.
     * En un encoder no se usa.
     *
     * @param nuevoValorLeidoMin valor minimo leido.
     * @param nuevoValorLeidoMax valor maximo leido.
     *
     * \~english
     * @brief Configures the range the board returns when reading.
     *
     * For example 0 to 1023 on Arduino Uno, or 0 to 4095 on Raspberry Pi Pico.
     * It is not used on an encoder.
     *
     * @param nuevoValorLeidoMin minimum read value.
     * @param nuevoValorLeidoMax maximum read value.
     */
    void setRangoLeido(uint16_t nuevoValorLeidoMin, uint16_t nuevoValorLeidoMax);

    /**
     * \~spanish
     * @brief Configura el rango al que se convierte la lectura.
     *
     * Por ejemplo de 0 a 100 para porcentaje, o de 0 a 255 para el brillo de un led.
     *
     * En un encoder es el rango en que se mueve el valor. Parte en el minimo,
     * cada clic horario avanza hacia el maximo, y se detiene en los extremos.
     *
     * @param nuevoValorMapeadoMin valor minimo mapeado.
     * @param nuevoValorMapeadoMax valor maximo mapeado.
     *
     * \~english
     * @brief Configures the range the reading is converted to.
     *
     * For example 0 to 100 for a percentage, or 0 to 255 for an LED's brightness.
     *
     * On an encoder it is the range the value moves in. It starts at the
     * minimum, each clockwise click moves toward the maximum, and it stops
     * at the ends.
     *
     * @param nuevoValorMapeadoMin minimum mapped value.
     * @param nuevoValorMapeadoMax maximum mapped value.
     */
    void setRangoMapeado(uint16_t nuevoValorMapeadoMin, uint16_t nuevoValorMapeadoMax);

    /**
     * \~spanish
     * @brief Activa un filtro para suavizar el ruido de la lectura.
     *
     * El porcentaje es la parte de la lectura nueva que entra en cada leer().
     * Por ejemplo, 20 incorpora un 20% de la lectura nueva y conserva un 80%
     * del valor anterior. 100 sigue la lectura nueva por completo.
     * Un 0 se toma como 1, y un valor mayor que 100 se toma como 100.
     *
     * Al activarlo, el filtro empieza desde la proxima lectura, no desde cero.
     * Cambiar el porcentaje mientras ya esta activo no reinicia ese valor.
     * En un encoder no se usa.
     *
     * @param porcentaje porcentaje de la lectura nueva, de 1 a 100.
     *
     * \~english
     * @brief Enables a filter to smooth noise in the reading.
     *
     * The percentage is how much of the new reading is mixed in on each leer().
     * For example, 20 mixes in 20% of the new reading and keeps 80% of the
     * previous value. 100 follows the new reading completely.
     * A 0 is treated as 1, and a value above 100 is treated as 100.
     *
     * When enabled, the filter starts from the next reading, not from zero.
     * Changing the percentage while it is already active does not restart
     * that value. It is not used on an encoder.
     *
     * @param porcentaje percentage of the new reading, from 1 to 100.
     */
    void setFiltro(uint8_t porcentaje);

    /**
     * \~spanish
     * @brief Desactiva el filtro.
     *
     * No cambia el ultimo valor guardado. La proxima leer() vuelve a guardar
     * la lectura directa de la patita.
     *
     * \~english
     * @brief Disables the filter.
     *
     * It does not change the last stored value. The next leer() stores the
     * direct pin reading again.
     */
    void quitarFiltro();

    /**
     * \~spanish
     * @brief Cuantos pasos suma o resta cada clic del encoder,
     * en los pasos y en el valor.
     *
     * El valor inicial es 1. Un 0 se toma como 1.
     * En un potenciometro no se usa.
     *
     * @param pasosPorClic pasos de cada clic.
     *
     * \~english
     * @brief How many steps each encoder click adds or subtracts,
     * to the steps and to the value.
     *
     * The initial value is 1. A 0 is treated as 1.
     * It is not used on a potentiometer.
     *
     * @param pasosPorClic steps per click.
     */
    void setSensibilidad(uint8_t pasosPorClic);

    /**
     * \~spanish
     * @brief Lee la perilla.
     *
     * En un potenciometro lee la patita y calcula el valor mapeado.
     * En un encoder actualiza el valor, la direccion y los pasos. Hay que llamarlo
     * en cada vuelta del loop, porque un giro que no se alcanza a ver
     * no vuelve.
     *
     * \~english
     * @brief Reads the knob.
     *
     * On a potentiometer it reads the pin and calculates the mapped value.
     * On an encoder it updates the value, the direction and the steps. Call it on
     * every pass of the loop, because a turn that is not seen does not
     * come back.
     */
    void leer();

    /**
     * \~spanish
     * @brief Ultima lectura del potenciometro, sin mapear.
     *
     * Si el filtro esta activo, es la lectura filtrada.
     * En un encoder es igual a getValorMapeado().
     *
     * @return valor dentro del rango leido.
     *
     * \~english
     * @brief Last potentiometer reading, not mapped.
     *
     * If the filter is active, it is the filtered reading.
     * On an encoder it is the same as getValorMapeado().
     *
     * @return value within the read range.
     */
    uint16_t getValor();

    /**
     * \~spanish
     * @brief Valor de la perilla, dentro del rango mapeado.
     *
     * En un potenciometro es la ultima lectura convertida al rango mapeado.
     * En un encoder es la posicion a la que llevaron los clics.
     *
     * @return valor dentro del rango mapeado.
     *
     * \~english
     * @brief Knob value, within the mapped range.
     *
     * On a potentiometer it is the last reading converted to the mapped range.
     * On an encoder it is the position the clicks have moved it to.
     *
     * @return value within the mapped range.
     */
    uint16_t getValorMapeado();

    /**
     * \~spanish
     * @brief Direccion vista por la ultima leer().
     *
     * Si en esa lectura no hubo un clic, es QUIETA.
     * En un potenciometro se queda en QUIETA.
     *
     * @return HORARIO, ANTIHORARIO o QUIETA.
     *
     * \~english
     * @brief Direction seen by the last leer().
     *
     * If that reading had no click, it is QUIETA.
     * On a potentiometer it stays QUIETA.
     *
     * @return HORARIO, ANTIHORARIO, or QUIETA.
     */
    Direccion getDireccion();

    /**
     * \~spanish
     * @brief Pasos acumulados del encoder.
     *
     * Cada clic suma o resta la sensibilidad. Puede ser negativo.
     * Parte en 0. En un potenciometro se queda en 0.
     *
     * @return total de pasos.
     *
     * \~english
     * @brief Accumulated encoder steps.
     *
     * Each click adds or subtracts the sensitivity. It can be negative.
     * It starts at 0. On a potentiometer it stays at 0.
     *
     * @return total steps.
     */
    int32_t getPasos();

private:
    void iniciar(uint8_t nuevaPatitaA, uint8_t nuevaPatitaB, Tipo nuevoTipo);
    void leerEncoder();
    void actualizarValorEncoder();

    uint8_t patita;
    uint8_t patitaB;
    Tipo tipo;
    uint16_t valorLeido;
    uint16_t valorLeidoMin;
    uint16_t valorLeidoMax;
    uint16_t valorMapeado;
    uint16_t valorMapeadoMin;
    uint16_t valorMapeadoMax;
    uint8_t sensibilidad;
    Direccion direccion;
    int32_t pasos;
    int32_t posicionEncoder;
    uint8_t estadoAnteriorEncoder;
    int8_t avanceEncoder;
    bool filtroActivo;
    bool filtroIniciado;
    uint8_t porcentajeFiltro;
    int32_t valorFiltradoEscalado;
};

/**
 * \~spanish
 * @brief Lee un encoder rotativo incremental en CLK y DT, con boton SW opcional.
 *
 * Es independiente de Perilla, que conserva su comportamiento previo para
 * potenciometros. Llame actualizar() frecuentemente en cada vuelta del loop:
 * el encoder se sondea y una transicion ocurrida entre lecturas puede perderse.
 * Las entradas usan pull-up interno y se conectan al comun del encoder.
 *
 * \~english
 * @brief Reads an incremental rotary encoder on CLK and DT, with optional SW button.
 *
 * It is independent of Perilla, which keeps its previous potentiometer behavior.
 * Call actualizar() frequently on every loop iteration: the encoder is polled
 * and a transition between reads can be missed. Inputs use internal pull-ups
 * and connect to the encoder common terminal.
 */
class EncoderRotativo
{
public:
    /**
     * \~spanish
     * @brief Direccion del ultimo paso detectado por actualizar().
     * \~english
     * @brief Direction of the last step detected by actualizar().
     */
    enum Direccion { QUIETA, HORARIO, ANTIHORARIO };

    /**
     * \~spanish
     * @brief Crea el encoder y configura CLK, DT y opcionalmente SW como entradas pull-up.
     * @param pinCLK pin CLK.
     * @param pinDT pin DT.
     * @param pinSW pin SW; usa -1 si no hay boton.
     *
     * \~english
     * @brief Creates the encoder and configures CLK, DT and optional SW as pull-up inputs.
     * @param pinCLK CLK pin.
     * @param pinDT DT pin.
     * @param pinSW SW pin; use -1 when there is no button.
     */
    EncoderRotativo(uint8_t pinCLK, uint8_t pinDT, int16_t pinSW = -1);

    /**
     * \~spanish
     * @brief Sondea encoder y boton; llamar frecuentemente, sin demoras largas en el loop.
     * @return true si cambio la posicion.
     * \~english
     * @brief Polls encoder and button; call frequently without long delays in the loop.
     * @return true if the position changed.
     */
    bool actualizar();

    /**
     * \~spanish
     * @brief Sentido del ultimo paso visto; QUIETA si no hubo paso en la lectura.
     * \~english
     * @brief Direction of the last observed step; QUIETA if this poll had no step.
     */
    Direccion getDireccion() const;

    /**
     * \~spanish
     * @brief Posicion firmada actual.
     * @return posicion positiva o negativa.
     * \~english
     * @brief Current signed position.
     * @return positive or negative position.
     */
    int32_t getPosicion() const;

    /**
     * \~spanish
     * @brief Establece la posicion sin cambiar el estado de cuadratura.
     * @param nuevaPosicion nueva posicion.
     * \~english
     * @brief Sets the position without changing the quadrature state.
     * @param nuevaPosicion new position.
     */
    void setPosicion(int32_t nuevaPosicion);

    /**
     * \~spanish
     * @brief Configura cuantas transiciones validas forman un paso; por defecto 4.
     * @param transiciones valor de 1 a 4; otros valores se limitan al rango.
     * \~english
     * @brief Sets the valid transitions per step; defaults to 4.
     * @param transiciones value from 1 to 4; other values are clamped.
     */
    void setTransicionesPorPaso(uint8_t transiciones);

    /**
     * \~spanish
     * @brief Invierte el signo del siguiente giro.
     * @param invertir true para invertir.
     * \~english
     * @brief Reverses the sign of the next turn.
     * @param invertir true to reverse.
     */
    void setInvertido(bool invertir);

    /**
     * \~spanish
     * @brief Configura el antirrebote del boton, en milisegundos (por defecto 25).
     * @param milisegundos duracion de estabilidad requerida.
     * \~english
     * @brief Sets button debounce time in milliseconds (25 by default).
     * @param milisegundos required stable duration.
     */
    void setAntirrebote(uint16_t milisegundos);

    /**
     * \~spanish
     * @brief Estado estable del boton; true cuando SW esta presionado.
     * @return estado del boton.
     * \~english
     * @brief Stable button state; true while SW is pressed.
     * @return button state.
     */
    bool getBotonPresionado() const;

    /**
     * \~spanish
     * @brief Devuelve true una sola vez al detectar una nueva pulsacion estable.
     * @return evento de pulsacion pendiente.
     * \~english
     * @brief Returns true once when a new stable press is detected.
     * @return pending press event.
     */
    bool fuePresionado();

    /**
     * \~spanish
     * @brief Devuelve true una sola vez al detectar una liberacion estable.
     * @return evento de liberacion pendiente.
     * \~english
     * @brief Returns true once when a stable release is detected.
     * @return pending release event.
     */
    bool fueLiberado();

private:
    uint8_t pinCLK;
    uint8_t pinDT;
    int16_t pinSW;
    uint8_t estadoAnterior;
    int8_t avance;
    int32_t posicion;
    uint8_t transicionesPorPaso;
    Direccion direccion;
    bool invertido;
    uint16_t antirreboteMs;
    bool botonLectura;
    bool botonEstable;
    uint32_t cambioBotonMs;
    bool eventoPresionado;
    bool eventoLiberado;
};

#endif
