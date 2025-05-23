#include <iostream>
using namespace std;

#include "base_datos.h"
#include "anfitrion.h"
#include "huesped.h"
#include "reserva.h"
#include "alojamiento.h"
#include "fecha.h"

void menuBuscarAlojamiento(Alojamiento* alojamientos, int cantAlojamientos);
void reservarAlojamiento(Huesped* h, Alojamiento* alojamientos, int& cantAlojamientos,
                         Reserva*& reservas, int& cantReservas, int& capReservas);
void anularReservacion(const string& codigo, Huesped* huesped, Anfitrion* anfitrion,
                       Reserva*& reservas, int& cantReservas);

void buscarYReservarAlojamiento(Huesped* h,
                                Alojamiento* alojamientos, int cantAlojamientos,
                                Reserva*& reservas, int& cantReservas, int& capReservas)
{
    cout << "--- Buscar y reservar alojamiento ---\n";
    string municipio;
    int d, m, a, noches;

    cout << "Municipio: ";
    cin >> ws;
    getline(cin, municipio);

    cout << "Fecha de entrada (D M A): ";
    cin >> d >> m >> a;
    cout << "Duración (noches): ";
    cin >> noches;
    Fecha entrada(d, m, a);

    // Filtros opcionales
    char aplicarFiltro;
    float precioMax = -1, puntMin = -1;
    cout << "¿Desea aplicar filtros de precio y puntuación? (s/n): ";
    cin >> aplicarFiltro;

    if (aplicarFiltro == 's' || aplicarFiltro == 'S') {
        cout << "Ingrese precio máximo por noche (-1 si no desea aplicar): ";
        cin >> precioMax;
        cout << "Ingrese puntuación mínima del anfitrión (-1 si no desea aplicar): ";
        cin >> puntMin;
    }

    // Buscar alojamientos disponibles
    const int MAX_DISPONIBLES = 100;
    Alojamiento* disponibles[MAX_DISPONIBLES];
    int cantDisponibles = 0;

    for (int i = 0; i < cantAlojamientos; ++i) {
        Alojamiento& a = alojamientos[i];
        if (a.getMunicipio() == municipio &&
            a.estaDisponible(entrada, noches) &&
            (precioMax == -1 || a.getPrecioNoche() <= precioMax) &&
            (puntMin == -1 || a.getAnfitrion()->getPuntuacion() >= puntMin))
        {
            if (cantDisponibles < MAX_DISPONIBLES)
                disponibles[cantDisponibles++] = &a;
        }
    }

    if (cantDisponibles == 0) {
        cout << "No hay alojamientos disponibles que cumplan con los criterios.\n";
        return;
    }

    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    for (int i = 0; i < cantDisponibles; ++i) {
        cout << i + 1 << ".\n";
        disponibles[i]->mostrar();
        cout << "Puntuación del anfitrión: "
             << disponibles[i]->getAnfitrion()->getPuntuacion() << " / 5.0\n";
        cout << "----------------------------\n";
    }

    int opcion;
    cout << "\nSeleccione el número del alojamiento para reservar (0 para cancelar): ";
    cin >> opcion;

    if (opcion < 1 || opcion > cantDisponibles) {
        cout << "Reserva cancelada.\n";
        return;
    }

    Alojamiento* aloj = disponibles[opcion - 1];

    // Pedir anotación
    cin.ignore(); // limpiar buffer
    string nota;
    cout << "Ingrese una anotación para el anfitrión (máx 1000 caracteres): ";
    getline(cin, nota);

    // Crear reserva
    string codigo = "RSV" + to_string(cantReservas + 1);
    string metodo = "PSE"; // fijo
    Fecha fechaPago = entrada;
    float monto = noches * aloj->getPrecio();

    Reserva* nueva = new Reserva(codigo, aloj, h, entrada, noches, metodo, fechaPago, monto, nota);

    if (!h->agregarReserva(nueva)) {
        cout << "Conflicto de fechas con otra reserva del huésped. No se realizó la reserva.\n";
        delete nueva;
        return;
    }

    if (cantReservas >= capReservas) {
        capReservas *= 2;
        Reserva* temp = new Reserva[capReservas];
        for (int i = 0; i < cantReservas; ++i)
            temp[i] = reservas[i];
        delete[] reservas;
        reservas = temp;
    }

    reservas[cantReservas] = *nueva;
    h->agregarReserva(&reservas[cantReservas]);
    aloj->reservarDias(entrada, noches);
    cantReservas++;

    delete nueva;

    cout << "Reserva confirmada. Código: " << codigo << "\n";
}

