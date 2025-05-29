#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"

class Anfitrion {
private:
    std::string documento;
    std::string nombre;
    std::string clave;
    int antiguedad;
    float puntuacion;
    int cantidadAlojamientos;
    int capacidadAlojamientos;
    Alojamiento** alojamientos;

public:
    // Constructores
    Anfitrion();
    Anfitrion(const std::string& documento, const std::string& nombre, int antiguedad, float puntuacion);

    // Constructor de copia y operador de asignación
    Anfitrion(const Anfitrion& otro);
    Anfitrion& operator=(const Anfitrion& otro);

    // Destructor
    ~Anfitrion();

    // Getters
    std::string getDocumento() const;
    std::string getNombre() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    std::string getClave() const;

    // Setters
    void setDocumento(const std::string& doc);
    void setAntiguedad(int antig);
    void setPuntuacion(float punt);
    void setClave(const std::string& c);

    // Funciones
    void agregarAlojamiento(Alojamiento* nuevo);
    void mostrar() const;

    int getCantidadAlojamientos() const { return cantidadAlojamientos; }

    Alojamiento* getAlojamiento(int i) const {
        return (i >= 0 && i < cantidadAlojamientos) ? alojamientos[i] : nullptr;
    }

    // Carga de datos desde archivo
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Anfitrion*& arreglo,
                                   int& cantidad,
                                   int& capacidad);
};

#endif // ANFITRION_H
