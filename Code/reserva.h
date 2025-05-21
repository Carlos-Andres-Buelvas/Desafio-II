#ifndef RESERVA_H
#define RESERVA_H

#include <string>
#include "fecha.h"
using namespace std;

// Declaración adelantada
class Huesped;
class Alojamiento;

class Reserva {
private:
    string codigo;
    Alojamiento* alojamiento;
    Huesped* huesped;
    Fecha fechaEntrada;
    int duracion;
    string metodoPago;
    Fecha fechaPago;
    float monto;
    char anotacion[1001];

public:
    Reserva();
    Reserva(string cod, Alojamiento* alo, Huesped* h,
            Fecha entrada, int dur, string metodo, Fecha pago, float monto, const string& nota);

    // Getters
    string getCodigo() const;
    string getMetodoPago() const;
    int getDuracion() const;
    Fecha getFechaEntrada() const;
    Alojamiento* getAlojamiento() const;
    Huesped* getHuesped() const;
    Fecha getFechaPago() const;
    float getMonto() const;
    string getAnotacion() const;


    // Métodos funcionales
    void mostrarComprobante() const;
    bool coincideCon(const Fecha& entrada, int duracion) const;
};

#endif // RESERVA_H
