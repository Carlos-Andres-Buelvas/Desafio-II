#include "anfitrion.h"
#include <iostream>
using namespace std;

Anfitrion::Anfitrion() {
    documento = "";
    antiguedad = 0;
    puntuacion = 0.0;
    capacidadAlojamientos = 5;
    cantidadAlojamientos = 0;
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

Anfitrion::Anfitrion(string doc, int antig, float punt) {
    documento = doc;
    antiguedad = antig;
    puntuacion = punt;
    capacidadAlojamientos = 5;
    cantidadAlojamientos = 0;
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

// Getters
string Anfitrion::getDocumento() const { return documento; }
int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

// Setters
void Anfitrion::setDocumento(const string& doc) { documento = doc; }
void Anfitrion::setAntiguedad(int antig) { antiguedad = antig; }
void Anfitrion::setPuntuacion(float punt) { puntuacion = punt; }

// Agregar un nuevo alojamiento
void Anfitrion::agregarAlojamiento(Alojamiento* nuevo) {
    if (cantidadAlojamientos >= capacidadAlojamientos) {
        capacidadAlojamientos *= 2;
        Alojamiento** temp = new Alojamiento*[capacidadAlojamientos];
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            temp[i] = alojamientos[i];
        }
        delete[] alojamientos;
        alojamientos = temp;
    }
    alojamientos[cantidadAlojamientos++] = nuevo;
}

// Mostrar información del anfitrión
void Anfitrion::mostrar() const {
    cout << "Anfitrión: " << documento << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;
    cout << "Alojamientos asignados: " << cantidadAlojamientos << endl;
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        alojamientos[i]->mostrar();  // esta función debe existir en Alojamiento
    }
}

// Constructor de copia
Anfitrion::Anfitrion(const Anfitrion& otro) {
    documento = otro.documento;
    antiguedad = otro.antiguedad;
    puntuacion = otro.puntuacion;
    capacidadAlojamientos = otro.capacidadAlojamientos;
    cantidadAlojamientos = otro.cantidadAlojamientos;

    alojamientos = new Alojamiento*[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        alojamientos[i] = otro.alojamientos[i];  // Copiamos los punteros, no duplicamos los objetos
    }
}

// Operador de asignación
Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this != &otro) {
        // Liberar lo que ya tiene este objeto
        delete[] alojamientos;

        // Copiar atributos
        documento = otro.documento;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        capacidadAlojamientos = otro.capacidadAlojamientos;
        cantidadAlojamientos = otro.cantidadAlojamientos;

        // Copiar alojamientos
        alojamientos = new Alojamiento*[capacidadAlojamientos];
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            alojamientos[i] = otro.alojamientos[i];
        }
    }
    return *this;
}

// Liberar memoria
Anfitrion::~Anfitrion() {
    if (alojamientos != nullptr){
        delete[] alojamientos;  // solo eliminamos el arreglo de punteros, no los objetos
        alojamientos = nullptr;
    }

}
