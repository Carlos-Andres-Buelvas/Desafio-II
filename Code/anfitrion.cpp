#include "anfitrion.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Variables globales para seguimiento de eficiencia
extern int totalIteraciones;
extern int totalMemoria;

/**
 * @brief Constructor por defecto de Anfitrión.
 * Inicializa campos vacíos y crea el arreglo dinámico para alojamientos.
 */
Anfitrion::Anfitrion()
    : documento(""), nombre(""), clave(""), antiguedad(0), puntuacion(0.0f),
    cantidadAlojamientos(0), capacidadAlojamientos(10) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    totalMemoria += sizeof(Alojamiento) * capacidadAlojamientos;
}

/**
 * @brief Constructor parametrizado de Anfitrión.
 * @param doc Documento de identidad.
 * @param nom Nombre del anfitrión.
 * @param antig Antigüedad en meses.
 * @param punt Puntuación promedio.
 */
Anfitrion::Anfitrion(const std::string& doc, const std::string& nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""), antiguedad(antig), puntuacion(punt),
    cantidadAlojamientos(0), capacidadAlojamientos(10) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    totalMemoria += sizeof(Alojamiento) * capacidadAlojamientos;
}

/**
 * @brief Constructor de copia.
 * Realiza copia superficial de punteros a alojamientos.
 */
Anfitrion::Anfitrion(const Anfitrion& otro)
    : documento(otro.documento), nombre(otro.nombre), clave(otro.clave),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    cantidadAlojamientos(otro.cantidadAlojamientos),
    capacidadAlojamientos(otro.capacidadAlojamientos) {
    alojamientos = new Alojamiento*[capacidadAlojamientos];
    totalMemoria += sizeof(Alojamiento) * capacidadAlojamientos;
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        totalIteraciones++;
        alojamientos[i] = otro.alojamientos[i];  // Copia superficial
    }
}

/**
 * @brief Operador de asignación.
 * Libera memoria previa y copia atributos del otro anfitrión.
 */
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
        totalMemoria += sizeof(Alojamiento) * capacidadAlojamientos;
        for (int i = 0; i < cantidadAlojamientos; ++i) {
            totalIteraciones++;
            alojamientos[i] = otro.alojamientos[i];
        }
    }
    return *this;
}

/**
 * @brief Destructor.
 * Libera la memoria dinámica del arreglo de alojamientos.
 */
Anfitrion::~Anfitrion() {
    delete[] alojamientos;
    alojamientos = nullptr;
}

// ===== Getters =====
std::string Anfitrion::getDocumento() const { return documento; }
std::string Anfitrion::getNombre() const { return nombre; }
std::string Anfitrion::getClave() const { return clave; }
int Anfitrion::getAntiguedad() const { return antiguedad; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

// ===== Setters =====
void Anfitrion::setDocumento(const std::string& doc) { documento = doc; }
void Anfitrion::setClave(const std::string& c) { clave = c; }
void Anfitrion::setAntiguedad(int antig) { antiguedad = antig; }
void Anfitrion::setPuntuacion(float punt) { puntuacion = punt; }

/**
 * @brief Agrega un alojamiento al anfitrión.
 * Redimensiona el arreglo si es necesario.
 * @param nuevo Puntero al nuevo alojamiento.
 */
void Anfitrion::agregarAlojamiento(Alojamiento* nuevo) {
    if (cantidadAlojamientos >= capacidadAlojamientos) {
        int nuevaCapacidad = capacidadAlojamientos * 2;
        Alojamiento** nuevoArray = new Alojamiento*[nuevaCapacidad];
        totalMemoria += sizeof(Alojamiento) * nuevaCapacidad;

        for (int i = 0; i < cantidadAlojamientos; ++i){
            totalIteraciones++;
            nuevoArray[i] = alojamientos[i];
        }

        delete[] alojamientos;
        alojamientos = nuevoArray;
        capacidadAlojamientos = nuevaCapacidad;
    }

    alojamientos[cantidadAlojamientos++] = nuevo;
}

/**
 * @brief Muestra la información del anfitrión y sus alojamientos.
 * Si algún puntero es nulo, se reporta el error.
 */
void Anfitrion::mostrar() const {
    std::cout << "\nAnfitrión: " << documento
              << " | Antigüedad: " << antiguedad << " meses"
              << " | Puntuación: " << puntuacion << std::endl;

    std::cout << "Alojamientos asignados: " << cantidadAlojamientos << std::endl;

    int totalMostrar = 0;

    for (int i = 0; i < cantidadAlojamientos; ++i) {
        totalIteraciones++;
        if (alojamientos[i]) {
            std::cout << (totalMostrar + 1) << ". \n";
            totalMostrar++;
            alojamientos[i]->mostrar();
        } else {
            std::cout << "[ERROR] puntero nulo\n";
        }
    }
}

/**
 * @brief Carga anfitriones desde un archivo TXT.
 * @param archivo Ruta del archivo.
 * @param arreglo Puntero al arreglo dinámico de anfitriones.
 * @param cantidad Número de anfitriones leídos.
 * @param capacidad Capacidad inicial del arreglo.
 */
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
    totalMemoria += sizeof(Anfitrion) * capacidad;

    while (std::getline(in, linea)) {
        totalIteraciones++;
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
            totalMemoria += sizeof(Anfitrion) * capacidad;
            for (int i = 0; i < cantidad; ++i){
                totalIteraciones++;
                nuevoArr[i] = arreglo[i];
            }
            delete[] arreglo;
            arreglo = nuevoArr;
        }

        arreglo[cantidad++] = nuevo;
    }

    in.close();
}
