#ifndef HUESPED_H
#define HUESPED_H

#include <string>
#include "fecha.h"
using namespace std;

class Reserva;

class Huesped {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    Reserva** reservas; // arreglo de punteros a reservas
    int cantidadReservas;
    int capacidadReservas;

public:
    Huesped();
    Huesped(string doc, int antig, float punt);

    // Getters
    string getDocumento() const;
    int getAntiguedad() const;
    float getPuntuacion() const;

    // Setters
    void setDocumento(const string& doc);
    void setAntiguedad(int antig);
    void setPuntuacion(float punt);

    // Métodos
    bool agregarReserva(Reserva* nueva);
    bool hayConflicto(const Fecha& entrada, int duracion) const;
    void mostrar() const;

    ~Huesped();
};

#endif // HUESPED_H
