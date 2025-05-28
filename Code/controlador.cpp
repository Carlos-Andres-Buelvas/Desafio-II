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
#include "controlador.h"

int siguienteNumeroReserva = 1;


Controlador::Controlador() {
    //anfitriones = nullptr;
    //alojamientos = nullptr;
    //huespedes = nullptr;
    //reservas = new Reserva[300];

    //cantAnfitriones = cantAlojamientos = 0;
    //cantHuespedes =
    //cantReservas = 300;
    //capReservas = 300;

    siguienteNumeroReserva = obtenerSiguienteNumeroReserva();

    cargarBaseDatos(anfitriones, cantAnfitriones,
                    alojamientos, cantAlojamientos,
                    huespedes, cantHuespedes,
                    reservas, cantReservas, capReservas);
}

void Controlador::iniciar(){
    menuPrincipal();
}

void Controlador::menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== MENÚ PRINCIPAL =====\n";
        cout << "1. Iniciar sesión\n";
        cout << "0. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            iniciarSesion();
            break;
        case 0:
            cout << "Gracias por usar el sistema. ¡Hasta luego!\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }

    } while (opcion != 0);
}

void Controlador::iniciarSesion(){
    string doc, claveIngresada;
    cout << "\n--- Iniciar sesión ---\n";
    cout << "Ingrese su número de documento: ";
    cin >> doc;
    cout << "Ingrese su clave: ";
    cin >> claveIngresada;

    for (int i = 0; i < cantHuespedes; ++i) {
        if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == claveIngresada) {
            mostrarMenuHuesped(&huespedes[i]);
            return;
        }
    }

    for (int i = 0; i < cantAnfitriones; ++i) {
        if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == claveIngresada) {
            mostrarMenuAnfitrion(&anfitriones[i]);
            return;
        }
    }

    cout << "Documento o clave incorrectos.\n";
}

void Controlador::mostrarMenuHuesped(Huesped* h){
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
            buscarYReservarAlojamiento(h);
            break;
        case 2:
            reservarAlojamiento(h);
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
            Reserva* r = &reservas[idx];              // Apunta antes de modificar nada
            string codigo = r->getCodigo();           // Guarda el código
            anularReservacion(codigo, h, nullptr);    // Anula por código, ya no usas índices viejos
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

void Controlador::mostrarMenuAnfitrion(Anfitrion* a){
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
            cout << "\nAntigüedad: " << a->getAntiguedad() << " meses\n";
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
            depurarReservas(fechaCorte);

            // Actualizar archivo reservas.txt después de depurar
            sobrescribirArchivoReservas();

            cout << "Histórico actualizado. Solo se conservaron reservas desde la fecha indicada.\n";
            break;
        }
        case 6:
            this->mostrarHistorico();
            break;
        case 0:
            cout << "Sesión finalizada.\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);
}

void Controlador::asegurarCapacidadReservas() {
    if (cantReservas >= capReservas) {
        capReservas *= 2;
        Reserva* nuevo = new Reserva[capReservas];
        for (int i = 0; i < cantReservas; ++i)
            nuevo[i] = reservas[i];
        delete[] reservas;
        reservas = nuevo;

        // Reparar punteros en los huéspedes
        for (int i = 0; i < cantHuespedes; ++i)
            huespedes[i].repararPunterosReservas(reservas, cantReservas);
    }
}

void Controlador::buscarYReservarAlojamiento(Huesped* h)
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
    if (metodo != "1" && metodo != "2"){
        cout << "Opción inválida. Reserva cancelada.\n";
        return;
    }
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
    float monto = noches * aloj->getPrecioNoche();

    // Verificar conflicto antes de crear la reserva
    if (h->hayConflicto(entrada, noches)) {
        cout << "Conflicto de fechas. No se realizó la reserva.\n";
        return;
    }

    // Redimensionar arreglo si hace falta
    void asegurarCapacidadReservas();

    // Crear y guardar la reserva
    reservas[cantReservas] = Reserva(codigo, aloj, h, entrada, noches, metodo, fechaPago, monto, nota);
    Reserva* ptrReserva = &reservas[cantReservas];
    ++cantReservas;

    h->agregarReserva(ptrReserva);
    aloj->reservarDias(entrada, noches);
    this->guardarReservaIndividual(*ptrReserva);

    // Mostrar comprobante usando método encapsulado
    ptrReserva->mostrarComprobante();
}

void Controlador::reservarAlojamiento(Huesped* h)
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
    if (metodo != "1" && metodo != "2"){
        cout << "Opción inválida. Reserva cancelada.\n";
        return;
    }
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
    std::ostringstream oss;
    oss << "RSV" << std::setw(3) << std::setfill('0') << siguienteNumeroReserva++;
    string codigo = oss.str();
    Fecha fechaPago = entrada;
    float monto = dur * aloj->getPrecioNoche();

    // 1) Creamos la reserva en HEAP para construirla:
    Reserva* nueva = new Reserva(codigo, aloj, h, entrada, dur, metodo, fechaPago, monto, nota);

    // 2) Comprobamos conflictos en el huesped con NUEVA:
    if (h->hayConflicto(entrada, dur)) {
        cout << "El huésped ya tiene una reserva en esas fechas.\n";
        delete nueva;
        return;
    }

    // 3) Aseguramos capacidad y copiamos en tu array principal:
    void asegurarCapacidadReservas();
    reservas[cantReservas] = *nueva;   // copia "por valor" dentro de tu buffer

    // 4) INSERTAMOS sólo una vez EN EL HUESPED:
    h->agregarReserva(&reservas[cantReservas]);

    // 5) Marcamos los días en el alojamiento:
    aloj->reservarDias(entrada, dur);

    // 6) Avanzamos el contador:
    cantReservas++;

    // 7) Guardamos en disco:
    this->guardarReservaIndividual(reservas[cantReservas - 1]);

    // 8) Limpieza del temporal:
    delete nueva;

    // Mostrar comprobante usando método encapsulado
    reservas[cantReservas - 1].mostrarComprobante();
}

