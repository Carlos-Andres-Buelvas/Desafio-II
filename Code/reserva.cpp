#include "reserva.h"
#include "alojamiento.h"
#include "huesped.h"
#include <iostream>
#include <cstring>
using namespace std;

Reserva::Reserva() {
    codigo = "";
    alojamiento = nullptr;
    huesped = nullptr;
    duracion = 0;
    metodoPago = "";
    monto = 0;
    anotacion[0] = '\0';
}

Reserva::Reserva(string cod, Alojamiento* alo, Huesped* h, Fecha entrada,
                 int dur, string metodo, Fecha pago, int m, const string& nota) {
    codigo = cod;
    alojamiento = alo;
    huesped = h;
    fechaEntrada = entrada;
    duracion = dur;
    metodoPago = metodo;
    fechaPago = pago;
    monto = m;
    strncpy(anotacion, nota.c_str(), 1000);
    anotacion[1000] = '\0';  // seguridad
}

string Reserva::getCodigo() const { return codigo; }
string Reserva::getMetodoPago() const { return metodoPago; }
int Reserva::getDuracion() const { return duracion; }

Alojamiento* Reserva::getAlojamiento() const { return alojamiento; }
Huesped* Reserva::getHuesped() const { return huesped; }
Fecha Reserva::getFechaEntrada() const { return fechaEntrada; }
Fecha Reserva::getFechaPago() const { return fechaPago; }
int Reserva::getMonto() const { return monto; }
string Reserva::getAnotacion() const { return string(anotacion); }

void Reserva::mostrarComprobante() const {
    Fecha salida = fechaEntrada.sumarDias(duracion);
    Fecha hoySistema = getFechaSistema();

    cout << "------------------------------" << endl;
    cout << "=== COMPROBANTE DE RESERVA ===\n";
    cout << "Reserva: " << codigo << endl;
    cout << "Nombre del huésped: " << (huesped ? huesped->getNombre(): "N/A") << endl;
    cout << "Alojamiento: " << (alojamiento ? alojamiento->getCodigo() : "N/A") << endl;
    cout << "Duración: " << duracion << " noche(s)" << endl;
    cout << "Entrada: "; fechaEntrada.mostrarExtendido();
    cout << "Salida: "; salida.mostrarExtendido();
    cout << "Método de pago: " << metodoPago << endl;
    cout << "Pago realizado el: "; hoySistema.mostrarExtendido();
    cout << "Monto: $" << monto << endl;
    cout << "Anotación: " << anotacion << endl;
    cout << "------------------------------" << endl;
}

bool Reserva::coincideCon(const Fecha& entrada, int dur) const {
    return fechaEntrada.toEntero() == entrada.toEntero() && duracion == dur;
}

// Constructor de copia
Reserva::Reserva(const Reserva& otra) {
    codigo = otra.codigo;
    alojamiento = otra.alojamiento;   // Copia de puntero, no duplicación
    huesped = otra.huesped;
    fechaEntrada = otra.fechaEntrada;
    duracion = otra.duracion;
    metodoPago = otra.metodoPago;
    fechaPago = otra.fechaPago;
    monto = otra.monto;

    // Copiar anotación manualmente (es un arreglo char[1001])
    strcpy(anotacion, otra.anotacion);
}

// Operador de asignación
Reserva& Reserva::operator=(const Reserva& otra) {
    if (this != &otra) {
        codigo = otra.codigo;
        alojamiento = otra.alojamiento;
        huesped = otra.huesped;
        fechaEntrada = otra.fechaEntrada;
        duracion = otra.duracion;
        metodoPago = otra.metodoPago;
        fechaPago = otra.fechaPago;
        monto = otra.monto;

        strcpy(anotacion, otra.anotacion);
    }
    return *this;
}
