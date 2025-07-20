#include "huesped.h"
#include "fecha.h"
#include "reserva.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern int totalIteraciones;
extern int totalMemoria;

/**
 * @brief Constructor por defecto de Huesped.
 * Inicializa atributos con valores por defecto y reserva espacio dinámico para las reservas.
 */
Huesped::Huesped() : documento(""), nombre(""), antiguedad(0),
    puntuacion(0.0), clave(""), cantidadReservas(0), capacidadReservas(10)
{
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
}

/**
 * @brief Constructor parametrizado de Huesped.
 * @param doc Documento del huésped.
 * @param nom Nombre del huésped.
 * @param antig Antigüedad en meses.
 * @param punt Puntuación del huésped.
 */
Huesped::Huesped(string doc, string nom, int antig, float punt)
    : documento(doc), nombre(nom), antiguedad(antig), puntuacion(punt),
    clave(""), cantidadReservas(0), capacidadReservas(10)
{
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
}

/**
 * @brief Constructor de copia.
 * Realiza una copia superficial de los punteros a reservas.
 */
Huesped::Huesped(const Huesped& otro)
    : documento(otro.documento), nombre(otro.nombre),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    clave(otro.clave), cantidadReservas(otro.cantidadReservas),
    capacidadReservas(otro.capacidadReservas)
{
    reservas = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
    for (int i = 0; i < cantidadReservas; ++i){
        totalIteraciones++;
        reservas[i] = otro.reservas[i];
    }
}

/**
 * @brief Operador de asignación.
 * Libera la memoria previa y copia los atributos desde otro huésped.
 */
Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservas;

        documento = otro.documento;
        nombre = otro.nombre;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        clave = otro.clave;
        capacidadReservas = otro.capacidadReservas;
        cantidadReservas = otro.cantidadReservas;

        reservas = new Reserva*[capacidadReservas];
        totalMemoria += sizeof(Reserva*) * capacidadReservas;
        for (int i = 0; i < cantidadReservas; ++i){
            totalIteraciones++;
            reservas[i] = otro.reservas[i];
        }
    }
    return *this;
}

// === Getters ===
string Huesped::getDocumento() const { return documento; }
string Huesped::getNombre() const { return nombre; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }
string Huesped::getClave() const { return clave; }

// === Setters ===
void Huesped::setDocumento(const string& doc) { documento = doc; }
void Huesped::setAntiguedad(int antig) { antiguedad = antig; }
void Huesped::setPuntuacion(float punt) { puntuacion = punt; }
void Huesped::setClave(const string& c) { clave = c; }

/**
 * @brief Agrega una reserva al huésped si no hay conflicto de fechas.
 * @param nueva Puntero a la nueva reserva.
 * @return true si se agregó exitosamente, false si hubo conflicto.
 */
bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion()))
        return false;

    if (cantidadReservas >= capacidadReservas)
        redimensionarReservas();

    reservas[cantidadReservas++] = nueva;
    return true;
}

/**
 * @brief Verifica si hay conflicto de fechas con reservas existentes.
 * @param entrada Fecha de entrada.
 * @param duracion Cantidad de noches.
 * @return true si hay conflicto, false si es posible reservar.
 */
bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int y1 = entrada.getAnio();
    int start1 = entrada.aDiaDelAnio();
    int end1 = start1 + duracion;

    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        Reserva* r = reservas[i];
        Fecha fe = r->getFechaEntrada();

        if (fe.getAnio() != y1) continue;

        int start2 = fe.aDiaDelAnio();
        int end2 = start2 + r->getDuracion();

        if (!(end1 <= start2 || end2 <= start1))
            return true;
    }
    return false;
}

/**
 * @brief Elimina una reserva específica del arreglo de reservas.
 * @param r Puntero a la reserva a eliminar.
 */
void Huesped::eliminarReserva(Reserva* r) {
    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j){
                totalIteraciones++;
                reservas[j] = reservas[j + 1];
            }
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

/**
 * @brief Elimina una reserva con base en su código.
 * @param codigo Código único de la reserva.
 */
void Huesped::eliminarReservaPorCodigo(const string& codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        if (reservas[i] && reservas[i]->getCodigo() == codigo) {
            for (int j = i; j < cantidadReservas - 1; ++j){
                totalIteraciones++;
                reservas[j] = reservas[j + 1];
            }
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

/**
 * @brief Muestra los datos del huésped y sus reservas activas.
 */
void Huesped::mostrar() const {
    cout << "\nHuésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    if (cantidadReservas == 0) {
        cout << "No tiene reservaciones activas.\n";
        return;
    }

    for (int i = 0; i < cantidadReservas; ++i) {
        totalIteraciones++;
        cout << i + 1 << ". ";
        reservas[i]->mostrarComprobante();
    }
}

/**
 * @brief Redimensiona el arreglo dinámico de reservas al doble.
 */
void Huesped::redimensionarReservas() {
    capacidadReservas *= 2;
    Reserva** temp = new Reserva*[capacidadReservas];
    totalMemoria += sizeof(Reserva*) * capacidadReservas;
    for (int i = 0; i < cantidadReservas; ++i){
        totalIteraciones++;
        temp[i] = reservas[i];
    }
    delete[] reservas;
    reservas = temp;
}

/**
 * @brief Actualiza los punteros de reserva tras una copia profunda.
 * @param nuevoArreglo Arreglo actualizado de reservas.
 * @param cantReservas Número de reservas.
 */
void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        totalIteraciones++;
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < cantidadReservas; ++j) {
            totalIteraciones++;
            if (reservas[j]->getCodigo() == actual->getCodigo())
                reservas[j] = actual;
        }
    }
}

/**
 * @brief Carga los datos de huéspedes desde archivo.
 * @param archivo Ruta del archivo.
 * @param arreglo Arreglo dinámico que contendrá los huéspedes.
 * @param cantidad Número total cargado.
 * @param capacidad Capacidad inicial del arreglo.
 */
void Huesped::cargarDesdeArchivo(const std::string& archivo,
                                 Huesped*& arreglo,
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
    arreglo = new Huesped[capacidad];
    totalMemoria += sizeof(Huesped) * capacidad;

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

        Huesped nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Huesped* nuevoArr = new Huesped[capacidad];
            totalMemoria += sizeof(Huesped) * capacidad;
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

/**
 * @brief Destructor. Libera la memoria de las reservas.
 */
Huesped::~Huesped() {
    delete[] reservas;
}