void reservarAlojamiento(Huesped* h, Alojamiento* alojamientos, int& cantAlojamientos,
                         Reserva*& reservas, int& cantReservas, int& capReservas)
{
    cout << "--- Reservar alojamiento ---\n";
    string cod;
    int d, m, a, dur;
    cout << "Código del alojamiento: ";
    cin >> cod;
    cout << "Fecha entrada (D M A): ";
    cin >> d >> m >> a;
    cout << "Duración (noches): ";
    cin >> dur;
    Fecha entrada(d, m, a);

    Alojamiento* aloj = nullptr;
    for (int i = 0; i < cantAlojamientos; ++i) {
        if (alojamientos[i].getCodigo() == cod) {
            aloj = &alojamientos[i];
            break;
        }
    }

    if (!aloj) {
        cout << "Alojamiento no encontrado.\n";
        return;
    }

    if (!aloj->estaDisponible(entrada, dur)) {
        cout << "El alojamiento no está disponible.\n";
        return;
    }

    // Generar los datos de la reserva
    string codigo = "RSV" + to_string(cantReservas + 1);
    string metodo = "PSE"; // puede pedirse al usuario si deseas
    Fecha fechaPago = entrada; // Simulación de pago en fecha de entrada
    float monto = dur * aloj->getPrecio();
    string nota = "Reserva manual.";

    // Crear el objeto reserva
    Reserva* nueva = new Reserva(codigo, aloj, h, entrada, dur, metodo, fechaPago, monto, nota);

    if (!h->agregarReserva(nueva)) {
        cout << "El huésped ya tiene una reserva en esas fechas. No se pudo realizar la reserva." << endl;
        delete nueva;
        return;
    }

    // Redimensionar si es necesario
    if (cantReservas >= capReservas) {
        capReservas *= 2;
        Reserva* temp = new Reserva[capReservas];
        for (int i = 0; i < cantReservas; ++i)
            temp[i] = reservas[i];
        delete[] reservas;
        reservas = temp;
    }

    reservas[cantReservas] = *nueva;
    h->agregarReserva(&reservas[cantReservas]);
    aloj->reservarDias(entrada, dur);
    cantReservas++;

    delete nueva;

    // Mostrar comprobante
    cout << "\nReserva confirmada. Código: " << codigo << endl;
    entrada.mostrarExtendido();
    Fecha salida = entrada.sumarDias(dur);
    salida.mostrarExtendido();
}

void mostrarMenuAnfitrion(Anfitrion* a, Reserva*& reservas, int& cantReservas) {
    int opcion;
    do {
        cout << "\n=== MENÚ ANFITRIÓN ===\n";
        cout << "Bienvenido, " << a->getNombre() << " (documento: " << a->getDocumento() << ")\n";
        cout << "1. Mostrar mis alojamientos\n";
        cout << "2. Ver antigüedad y puntuación\n";
        cout << "3. Anular una reservación asociada a mis alojamientos\n";
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
        case 3: {
            string cod;
            cout << "Código de la reservación a anular: ";
            cin >> cod;
            anularReservacion(cod, nullptr, a, reservas, cantReservas);
            break;
        }
        case 0:
            cout << "Sesión finalizada.\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

void mostrarMenuHuesped(Huesped* h,
                        Alojamiento* alojamientos, int cantAlojamientos,
                        Reserva*& reservas, int& cantReservas, int& capReservas)
{
    int opcion;
    do {
        cout << "\n=== MENÚ HUÉSPED ===\n";
        cout << "Bienvenido, " << h->getNombre() << " (documento: " << h->getDocumento() << ")\n";
        cout << "1. Buscar alojamientos disponibles\n";
        cout << "2. Reservar alojamiento\n";
        cout << "3. Mostrar mis reservaciones\n";
        cout << "4. Ver antigüedad y puntuación\n";
        cout << "5. Anular una reservación\n";
        cout << "0. Cerrar sesión\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            buscarYReservarAlojamiento(h, alojamientos, cantAlojamientos, reservas, cantReservas, capReservas);
            break;
        case 2:
            reservarAlojamiento(h, alojamientos, cantAlojamientos,
                                reservas, cantReservas, capReservas);
            break;
        case 3:
            h->mostrar(); // Mostrar reservas del huésped
            break;
        case 4:
            cout << "Antigüedad: " << h->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << h->getPuntuacion() << "/5.0\n";
            break;
        case 5: {
            string cod;
            cout << "Código de la reservación a anular: ";
            cin >> cod;
            anularReservacion(cod, h, nullptr, reservas, cantReservas);
            break;
        }
        case 0:
            cout << "Sesión finalizada.\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

void iniciarSesion(Huesped* huespedes, int cantHuespedes,
                   Anfitrion* anfitriones, int cantAnfitriones,
                   Alojamiento* alojamientos, int cantAlojamientos,
                   Reserva*& reservas, int& cantReservas, int& capReservas)
{
    string doc, claveIngresada;
    cout << "\n--- Iniciar sesión ---\n";
    cout << "Ingrese su número de documento: ";
    cin >> doc;
    cout << "Ingrese su clave: ";
    cin >> claveIngresada;

    for (int i = 0; i < cantHuespedes; ++i) {
        if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == claveIngresada) {
            mostrarMenuHuesped(&huespedes[i], alojamientos, cantAlojamientos, reservas, cantReservas, capReservas);
            return;
        }
    }

    for (int i = 0; i < cantAnfitriones; ++i) {
        if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == claveIngresada) {
            mostrarMenuAnfitrion(&anfitriones[i], reservas, cantReservas);
            return;
        }
    }

    cout << "Documento o clave incorrectos.\n";
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
    int capReservas = 100; // capacidad inicial
    reservas = new Reserva[capReservas];

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
            iniciarSesion(huespedes, cantHuespedes,
                          anfitriones, cantAnfitriones,
                          alojamientos, cantAlojamientos,
                          reservas, cantReservas, capReservas);
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
