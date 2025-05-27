#include "anfitrion.h"
#include <iostream>

// Constructor por defecto
Anfitrion::Anfitrion()
    : documento(""), nombre(""), clave(""), antiguedad(0), puntuacion(0.0f),
    capacidadAlojamientos(5), cantidadAlojamientos(0) {
    // Se inicializa el arreglo dinámico de alojamientos con capacidad inicial
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

// Constructor con parámetros
Anfitrion::Anfitrion(const std::string& doc, const std::string& nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""), antiguedad(antig), puntuacion(punt),
    capacidadAlojamientos(5), cantidadAlojamientos(0) {
    // Se reserva espacio para el arreglo de punteros a alojamientos
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

// Constructor copia
Anfitrion::Anfitrion(const Anfitrion& otro)
    : documento(otro.documento), nombre(otro.nombre), clave(otro.clave),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    capacidadAlojamientos(otro.capacidadAlojamientos),
    cantidadAlojamientos(otro.cantidadAlojamientos) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        alojamientos[i] = otro.alojamientos[i];  // Copia superficial (mismo puntero)
    }
}

// Operador de asignación (deep copy controlado)
Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this != &otro) {  // Previene autoasignación
        delete[] alojamientos;  // Libera el arreglo anterior

        // Copia atributos básicos
        documento = otro.documento;
        nombre = otro.nombre;
        clave = otro.clave;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        capacidadAlojamientos = otro.capacidadAlojamientos;
        cantidadAlojamientos = otro.cantidadAlojamientos;

        // Copia punteros a alojamientos
        alojamientos = new Alojamiento*[capacidadAlojamientos];
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            alojamientos[i] = otro.alojamientos[i];
        }
    }
    return *this;
}

// Destructor
Anfitrion::~Anfitrion() {
    delete[] alojamientos;  // Libera el arreglo de alojamientos
    alojamientos = nullptr;
}

// ----------- Getters -----------

std::string Anfitrion::getDocumento() const { return documento; }
std::string Anfitrion::getNombre() const { return nombre; }
std::string Anfitrion::getClave() const { return clave; }
int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

// ----------- Setters -----------

void Anfitrion::setDocumento(const std::string& doc) { documento = doc; }
void Anfitrion::setClave(const std::string& c) { clave = c; }
void Anfitrion::setAntiguedad(int antig) { antiguedad = antig; }
void Anfitrion::setPuntuacion(float punt) { puntuacion = punt; }

// ----------- Funcionalidad principal -----------

// Agrega un nuevo alojamiento al arreglo del anfitrión
void Anfitrion::agregarAlojamiento(Alojamiento* nuevo) {
    // Si se supera la capacidad, se redimensiona el arreglo al doble
    if (cantidadAlojamientos >= capacidadAlojamientos) {
        int nuevaCapacidad = capacidadAlojamientos * 2;
        Alojamiento** nuevoArray = new Alojamiento*[nuevaCapacidad];

        // Se copian los punteros actuales al nuevo arreglo
        for (int i = 0; i < cantidadAlojamientos; ++i)
            nuevoArray[i] = alojamientos[i];

        // Se libera el arreglo antiguo y se reasigna
        delete[] alojamientos;
        alojamientos = nuevoArray;
        capacidadAlojamientos = nuevaCapacidad;
    }

    // Se agrega el nuevo alojamiento
    alojamientos[cantidadAlojamientos++] = nuevo;
}

// Muestra los datos del anfitrión y sus alojamientos
void Anfitrion::mostrar() const {
    std::cout << "Anfitrión: " << documento
              << " | Antigüedad: " << antiguedad << " meses"
              << " | Puntuación: " << puntuacion << std::endl;

    std::cout << "Alojamientos asignados: " << cantidadAlojamientos << std::endl;

    // Se listan los alojamientos si existen
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        if (alojamientos[i])
            alojamientos[i]->mostrar();  // Llama a la función mostrar() de cada alojamiento
    }
}
