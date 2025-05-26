#include "fecha.h"
#include <iostream>
#include <ctime>
using namespace std;

Fecha::Fecha() {
    dia = 1;
    mes = 1;
    anio = 2000;
}

Fecha::Fecha(int d, int m, int a) {
    dia = d;
    mes = m;
    anio = a;
}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }


void Fecha::setDia(int d) { dia = d; }
void Fecha::setMes(int m) { mes = m; }
void Fecha::setAnio(int a) { anio = a; }

void Fecha::mostrar() const {
    if (dia < 10) cout << "0";
    cout << dia << "/";
    if (mes < 10) cout << "0";
    cout << mes << "/";
    cout << anio << endl;
}

int Fecha::toEntero() const {
    return anio * 10000 + mes * 100 + dia;
}

int Fecha::aDiaDelAnio() const {
    static const int diasPorMes[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    int suma = 0;
    for (int i = 0; i < mes - 1; ++i) {
        suma += diasPorMes[i];
        if (i == 1 && esBisiesto())  // febrero
            suma += 1;
    }

    suma += dia - 1;  // Día 1 de enero = 0
    return suma;
}

bool Fecha::esBisiesto() const {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool Fecha::esMayorQue(const Fecha& otra) const {
    return toEntero() > otra.toEntero();
}

bool Fecha::esMenorQue(const Fecha& otra) const {
    return toEntero() < otra.toEntero();
}

string Fecha::obtenerDiaSemana() const {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int y = anio;
    if (mes < 3) y -= 1;
    int diaSemana = (y + y/4 - y/100 + y/400 + t[mes - 1] + dia) % 7;
    string dias[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
    return dias[diaSemana];
}

void Fecha::mostrarExtendido() const {
    string meses[] = {"enero", "febrero", "marzo", "abril", "mayo", "junio",
                      "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"};

    cout << obtenerDiaSemana() << ", "
         << dia << " de " << meses[mes - 1] << " del " << anio << endl;
}

Fecha Fecha::sumarDias(int dias) const {
    tm t = {};
    t.tm_mday = dia + dias;
    t.tm_mon = mes - 1;
    t.tm_year = anio - 1900;

    mktime(&t); // normaliza la fecha

    return Fecha(t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
}

Fecha getFechaSistema() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return Fecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}
