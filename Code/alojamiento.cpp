#include "alojamiento.h"
#include "anfitrion.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Contadores globales para estadísticas
extern int totalIteraciones;
extern int totalMemoria;

/**
 * @brief Constructor por defecto.
 * Inicializa un alojamiento vacío con memoria para 30 fechas reservadas y sin amenidades activadas.
 */
Alojamiento::Alojamiento() : anfitrion(nullptr), precioNoche(0.0f),
    cantFechas(0), capFechas(10), fechasOcupadas(new Fecha[10]) {

    amenidades[0] = amenidades[1] = amenidades[2] = false;
    amenidades[3] = amenidades[4] = amenidades[5] = false;
    totalMemoria += sizeof(Fecha) * 30;
}

/**
 * @brief Constructor parametrizado de alojamiento.
 *
 * @param cod Código del alojamiento.
 * @param dep Departamento.
 * @param mun Municipio.
 * @param nom Nombre del alojamiento.
 * @param tipo_ Tipo de alojamiento (e.g., Casa, Hotel).
 * @param dir Dirección del alojamiento.
 * @param a Puntero al anfitrión dueño.
 * @param precio Precio por noche.
 * @param amen Arreglo booleano de amenidades.
 */
Alojamiento::Alojamiento(std::string cod, std::string dep, std::string mun, std::string nom,
                         std::string tipo_, std::string dir, Anfitrion* a, float precio,
                         const bool amen[NUM_AMENIDADES]) :
    codigo(cod), departamento(dep), municipio(mun), nombre(nom), tipo(tipo_),
    direccion(dir), anfitrion(a), precioNoche(precio),
    cantFechas(0), capFechas(10), fechasOcupadas(new Fecha[capFechas]) {

    totalMemoria += sizeof(Fecha) * capFechas;
    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = amen[i];
}

// ===== Getters y Setters =====
std::string Alojamiento::getCodigo() const { return codigo; }
void Alojamiento::setCodigo(const std::string& cod) { codigo = cod; }

std::string Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecioNoche() const { return precioNoche; }
void Alojamiento::setPrecioNoche(float precio) { precioNoche = precio; }

Anfitrion* Alojamiento::getAnfitrion() const { return anfitrion; }

/**
 * @brief Verifica si una fecha ya está ocupada.
 * @param f Fecha a verificar.
 * @return true si la fecha ya está ocupada, false si está libre.
 */
bool Alojamiento::contieneFecha(const Fecha& f) const {
    for (int i = 0; i < cantFechas; ++i) {
        totalIteraciones++;
        if (fechasOcupadas[i].toEntero() == f.toEntero())
            return true;
    }
    return false;
}

/**
 * @brief Agrega una nueva fecha ocupada.
 * @param f Fecha a marcar como reservada.
 */
void Alojamiento::agregarFecha(const Fecha& f) {
    if (contieneFecha(f)) return;

    if (cantFechas >= capFechas) {
        capFechas *= 2;
        Fecha* nuevo = new Fecha[capFechas];
        totalMemoria += sizeof(Fecha) * capFechas;
        for (int i = 0; i < cantFechas; ++i)
            nuevo[i] = fechasOcupadas[i];
        delete[] fechasOcupadas;
        fechasOcupadas = nuevo;
    }

    fechasOcupadas[cantFechas++] = f;
}

/**
 * @brief Verifica si un alojamiento está disponible para una duración a partir de una fecha dada.
 * @param entrada Fecha de entrada.
 * @param duracion Número de noches.
 * @return true si está disponible, false si ya está ocupado.
 */
bool Alojamiento::estaDisponible(const Fecha& entrada, int duracion) const {
    for (int i = 0; i < duracion; ++i){
        totalIteraciones++;
        if (contieneFecha(entrada.sumarDias(i)))
            return false;
    }
    return true;
}

/**
 * @brief Marca días como ocupados a partir de una fecha y duración.
 */
void Alojamiento::reservarDias(const Fecha& entrada, int duracion) {
    for (int i = 0; i < duracion; ++i){
        totalIteraciones++;
        agregarFecha(entrada.sumarDias(i));
    }
}

/**
 * @brief Libera días previamente reservados.
 * @param inicio Fecha de inicio.
 * @param noches Número de noches a liberar.
 */
void Alojamiento::liberarDias(const Fecha& inicio, int noches) {
    for (int i = 0; i < noches; ++i) {
        totalIteraciones++;
        Fecha f = inicio.sumarDias(i);
        for (int j = 0; j < cantFechas; ++j) {
            totalIteraciones++;
            if (fechasOcupadas[j].toEntero() == f.toEntero()) {
                for (int k = j; k < cantFechas - 1; ++k){
                    totalIteraciones++;
                    fechasOcupadas[k] = fechasOcupadas[k + 1];
                }
                --cantFechas;
                break;
            }
        }
    }
}

/**
 * @brief Muestra todos los datos del alojamiento por consola.
 */
void Alojamiento::mostrar() const {
    if (codigo.empty()) return;

    std::cout << "Código: " << codigo << '\n'
              << "Nombre: " << nombre << '\n'
              << "Ubicación: " << departamento << ", " << municipio << '\n'
              << "Tipo: " << tipo << " | Dirección: " << direccion << '\n'
              << "Precio por noche: $" << precioNoche << '\n'
              << "Amenidades: ";

    const std::string nombres[NUM_AMENIDADES] = {
        "ascensor", "piscina", "aire acondicionado",
        "caja fuerte", "parqueadero", "patio"
    };

    bool hayAlguna = false;
    for (int i = 0; i < NUM_AMENIDADES; ++i) {
        totalIteraciones++;
        if (amenidades[i]) {
            if (hayAlguna) std::cout << ", ";
            std::cout << nombres[i];
            hayAlguna = true;
        }
    }
    if (!hayAlguna) std::cout << "ninguna";
    std::cout << '\n';
}

