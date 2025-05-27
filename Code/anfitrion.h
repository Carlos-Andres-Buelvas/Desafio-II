#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"  // Inclusión necesaria para usar punteros a Alojamiento

class Anfitrion {
private:
    // Información personal del anfitrión
    std::string documento;     // Identificación única
    std::string nombre;        // Nombre completo
    std::string clave;         // Contraseña o PIN de acceso
    int antiguedad;            // Tiempo como usuario (en meses)
    float puntuacion;          // Valoración promedio dada por huéspedes

    // Alojamiento asociado
    Alojamiento** alojamientos;     // Arreglo dinámico de punteros a alojamientos
    int cantidadAlojamientos;       // Número actual de alojamientos registrados
    int capacidadAlojamientos;      // Capacidad total del arreglo dinámico

public:
    // ----------- Constructores -----------

    Anfitrion();  // Constructor por defecto

    // Constructor con parámetros
    Anfitrion(const std::string& documento, const std::string& nombre, int antiguedad, float puntuacion);

    // Constructor copia (copia superficial de punteros)
    Anfitrion(const Anfitrion& otro);

    // Operador de asignación (deep copy controlado)
    Anfitrion& operator=(const Anfitrion& otro);

    // Destructor (libera memoria del arreglo dinámico)
    ~Anfitrion();

    // ----------- Getters -----------

    std::string getDocumento() const;     // Devuelve documento del anfitrión
    std::string getNombre() const;        // Devuelve nombre del anfitrión
    int getAntiguedad() const;            // Devuelve la antigüedad en meses
    float getPuntuacion() const;          // Devuelve puntuación del anfitrión
    std::string getClave() const;         // Devuelve clave de acceso

    // ----------- Setters -----------

    void setDocumento(const std::string& doc);  // Modifica documento
    void setAntiguedad(int antig);              // Modifica antigüedad
    void setPuntuacion(float punt);             // Modifica puntuación
    void setClave(const std::string& c);        // Modifica clave

    // ----------- Funcionalidades -----------

    // Agrega un nuevo alojamiento al anfitrión (redimensiona si es necesario)
    void agregarAlojamiento(Alojamiento* nuevo);

    // Muestra en consola los datos del anfitrión y sus alojamientos
    void mostrar() const;

    // Devuelve el número actual de alojamientos registrados
    int getCantidadAlojamientos() const { return cantidadAlojamientos; }

    // Devuelve el puntero al alojamiento en la posición i (si es válido)
    Alojamiento* getAlojamiento(int i) const {
        return (i >= 0 && i < cantidadAlojamientos) ? alojamientos[i] : nullptr;
    }
};

#endif // ANFITRION_H
