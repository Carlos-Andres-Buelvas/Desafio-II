#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;

#include "base_datos.h"
#include "anfitrion.h"
#include "huesped.h"
#include "reserva.h"
#include "alojamiento.h"
#include "fecha.h"

int siguienteNumeroReserva = 1;

void sobrescribirArchivoReservas(Reserva* reservas, int cantReservas);

int obtenerSiguienteNumeroReserva() {
    ifstream archivo("reservas.txt");
    if (!archivo.is_open()) return 1;

    string linea, ultimoCodigo;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        size_t pos = linea.find(';');
        if (pos != string::npos)
            ultimoCodigo = linea.substr(0, pos);
    }
    archivo.close();

    if (ultimoCodigo.size() < 4) return 1;
    int numero = stoi(ultimoCodigo.substr(3));
    return numero + 1;
}

void guardarReservaIndividual(const Reserva& r) {
    ofstream archivo("reservas.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "[ERROR] No se pudo abrir reservas.txt para escribir.\n";
        return;
    }

    archivo << r.getCodigo() << ";"
            << r.getAlojamiento()->getCodigo() << ";"
            << r.getHuesped()->getDocumento() << ";"
            << setfill('0') << setw(2) << r.getFechaEntrada().getDia() << "/"
            << setw(2) << r.getFechaEntrada().getMes() << "/"
            << r.getFechaEntrada().getAnio() << ";"
            << r.getDuracion() << ";"
            << r.getMetodoPago() << ";"
            << setfill('0') << setw(2) << r.getFechaPago().getDia() << "/"
            << setw(2) << r.getFechaPago().getMes() << "/"
            << r.getFechaPago().getAnio() << ";"
            << (int)r.getMonto() << ";"
            << r.getAnotacion() << "\n";

    archivo.close();
}

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

    Fecha hoy = getFechaSistema();
    Fecha maxPermitida = hoy.sumarDias(365);

    if (entrada.esMenorQue(hoy) || entrada.esMayorQue(maxPermitida)) {
        cout << "La fecha de entrada debe estar entre hoy y dentro de 12 meses máximo.\n";
        return;
    }

    // Filtros opcionales
    char aplicarFiltro;
    float precioMax = -1, puntMin = -1;
    cout << "¿Desea aplicar filtros de precio y puntuación? (s/n): ";
    cin >> aplicarFiltro;
    if (aplicarFiltro=='s' || aplicarFiltro=='S') {
        cout << "Ingrese precio máximo por noche (-1 si no desea aplicar): ";
        cin >> precioMax;
        cout << "Ingrese puntuación mínima del anfitrión (-1 si no desea aplicar): ";
        cin >> puntMin;
    }

    // Recolectar disponibles
    const int MAX_DISPONIBLES = 100;
    Alojamiento* disponibles[MAX_DISPONIBLES];
    int cantDisponibles = 0;
    for (int i = 0; i < cantAlojamientos; ++i) {
        Alojamiento& a = alojamientos[i];

        cout << "[DEBUG] Revisando: " << a.getCodigo() << ", "
             << a.getMunicipio() << ", disponible: "
             << a.estaDisponible(entrada, noches) << endl;

        if (a.getMunicipio() == municipio &&
            a.estaDisponible(entrada, noches) &&
            (precioMax < 0 || a.getPrecioNoche() <= precioMax) &&
            (puntMin < 0 || a.getAnfitrion()->getPuntuacion() >= puntMin))
        {
            if (cantDisponibles < MAX_DISPONIBLES)
                disponibles[cantDisponibles++] = &a;
        }
    }

    if (cantDisponibles == 0) {
        cout << "No hay alojamientos disponibles.\n";
        return;
    }

    // Mostrar lista y elegir
    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    for (int i = 0; i < cantDisponibles; ++i) {
        cout << i + 1 << ".\n";
        disponibles[i]->mostrar();
        cout << "Puntuación del anfitrión: "
             << disponibles[i]->getAnfitrion()->getPuntuacion()
             << " / 5.0\n---------------------\n";
    }

    int opcion;
    cout << "Seleccione número (0 cancelar): ";
    cin >> opcion;
    if (opcion < 1 || opcion > cantDisponibles) {
        cout << "Reserva cancelada.\n";
        return;
    }

    Alojamiento* aloj = disponibles[opcion - 1];

    // Pedir pago
    string metodo;
    cout << "Metodo de pago" << "\n";
    cout << "1. PSE" << "\n";
    cout << "2. TCredito" << "\n";
    cout << "Opción: ";
    cin >> ws;
    getline(cin, metodo);
    if (metodo == "1"){
        metodo = "PSE";
    } else{
        metodo = "TCredito";
    }

    // Pedir anotación
    string nota;
    cout << "Ingrese anotación (max 1000 chars): ";
    cin >> ws;
    getline(cin, nota);

    // ... justo antes de crear la Reserva
    extern int siguienteNumeroReserva; // si está fuera del main()
    std::ostringstream oss;
    oss << "RSV" << std::setw(3) << std::setfill('0') << siguienteNumeroReserva++;
    string codigo = oss.str();
    Fecha fechaPago = entrada;
    float monto = noches * aloj->getPrecio();

    // Verificar conflicto antes de crear la reserva
    if (h->hayConflicto(entrada, noches)) {
        cout << "Conflicto de fechas. No se realizó la reserva.\n";
        return;
    }

    // Redimensionar arreglo si hace falta
    if (cantReservas >= capReservas) {
        capReservas *= 2;
        Reserva* temp = new Reserva[capReservas];
        for (int i = 0; i < cantReservas; ++i)
            temp[i] = reservas[i];
        delete[] reservas;
        reservas = temp;
    }

    // Crear y guardar la reserva
    reservas[cantReservas] = Reserva(codigo, aloj, h, entrada, noches, metodo, fechaPago, monto, nota);
    Reserva* ptrReserva = &reservas[cantReservas];
    ++cantReservas;

    h->agregarReserva(ptrReserva);
    aloj->reservarDias(entrada, noches);
    guardarReservaIndividual(*ptrReserva);

    // Mostrar comprobante usando método encapsulado
    ptrReserva->mostrarComprobante();
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

    Fecha hoy = getFechaSistema();
    Fecha maxPermitida = hoy.sumarDias(365);

    if (entrada.esMenorQue(hoy) || entrada.esMayorQue(maxPermitida)) {
        cout << "La fecha de entrada debe estar entre hoy y dentro de 12 meses máximo.\n";
        return;
    }

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

    // Pedir pago
    string metodo;
    cout << "Metodo de pago" << "\n";
    cout << "1. PSE" << "\n";
    cout << "2. TCredito" << "\n";
    cout << "Opción: ";
    cin >> ws;
    getline(cin, metodo);
    if (metodo == "1"){
        metodo = "PSE";
    } else{
        metodo = "TCredito";
    }

    // Pedir anotación
    string nota;
    cout << "Ingrese anotación (max 1000 chars): ";
    cin >> ws;
    getline(cin, nota);
    /*
    // Generar los datos de la reserva
    string codigo = "RSV" + to_string(cantReservas + 1);
    Fecha fechaPago = entrada; // Simulación de pago en fecha de entrada
    float monto = dur * aloj->getPrecio();
    */
    // ... justo antes de crear la Reserva
    extern int siguienteNumeroReserva; // si está fuera del main()
    std::ostringstream oss;
    oss << "RSV" << std::setw(3) << std::setfill('0') << siguienteNumeroReserva++;
    string codigo = oss.str();
    Fecha fechaPago = entrada;
    float monto = dur * aloj->getPrecio();

    // 1) Creamos la reserva en HEAP para construirla:
    Reserva* nueva = new Reserva(codigo, aloj, h, entrada, dur, metodo, fechaPago, monto, nota);

    // 2) Comprobamos conflictos en el huesped con NUEVA:
    if (h->hayConflicto(entrada, dur)) {
        cout << "El huésped ya tiene una reserva en esas fechas.\n";
        delete nueva;
        return;
    }

    // 3) Aseguramos capacidad y copiamos en tu array principal:
    if (cantReservas >= capReservas) {
        // ... redimensionas ...
    }
    reservas[cantReservas] = *nueva;   // copia "por valor" dentro de tu buffer

    // 4) INSERTAMOS sólo una vez EN EL HUESPED:
    h->agregarReserva(&reservas[cantReservas]);

    // 5) Marcamos los días en el alojamiento:
    aloj->reservarDias(entrada, dur);

    // 6) Avanzamos el contador:
    cantReservas++;

    // 7) Guardamos en disco:
    guardarReservaIndividual(reservas[cantReservas - 1]);

    // 8) Limpieza del temporal:
    delete nueva;

    // Mostrar comprobante usando método encapsulado
    reservas[cantReservas - 1].mostrarComprobante();
}

