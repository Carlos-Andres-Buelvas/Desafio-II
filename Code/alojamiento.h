#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>
#include "fecha.h"

// Declaración adelantada para evitar ciclo de inclusión
class Anfitrion;

class Alojamiento {
private:
    std::string codigo;
    std::string departamento;
    std::string municipio;
    std::string nombre;
    std::string tipo;
    std::string direccion;
    Anfitrion* anfitrion;
    float precioNoche;
    static const int NUM_AMENIDADES = 6;
    bool amenidades[NUM_AMENIDADES]; // Ej: {wifi, baño privado, cocina, tv, parqueadero, mascotas}

    // Fechas reservadas
    Fecha* fechasOcupadas;
    int cantFechas;
    int capFechas;

    // Métodos auxiliares
    bool contieneFecha(const Fecha& f) const;
    void agregarFecha(const Fecha& f);

public:
    // Constructores
    Alojamiento();
    Alojamiento(std::string cod, std::string dep, std::string mun, std::string nom, std::string tipo, std::string dir,
                Anfitrion* anfitrion, float precio, const bool amen[NUM_AMENIDADES]);

    // Constructor de copia y operador de asignación (manejo seguro de memoria dinámica)
    Alojamiento(const Alojamiento& otro);
    Alojamiento& operator=(const Alojamiento& otro);

    // Destructor
    ~Alojamiento();

    // Getters y setters
    std::string getCodigo() const;
    void setCodigo(const std::string& cod);
    std::string getMunicipio() const;
    float getPrecioNoche() const;
    void setPrecioNoche(float precio);
    Anfitrion* getAnfitrion() const;

    // Funciones relacionadas con disponibilidad
    bool estaDisponible(const Fecha& entrada, int duracion) const;
    void reservarDias(const Fecha& entrada, int duracion);
    void liberarDias(const Fecha& inicio, int noches);

    // Utilidad
    void mostrar() const;
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Alojamiento*& arreglo,
                                   int& cantidad,
                                   int& capacidad,
                                   Anfitrion* anfitriones,
                                   int cantAnfitriones);
};

#endif // ALOJAMIENTO_H
