#ifndef RESERVA_H
#define RESERVA_H

#include <string>
#include "fecha.h"

// Declaraciones adelantadas para evitar inclusión circular
class Huesped;
class Alojamiento;

/**
 * @class Reserva
 * @brief Representa una reserva de alojamiento realizada por un huésped.
 *
 * Contiene toda la información relevante sobre la reserva: alojamiento, huésped, fecha de entrada,
 * duración, método y fecha de pago, monto pagado y anotaciones opcionales.
 */
class Reserva {
private:
    std::string codigo;         ///< Código único de la reserva (e.g., RSV001).
    Alojamiento* alojamiento;   ///< Puntero al alojamiento reservado.
    Huesped* huesped;           ///< Puntero al huésped que realizó la reserva.
    Fecha fechaEntrada;         ///< Fecha de entrada al alojamiento.
    int duracion;               ///< Número de noches reservadas.
    std::string metodoPago;     ///< Método de pago ("PSE", "TCredito", etc).
    Fecha fechaPago;            ///< Fecha en la que se efectuó el pago.
    int monto;                  ///< Monto total pagado.
    std::string anotacion;      ///< Comentario adicional opcional (observaciones del huésped).

public:
    // ====== Constructores ======

    /**
     * @brief Constructor por defecto.
     * Inicializa todos los valores con datos neutros.
     */
    Reserva();

    /**
     * @brief Constructor parametrizado.
     * @param cod Código de la reserva.
     * @param alo Puntero al alojamiento reservado.
     * @param h Puntero al huésped que reserva.
     * @param entrada Fecha de entrada.
     * @param dur Duración de la reserva (en noches).
     * @param metodo Método de pago.
     * @param pago Fecha del pago.
     * @param monto Monto pagado.
     * @param nota Anotación adicional.
     */
    Reserva(const std::string& cod, Alojamiento* alo, Huesped* h,
            const Fecha& entrada, int dur, const std::string& metodo,
            const Fecha& pago, int monto, const std::string& nota);

    /**
     * @brief Constructor de copia.
     * @param otra Reserva a copiar.
     */
    Reserva(const Reserva& otra);

    // ====== Operador de asignación ======

    /**
     * @brief Operador de asignación.
     * Copia los datos de otra reserva (punteros incluidos, no se realiza copia profunda).
     * @param otra Reserva fuente.
     * @return Referencia al objeto actual.
     */
    Reserva& operator=(const Reserva& otra);

    // ====== Getters ======

    /** @return Código de la reserva. */
    std::string getCodigo() const;

    /** @return Puntero al alojamiento reservado. */
    Alojamiento* getAlojamiento() const;

    /** @return Puntero al huésped asociado. */
    Huesped* getHuesped() const;

    /** @return Fecha de entrada. */
    Fecha getFechaEntrada() const;

    /** @return Duración en noches. */
    int getDuracion() const;

    /** @return Método de pago. */
    std::string getMetodoPago() const;

    /** @return Fecha de pago. */
    Fecha getFechaPago() const;

    /** @return Monto pagado. */
    int getMonto() const;

    /** @return Anotación adicional. */
    std::string getAnotacion() const;

    // ====== Funcionalidad ======

    /**
     * @brief Muestra en consola el comprobante completo de la reserva.
     */
    void mostrarComprobante() const;

    /**
     * @brief Compara si la reserva coincide con una entrada y duración dada.
     * @param entrada Fecha de entrada buscada.
     * @param duracion Duración esperada.
     * @return true si la reserva coincide, false si no.
     */
    bool coincideCon(const Fecha& entrada, int duracion) const;

    // ====== Carga desde archivo ======

    /**
     * @brief Carga las reservas desde un archivo TXT y las enlaza con huéspedes y alojamientos.
     *
     * Realiza validaciones cruzadas, evita conflictos de fechas y redimensiona el arreglo si es necesario.
     *
     * @param archivo Ruta del archivo de reservas.
     * @param arreglo Puntero a arreglo de reservas a llenar.
     * @param cantidad Número de reservas cargadas.
     * @param capacidad Capacidad total del arreglo de reservas.
     * @param alojamientos Arreglo de alojamientos existentes.
     * @param cantAlojamientos Número de alojamientos.
     * @param huespedes Arreglo de huéspedes existentes.
     * @param cantHuespedes Número de huéspedes.
     */
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Reserva*& arreglo,
                                   int& cantidad,
                                   int& capacidad,
                                   Alojamiento* alojamientos,
                                   int cantAlojamientos,
                                   Huesped* huespedes,
                                   int cantHuespedes);
};

#endif // RESERVA_H
