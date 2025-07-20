#include "fecha.h"
#include <iostream>
#include <ctime>
#include <iomanip>

// Variables globales para eficiencia
extern int totalIteraciones;

/**
 * @brief Constructor por defecto.
 * Inicializa la fecha con valores por defecto: 01/01/2000.
 */
Fecha::Fecha() : dia(1), mes(1), anio(2000) {}

/**
 * @brief Constructor parametrizado.
 * @param d Día.
 * @param m Mes.
 * @param a Año.
 */
Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

// ==== Getters ====
int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

// ==== Setters ====
void Fecha::setDia(int d) { dia = d; }
void Fecha::setMes(int m) { mes = m; }
void Fecha::setAnio(int a) { anio = a; }

/**
 * @brief Muestra la fecha en formato dd/mm/aaaa.
 */
void Fecha::mostrar() const {
    std::cout << std::setfill('0') << std::setw(2) << dia << "/"
              << std::setw(2) << mes << "/"
              << anio << std::endl;
}

/**
 * @brief Convierte la fecha a formato entero AAAAMMDD.
 * @return Fecha como número entero.
 */
int Fecha::toEntero() const {
    return anio * 10000 + mes * 100 + dia;
}

/**
 * @brief Verifica si el año es bisiesto.
 * @return true si es bisiesto, false en caso contrario.
 */
bool Fecha::esBisiesto() const {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

/**
 * @brief Convierte la fecha a número de día del año.
 * @return Número del día (0 a 365).
 */
int Fecha::aDiaDelAnio() const {
    static const int diasPorMes[12] = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};
    int suma = 0;
    for (int i = 0; i < mes - 1; ++i) {
        totalIteraciones++;
        suma += diasPorMes[i];
        if (i == 1 && esBisiesto()) suma++; // Ajuste para febrero bisiesto
    }
    return suma + dia - 1;
}

/**
 * @brief Compara si esta fecha es mayor que otra.
 * @param otra Fecha con la que se compara.
 * @return true si esta fecha es posterior.
 */
bool Fecha::esMayorQue(const Fecha& otra) const {
    return toEntero() > otra.toEntero();
}

/**
 * @brief Compara si esta fecha es menor que otra.
 * @param otra Fecha con la que se compara.
 * @return true si esta fecha es anterior.
 */
bool Fecha::esMenorQue(const Fecha& otra) const {
    return toEntero() < otra.toEntero();
}

/**
 * @brief Obtiene el nombre del día de la semana.
 * @return Nombre del día (e.g., "Lunes").
 */
std::string Fecha::obtenerDiaSemana() const {
    static const std::string dias[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
    static const int ajustes[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    int y = anio;
    if (mes < 3) y -= 1;

    int diaSemana = (y + y / 4 - y / 100 + y / 400 + ajustes[mes - 1] + dia) % 7;
    return dias[diaSemana];
}

/**
 * @brief Muestra la fecha en formato extendido.
 * Ejemplo: "Lunes, 01 de enero del 2025".
 */
void Fecha::mostrarExtendido() const {
    static const std::string meses[] = {
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    std::cout << obtenerDiaSemana() << ", "
              << dia << " de " << meses[mes - 1]
              << " del " << anio << std::endl;
}

/**
 * @brief Suma una cantidad de días a la fecha actual.
 * Usa mktime para ajustar correctamente meses y años.
 * @param dias Número de días a sumar.
 * @return Nueva fecha resultante.
 */
Fecha Fecha::sumarDias(int dias) const {
    std::tm t = {};
    t.tm_mday = dia + dias;
    t.tm_mon = mes - 1;
    t.tm_year = anio - 1900;

    std::mktime(&t); // Normaliza la estructura de fecha

    return Fecha(t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
}

/**
 * @brief Obtiene la fecha actual del sistema.
 * @return Fecha correspondiente al día de ejecución.
 */
Fecha getFechaSistema() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    return Fecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

/**
 * @brief Constructor de copia.
 * @param otra Objeto fecha a copiar.
 */
Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

/**
 * @brief Operador de asignación.
 * @param otra Fecha fuente.
 * @return Referencia a la fecha modificada.
 */
Fecha& Fecha::operator=(const Fecha& otra) {
    if (this != &otra) {
        dia = otra.dia;
        mes = otra.mes;
        anio = otra.anio;
    }
    return *this;
}
