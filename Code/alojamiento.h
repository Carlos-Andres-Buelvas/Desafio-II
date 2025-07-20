#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>
#include "fecha.h"

// Declaración adelantada para evitar ciclo de inclusión
class Anfitrion;

/**
 * @class Alojamiento
 * @brief Representa un alojamiento en el sistema UdeAStay.
 *
 * Cada alojamiento tiene un anfitrión asociado, información de ubicación, tipo,
 * dirección, precio por noche, conjunto de amenidades, y una lista de fechas ocupadas.
 */
class Alojamiento {
private:
    std::string codigo;           ///< Código identificador del alojamiento.
    std::string departamento;     ///< Departamento donde se encuentra.
    std::string municipio;        ///< Municipio donde se encuentra.
    std::string nombre;           ///< Nombre del alojamiento.
    std::string tipo;             ///< Tipo de alojamiento (casa, apartamento, etc.).
    std::string direccion;        ///< Dirección física del alojamiento.
    Anfitrion* anfitrion;         ///< Puntero al anfitrión propietario.

    float precioNoche;            ///< Precio por noche.

    static const int NUM_AMENIDADES = 6; ///< Número fijo de amenidades.
    bool amenidades[NUM_AMENIDADES];     ///< Arreglo de flags de amenidades.

    int cantFechas;              ///< Cantidad actual de fechas ocupadas.
    int capFechas;               ///< Capacidad máxima del arreglo de fechas.
    Fecha* fechasOcupadas;       ///< Arreglo dinámico de fechas ocupadas.

    /**
     * @brief Verifica si una fecha ya está ocupada.
     * @param f Fecha a verificar.
     * @return true si la fecha ya está ocupada.
     */
    bool contieneFecha(const Fecha& f) const;

    /**
     * @brief Agrega una fecha ocupada, redimensionando si es necesario.
     * @param f Fecha a agregar.
     */
    void agregarFecha(const Fecha& f);

public:
    /** @brief Constructor por defecto. Inicializa atributos básicos. */
    Alojamiento();

    /**
     * @brief Constructor parametrizado para inicializar todos los atributos.
     * @param cod Código del alojamiento.
     * @param dep Departamento.
     * @param mun Municipio.
     * @param nom Nombre del alojamiento.
     * @param tipo Tipo de alojamiento.
     * @param dir Dirección física.
     * @param anfitrion Puntero al anfitrión.
     * @param precio Precio por noche.
     * @param amen Arreglo de flags de amenidades.
     */
    Alojamiento(std::string cod, std::string dep, std::string mun, std::string nom, std::string tipo, std::string dir,
                Anfitrion* anfitrion, float precio, const bool amen[NUM_AMENIDADES]);

    /**
     * @brief Constructor de copia (copia profunda del arreglo de fechas).
     * @param otro Alojamiento a copiar.
     */
    Alojamiento(const Alojamiento& otro);

    /**
     * @brief Operador de asignación.
     * @param otro Alojamiento a asignar.
     * @return Referencia al objeto actual.
     */
    Alojamiento& operator=(const Alojamiento& otro);

    /** @brief Destructor. Libera memoria dinámica usada para las fechas. */
    ~Alojamiento();

    // ==== Getters y setters ====

    /** @return Código del alojamiento. */
    std::string getCodigo() const;

    /** @brief Establece el código del alojamiento. */
    void setCodigo(const std::string& cod);

    /** @return Municipio del alojamiento. */
    std::string getMunicipio() const;

    /** @return Precio por noche. */
    float getPrecioNoche() const;

    /** @brief Establece el precio por noche. */
    void setPrecioNoche(float precio);

    /** @return Puntero al anfitrión asociado. */
    Anfitrion* getAnfitrion() const;

    // ==== Gestión de disponibilidad ====

    /**
     * @brief Verifica si el alojamiento está disponible en un rango de fechas.
     * @param entrada Fecha de entrada.
     * @param duracion Número de noches.
     * @return true si está disponible.
     */
    bool estaDisponible(const Fecha& entrada, int duracion) const;

    /**
     * @brief Marca como ocupadas las fechas correspondientes a una reserva.
     * @param entrada Fecha de entrada.
     * @param duracion Duración en noches.
     */
    void reservarDias(const Fecha& entrada, int duracion);

    /**
     * @brief Libera los días ocupados por una reserva.
     * @param inicio Fecha inicial.
     * @param noches Cantidad de noches a liberar.
     */
    void liberarDias(const Fecha& inicio, int noches);

    /**
     * @brief Muestra la información completa del alojamiento.
     */
    void mostrar() const;

    /**
     * @brief Carga todos los alojamientos desde archivo.
     *
     * Además de crear los objetos Alojamiento, también enlaza cada uno con su respectivo anfitrión.
     *
     * @param archivo Nombre del archivo fuente.
     * @param arreglo Referencia al puntero que almacenará el arreglo dinámico.
     * @param cantidad Variable que devolverá la cantidad de alojamientos cargados.
     * @param capacidad Capacidad máxima inicial del arreglo.
     * @param anfitriones Arreglo de anfitriones cargados previamente.
     * @param cantAnfitriones Cantidad de anfitriones existentes.
     */
    static void cargarDesdeArchivo(const std::string& archivo,
                                   Alojamiento*& arreglo,
                                   int& cantidad,
                                   int& capacidad,
                                   Anfitrion* anfitriones,
                                   int cantAnfitriones);
};

#endif // ALOJAMIENTO_H
