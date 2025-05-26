#include <iostream>
using namespace std;

#include "base_datos.h"
#include "anfitrion.h"
#include "huesped.h"

void mostrarMenuAnfitrion(Anfitrion* a) {
    int opcion;
    do {
        cout << "\n=== MENÚ ANFITRIÓN ===\n";
        cout << "Bienvenido, documento: " << a->getDocumento() << endl;
        cout << "1. Mostrar mis alojamientos\n";
        cout << "2. Ver antigüedad y puntuación\n";
        cout << "0. Cerrar sesión\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            a->mostrar();
            break;
        case 2:
            cout << "Antigüedad: " << a->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << a->getPuntuacion() << "/5.0\n";
            break;
        case 0:
            cout << "Sesión finalizada.\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

void mostrarMenuHuesped(Huesped* h) {
    int opcion;
    do {
        cout << "\n=== MENÚ HUÉSPED ===\n";
        cout << "Bienvenido, documento: " << h->getDocumento() << endl;
        cout << "1. Mostrar mis reservaciones\n";
        cout << "2. Ver antigüedad y puntuación\n";
        cout << "0. Cerrar sesión\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            h->mostrar(); // debe mostrar sus reservas
            break;
        case 2:
            cout << "Antigüedad: " << h->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << h->getPuntuacion() << "/5.0\n";
            break;
        case 0:
            cout << "Sesión finalizada.\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

void iniciarSesion(Huesped* huespedes, int cantHuespedes,
                   Anfitrion* anfitriones, int cantAnfitriones)
{
    string doc;
    cout << "\n--- Iniciar sesión ---\n";
    cout << "Ingrese su número de documento: ";
    cin >> doc;

    // Buscar en huéspedes
    for (int i = 0; i < cantHuespedes; ++i) {
        if (huespedes[i].getDocumento() == doc) {
            mostrarMenuHuesped(&huespedes[i]);
            return;
        }
    }

    // Buscar en anfitriones
    for (int i = 0; i < cantAnfitriones; ++i) {
        if (anfitriones[i].getDocumento() == doc) {
            mostrarMenuAnfitrion(&anfitriones[i]);
            return;
        }
    }

    cout << "Documento no encontrado en la plataforma.\n";
}

// Main principal
int main() {
    // Variables principales
    Anfitrion* anfitriones = nullptr;
    Alojamiento* alojamientos = nullptr;
    Huesped* huespedes = nullptr;
    Reserva* reservas = nullptr;

    int cantAnfitriones = 0;
    int cantAlojamientos = 0;
    int cantHuespedes = 0;
    int cantReservas = 0;

    // Cargar datos desde archivos
    cargarBaseDatos(anfitriones, cantAnfitriones,
                    alojamientos, cantAlojamientos,
                    huespedes, cantHuespedes,
                    reservas, cantReservas);

    // Menú principal
    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1. Iniciar sesión\n";
        cout << "2. Mostrar histórico de reservas\n";
        cout << "3. Aplicar fecha de corte (depurar reservas)\n";
        cout << "0. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            iniciarSesion(huespedes, cantHuespedes, anfitriones, cantAnfitriones);
            break;
        case 2:
            mostrarHistorico();
            break;
        case 3: {
            int d, m, a;
            cout << "Ingrese fecha de corte (D M A): ";
            cin >> d >> m >> a;
            Fecha corte(d, m, a);
            depurarReservas(reservas, cantReservas, corte);
            cout << "Reservas depuradas.\n";
            break;
        }
        case 0:
            cout << "Gracias por usar el sistema. ¡Hasta luego!\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }

    } while (opcion != 0);

    // Liberar memoria
    delete[] reservas;
    delete[] huespedes;
    delete[] alojamientos;
    delete[] anfitriones;

    return 0;
}