void mostrarHistorico();  // Declaración requerida si se llama en main
void depurarReservas(Reserva*& reservas, int& cantReservas, const Fecha& corte); // También

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
            reservarAlojamiento(h, alojamientos, cantAlojamientos, reservas, cantReservas, capReservas);
            break;
        case 3:
            h->mostrar();
            break;
        case 4:
            cout << "Antigüedad: " << h->getAntiguedad() << " meses\n";
            cout << "Puntuación: " << h->getPuntuacion() << "/5.0\n";
            break;
        case 5: {
            cout << "\n--- Reservaciones activas ---\n";
            int seleccionables[100], totalMostrar = 0;

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
            cout << "Seleccione el número de la reserva a anular (0 para cancelar): ";
            cin >> eleccion;
            if (eleccion < 1 || eleccion > totalMostrar) break;

            int idx = seleccionables[eleccion - 1];
            anularReservacion(reservas[idx].getCodigo(), h, nullptr, reservas, cantReservas);
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

void mostrarMenuAnfitrion(Anfitrion* a, Reserva*& reservas, int& cantReservas)
{
    int opcion;
    do {
        cout << "\n=== MENÚ ANFITRIÓN ===\n";
        cout << "Bienvenido, " << a->getNombre() << " (documento: " << a->getDocumento() << ")\n";
        cout << "1. Mostrar mis alojamientos\n";
        cout << "2. Ver antigüedad y puntuación\n";
        cout << "3. Anular una reservación asociada a mis alojamientos\n";
        cout << "4. Consultar reservaciones en rango de fechas\n";
        cout << "5. Actualizar histórico de reservas\n";
        cout << "6. Mostrar histórico de reservas\n";
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
        case 4: {
            int d1, m1, a1, d2, m2, a2;
            cout << "Ingrese fecha inicial (D M A): ";
            cin >> d1 >> m1 >> a1;
            cout << "Ingrese fecha final (D M A): ";
            cin >> d2 >> m2 >> a2;
            Fecha ini(d1, m1, a1), fin(d2, m2, a2);

            int total = 0;
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
            cout << "--- Actualizar histórico ---\n";
            cout << "Ingrese fecha de corte (D M A): ";
            cin >> d >> m >> a;
            Fecha fechaCorte(d, m, a);

            // Ejecutar depuración
            depurarReservas(reservas, cantReservas, fechaCorte);

            // Actualizar archivo reservas.txt después de depurar
            sobrescribirArchivoReservas(reservas, cantReservas);

            cout << "Histórico actualizado. Solo se conservaron reservas desde la fecha indicada.\n";
            break;
        }
        case 6:
            mostrarHistorico();
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

int main() {
    Anfitrion* anfitriones = nullptr;
    Alojamiento* alojamientos = nullptr;
    Huesped* huespedes = nullptr;
    Reserva* reservas = nullptr;

    int cantAnfitriones = 0, cantAlojamientos = 0;
    int cantHuespedes = 0, cantReservas = 0;
    int capReservas = 10;

    reservas = new Reserva[capReservas];
    siguienteNumeroReserva = obtenerSiguienteNumeroReserva();

    cargarBaseDatos(anfitriones, cantAnfitriones,
                    alojamientos, cantAlojamientos,
                    huespedes, cantHuespedes,
                    reservas, cantReservas, capReservas);

    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1. Iniciar sesión\n";
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
        case 0:
            cout << "Gracias por usar el sistema. ¡Hasta luego!\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }

    } while (opcion != 0);

    delete[] reservas;
    delete[] huespedes;
    delete[] alojamientos;
    delete[] anfitriones;

    return 0;
}
