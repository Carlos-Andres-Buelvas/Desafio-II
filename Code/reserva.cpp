#include "reserva.h"
#include "alojamiento.h"
#include "huesped.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

// Constructor por defecto: inicializa la reserva vacía y punteros nulos
Reserva::Reserva()
    : codigo(""), alojamiento(nullptr), huesped(nullptr),
    fechaEntrada(), duracion(0), metodoPago(""),
    fechaPago(), monto(0), anotacion("") {}

// Constructor parametrizado: crea una reserva con datos completos
Reserva::Reserva(const std::string& cod, Alojamiento* alo, Huesped* h,
                 const Fecha& entrada, int dur, const std::string& metodo,
                 const Fecha& pago, int m, const std::string& nota)
    : codigo(cod), alojamiento(alo), huesped(h),
    fechaEntrada(entrada), duracion(dur), metodoPago(metodo),
    fechaPago(pago), monto(m), anotacion(nota) {}

// Constructor copia: copia todos los atributos, incluyendo punteros
Reserva::Reserva(const Reserva& otra)
    : codigo(otra.codigo), alojamiento(otra.alojamiento),
    huesped(otra.huesped), fechaEntrada(otra.fechaEntrada),
    duracion(otra.duracion), metodoPago(otra.metodoPago),
    fechaPago(otra.fechaPago), monto(otra.monto),
    anotacion(otra.anotacion) {}

// Operador de asignación: copia los campos si no es autoasignación
Reserva& Reserva::operator=(const Reserva& otra) {
    if (this != &otra) {
        codigo = otra.codigo;
        alojamiento = otra.alojamiento;
        huesped = otra.huesped;
        fechaEntrada = otra.fechaEntrada;
        duracion = otra.duracion;
        metodoPago = otra.metodoPago;
        fechaPago = otra.fechaPago;
        monto = otra.monto;
        anotacion = otra.anotacion;
    }
    return *this;
}

// Getters: permiten acceso seguro a los atributos privados
std::string Reserva::getCodigo() const { return codigo; }
Alojamiento* Reserva::getAlojamiento() const { return alojamiento; }
Huesped* Reserva::getHuesped() const { return huesped; }
Fecha Reserva::getFechaEntrada() const { return fechaEntrada; }
int Reserva::getDuracion() const { return duracion; }
std::string Reserva::getMetodoPago() const { return metodoPago; }
Fecha Reserva::getFechaPago() const { return fechaPago; }
int Reserva::getMonto() const { return monto; }
std::string Reserva::getAnotacion() const { return anotacion; }

// Muestra en consola el comprobante detallado de la reserva
void Reserva::mostrarComprobante() const {
    Fecha salida = fechaEntrada.sumarDias(duracion);
    std::cout << "------------------------------\n";
    std::cout << "=== COMPROBANTE DE RESERVA ===\n";
    std::cout << "Reserva: " << codigo << '\n';
    std::cout << "Nombre del huésped: " << (huesped ? huesped->getNombre() : "N/A") << '\n';
    std::cout << "Alojamiento: " << (alojamiento ? alojamiento->getCodigo() : "N/A") << '\n';
    std::cout << "Ubicación: Antioquia, " << alojamiento->getMunicipio() << '\n';  // Se asume Antioquia
    std::cout << "Duración: " << duracion << " noche(s)\n";
    std::cout << "Entrada: "; fechaEntrada.mostrarExtendido();
    std::cout << "Salida: "; salida.mostrarExtendido();
    std::cout << "Método de pago: " << metodoPago << '\n';
    std::cout << "Pago realizado el: "; fechaPago.mostrarExtendido();
    std::cout << "Monto: $" << monto << '\n';
    std::cout << "Anotación: " << anotacion << '\n';
    std::cout << "------------------------------\n";
}

