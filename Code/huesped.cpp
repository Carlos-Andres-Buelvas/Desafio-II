// Constructor por defecto: inicializa todos los atributos y reserva espacio para 3000 reservas
Huesped::Huesped() : documento(""), nombre(""), clave(""),
    antiguedad(0), puntuacion(0.0),
    capacidadReservas(3000), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];
}

// Constructor con parámetros: permite crear un huésped con datos específicos
Huesped::Huesped(string doc, string nom, int antig, float punt)
    : documento(doc), nombre(nom), clave(""),
    antiguedad(antig), puntuacion(punt),
    capacidadReservas(3000), cantidadReservas(0) {
    reservas = new Reserva*[capacidadReservas];
}

// Constructor copia: realiza una copia superficial del arreglo de reservas (solo punteros)
Huesped::Huesped(const Huesped& otro)
    : documento(otro.documento), nombre(otro.nombre),
    antiguedad(otro.antiguedad), puntuacion(otro.puntuacion),
    clave(otro.clave), capacidadReservas(otro.capacidadReservas),
    cantidadReservas(otro.cantidadReservas) {
    reservas = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        reservas[i] = otro.reservas[i];
}

// Operador de asignación: libera memoria y copia los punteros del otro huésped
Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservas;

        documento = otro.documento;
        nombre = otro.nombre;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        clave = otro.clave;
        capacidadReservas = otro.capacidadReservas;
        cantidadReservas = otro.cantidadReservas;

        reservas = new Reserva*[capacidadReservas];
        for (int i = 0; i < cantidadReservas; ++i)
            reservas[i] = otro.reservas[i];
    }
    return *this;
}

// ------- MÉTODOS CLAVE --------

// Intenta agregar una nueva reserva si no hay conflicto de fechas
bool Huesped::agregarReserva(Reserva* nueva) {
    if (hayConflicto(nueva->getFechaEntrada(), nueva->getDuracion()))
        return false;

    if (cantidadReservas >= capacidadReservas)
        redimensionarReservas();

    reservas[cantidadReservas++] = nueva;
    return true;
}

// Verifica si una nueva reserva se solapa con alguna existente (por año y días del año)
bool Huesped::hayConflicto(const Fecha& entrada, int duracion) const {
    int y1 = entrada.getAnio();
    int start1 = entrada.aDiaDelAnio();
    int end1 = start1 + duracion;

    for (int i = 0; i < cantidadReservas; ++i) {
        Reserva* r = reservas[i];
        Fecha fe = r->getFechaEntrada();

        if (fe.getAnio() != y1) continue;

        int start2 = fe.aDiaDelAnio();
        int end2 = start2 + r->getDuracion();

        if (!(end1 <= start2 || end2 <= start1))  // Hay traslape
            return true;
    }
    return false;
}

// Elimina una reserva por puntero (ajusta el arreglo dinámico)
void Huesped::eliminarReserva(Reserva* r) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] == r) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

// Elimina una reserva buscando por código
void Huesped::eliminarReservaPorCodigo(const string& codigo) {
    for (int i = 0; i < cantidadReservas; ++i) {
        if (reservas[i] && reservas[i]->getCodigo() == codigo) {
            for (int j = i; j < cantidadReservas - 1; ++j)
                reservas[j] = reservas[j + 1];
            reservas[--cantidadReservas] = nullptr;
            return;
        }
    }
}

// Muestra la información del huésped y sus reservas activas
void Huesped::mostrar() const {
    cout << "Huésped: " << documento
         << " | Antigüedad: " << antiguedad
         << " meses | Puntuación: " << puntuacion << endl;

    if (cantidadReservas == 0) {
        cout << "No tiene reservaciones activas.\n";
        return;
    }

    for (int i = 0; i < cantidadReservas; ++i) {
        cout << i + 1 << ". ";
        reservas[i]->mostrarComprobante();  // Imprime el comprobante de reserva
    }
}

// Duplica la capacidad del arreglo de reservas cuando está lleno
void Huesped::redimensionarReservas() {
    capacidadReservas *= 2;
    Reserva** temp = new Reserva*[capacidadReservas];
    for (int i = 0; i < cantidadReservas; ++i)
        temp[i] = reservas[i];
    delete[] reservas;
    reservas = temp;
}

// Reasocia las reservas después de cargar un nuevo arreglo desde archivo
void Huesped::repararPunterosReservas(Reserva* nuevoArreglo, int cantReservas) {
    for (int i = 0; i < cantReservas; ++i) {
        Reserva* actual = &nuevoArreglo[i];
        for (int j = 0; j < cantidadReservas; ++j) {
            if (reservas[j]->getCodigo() == actual->getCodigo()) // Actualiza puntero si coincide el código
                reservas[j] = actual;
        }
    }
}

// Carga huéspedes desde archivo de texto delimitado por ;
void Huesped::cargarDesdeArchivo(const std::string& archivo,
                                 Huesped*& arreglo,
                                 int& cantidad,
                                 int& capacidad)
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
    arreglo = new Huesped[capacidad];  // Crear arreglo dinámico

    while (std::getline(in, linea)) {
        std::stringstream ss(linea);
        std::string doc, nombre, clave, antigStr, puntStr;

        std::getline(ss, doc, ';');
        std::getline(ss, nombre, ';');
        std::getline(ss, antigStr, ';');
        std::getline(ss, puntStr, ';');
        std::getline(ss, clave, ';');

        int antig = std::stoi(antigStr);
        float punt = std::stof(puntStr);

        Huesped nuevo(doc, nombre, antig, punt);
        nuevo.setClave(clave);

        if (cantidad >= capacidad) {
            capacidad *= 2;
            Huesped* nuevoArr = new Huesped[capacidad];
            for (int i = 0; i < cantidad; ++i)
                nuevoArr[i] = arreglo[i];
            delete[] arreglo;
            arreglo = nuevoArr;
        }

        arreglo[cantidad++] = nuevo;
    }

    in.close();
    std::cout << "[OK] Huéspedes cargados: " << cantidad << "\n";
}

// Destructor: libera la memoria dinámica del arreglo de reservas
Huesped::~Huesped() {
    delete[] reservas;
}
