#ifndef HUESPED_H
#define HUESPED_H

#include <string>
using namespace std;

// Declaraciones adelantadas (forward declarations) para evitar dependencias circulares
class Fecha;
class Reserva;

class Huesped {
private:
    // Datos personales del huésped
    string documento;       // Identificación única
    string nombre;          // Nombre completo
    int antiguedad;         // Tiempo registrado en meses
    float puntuacion;       // Valoración promedio
    string clave;           // Contraseña para autenticación (simple)

    // Manejo de reservas activas
    Reserva** reservas;     // Arreglo dinámico de punteros a reservas
    int cantidadReservas;   // Número actual de reservas activas
    int capacidadReservas;  // Tamaño del arreglo reservado

    // Función interna para duplicar el tamaño del arreglo si se llena
    void redimensionarReservas();

public:
    // Constructores
    Huesped();  // Constructor por defecto
    Huesped(string doc, string nombre, int antig, float punt);  // Constructor parametrizado

    // ----------- Getters -----------
    string getDocumento() const;     // Devuelve el documento
    string getNombre() const;        // Devuelve el nombre
    int getAntiguedad() const;       // Devuelve la antigüedad
    float getPuntuacion() const;     // Devuelve la puntuación
    string getClave() const;         // Devuelve la clave

    // ----------- Setters -----------
    void setDocumento(const string& doc);  // Modifica el documento
    void setAntiguedad(int antig);         // Modifica la antigüedad
    void setPuntuacion(float punt);        // Modifica la puntuación
    void setClave(const string& c);        // Modifica la clave

    // ----------- Manejo de reservas -----------

    // Intenta agregar una reserva nueva si no hay conflicto de fechas
    bool agregarReserva(Reserva* nueva);

    // Verifica si una nueva reserva se traslapa con las existentes
    bool hayConflicto(const Fecha& entrada, int duracion) const;

    // Elimina una reserva por referencia directa
    void eliminarReserva(Reserva* r);

    // Elimina una reserva buscando por su código
    void eliminarReservaPorCodigo(const string& codigo);

    // Muestra por consola los datos del huésped y sus reservas activas
    void mostrar() const;

    // ----------- Gestión de copias -----------

    // Constructor copia (deep copy superficial: solo punteros)
    Huesped(const Huesped& otro);

    // Operador de asignación (también copia profunda controlada)
    Huesped& operator=(const Huesped& otro);

    // Reasocia las reservas desde un nuevo arreglo cargado desde archivo
    void repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas);

    // Destructor
    ~Huesped();
};

#endif // HUESPED_H


