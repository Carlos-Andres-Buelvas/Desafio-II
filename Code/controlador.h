#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "anfitrion.h"
#include "huesped.h"
#include "alojamiento.h"
#include "reserva.h"
#include "fecha.h"

#include <string>

class Controlador {
private:
    // Datos principales del sistema
    Anfitrion* anfitriones;
    Alojamiento* alojamientos;
    Huesped* huespedes;
    Reserva* reservas;

    int cantAnfitriones;
    int cantAlojamientos;
    int cantHuespedes;
    int cantReservas;
    int capReservas;

    // === MÉTODOS INTERNOS ===

    // Control de navegación
    void menuPrincipal();
    void iniciarSesion();
    void mostrarMenuHuesped(Huesped* h);
    void mostrarMenuAnfitrion(Anfitrion* a);

    // Gestión de reservas
    void buscarYReservarAlojamiento(Huesped* h);
    void reservarAlojamiento(Huesped* h);
    void anularReservacion(const std::string& codigo,
                           Huesped* h,
                           Anfitrion* a);
    int obtenerSiguienteNumeroReserva();
    void asegurarCapacidadReservas();

    // Persistencia
    void guardarReservaIndividual(const Reserva& r);
    void sobrescribirArchivoReservas();

    // Utilidades y mantenimiento
    void mostrarHistorico();
    void depurarReservas(const Fecha& fechaCorte);

public:
    // Constructor y destructor
    Controlador();
    ~Controlador();

    // Punto de entrada del sistema
    void iniciar();
};

#endif
