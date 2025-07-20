#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"

/**
 * @class Anfitrion
 * @brief Representa a un anfitrión en el sistema UdeAStay.
 *
 * Un anfitrión tiene alojamientos asociados y características como documento,
 * nombre, clave, antigüedad, y puntuación. Puede administrar sus alojamientos.
 */
class Anfitrion {
private:
    std::string documento;            ///< Número de documento del anfitrión.
    std::string nombre;               ///< Nombre del anfitrión.
    std::string clave;                ///< Clave de acceso.
    int antiguedad;                   ///< Antigüedad en meses.
    float puntuacion;                 ///< Puntuación promedio.

    int cantidadAlojamientos;         ///< Cantidad actual de alojamientos asignados.
    int capacidadAlojamientos;        ///< Capacidad máxima del arreglo.
    Alojamiento** alojamientos;       ///< Arreglo dinámico de punteros a alojamientos.

public:
    /** @brief Constructor por defecto. Inicializa con valores neutros y capacidad mínima. */
    Anfitrion();

    /**
     * @brief Constructor con parámetros básicos.
     * @param documento Documento del anfitrión.
     * @param nombre Nombre del anfitrión.
     * @param antiguedad Antigüedad en meses.
     * @param puntuacion Puntuación promedio.
     */
    Anfitrion(const std::string& documento, const std::string& nombre, int antiguedad, float puntuacion);

    /**
     * @brief Constructor de copia (shallow copy de alojamientos).
     * @param otro Otro objeto anfitrión.
     */
    Anfitrion(const Anfitrion& otro);

    /**
     * @brief Operador de asignación (shallow copy de alojamientos).
     * @param otro Objeto anfitrión fuente.
     * @return Referencia al anfitrión modificado.
     */
    Anfitrion& operator=(const Anfitrion& otro);

    /** @brief Destructor. Libera la memoria del arreglo de alojamientos. */
    ~Anfitrion();

    // ======= Getters =======

    /** @return Documento del anfitrión. */
    std::string getDocumento() const;

    /** @return Nombre del anfitrión. */
    std::string getNombre() const;

    /** @return Antigüedad en meses. */
    int getAntiguedad() const;

    /** @return Puntuación promedio del anfitrión. */
    float getPuntuacion() const;

    /** @return Clave del anfitrión. */
    std::string getClave() const;

    // ======= Setters =======

    /** @brief Establece el documento del anfitrión. */
    void setDocumento(const std::string& doc);

    /** @brief Establece la antigüedad en meses. */
    void setAntiguedad(int antig);

    /** @brief Establece la puntuación promedio. */
    void setPuntuacion(float punt);

    /** @brief Establece la clave del anfitrión. */
    void setClave(const std::string& c);

    // ======= Métodos funcionales =======

    /**
     * @brief Agrega un nuevo alojamiento al arreglo.
     *
     * Redimensiona el arreglo si ya se alcanzó la capacidad máxima.
     * @param nuevo Puntero al nuevo alojamiento.
     */
    void agregarAlojamiento(Alojamiento* nuevo);

    /**
     * @brief Muestra los datos del anfitrión y de sus alojamientos.
     */
    void mostrar() const;

    /**
     * @brief Devuelve la cantidad actual de alojamientos asociados.
     * @return Cantidad de alojamientos.
     */
    int getCantidadAlojamientos() const { return cantidadAlojamientos; }

    /**
     * @brief Obtiene un alojamiento dado su índice.
     * @param i Índice del alojamiento.
     * @return Puntero al alojamiento, o nullptr si el índice es inválido.
     */
    Alojamiento* getAlojamiento(int i) const {
        return (i >= 0 && i < cantidadAlojamientos) ? alojamientos[i] : nullptr;
    }

    /**
     * @brief Carga anfitriones desde un archivo TXT.
     *
     * Redimensiona el arreglo de anfitriones dinámicamente si es necesario.
     * @param archivo Ruta al archivo.
     * @param arreglo Referencia al puntero del arreglo de anfitriones.
     * @param cantidad Número de anfitriones cargados (por referencia).
     * @param capacidad Capacidad actual del arreglo (por referencia).
     */
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Anfitrion*& arreglo,
                                   int& cantidad,
                                   int& capacidad);
};

#endif // ANFITRION_H
