#include "alojamiento.h"
#include "anfitrion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>  // Solo si se usa entorno Qt

// Constructor por defecto
Alojamiento::Alojamiento()
    : anfitrion(nullptr), precioNoche(0.0f),
    fechasOcupadas(new Fecha[30]), cantFechas(0), capFechas(100) {
    // Inicializa todas las amenidades como "false"
    amenidades[0] = amenidades[1] = amenidades[2] = false;
    amenidades[3] = amenidades[4] = amenidades[5] = false;
}

// Constructor parametrizado
Alojamiento::Alojamiento(std::string cod, std::string dep, std::string mun, std::string nom,
                         std::string tipo_, std::string dir, Anfitrion* a, float precio,
                         const bool amen[NUM_AMENIDADES])
    : codigo(cod), departamento(dep), municipio(mun), nombre(nom), tipo(tipo_),
    direccion(dir), anfitrion(a), precioNoche(precio),
    fechasOcupadas(new Fecha[capFechas]), cantFechas(0), capFechas(10) {
    // Copia las amenidades desde el arreglo recibido
    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = amen[i];
}

// ------------ Getters y Setters ------------
std::string Alojamiento::getCodigo() const { return codigo; }
void Alojamiento::setCodigo(const std::string& cod) { codigo = cod; }

std::string Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecioNoche() const { return precioNoche; }
void Alojamiento::setPrecioNoche(float precio) { precioNoche = precio; }

Anfitrion* Alojamiento::getAnfitrion() const { return anfitrion; }

// ------------ Gestión de Fechas de Reserva ------------

// Verifica si una fecha ya está ocupada en este alojamiento
bool Alojamiento::contieneFecha(const Fecha& f) const {
    for (int i = 0; i < cantFechas; ++i)
        if (fechasOcupadas[i].toEntero() == f.toEntero())
            return true;
    return false;
}

// Agrega una nueva fecha ocupada, redimensionando si es necesario
void Alojamiento::agregarFecha(const Fecha& f) {
    if (contieneFecha(f)) return;

    if (cantFechas >= capFechas) {
        capFechas *= 2;
        Fecha* nuevo = new Fecha[capFechas];
        for (int i = 0; i < cantFechas; ++i)
            nuevo[i] = fechasOcupadas[i];
        delete[] fechasOcupadas;
        fechasOcupadas = nuevo;
    }

    fechasOcupadas[cantFechas++] = f;
}

// Verifica disponibilidad de un rango de fechas
bool Alojamiento::estaDisponible(const Fecha& entrada, int duracion) const {
    for (int i = 0; i < duracion; ++i)
        if (contieneFecha(entrada.sumarDias(i)))
            return false;
    return true;
}

// Marca fechas como ocupadas tras una reserva
void Alojamiento::reservarDias(const Fecha& entrada, int duracion) {
    for (int i = 0; i < duracion; ++i)
        agregarFecha(entrada.sumarDias(i));
}

// Libera fechas en caso de anulación
void Alojamiento::liberarDias(const Fecha& inicio, int noches) {
    for (int i = 0; i < noches; ++i) {
        Fecha f = inicio.sumarDias(i);
        for (int j = 0; j < cantFechas; ++j) {
            if (fechasOcupadas[j].toEntero() == f.toEntero()) {
                for (int k = j; k < cantFechas - 1; ++k)
                    fechasOcupadas[k] = fechasOcupadas[k + 1];
                --cantFechas;
                break;
            }
        }
    }
}

// ------------ Mostrar Información del Alojamiento ------------

void Alojamiento::mostrar() const {
    if (codigo.empty()) {
        std::cout << "[ERROR] Alojamiento con código vacío. Posible dato corrupto.\n";
        return;
    }

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
        if (amenidades[i]) {
            if (hayAlguna) std::cout << ", ";
            std::cout << nombres[i];
            hayAlguna = true;
        }
    }
    if (!hayAlguna) std::cout << "ninguna";
    std::cout << '\n';
}

// ------------ Copia y Asignación ------------

Alojamiento::Alojamiento(const Alojamiento& otro)
    : codigo(otro.codigo), departamento(otro.departamento), municipio(otro.municipio),
    nombre(otro.nombre), tipo(otro.tipo), direccion(otro.direccion),
    anfitrion(otro.anfitrion), precioNoche(otro.precioNoche),
    fechasOcupadas(new Fecha[otro.capFechas]), cantFechas(otro.cantFechas),
    capFechas(otro.capFechas) {

    for (int i = 0; i < NUM_AMENIDADES; ++i)
        amenidades[i] = otro.amenidades[i];

    for (int i = 0; i < cantFechas; ++i)
        fechasOcupadas[i] = otro.fechasOcupadas[i];
}

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

        fechasOcupadas = new Fecha[otro.capFechas];
        cantFechas = otro.cantFechas;
        capFechas = otro.capFechas;
        for (int i = 0; i < cantFechas; ++i)
            fechasOcupadas[i] = otro.fechasOcupadas[i];
    }
    return *this;
}

// ------------ Carga desde archivo ------------

// Función auxiliar para encontrar anfitrión por documento
static Anfitrion* buscarAnfitrionPorDocumento(Anfitrion* anfitriones, int cantidad, const std::string& doc) {
    for (int i = 0; i < cantidad; ++i) {
        if (anfitriones[i].getDocumento() == doc)
            return &anfitriones[i];
    }
    return nullptr;
}

// Carga los alojamientos desde archivo .txt
void Alojamiento::cargarDesdeArchivo(const std::string& archivo,
                                     Alojamiento*& arreglo,
                                     int& cantidad,
                                     int& capacidad,
                                     Anfitrion* anfitriones,
                                     int cantAnfitriones) {
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
    Anfitrion** anfitrionesTemp = new Anfitrion*[capacidad]; // arreglo auxiliar

    while (std::getline(in, linea)) {
        std::stringstream ss(linea);
        std::string cod, nom, docAnfit, dep, mun, tipo, dir, precioStr, amenStr;

        std::getline(ss, cod, ';'); std::getline(ss, nom, ';'); std::getline(ss, docAnfit, ';');
        std::getline(ss, dep, ';'); std::getline(ss, mun, ';'); std::getline(ss, tipo, ';');
        std::getline(ss, dir, ';'); std::getline(ss, precioStr, ';'); std::getline(ss, amenStr, ';');

        float precio = std::stof(precioStr);
        bool amen[NUM_AMENIDADES] = {false};
        std::stringstream ssAmen(amenStr);
        std::string bit; int i = 0;
        while (std::getline(ssAmen, bit, ',') && i < NUM_AMENIDADES)
            amen[i++] = (bit == "1");

        Anfitrion* anfitrion = buscarAnfitrionPorDocumento(anfitriones, cantAnfitriones, docAnfit);
        if (!anfitrion) {
            std::cerr << "[ERROR] Anfitrión no encontrado para alojamiento " << cod << "\n";
            continue;
        }

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Alojamiento* nuevoArr = new Alojamiento[capacidad];
            Anfitrion** nuevoAnfit = new Anfitrion*[capacidad];
            for (int j = 0; j < cantidad; ++j) {
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

    // Relaciona los alojamientos con sus anfitriones
    for (int i = 0; i < cantidad; ++i)
        anfitrionesTemp[i]->agregarAlojamiento(&arreglo[i]);

    delete[] anfitrionesTemp;
}

// Destructor: libera memoria del arreglo de fechas ocupadas
Alojamiento::~Alojamiento() {
    delete[] fechasOcupadas;
}
