#include "CuentaCliente.h"

int main() {
    // Crear dos clientes que supuestamente usarán la misma instancia del Singleton
    CuentaCliente cliente1("Juan Perez");
    CuentaCliente cliente2("Maria Garcia");

    // Registrar cuentas a través del cliente1
    cliente1.registrarCuentaCliente("Ahorro", 5000.0);
    cliente1.registrarCuentaCliente("Credito", 10000.0);

    // Mostrar cuentas desde cliente1
    std::cout << "\n--- Cuentas vistas por cliente1 ---\n";
    cliente1.mostrarCuentasCliente();

    // Mostrar cuentas desde cliente2 (deberían ser las mismas, pues es el mismo Singleton)
    std::cout << "\n--- Cuentas vistas por cliente2 (deberían coincidir con las de cliente1) ---\n";
    cliente2.mostrarCuentasCliente();

    // Calcular intereses
    std::cout << "\n--- Cálculo de Intereses ---\n";
    cliente1.calcularInteresCliente("Ahorro", 5000.0);
    cliente2.calcularInteresCliente("Credito", 10000.0);

    // Prueba con un tipo de cuenta desconocido
    std::cout << "\n--- Prueba con tipo de cuenta desconocido ---\n";
    cliente1.calcularInteresCliente("Desconocido", 2000.0);

    // "Pruebas" simples:
    // Verificar manualmente la salida, o implementar condiciones lógicas aquí
    // Por ejemplo:
    double interesAhorro = CuentaManager::getInstance()->calcularInteres("Ahorro", 1000.0);
    if (interesAhorro == 50.0) {
        std::cout << "\n[TEST PASSED] Interés en cuenta Ahorro para 1000.0 es 50.0\n";
    } else {
        std::cout << "\n[TEST FAILED] Interés en cuenta Ahorro debería ser 50.0, obtenido: " << interesAhorro << "\n";
    }

    return 0;
}
