#include "controlador.h"
#include <iostream>

//Variables globales para eficiencia
int totalIteraciones = 0;
int totalMemoria = 0;

int main() {
    system("chcp 65001"); // Cambia la consola de Windows a UTF-8
    Controlador app;
    app.iniciar();

    std::cout << "\n=== ESTADÍSTICAS DE EJECUCIÓN ===\n";
    std::cout << "Total de iteraciones en bucles: " << totalIteraciones << "\n";
    std::cout << "Memoria dinámica consumida: " << totalMemoria << " bytes\n";

    return 0;
}
