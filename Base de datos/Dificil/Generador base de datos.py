import random
import string
from datetime import datetime, timedelta

# ---------- Utilidades ----------
def generar_clave(longitud=8):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=longitud))

# ---------- Datos base ----------
nombres_genericos = [f"Usuario{i:04d}" for i in range(1, 2001)]
departamentos = ["Antioquia"]
municipios = ["Abejorral", "Abriaqui", "Alejandria", "Amalfi", "Andes", "Angelopolis", "Angostura", "Anori", 
              "Anza", "Apartado", "Arboletes", "Argelia", "Armenia", "Barbosa", "Belen de Bajira", "Bello", 
              "Betania", "Betulia", "Bolivar", "Briceno", "Buritica", "Caceres", "Caicedo", "Caldas", 
              "Campamento", "Canasgordas", "Caracoli", "Caramanta", "Carepa", "Carolina del Principe", 
              "Caucasia", "Chigorodo", "Cisneros", "Cocorna", "Concepcion", "Concordia", "Copacabana", 
              "Dabeiba", "Don Matias", "Ebejico", "El Bagre", "El Carmen de Viboral", "El Penol", "El Retiro", 
              "El Santuario", "Entrerrios", "Envigado", "Fredonia", "Frontino", "Giraldo", "Girardota", 
              "Gomez Plata", "Granada", "Guadalupe", "Guarne", "Guatape", "Heliconia", "Hispania", "Itagui", 
              "Ituango", "Jardin", "Jerico", "La Ceja", "La Estrella", "La Pintada", "La Union", "Liborina", 
              "Maceo", "Marinilla", "Medellin", "Montebello", "Murindo", "Mutata", "Narino", "Nechi", "Olaya", 
              "Peque", "Pueblorrico", "Puerto Berrio", "Puerto Nare", "Puerto Triunfo", "Remedios", "Rionegro", 
              "Sabanalarga", "Sabaneta", "Salgar", "San Andres de Cuerquia", "San Carlos", "San Francisco", 
              "San Jeronimo", "San Jose de la Montana", "San Juan de Uraba", "San Luis", "San Pedro", 
              "San Pedro de Uraba", "San Rafael", "San Roque", "San Vicente", "Santa Barbara", 
              "Santa Fe de Antioquia", "Santa Rosa de Osos", "Santo Domingo", "Segovia", "Sonson", 
              "Sopetran", "Tamesis", "Taraza", "Tarso", "Titiribi", "Toledo", "Turbo", "Uramita", "Urrao", 
              "Valdivia", "Valparaiso", "Vegachi", "Venecia", "Vigia del Fuerte", "Yali", "Yarumal", "Yolombo", 
              "Yondo", "Zaragoza"]
tipos = ["casa", "apartamento"]
metodos_pago = ["PSE", "TCredito"]
direcciones = [f"Calle {i} #{random.randint(1, 99)}-{random.randint(1, 99)}" for i in range(1, 1201)]
nombres_alojamientos = [f"Alojamiento{i:04d}" for i in range(1, 1201)]
anotaciones = [
    "Sin comentarios", "Llegaré tarde", "¿Tiene aire acondicionado?",
    "Por favor dejar instrucciones para ingreso.", "¿Se permite fumar?"
]
amenidades = lambda: ",".join(str(random.randint(0, 1)) for _ in range(6))

# ---------- Anfitriones ----------
anfitriones = ["documento;nombre;antiguedad;puntuacion;clave"]
docs_anfitriones = []
for i in range(1, 701):
    doc = f"10000{i:04d}"
    docs_anfitriones.append(doc)
    nombre = nombres_genericos[i - 1]
    anfitriones.append(f"{doc};{nombre};{random.randint(6, 60)};{round(random.uniform(2.5, 5.0), 1)};{generar_clave()}")

# ---------- Huespedes ----------
huespedes = ["documento;nombre;antiguedad;puntuacion;clave"]
docs_huespedes = []
for i in range(1, 501):
    doc = f"20000{i:04d}"
    docs_huespedes.append(doc)
    nombre = nombres_genericos[i + 699]
    huespedes.append(f"{doc};{nombre};{random.randint(3, 48)};{round(random.uniform(1.5, 5.0), 1)};{generar_clave()}")

# ---------- Alojamientos ----------
alojamientos = ["codigo;nombre;docAnfitrion;departamento;municipio;tipo;direccion;precio;amenidades"]
codigos_alojamiento = []
contador_alojamientos = 1
for docAnfitrion in docs_anfitriones:
    cantidad = random.randint(1, 3)
    for _ in range(cantidad):
        if contador_alojamientos > 1200:
            break
        codigo = f"ALOJ{contador_alojamientos:04d}"
        codigos_alojamiento.append(codigo)
        alojamientos.append(f"{codigo};{nombres_alojamientos[contador_alojamientos - 1]};{docAnfitrion};"
                            f"{random.choice(departamentos)};{random.choice(municipios)};"
                            f"{random.choice(tipos)};{direcciones[contador_alojamientos - 1]};"
                            f"{random.randint(80000, 250000)};{amenidades()}")
        contador_alojamientos += 1
    if contador_alojamientos > 1200:
        break

# ---------- Reservas ----------
reservas = ["codigoReserva;codAlojamiento;docHuesped;fechaEntrada;duracion;metodoPago;fechaPago;monto;anotacion"]
base_date = datetime.strptime("01/06/2025", "%d/%m/%Y")
reserva_id = 1
ocupacion_huesped = {}

while len(reservas) < 1201:
    codAloj = random.choice(codigos_alojamiento)
    docHuesped = random.choice(docs_huespedes)
    entrada = base_date + timedelta(days=random.randint(0, 60))
    duracion = random.randint(1, 7)
    salida = entrada + timedelta(days=duracion)

    ocupaciones = ocupacion_huesped.get(docHuesped, [])
    conflicto = any(not (salida <= e or entrada >= s) for e, s in ocupaciones)
    if conflicto:
        continue

    ocupaciones.append((entrada, salida))
    ocupacion_huesped[docHuesped] = ocupaciones

    codigo = f"RSV{reserva_id:0{max(3,len(str(reserva_id)))}}"
    reserva_id += 1
    fechaEntrada = entrada.strftime("%d/%m/%Y")
    metodo = random.choice(metodos_pago)
    fechaPago = (entrada - timedelta(days=random.randint(1, 6))).strftime("%d/%m/%Y")
    monto = duracion * random.randint(80000, 250000)
    nota = random.choice(anotaciones)

    reservas.append(f"{codigo};{codAloj};{docHuesped};{fechaEntrada};{duracion};{metodo};{fechaPago};{monto};{nota}")

# ---------- Guardar archivos ----------
file_paths = {
    "anfitriones.txt": anfitriones,
    "huespedes.txt": huespedes,
    "alojamientos.txt": alojamientos,
    "reservas.txt": reservas,
    "historico_reservas.txt": reservas[:1]  # solo encabezado
}

for filename, lines in file_paths.items():
    with open(filename, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
        if filename in ["reservas.txt", "historico_reservas.txt"]:
            f.write("\n")

print("Archivos generados: 700 anfitriones, 500 huéspedes, 1200 alojamientos, 1200 reservas.")
