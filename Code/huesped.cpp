#include "huesped.h"
#include "fecha.h"
#include "reserva.h"
#include <iostream>
using namespace std;

// Constructor por defecto
Huesped::Huesped() : documento(""), nombre(""), clave(""),
    antiguedad(0), puntuacion(0.0),
    capacidadReservas(5), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];  // Inicializa arreglo dinámico
}

// Constructor con parámetros
Huesped::Huesped(string doc, string nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""),
    antiguedad(antig), puntuacion(punt),
    capacidadReservas(5), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];  // Inicializa arreglo con capacidad fija
}

// Constructor copia
Huesped::Huesped(const Huesped& otro)
    : documento(otro.documento), nombre(otro.nombre),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    clave(otro.clave), capacidadReservas(otro.capacidadReservas),
    cantidadReservas(otro.cantidadReservas) {
    reservas = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        reservas[i] = otro.reservas[i];  // Copia los punteros, no los objetos
}

// Sobrecarga del operador de asignación
Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservas;  // Libera memoria anterior

        // Copia atributos
        documento = otro.documento;
        nombre = otro.nombre;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        clave = otro.clave;
        capacidadReservas = otro.capacidadReservas;
        cantidadReservas = otro.cantidadReservas;

        // Copia punteros
        reservas = new Reserva*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            reservas[i] = otro.reservas[i];
    }
    return *this;
}

// ----------- Getters & Setters -----------

string Huesped::getDocumento() const { return documento; }
string Huesped::getNombre() const { return nombre; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }
string Huesped::getClave() const { return clave; }

void Huesped::setDocumento(const string& doc) { documento = doc; }
void Huesped::setAntiguedad(int antig) { antiguedad = antig; }
void Huesped::setPuntuacion(float punt) { puntuacion = punt; }
void Huesped::setClave(const string& c) { clave = c; }

// ----------- Manejo de Reservas -----------

// Intenta agregar una reserva nueva si no hay conflicto de fechas
bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion()))
        return false;  // No se puede agregar por traslape

    if (cantidadReservas >= capacidadReservas)
        redimensionarReservas();  // Duplicar capacidad

    reservas[cantidadReservas++] = nueva;
    return true;
}

// Verifica si la nueva reserva se traslapa con alguna existente
bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int y1 = entrada.getAnio();  // Año de la nueva reserva
    int start1 = entrada.aDiaDelAnio();
    int end1 = start1 + duracion;

    for (int i = 0; i < cantidadReservas; ++i) {
        Reserva* r = reservas[i];
        Fecha fe = r->getFechaEntrada();

        if (fe.getAnio() != y1) continue;  // Solo compara dentro del mismo año

        int start2 = fe.aDiaDelAnio();
        int end2 = start2 + r->getDuracion();

        // Verifica si los intervalos se superponen
        if (!(end1 <= start2 || end2 <= start1))
            return true;
    }
    return false;
}

// Elimina una reserva por puntero
void Huesped::eliminarReserva(Reserva* r) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];  // Reorganiza el arreglo
            reservas[--cantidadReservas] = nullptr;  // Libera último espacio
            return;
        }
    }
}

// Elimina una reserva buscando por código
void Huesped::eliminarReservaPorCodigo(const string& codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] && reservas[i]->getCodigo() == codigo) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

// Muestra los datos del huésped y sus reservas
void Huesped::mostrar() const {
    cout << "Huésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    bool tiene = false;
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] && reservas[i]->getHuesped()->getDocumento() == documento) {
            cout << i + 1 << ". ";
            reservas[i]->mostrarComprobante();  // Muestra los detalles de la reserva
            tiene = true;
        }
    }
    if (!tiene)
        cout << "No tiene reservaciones activas.\n";
}

// ----------- Utilidades internas -----------

// Duplica la capacidad del arreglo de reservas
void Huesped::redimensionarReservas() {
    capacidadReservas *= 2;
    Reserva** temp = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        temp[i] = reservas[i];
    delete[] reservas;
    reservas = temp;
}

// Reasocia las reservas al nuevo arreglo después de cargar desde archivo
void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < cantidadReservas; ++j) {
            if (reservas[j]->getCodigo() == actual->getCodigo())
                reservas[j] = actual;  // Actualiza puntero con el nuevo objeto cargado
        }
    }
}

// Destructor: libera memoria del arreglo de reservas
Huesped::~Huesped() {
    delete[] reservas;
}