/**
 * @brief Constructor de copia de alojamiento.
 */
Alojamiento::Alojamiento(const Alojamiento& otro) :
    codigo(otro.codigo), departamento(otro.departamento), municipio(otro.municipio),
    nombre(otro.nombre), tipo(otro.tipo), direccion(otro.direccion),
    anfitrion(otro.anfitrion), precioNoche(otro.precioNoche),
    cantFechas(otro.cantFechas), capFechas(otro.capFechas),
    fechasOcupadas(new Fecha[otro.capFechas]) {

    totalMemoria += sizeof(Fecha) * otro.capFechas;
    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = otro.amenidades[i];

    for (int i = 0; i < cantFechas; ++i)
        fechasOcupadas[i] = otro.fechasOcupadas[i];
}

/**
 * @brief Operador de asignación para alojamiento.
 */
Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this != &otro) {
        delete[] fechasOcupadas;

        codigo = otro.codigo;
        departamento = otro.departamento;
        municipio = otro.municipio;
        nombre = otro.nombre;
        tipo = otro.tipo;
        direccion = otro.direccion;
        anfitrion = otro.anfitrion;
        precioNoche = otro.precioNoche;

        for (int i = 0; i < NUM_AMENIDADES; ++i)
            amenidades[i] = otro.amenidades[i];

        cantFechas = otro.cantFechas;
        capFechas = otro.capFechas;
        fechasOcupadas = new Fecha[capFechas];
        for (int i = 0; i < cantFechas; ++i)
            fechasOcupadas[i] = otro.fechasOcupadas[i];
    }
    return *this;
}

static Anfitrion* buscarAnfitrionPorDocumento(Anfitrion* anfitriones, int cantidad, const std::string& doc) {
    for (int i = 0; i < cantidad; ++i) {
        if (anfitriones[i].getDocumento() == doc)
            return &anfitriones[i];
    }
    return nullptr;
}

/**
 * @brief Carga todos los alojamientos desde un archivo TXT y los enlaza a sus anfitriones.
 *
 * @param archivo Ruta del archivo TXT.
 * @param arreglo Arreglo de alojamientos dinámico.
 * @param cantidad Número de alojamientos leídos.
 * @param capacidad Capacidad del arreglo dinámico.
 * @param anfitriones Arreglo de anfitriones ya cargado.
 * @param cantAnfitriones Número de anfitriones.
 */
void Alojamiento::cargarDesdeArchivo(const std::string& archivo,
                                     Alojamiento*& arreglo,
                                     int& cantidad,
                                     int& capacidad,
                                     Anfitrion* anfitriones,
                                     int cantAnfitriones)
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
    arreglo = new Alojamiento[capacidad];
    totalMemoria += sizeof(Alojamiento) * capacidad;

    Anfitrion** anfitrionesTemp = new Anfitrion*[capacidad];
    totalMemoria += sizeof(Anfitrion) * capacidad;

    while (std::getline(in, linea)) {
        totalIteraciones++;
        std::stringstream ss(linea);
        std::string cod, nom, docAnfit, dep, mun, tipo, dir, precioStr, amenStr;

        std::getline(ss, cod, ';'); std::getline(ss, nom, ';'); std::getline(ss, docAnfit, ';');
        std::getline(ss, dep, ';'); std::getline(ss, mun, ';'); std::getline(ss, tipo, ';');
        std::getline(ss, dir, ';'); std::getline(ss, precioStr, ';'); std::getline(ss, amenStr, ';');

        float precio = std::stof(precioStr);
        bool amen[NUM_AMENIDADES] = {false};
        std::stringstream ssAmen(amenStr);
        std::string bit; int i = 0;
        while (std::getline(ssAmen, bit, ',') && i < NUM_AMENIDADES){
            totalIteraciones++;
            amen[i++] = (bit == "1");
        }

        Anfitrion* anfitrion = buscarAnfitrionPorDocumento(anfitriones, cantAnfitriones, docAnfit);
        if (!anfitrion) {
            std::cerr << "[ERROR] Anfitrión no encontrado para alojamiento " << cod << "\n";
            continue;
        }

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Alojamiento* nuevoArr = new Alojamiento[capacidad];
            totalMemoria += sizeof(Alojamiento) * capacidad;
            Anfitrion** nuevoAnfit = new Anfitrion*[capacidad];
            totalMemoria += sizeof(Anfitrion) * capacidad;

            for (int j = 0; j < cantidad; ++j) {
                totalIteraciones++;
                nuevoArr[j] = arreglo[j];
                nuevoAnfit[j] = anfitrionesTemp[j];
            }

            delete[] arreglo;
            delete[] anfitrionesTemp;
            arreglo = nuevoArr;
            anfitrionesTemp = nuevoAnfit;
        }

        arreglo[cantidad] = Alojamiento(cod, dep, mun, nom, tipo, dir, anfitrion, precio, amen);
        anfitrionesTemp[cantidad] = anfitrion;
        cantidad++;
    }

    in.close();

    for (int i = 0; i < cantidad; ++i) {
        totalIteraciones++;
        anfitrionesTemp[i]->agregarAlojamiento(&arreglo[i]);
    }

    delete[] anfitrionesTemp;
}

/**
 * @brief Destructor. Libera memoria dinámica de fechas ocupadas.
 */
Alojamiento::~Alojamiento() {
    delete[] fechasOcupadas;
}
