#ifndef HUESPED_H
#define HUESPED_H

#include <string>
using namespace std;

// Declaraciones adelantadas para evitar inclusión circular
class Fecha;
class Reserva;

/**
 * @class Huesped
 * @brief Representa a un huésped del sistema UdeAStay con sus datos personales y sus reservas activas.
 *
 * Administra dinámicamente las reservas asociadas al huésped y ofrece operaciones como agregar, eliminar,
 * verificar conflictos de fechas, y cargar desde archivo.
 */
class Huesped {
private:
    string documento;               ///< Número de documento del huésped.
    string nombre;                  ///< Nombre completo.
    int antiguedad;                 ///< Antigüedad en meses.
    float puntuacion;              ///< Puntuación promedio.
    string clave;                   ///< Clave de acceso.

    Reserva** reservas;             ///< Arreglo dinámico de punteros a reservas.
    int cantidadReservas;           ///< Número de reservas activas.
    int capacidadReservas;          ///< Capacidad total del arreglo de reservas.

    /**
     * @brief Duplica la capacidad del arreglo de reservas cuando está lleno.
     */
    void redimensionarReservas();

public:
    /**
     * @brief Constructor por defecto.
     * Inicializa los atributos en valores neutros y reserva espacio para 10 reservas.
     */
    Huesped();

    /**
     * @brief Constructor parametrizado.
     * @param doc Documento de identidad.
     * @param nombre Nombre del huésped.
     * @param antig Antigüedad en meses.
     * @param punt Puntuación inicial.
     */
    Huesped(string doc, string nombre, int antig, float punt);

    // ==== Getters ====

    /** @return Documento del huésped. */
    string getDocumento() const;

    /** @return Nombre del huésped. */
    string getNombre() const;

    /** @return Antigüedad en meses. */
    int getAntiguedad() const;

    /** @return Puntuación del huésped. */
    float getPuntuacion() const;

    /** @return Clave del huésped. */
    string getClave() const;

    // ==== Setters ====

    /** @param doc Documento a asignar. */
    void setDocumento(const string& doc);

    /** @param antig Antigüedad a asignar. */
    void setAntiguedad(int antig);

    /** @param punt Puntuación a asignar. */
    void setPuntuacion(float punt);

    /** @param c Clave a asignar. */
    void setClave(const string& c);

    // ==== Funciones de reservas ====

    /**
     * @brief Agrega una nueva reserva si no hay conflicto de fechas.
     * @param nueva Reserva a agregar.
     * @return true si se agregó correctamente, false si hubo conflicto.
     */
    bool agregarReserva(Reserva* nueva);

    /**
     * @brief Verifica si una reserva se solapa con las existentes.
     * @param entrada Fecha de inicio de la reserva.
     * @param duracion Número de noches.
     * @return true si hay conflicto, false si no.
     */
    bool hayConflicto(const Fecha& entrada, int duracion) const;

    /**
     * @brief Elimina una reserva dada su dirección.
     * @param r Puntero a la reserva a eliminar.
     */
    void eliminarReserva(Reserva* r);

    /**
     * @brief Elimina una reserva buscándola por su código.
     * @param codigo Código de la reserva a eliminar.
     */
    void eliminarReservaPorCodigo(const string& codigo);

    /**
     * @brief Muestra por consola los datos del huésped y sus reservas activas.
     */
    void mostrar() const;

    // ==== Manejo de copia profunda ====

    /**
     * @brief Constructor de copia.
     * @param otro Instancia a copiar.
     */
    Huesped(const Huesped& otro);

    /**
     * @brief Operador de asignación.
     * @param otro Huesped a copiar.
     * @return Referencia al objeto modificado.
     */
    Huesped& operator=(const Huesped& otro);

    /**
     * @brief Repara los punteros a reservas tras redimensionar el arreglo global de reservas.
     * @param nuevoArreglo Arreglo nuevo de reservas (copiado).
     * @param cantReservas Cantidad total de reservas.
     */
    void repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas);

    // ==== Carga de datos ====

    /**
     * @brief Carga los huéspedes desde un archivo TXT.
     * @param archivo Ruta del archivo.
     * @param arreglo Puntero que se actualizará con el nuevo arreglo dinámico.
     * @param cantidad Cantidad de huéspedes leídos.
     * @param capacidad Capacidad del arreglo asignado.
     */
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Huesped*& arreglo,
                                   int& cantidad,
                                   int& capacidad);

    /**
     * @brief Destructor. Libera la memoria del arreglo de reservas.
     */
    ~Huesped();
};

#endif // HUESPED_H
