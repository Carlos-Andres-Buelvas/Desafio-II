#include "base_datos.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#include "alojamiento.h"
#include "fecha.h"
#include <iomanip>

using namespace std;

// ----------------------------
// 1. Cargar todos los datos
// ----------------------------
void cargarBaseDatos(Anfitrion*& anfitriones, int& cantAnfitriones,
                     Alojamiento*& alojamientos, int& cantAlojamientos,
                     Huesped*& huespedes, int& cantHuespedes,
                     Reserva*& reservas, int& cantReservas)
{
    // ---------- ANFITRIONES ----------
    ifstream archAnfit("anfitriones.txt");
    if (!archAnfit.is_open()) {
        cout << "Error abriendo anfitriones.txt" << endl;
        return;
    }
    string linea;
    getline(archAnfit, linea); // encabezado
    int capAnfit = 10;
    cantAnfitriones = 0;
    anfitriones = new Anfitrion[capAnfit];

    while (getline(archAnfit, linea)) {
        stringstream ss(linea);
        string doc, antigStr, puntStr;
        getline(ss, doc, ';');
        getline(ss, antigStr, ';');
        getline(ss, puntStr, ';');
        int antig = stoi(antigStr);
        float punt = stof(puntStr);

        if (cantAnfitriones >= capAnfit) {
            capAnfit *= 2;
            Anfitrion* temp = new Anfitrion[capAnfit];
            for (int i = 0; i < cantAnfitriones; ++i)
                temp[i] = anfitriones[i];  // usa operador de copia

            delete[] anfitriones;
            anfitriones = temp;
        }

        anfitriones[cantAnfitriones++] = Anfitrion(doc, antig, punt);
    }
    archAnfit.close();

    // ---------- ALOJAMIENTOS ----------
    ifstream archAloj("alojamientos.txt");
    if (!archAloj.is_open()) {
        cout << "Error abriendo alojamientos.txt" << endl;
        return;
    }
    getline(archAloj, linea); // encabezado
    int capAloj = 10;
    cantAlojamientos = 0;
    alojamientos = new Alojamiento[capAloj];

    while (getline(archAloj, linea)) {
        stringstream ss(linea);
        string cod, nom, docAnfit, dep, mun, tipo, dir, precioStr, amenStr;

        getline(ss, cod, ';');
        getline(ss, nom, ';');
        getline(ss, docAnfit, ';');
        getline(ss, dep, ';');
        getline(ss, mun, ';');
        getline(ss, tipo, ';');
        getline(ss, dir, ';');
        getline(ss, precioStr, ';');
        getline(ss, amenStr, ';');

        float precio = stof(precioStr);
        bool amen[6] = {0};
        stringstream ssAmen(amenStr);
        string bit; int i = 0;
        while (getline(ssAmen, bit, ',') && i < 6)
            amen[i++] = (bit == "1");

        // Buscar anfitrión
        Anfitrion* a = nullptr;
        for (int j = 0; j < cantAnfitriones; ++j)
            if (anfitriones[j].getDocumento() == docAnfit) {
                a = &anfitriones[j];
                break;
            }

        if (!a) {
            cout << "Anfitrión no encontrado para alojamiento " << cod << endl;
            continue;
        }

        if (cantAlojamientos >= capAloj) {
            capAloj *= 2;
            Alojamiento* temp = new Alojamiento[capAloj];
            for (int i = 0; i < cantAlojamientos; ++i) temp[i] = alojamientos[i];
            delete[] alojamientos;
            alojamientos = temp;
        }

        alojamientos[cantAlojamientos] = Alojamiento(cod, dep, mun, nom, tipo, dir, a, precio, amen);
        a->agregarAlojamiento(&alojamientos[cantAlojamientos]);
        cantAlojamientos++;
    }
    archAloj.close();

    // ---------- HUESPEDES ----------
    ifstream archHuesp("huespedes.txt");
    if (!archHuesp.is_open()) {
        cout << "Error abriendo huespedes.txt" << endl;
        return;
    }
    getline(archHuesp, linea); // encabezado
    int capHuesp = 10;
    cantHuespedes = 0;
    huespedes = new Huesped[capHuesp];

    while (getline(archHuesp, linea)) {
        stringstream ss(linea);
        string doc, antigStr, puntStr;
        getline(ss, doc, ';');
        getline(ss, antigStr, ';');
        getline(ss, puntStr, ';');
        int antig = stoi(antigStr);
        float punt = stof(puntStr);

        if (cantHuespedes >= capHuesp) {
            capHuesp *= 2;
            Huesped* temp = new Huesped[capHuesp];
            for (int i = 0; i < cantHuespedes; ++i) temp[i] = huespedes[i];
            delete[] huespedes;
            huespedes = temp;
        }

        huespedes[cantHuespedes++] = Huesped(doc, antig, punt);
    }
    archHuesp.close();


    // ---------- RESERVAS ----------
    ifstream archRes("reservas.txt");
    if (!archRes.is_open()) {
        cout << "Error abriendo reservas.txt" << endl;
        return;
    }

    getline(archRes, linea); // Leer encabezado
    int capRes = 10;
    cantReservas = 0;
    reservas = new Reserva[capRes];

    while (getline(archRes, linea)) {
        stringstream ss(linea);
        string cod, codAloj, docHuesp, fechaIn, durStr, metodo, fechaPag, montoStr, nota;

        getline(ss, cod, ';');
        getline(ss, codAloj, ';');
        getline(ss, docHuesp, ';');
        getline(ss, fechaIn, ';');
        getline(ss, durStr, ';');
        getline(ss, metodo, ';');
        getline(ss, fechaPag, ';');
        getline(ss, montoStr, ';');
        getline(ss, nota, ';');

        int dur = stoi(durStr);
        float monto = stof(montoStr);
        int d, m, a;

        sscanf(fechaIn.c_str(), "%d/%d/%d", &d, &m, &a);
        Fecha entrada(d, m, a);

        sscanf(fechaPag.c_str(), "%d/%d/%d", &d, &m, &a);
        Fecha pago(d, m, a);

        // Buscar huésped y alojamiento
        Huesped* h = nullptr;
        Alojamiento* alo = nullptr;

        for (int i = 0; i < cantHuespedes; ++i) {
            if (huespedes[i].getDocumento() == docHuesp) {
                h = &huespedes[i];
                break;
            }
        }

        for (int i = 0; i < cantAlojamientos; ++i) {
            if (alojamientos[i].getCodigo() == codAloj) {
                alo = &alojamientos[i];
                break;
            }
        }

        if (!h || !alo) {
            cout << "Error: reserva " << cod << " inválida (huésped o alojamiento no encontrado)." << endl;
            continue;
        }

        // Validar conflicto antes de crear la reserva
        if (h->hayConflicto(entrada, dur)) {
            cout << "Reserva " << cod << " NO agregada al huésped por conflicto de fechas." << endl;
            continue;
        }

        // Redimensionar si es necesario
        if (cantReservas >= capRes) {
            capRes *= 2;
            Reserva* temp = new Reserva[capRes];
            for (int i = 0; i < cantReservas; ++i)
                temp[i] = reservas[i];
            delete[] reservas;
            reservas = temp;
        }

        // Crear reserva, asignar a huésped y marcar días ocupados
        reservas[cantReservas] = Reserva(cod, alo, h, entrada, dur, metodo, pago, monto, nota);
        h->agregarReserva(&reservas[cantReservas]);
        alo->reservarDias(entrada, dur);
        cantReservas++;
    }

    archRes.close();
}

