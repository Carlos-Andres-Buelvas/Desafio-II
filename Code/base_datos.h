#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include "reserva.h"
#include "fecha.h"

void cargarBaseDatos(Anfitrion*& anfitriones, int& cantAnfitriones,
                     Alojamiento*& alojamientos, int& cantAlojamientos,
                     Huesped*& huespedes, int& cantHuespedes,
                     Reserva*& reservas, int& cantReservas, int& capacidad);

void depurarReservas(Reserva*& reservas, int& cantidad, const Fecha& fechaCorte);

void mostrarHistorico();

#endif // BASE_DATOS_H
