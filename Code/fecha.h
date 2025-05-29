#ifndef FECHA_H
#define FECHA_H

#include <string>

class Fecha {
private:
    int dia;
    int mes;
    int anio;

    // Devuelve el nombre del día de la semana (e.g., "Lunes")
    std::string obtenerDiaSemana() const;

public:
    // Constructores
    Fecha();
    Fecha(int dia, int mes, int anio);

    // Getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;

    // Setters
    void setDia(int d);
    void setMes(int m);
    void setAnio(int a);

    // Utilidades
    void mostrar() const;
    void mostrarExtendido() const;        // Muestra "Lunes, 15 de enero de 2025"
    Fecha sumarDias(int dias) const;      // Retorna una nueva fecha sumando días
    int toEntero() const;                 // Retorna formato AAAAMMDD como entero
    bool esMayorQue(const Fecha& otra) const;
    bool esMenorQue(const Fecha& otra) const;
    bool esBisiesto() const;
    int aDiaDelAnio() const;              // Retorna el día dentro del año (1-365/366)

    // En fecha.h dentro de la clase Fecha
    Fecha(const Fecha& otra);               // Constructor de copia
    Fecha& operator=(const Fecha& otra);   // Operador de asignación
};

// Devuelve la fecha actual del sistema
Fecha getFechaSistema();

#endif // FECHA_H
