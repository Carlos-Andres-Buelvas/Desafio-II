#ifndef HUESPED_H
#define HUESPED_H

#include <string>
using namespace std;

class Fecha;
class Reserva;

class Huesped {
private:
    string documento;
    string nombre;
    int antiguedad;
    float puntuacion;
    string clave;

    Reserva** reservas;
    int cantidadReservas;
    int capacidadReservas;

    void redimensionarReservas();  // Nuevo: para encapsular la expansión del arreglo

public:
    Huesped();
    Huesped(string doc, string nombre, int antig, float punt);

    // Getters
    string getDocumento() const;
    string getNombre() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    string getClave() const;

    // Setters
    void setDocumento(const string& doc);
    void setAntiguedad(int antig);
    void setPuntuacion(float punt);
    void setClave(const string& c);

    // Funciones de reservas
    bool agregarReserva(Reserva* nueva);
    bool hayConflicto(const Fecha& entrada, int duracion) const;
    void eliminarReserva(Reserva* r);
    void eliminarReservaPorCodigo(const string& codigo);

    // Utilidades
    void mostrar() const;

    // Copia profunda
    Huesped(const Huesped& otro);
    Huesped& operator=(const Huesped& otro);

    void repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas);

    // Carga de datos desde archivo
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Huesped*& arreglo,
                                   int& cantidad,
                                   int& capacidad);

    ~Huesped();
};

#endif // HUESPED_H
