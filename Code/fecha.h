#ifndef FECHA_H
#define FECHA_H

#include <string>
using namespace std;

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha();
    Fecha(int dia, int mes, int anio);

    int getDia() const;
    int getMes() const;
    int getAnio() const;

    void setDia(int d);
    void setMes(int m);
    void setAnio(int a);

    void mostrar() const;
    void mostrarExtendido() const; // nuevo formato con día y nombre del mes
    Fecha sumarDias(int dias) const;
    int toEntero() const;
    bool esMayorQue(const Fecha& otra) const;
    bool esMenorQue(const Fecha& otra) const;
    bool esBisiesto() const;
    int aDiaDelAnio() const;

private:
    string obtenerDiaSemana() const;
};

Fecha getFechaSistema();

#endif // FECHA_H