// ----------------------------
// 2. Depurar reservas antiguas
// ----------------------------
void depurarReservas(Reserva*& reservas, int& cantidad, const Fecha& fechaCorte) {
    ofstream archivo("historico_reservas.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo histórico." << endl;
        return;
    }

    int nuevaCantidad = 0;
    for (int i = 0; i < cantidad; ++i) {
        if (reservas[i].getFechaEntrada().esMayorQue(fechaCorte) ||
            reservas[i].getFechaEntrada().toEntero() == fechaCorte.toEntero()) {
            reservas[nuevaCantidad++] = reservas[i];
        } else {
            archivo << reservas[i].getCodigo() << ";"
                    << reservas[i].getAlojamiento()->getCodigo() << ";"
                    << reservas[i].getHuesped()->getDocumento() << ";"
                    << reservas[i].getFechaEntrada().getDia() << "/"
                    << reservas[i].getFechaEntrada().getMes() << "/"
                    << reservas[i].getFechaEntrada().getAnio() << ";"
                    << reservas[i].getDuracion() << ";"
                    << reservas[i].getMetodoPago() << ";"
                    << reservas[i].getFechaPago().getDia() << "/"
                    << reservas[i].getFechaPago().getMes() << "/"
                    << reservas[i].getFechaPago().getAnio() << ";"
                    << reservas[i].getMonto() << ";"
                    << reservas[i].getAnotacion() << endl;
        }
    }

    cantidad = nuevaCantidad;
    archivo.close();
}

