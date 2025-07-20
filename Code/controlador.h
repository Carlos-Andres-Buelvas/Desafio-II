#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "anfitrion.h"
#include "huesped.h"
#include "alojamiento.h"
#include "reserva.h"
#include "fecha.h"

#include <string>

/**
 * @class Controlador
 * @brief Clase principal que coordina la lógica de negocio del sistema UdeAStay.
 *
 * Esta clase maneja la interacción con el usuario, la navegación por menús,
 * la gestión de reservas, la autenticación, la carga de datos y la persistencia.
 */
class Controlador {
private:
    // ==== Datos principales del sistema ====

    Anfitrion* anfitriones;         ///< Arreglo dinámico de anfitriones.
    Alojamiento* alojamientos;      ///< Arreglo dinámico de alojamientos.
    Huesped* huespedes;             ///< Arreglo dinámico de huéspedes.
    Reserva* reservas;              ///< Arreglo dinámico de reservas.

    int cantAnfitriones;            ///< Cantidad actual de anfitriones.
    int cantAlojamientos;           ///< Cantidad actual de alojamientos.
    int cantHuespedes;              ///< Cantidad actual de huéspedes.
    int cantReservas;               ///< Cantidad actual de reservas.

    int capReservas;                ///< Capacidad del arreglo de reservas.
    int capAnfitriones;             ///< Capacidad del arreglo de anfitriones.
    int capHuespedes;               ///< Capacidad del arreglo de huéspedes.
    int capAlojamientos;            ///< Capacidad del arreglo de alojamientos.

    // ==== MÉTODOS INTERNOS ====

    /** @brief Muestra el menú principal y gestiona la navegación inicial. */
    void menuPrincipal();

    /** @brief Maneja el proceso de inicio de sesión para usuarios. */
    void iniciarSesion();

    /**
     * @brief Muestra el menú de opciones específicas para un huésped.
     * @param h Puntero al huésped autenticado.
     */
    void mostrarMenuHuesped(Huesped* h);

    /**
     * @brief Muestra el menú de opciones específicas para un anfitrión.
     * @param a Puntero al anfitrión autenticado.
     */
    void mostrarMenuAnfitrion(Anfitrion* a);

    // ==== Gestión de reservas ====

    /**
     * @brief Permite buscar alojamientos disponibles y realizar una reserva.
     * @param h Puntero al huésped que realiza la búsqueda.
     */
    void buscarYReservarAlojamiento(Huesped* h);

    /**
     * @brief Realiza una reserva directamente proporcionando código del alojamiento.
     * @param h Puntero al huésped.
     */
    void reservarAlojamiento(Huesped* h);

    /**
     * @brief Anula una reservación identificada por su código.
     * @param codigo Código de la reserva a anular.
     * @param h Puntero al huésped (si aplica).
     * @param a Puntero al anfitrión (si aplica).
     */
    void anularReservacion(const std::string& codigo,
                           Huesped* h,
                           Anfitrion* a);

    /**
     * @brief Genera el siguiente código único para una reserva.
     * @return Entero que representa el número consecutivo.
     */
    int obtenerSiguienteNumeroReserva();

    /** @brief Verifica y redimensiona el arreglo de reservas si es necesario. */
    void asegurarCapacidadReservas();

    // ==== Persistencia en archivos ====

    /**
     * @brief Guarda una única reserva en el archivo de texto.
     * @param r Referencia a la reserva a guardar.
     */
    void guardarReservaIndividual(const Reserva& r);

    /**
     * @brief Sobrescribe por completo el archivo de reservas con las reservas actuales.
     */
    void sobrescribirArchivoReservas();

    // ==== Utilidades y mantenimiento ====

    /** @brief Muestra todas las reservas históricas registradas. */
    void mostrarHistorico();

    /**
     * @brief Elimina reservas anteriores a la fecha dada.
     * @param fechaCorte Fecha límite a partir de la cual conservar reservas.
     */
    void depurarReservas(const Fecha& fechaCorte);

public:
    /**
     * @brief Constructor. Inicializa el sistema, reserva memoria y carga los datos.
     */
    Controlador();

    /**
     * @brief Destructor. Libera toda la memoria dinámica utilizada.
     */
    ~Controlador();

    /**
     * @brief Función principal que inicia la ejecución del sistema.
     */
    void iniciar();
};

#endif
