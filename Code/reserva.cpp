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
    monto = 0.0;
    anotacion[0] = '\0';
}

Reserva::Reserva(string cod, Alojamiento* alo, Huesped* h, Fecha entrada,
                 int dur, string metodo, Fecha pago, float m, const string& nota) {
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
float Reserva::getMonto() const { return monto; }
string Reserva::getAnotacion() const { return string(anotacion); }

void Reserva::mostrarComprobante() const {
    cout << "------------------------------" << endl;
    cout << "Reserva: " << codigo << endl;
    cout << "Alojamiento: " << (alojamiento ? alojamiento->getCodigo() : "N/A") << endl;
    cout << "Duración: " << duracion << " noche(s)" << endl;
    cout << "Entrada: "; fechaEntrada.mostrarExtendido();
    cout << "Método de pago: " << metodoPago << endl;
    cout << "Pago realizado el: "; fechaPago.mostrarExtendido();
    cout << "Monto: $" << monto << endl;
    cout << "Anotación: " << anotacion << endl;
    cout << "------------------------------" << endl;
}

bool Reserva::coincideCon(const Fecha& entrada, int dur) const {
    return fechaEntrada.toEntero() == entrada.toEntero() && duracion == dur;
}