// Verifica si una reserva coincide con una entrada específica
bool Reserva::coincideCon(const Fecha& entrada, int dur) const {
    return fechaEntrada.toEntero() == entrada.toEntero() && duracion == dur;
}

// Función auxiliar para encontrar un huésped por documento
static Huesped* buscarHuesped(Huesped* huespedes, int cant, const std::string& doc) {
    for (int i = 0; i < cant; ++i)
        if (huespedes[i].getDocumento() == doc)
            return &huespedes[i];
    return nullptr;
}

// Función auxiliar para encontrar un alojamiento por código
static Alojamiento* buscarAlojamiento(Alojamiento* alojamientos, int cant, const std::string& cod) {
    for (int i = 0; i < cant; ++i)
        if (alojamientos[i].getCodigo() == cod)
            return &alojamientos[i];
    return nullptr;
}

// Carga reservas desde archivo .txt y las enlaza con huéspedes y alojamientos
void Reserva::cargarDesdeArchivo(const std::string& archivo,
                                 Reserva*& arreglo,
                                 int& cantidad,
                                 int& capacidad,
                                 Alojamiento* alojamientos,
                                 int cantAlojamientos,
                                 Huesped* huespedes,
                                 int cantHuespedes)
{
    std::ifstream in(archivo);
    if (!in.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir " << archivo << "\n";
        return;
    }

    std::string linea;
    std::getline(in, linea);  // Saltar encabezado
    capacidad = 10;
    cantidad = 0;
    arreglo = new Reserva[capacidad];

    while (std::getline(in, linea)) {
        std::stringstream ss(linea);
        std::string cod, codAloj, docHuesp, fechaIn, durStr, metodo, fechaPag, montoStr, nota;

        // Lectura de campos delimitados por ;
        std::getline(ss, cod, ';'); std::getline(ss, codAloj, ';'); std::getline(ss, docHuesp, ';');
        std::getline(ss, fechaIn, ';'); std::getline(ss, durStr, ';'); std::getline(ss, metodo, ';');
        std::getline(ss, fechaPag, ';'); std::getline(ss, montoStr, ';'); std::getline(ss, nota, ';');

        // Conversión de fechas
        int d, m, a;
        sscanf(fechaIn.c_str(), "%d/%d/%d", &d, &m, &a);
        Fecha entrada(d, m, a);
        sscanf(fechaPag.c_str(), "%d/%d/%d", &d, &m, &a);
        Fecha pago(d, m, a);

        int dur = std::stoi(durStr);
        int monto = std::stoi(montoStr);

        // Buscar huésped y alojamiento
        Huesped* h = buscarHuesped(huespedes, cantHuespedes, docHuesp);
        Alojamiento* aObj = buscarAlojamiento(alojamientos, cantAlojamientos, codAloj);

        if (!h || !aObj) {
            std::cerr << "[WARN] Reserva " << cod << " inválida (huesped o alojamiento no encontrado).\n";
            continue;
        }

        // Validar conflicto de fechas
        if (h->hayConflicto(entrada, dur)) {
            std::cerr << "[WARN] Conflicto de fechas en reserva " << cod << " → omitida.\n";
            continue;
        }

        // Redimensionar arreglo si está lleno
        if (cantidad >= capacidad) {
            int nuevaCap = capacidad * 2;
            Reserva* nuevoArr = new Reserva[nuevaCap];
            for (int j = 0; j < cantidad; ++j)
                nuevoArr[j] = arreglo[j];
            delete[] arreglo;
            arreglo = nuevoArr;
            capacidad = nuevaCap;
        }

        // Crear reserva válida y actualizar vínculos cruzados
        arreglo[cantidad] = Reserva(cod, aObj, h, entrada, dur, metodo, pago, monto, nota);
        h->agregarReserva(&arreglo[cantidad]);          // Añadir a huésped
        aObj->reservarDias(entrada, dur);               // Marcar fechas como ocupadas
        cantidad++;
    }

    in.close();
}
