#include "huesped.h"
#include <iostream>
#include "fecha.h"
#include "reserva.h"
using namespace std;

Huesped::Huesped() {
    documento = "";
    antiguedad = 0;
    puntuacion = 0.0;
    capacidadReservas = 5;
    cantidadReservas = 0;
    reservas = new Reserva*[capacidadReservas];
}

Huesped::Huesped(string doc, int antig, float punt) {
    documento = doc;
    antiguedad = antig;
    puntuacion = punt;
}

string Huesped::getDocumento() const { return documento; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }

void Huesped::setDocumento(const string& doc) { documento = doc; }
void Huesped::setAntiguedad(int antig) { antiguedad = antig; }
void Huesped::setPuntuacion(float punt) { puntuacion = punt; }

void Huesped::mostrar() const {
    cout << "Huésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;
}

bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion())) {
        return false; // No se puede agregar por cruce de fechas
    }

    if (cantidadReservas >= capacidadReservas) {
        capacidadReservas *= 2;
        Reserva** temp = new Reserva*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            temp[i] = reservas[i];
        delete[] reservas;
        reservas = temp;
    }

    reservas[cantidadReservas++] = nueva;
    return true;
}

bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int entrada1 = entrada.toEntero();
    int salida1 = entrada.sumarDias(duracion).toEntero();

    for (int i = 0; i < cantidadReservas; ++i) {
        Fecha otraEntrada = reservas[i]->getFechaEntrada();
        int entrada2 = otraEntrada.toEntero();
        int salida2 = otraEntrada.sumarDias(reservas[i]->getDuracion()).toEntero();

        // Verificar si se cruzan las fechas
        if (!(salida1 <= entrada2 || salida2 <= entrada1)) {
            return true;  // hay cruce
        }
    }

    return false;  // sin conflictos
}

Huesped::~Huesped() {
    delete[] reservas;
}