// ----------------------------
// 3. Mostrar reservas históricas
// ----------------------------
void mostrarHistorico() {
    ifstream archivo("historico_reservas.txt");
    if (!archivo.is_open()) {
        cout << "No hay historial disponible." << endl;
        return;
    }

    string linea;
    cout << "------ HISTÓRICO DE RESERVAS ------\n";
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    cout << "-----------------------------------\n";
    archivo.close();
}

void buscarAlojamientosDisponibles(Alojamiento* alojamientos, int cantA,
                                   const Fecha& entrada, int duracion,
                                   const string& municipio,
                                   float precioMax = -1,
                                   float puntMin = -1)
{
    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    bool encontrados = false;

    for (int i = 0; i < cantA; ++i) {
        Alojamiento& a = alojamientos[i];

        // Filtrar por municipio
        if (a.getMunicipio() != municipio)
            continue;

        // Filtrar por disponibilidad
        if (!a.estaDisponible(entrada, duracion))
            continue;

        // Filtro por precio (si se aplicó)
        if (precioMax != -1 && a.getPrecioNoche() > precioMax)
            continue;

        // Filtro por puntuación del anfitrión (si se aplicó)
        if (puntMin != -1 && a.getAnfitrion()->getPuntuacion() < puntMin)
            continue;

        // Si pasa todos los filtros, lo mostramos
        a.mostrar();
        cout << "-------------------------\n";
        encontrados = true;
    }

    if (!encontrados)
        cout << "No se encontraron alojamientos disponibles que cumplan con los criterios." << endl;
}

void anularReservacion(const string& codigo, Huesped* huesped, Anfitrion* anfitrion,
                       Reserva*& reservas, int& cantReservas)
{
    for (int i = 0; i < cantReservas; ++i) {
        if (reservas[i].getCodigo() == codigo) {
            // Validación según tipo de usuario
            bool autorizado = false;

            if (huesped && reservas[i].getHuesped()->getDocumento() == huesped->getDocumento()) {
                autorizado = true;
            } else if (anfitrion && reservas[i].getAlojamiento()->getAnfitrion()->getDocumento() == anfitrion->getDocumento()) {
                autorizado = true;
            }

            if (!autorizado) {
                cout << "No tiene permiso para anular esta reservación.\n";
                return;
            }

            // Eliminar reserva
            for (int j = i; j < cantReservas - 1; ++j)
                reservas[j] = reservas[j + 1];

            cantReservas--;
            cout << "Reservación anulada con éxito.\n";
            return;
        }
    }

    cout << "Reservación no encontrada.\n";
}
