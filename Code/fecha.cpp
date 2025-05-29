#include "fecha.h"
#include <iostream>
#include <ctime>
#include <iomanip>

// Constructor por defecto
Fecha::Fecha() : dia(1), mes(1), anio(2000) {}

// Constructor con parámetros
Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

// Getters
int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

// Setters
void Fecha::setDia(int d) { dia = d; }
void Fecha::setMes(int m) { mes = m; }
void Fecha::setAnio(int a) { anio = a; }

// Mostrar fecha en formato dd/mm/aaaa
void Fecha::mostrar() const {
    std::cout << std::setfill('0') << std::setw(2) << dia << "/"
              << std::setw(2) << mes << "/"
              << anio << std::endl;
}

// Convertir a formato AAAAMMDD como entero
int Fecha::toEntero() const {
    return anio * 10000 + mes * 100 + dia;
}

// Determinar si es bisiesto
bool Fecha::esBisiesto() const {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// Convertir a día del año (0-365)
int Fecha::aDiaDelAnio() const {
    static const int diasPorMes[12] = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};
    int suma = 0;
    for (int i = 0; i < mes - 1; ++i) {
        suma += diasPorMes[i];
        if (i == 1 && esBisiesto()) suma++; // febrero bisiesto
    }
    return suma + dia - 1;
}

// Comparación
bool Fecha::esMayorQue(const Fecha& otra) const {
    return toEntero() > otra.toEntero();
}

bool Fecha::esMenorQue(const Fecha& otra) const {
    return toEntero() < otra.toEntero();
}

// Obtener nombre del día de la semana (usando algoritmo de Zeller modificado)
std::string Fecha::obtenerDiaSemana() const {
    static const std::string dias[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
    static const int ajustes[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    int y = anio;
    if (mes < 3) y -= 1;

    int diaSemana = (y + y / 4 - y / 100 + y / 400 + ajustes[mes - 1] + dia) % 7;
    return dias[diaSemana];
}

// Mostrar fecha en formato extendido
void Fecha::mostrarExtendido() const {
    static const std::string meses[] = {
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    std::cout << obtenerDiaSemana() << ", "
              << dia << " de " << meses[mes - 1]
              << " del " << anio << std::endl;
}

// Retorna una nueva fecha sumando días (usa mktime)
Fecha Fecha::sumarDias(int dias) const {
    std::tm t = {};
    t.tm_mday = dia + dias;
    t.tm_mon = mes - 1;
    t.tm_year = anio - 1900;

    std::mktime(&t); // normaliza la fecha

    return Fecha(t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
}

// Obtener fecha actual del sistema
Fecha getFechaSistema() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    return Fecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

// Constructor de copia
Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

// Operador de asignación
Fecha& Fecha::operator=(const Fecha& otra) {
    if (this != &otra) {
        dia = otra.dia;
        mes = otra.mes;
        anio = otra.anio;
    }
    return *this;
}
