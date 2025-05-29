#include "anfitrion.h"
#include <iostream>
#include <fstream>
#include <sstream>

Anfitrion::Anfitrion()
    : documento(""), nombre(""), clave(""), antiguedad(0), puntuacion(0.0f),
    cantidadAlojamientos(0), capacidadAlojamientos(10) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

Anfitrion::Anfitrion(const std::string& doc, const std::string& nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""), antiguedad(antig), puntuacion(punt),
    cantidadAlojamientos(0), capacidadAlojamientos(10) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
}

Anfitrion::Anfitrion(const Anfitrion& otro)
    : documento(otro.documento), nombre(otro.nombre), clave(otro.clave),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    cantidadAlojamientos(otro.cantidadAlojamientos),
    capacidadAlojamientos(otro.capacidadAlojamientos) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        alojamientos[i] = otro.alojamientos[i];  // Copia superficial
    }
}

Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this != &otro) {
        delete[] alojamientos;

        documento = otro.documento;
        nombre = otro.nombre;
        clave = otro.clave;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        capacidadAlojamientos = otro.capacidadAlojamientos;
        cantidadAlojamientos = otro.cantidadAlojamientos;

        alojamientos = new Alojamiento*[capacidadAlojamientos];
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            alojamientos[i] = otro.alojamientos[i];
        }
    }
    return *this;
}

Anfitrion::~Anfitrion() {
    delete[] alojamientos;
    alojamientos = nullptr;
}

// Getters
std::string Anfitrion::getDocumento() const { return documento; }
std::string Anfitrion::getNombre() const { return nombre; }
std::string Anfitrion::getClave() const { return clave; }
int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

// Setters
void Anfitrion::setDocumento(const std::string& doc) { documento = doc; }
void Anfitrion::setClave(const std::string& c) { clave = c; }
void Anfitrion::setAntiguedad(int antig) { antiguedad = antig; }
void Anfitrion::setPuntuacion(float punt) { puntuacion = punt; }

// Función para agregar alojamiento
void Anfitrion::agregarAlojamiento(Alojamiento* nuevo) {
    if (cantidadAlojamientos >= capacidadAlojamientos) {
        int nuevaCapacidad = capacidadAlojamientos * 2;
        Alojamiento** nuevoArray = new Alojamiento*[nuevaCapacidad];

        for (int i = 0; i < cantidadAlojamientos; ++i)
            nuevoArray[i] = alojamientos[i];

        delete[] alojamientos;
        alojamientos = nuevoArray;
        capacidadAlojamientos = nuevaCapacidad;
    }

    alojamientos[cantidadAlojamientos++] = nuevo;
}

// Mostrar información del anfitrión
void Anfitrion::mostrar() const {
    std::cout << "\nAnfitrión: " << documento
              << " | Antigüedad: " << antiguedad << " meses"
              << " | Puntuación: " << puntuacion << std::endl;

    std::cout << "Alojamientos asignados: " << cantidadAlojamientos << std::endl;

    for (int i = 0; i < cantidadAlojamientos; ++i) {
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            if (alojamientos[i]) {
                alojamientos[i]->mostrar();
            } else {
                std::cout << "[ERROR] puntero nulo\n";
            }
        }
    }
}

void Anfitrion::cargarDesdeArchivo(const std::string& archivo,
                                   Anfitrion*& arreglo,
                                   int& cantidad,
                                   int& capacidad)
{
    std::ifstream in(archivo);
    if (!in.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir " << archivo << "\n";
        return;
    }

    std::string linea;
    std::getline(in, linea); // Saltar encabezado
    cantidad = 0;
    capacidad = 10;
    arreglo = new Anfitrion[capacidad];

    while (std::getline(in, linea)) {
        std::stringstream ss(linea);
        std::string doc, nombre, clave, antigStr, puntStr;

        std::getline(ss, doc, ';');
        std::getline(ss, nombre, ';');
        std::getline(ss, antigStr, ';');
        std::getline(ss, puntStr, ';');
        std::getline(ss, clave, ';');

        int antig = std::stoi(antigStr);
        float punt = std::stof(puntStr);

        Anfitrion nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Anfitrion* nuevoArr = new Anfitrion[capacidad];
            for (int i = 0; i < cantidad; ++i)
                nuevoArr[i] = arreglo[i];
            delete[] arreglo;
            arreglo = nuevoArr;
        }

        arreglo[cantidad++] = nuevo;
    }

    in.close();
}
