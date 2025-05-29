#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <locale>
using namespace std;

#include "anfitrion.h"
#include "huesped.h"
#include "reserva.h"
#include "alojamiento.h"
#include "fecha.h"
#include "controlador.h"

int siguienteNumeroReserva; // Variable global para generar RSV###

Controlador::Controlador() {
    // Inicialización de punteros y arreglo principal de reservas
    anfitriones = nullptr;
    alojamientos = nullptr;
    huespedes = nullptr;
    reservas = new Reserva[10]; // Capacidad inicial

    // Se obtiene el siguiente número de reserva basado en el archivo existente
    siguienteNumeroReserva = obtenerSiguienteNumeroReserva();

    // Carga de archivos externos
    Anfitrion::cargarDesdeArchivo("anfitriones.txt", anfitriones, cantAnfitriones, capAnfitriones);
    Huesped::cargarDesdeArchivo("huespedes.txt", huespedes, cantHuespedes, capHuespedes);
    Alojamiento::cargarDesdeArchivo("alojamientos.txt", alojamientos, cantAlojamientos, capAlojamientos, anfitriones, cantAnfitriones);
    Reserva::cargarDesdeArchivo("reservas.txt", reservas, cantReservas, capReservas, alojamientos, cantAlojamientos, huespedes, cantHuespedes);
}

// Punto de entrada del sistema
void Controlador::iniciar() {
    menuPrincipal();
}

// Menú principal con opción de iniciar sesión
void Controlador::menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1. Iniciar sesión\n";
        cout << "0. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1: iniciarSesion(); break;
        case 0: cout << "\nGracias por usar el sistema. ¡Hasta luego!\n"; break;
        default: cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

// Verifica documento y clave para iniciar sesión
void Controlador::iniciarSesion() {
    string doc, claveIngresada;
    cout << "\n--- Iniciar sesión ---\n";
    cout << "Ingrese su número de documento: ";
    cin >> doc;
    cout << "Ingrese su clave: ";
    cin >> claveIngresada;

    // Validación para huésped
    for (int i = 0; i < cantHuespedes; ++i) {
        if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == claveIngresada) {
            mostrarMenuHuesped(&huespedes[i]);
            return;
        }
    }

    // Validación para anfitrión
    for (int i = 0; i < cantAnfitriones; ++i) {
        if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == claveIngresada) {
            mostrarMenuAnfitrion(&anfitriones[i]);
            return;
        }
    }

    cout << "Documento o clave incorrectos.\n";
}

// Menú con funcionalidades exclusivas para huéspedes
void Controlador::mostrarMenuHuesped(Huesped* h) {
    int opcion;
    do {
        cout << "\n=== MENÚ HUÉSPED ===\n";
        cout << "Bienvenido, " << h->getNombre() << " (documento: " << h->getDocumento() << ")\n";
        cout << "1. Buscar alojamientos disponibles\n";
        cout << "2. Reservar alojamiento\n";
        cout << "3. Ver antigüedad y puntuación\n";
        cout << "4. Mostrar y/o anular una reservacion\n";
        cout << "0. Cerrar sesión\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1: buscarYReservarAlojamiento(h); break;
        case 2: reservarAlojamiento(h); break;
        case 3:
            cout << "Antigüedad: " << h->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << h->getPuntuacion() << "/5.0\n";
            break;
        case 4: {
            cout << "\n--- Reservaciones activas ---\n";
            int seleccionables[cantReservas], totalMostrar = 0;

            // Mostrar reservas activas del huésped
            for (int i = 0; i < cantReservas; ++i) {
                if (reservas[i].getHuesped()->getDocumento() == h->getDocumento()) {
                    cout << (totalMostrar + 1) << ". ";
                    reservas[i].mostrarComprobante();
                    seleccionables[totalMostrar++] = i;
                }
            }

            if (totalMostrar == 0) {
                cout << "No tiene reservaciones activas.\n";
                break;
            }

            int eleccion;
            cout << "Seleccione la reserva a anular (0 para cancelar): ";
            cin >> eleccion;
            if (eleccion < 1 || eleccion > totalMostrar) break;

            int idx = seleccionables[eleccion - 1];
            Reserva* r = &reservas[idx];
            anularReservacion(r->getCodigo(), h, nullptr);
            break;
        }
        case 0: cout << "Sesión finalizada.\n"; break;
        default: cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

// Menú con funciones para anfitriones (gestión de alojamientos y reservas)
void Controlador::mostrarMenuAnfitrion(Anfitrion* a) {
    int opcion;
    do {
        cout << "\n=== MENÚ ANFITRIÓN ===\n";
        cout << "Bienvenido, " << a->getNombre() << " (documento: " << a->getDocumento() << ")\n";
        cout << "1. Mostrar mis alojamientos\n";
        cout << "2. Ver antigüedad y puntuación\n";
        cout << "3. Anular una reservación asociada\n";
        cout << "4. Consultar reservaciones en rango de fechas\n";
        cout << "5. Actualizar histórico de reservas\n";
        cout << "6. Mostrar histórico de reservas\n";
        cout << "0. Cerrar sesión\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1: a->mostrar(); break;
        case 2:
            cout << "Antigüedad: " << a->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << a->getPuntuacion() << "/5.0\n";
            break;
        case 3: {
            string cod;
            cout << "Código de la reservación a anular: ";
            cin >> cod;
            anularReservacion(cod, nullptr, a);
            break;
        }
        case 4: {
            int d1, m1, a1, d2, m2, a2;
            cout << "Ingrese fecha inicial (D M A): ";
            cin >> d1 >> m1 >> a1;
            cout << "Ingrese fecha final (D M A): ";
            cin >> d2 >> m2 >> a2;
            Fecha ini(d1, m1, a1), fin(d2, m2, a2);
            int total = 0;

            // Mostrar reservas en el rango indicado
            for (int i = 0; i < cantReservas; ++i) {
                Reserva& r = reservas[i];
                if (r.getAlojamiento()->getAnfitrion() == a &&
                    r.getFechaEntrada().esMayorQue(ini) &&
                    r.getFechaEntrada().esMenorQue(fin)) {
                    cout << "\n[" << ++total << "] ";
                    r.mostrarComprobante();
                }
            }

            if (total == 0)
                cout << "No se encontraron reservaciones en ese rango.\n";

            break;
        }
        case 5: {
            int d, m, a;
            cout << "Ingrese fecha de corte (D M A): ";
            cin >> d >> m >> a;
            Fecha fechaCorte(d, m, a);
            depurarReservas(fechaCorte);              // Mueve a histórico
            sobrescribirArchivoReservas();            // Actualiza archivo principal
            cout << "Histórico actualizado.\n";
            break;
        }
        case 6: mostrarHistorico(); break;
        case 0: cout << "Sesión finalizada.\n"; break;
        default: cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}
