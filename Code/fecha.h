#ifndef FECHA_H
#define FECHA_H

#include <string>

/**
 * @class Fecha
 * @brief Representa una fecha con día, mes y año, e incluye utilidades de comparación, formato y manipulación.
 *
 * La clase permite mostrar fechas en diferentes formatos, sumar días, determinar si una fecha es bisiesta
 * o convertirla a diferentes representaciones numéricas útiles para ordenamiento y comparación.
 */
class Fecha {
private:
    int dia;   ///< Día del mes (1-31).
    int mes;   ///< Mes del año (1-12).
    int anio;  ///< Año (ejemplo: 2025).

    /**
     * @brief Devuelve el nombre del día de la semana para la fecha actual.
     * @return Nombre del día (ej. "Lunes").
     */
    std::string obtenerDiaSemana() const;

public:
    /** @brief Constructor por defecto. Inicializa en 01/01/2000. */
    Fecha();

    /**
     * @brief Constructor con parámetros.
     * @param dia Día del mes.
     * @param mes Mes del año.
     * @param anio Año completo.
     */
    Fecha(int dia, int mes, int anio);

    // ==== Getters ====

    /** @return Día de la fecha. */
    int getDia() const;

    /** @return Mes de la fecha. */
    int getMes() const;

    /** @return Año de la fecha. */
    int getAnio() const;

    // ==== Setters ====

    /**
     * @brief Establece el día.
     * @param d Nuevo día.
     */
    void setDia(int d);

    /**
     * @brief Establece el mes.
     * @param m Nuevo mes.
     */
    void setMes(int m);

    /**
     * @brief Establece el año.
     * @param a Nuevo año.
     */
    void setAnio(int a);

    // ==== Utilidades ====

    /** @brief Muestra la fecha en formato corto (dd/mm/aaaa). */
    void mostrar() const;

    /** @brief Muestra la fecha en formato largo, incluyendo el día de la semana. */
    void mostrarExtendido() const;

    /**
     * @brief Retorna una nueva fecha sumando una cantidad de días a la actual.
     * @param dias Número de días a sumar.
     * @return Nueva instancia de Fecha con el resultado.
     */
    Fecha sumarDias(int dias) const;

    /**
     * @brief Convierte la fecha a un entero con formato AAAAMMDD.
     * @return Entero que representa la fecha.
     */
    int toEntero() const;

    /**
     * @brief Compara si esta fecha es posterior a otra.
     * @param otra Fecha contra la cual comparar.
     * @return true si es mayor, false en caso contrario.
     */
    bool esMayorQue(const Fecha& otra) const;

    /**
     * @brief Compara si esta fecha es anterior a otra.
     * @param otra Fecha contra la cual comparar.
     * @return true si es menor, false en caso contrario.
     */
    bool esMenorQue(const Fecha& otra) const;

    /**
     * @brief Indica si el año de la fecha es bisiesto.
     * @return true si es bisiesto, false si no lo es.
     */
    bool esBisiesto() const;

    /**
     * @brief Retorna el número de día dentro del año (1 a 365/366).
     * @return Día del año correspondiente.
     */
    int aDiaDelAnio() const;

    // ==== Manejo de copias ====

    /**
     * @brief Constructor de copia.
     * @param otra Fecha a copiar.
     */
    Fecha(const Fecha& otra);

    /**
     * @brief Operador de asignación.
     * @param otra Fecha a asignar.
     * @return Referencia a la instancia modificada.
     */
    Fecha& operator=(const Fecha& otra);
};

/**
 * @brief Obtiene la fecha actual del sistema (según el reloj del sistema operativo).
 * @return Instancia de Fecha correspondiente al día actual.
 */
Fecha getFechaSistema();

#endif // FECHA_H
