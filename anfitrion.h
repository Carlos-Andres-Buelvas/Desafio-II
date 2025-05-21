#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"
using namespace std;

class Anfitrion {
private:
    string documento;
    int antiguedad;
    float puntuacion;
    Alojamiento** alojamientos;
    int cantidadAlojamientos;
    int capacidadAlojamientos;

public:
    Anfitrion();
    Anfitrion(string documento, int antiguedad, float puntuacion);

    // Getters
    string getDocumento() const;
    int getAntiguedad() const;
    float getPuntuacion() const;

    // Setters
    void setDocumento(const string& doc);
    void setAntiguedad(int antig);
    void setPuntuacion(float punt);

    // Funciones
    void agregarAlojamiento(Alojamiento* nuevo);
    void mostrar() const;

    // Destructor
    ~Anfitrion();

    Anfitrion(const Anfitrion& otro);                 // Constructor de copia
    Anfitrion& operator=(const Anfitrion& otro);      // Operador de asignación

};

#endif // ANFITRION_H