int Controlador::obtenerSiguienteNumeroReserva(){
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

void Controlador::anularReservacion(const string& codigo, Huesped* h, Anfitrion* a) {
    for (int i = 0; i < cantReservas; ++i) {
        if (reservas[i].getCodigo() == codigo) {
            bool autorizado = false;
            Huesped* h_afectado = nullptr;

            if (h && reservas[i].getHuesped()->getDocumento() == h->getDocumento()) {
                autorizado = true;
                h_afectado = h;
            } else if (a && reservas[i].getAlojamiento()->getAnfitrion()->getDocumento() == a->getDocumento()) {
                autorizado = true;
                h_afectado = reservas[i].getHuesped();
            }

            if (!autorizado) {
                cout << "No tiene permiso para anular esta reservación.\n";
                return;
            }

            // Eliminar del huésped
            if (h_afectado)
                //h_afectado->eliminarReservaPorCodigo(codigo);

                // Compactar arreglo global de reservas
                for (int j = i; j < cantReservas - 1; ++j)
                    reservas[j] = reservas[j + 1];
            cantReservas--;

            // Reparar punteros internos del huésped (opcional si ya se eliminó la reserva)
            if (h_afectado && cantReservas > 0)
                h_afectado->repararPunterosReservas(reservas, cantReservas);

            sobrescribirArchivoReservas();
            cout << "Reservación anulada con éxito.\n";
            return;
        }
    }

    cout << "Reservación no encontrada.\n";
}

void Controlador::mostrarHistorico() {
    std::ifstream archivo("historico_reservas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo histórico.\n";
        return;
    }

    std::string linea;
    int contador = 1;
    std::cout << "\n--- Histórico de reservaciones ---\n";
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        std::cout << contador++ << ". " << linea << "\n";
    }

    archivo.close();
    if (contador == 1) std::cout << "No hay reservas históricas registradas.\n";
}

void Controlador::guardarReservaIndividual(const Reserva& r) {
    std::ofstream archivo("reservas.txt", std::ios::app);
    if (!archivo.is_open()) {
        std::cout << "Error al guardar la reserva en archivo.\n";
        return;
    }

    archivo << r.getCodigo() << ";"
            << r.getAlojamiento()->getCodigo() << ";"
            << r.getHuesped()->getDocumento() << ";"
            << r.getFechaEntrada().getDia() << "/"
            << r.getFechaEntrada().getMes() << "/"
            << r.getFechaEntrada().getAnio() << ";"
            << r.getDuracion() << ";"
            << r.getMetodoPago() << ";"
            << r.getFechaPago().getDia() << "/"
            << r.getFechaPago().getMes() << "/"
            << r.getFechaPago().getAnio() << ";"
            << r.getMonto() << ";"
            << r.getAnotacion() << "\n";

    archivo.close();
}

void Controlador::sobrescribirArchivoReservas() {
    std::ofstream archivo("reservas.txt");
    if (!archivo.is_open()) {
        std::cout << "Error al escribir en reservas.txt\n";
        return;
    }

    archivo << "codigoReserva;codAlojamiento;docHuesped;fechaEntrada;duracion;metodoPago;fechaPago;monto;anotacion\n";

    for (int i = 0; i < cantReservas; ++i) {
        const Reserva& r = reservas[i];
        archivo << r.getCodigo() << ";"
                << r.getAlojamiento()->getCodigo() << ";"
                << r.getHuesped()->getDocumento() << ";"
                << r.getFechaEntrada().getDia() << "/"
                << r.getFechaEntrada().getMes() << "/"
                << r.getFechaEntrada().getAnio() << ";"
                << r.getDuracion() << ";"
                << r.getMetodoPago() << ";"
                << r.getFechaPago().getDia() << "/"
                << r.getFechaPago().getMes() << "/"
                << r.getFechaPago().getAnio() << ";"
                << r.getMonto() << ";"
                << r.getAnotacion() << "\n";
    }

    archivo.close();
}

void Controlador::depurarReservas(const Fecha& fechaCorte) {
    std::ofstream archivo("historico_reservas.txt", std::ios::app);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo histórico.\n";
        return;
    }

    int nuevaCantidad = 0;
    for (int i = 0; i < cantReservas; ++i) {
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
                    << reservas[i].getAnotacion() << "\n";
        }
    }

    cantReservas = nuevaCantidad;
    archivo.close();
}


Controlador::~Controlador() {
    delete[] reservas;
    delete[] huespedes;
    delete[] alojamientos;
    delete[] anfitriones;
}
