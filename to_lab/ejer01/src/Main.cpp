#include "GlobalStore.h"
#include "RepositorioSingleton.h"
#include <iostream>

int main() {
    GlobalStore store;
    // Inicializamos el Singleton con la referencia al global store
    RepositorioSingleton::inicializar(&store);

    // Creamos algunas cuentas (Ahorro, Credito, Empresarial mancomunada)
    store.crearCuenta({"Juan Perez"}, "Ahorro", 5000.0);
    store.crearCuenta({"Maria Garcia"}, "Credito", 2000.0);
    store.crearCuenta({"Juan Perez", "Carlos Lopez"}, "Empresarial", 10000.0);

    store.listarTodasLasCuentas();

    // Obtenemos repositorios por cliente
    RepositorioCuentasUsuario* repoJuan = RepositorioSingleton::obtenerInstancia()->getRepositorioParaCliente("Juan Perez");
    RepositorioCuentasUsuario* repoMaria = RepositorioSingleton::obtenerInstancia()->getRepositorioParaCliente("Maria Garcia");
    RepositorioCuentasUsuario* repoCarlos = RepositorioSingleton::obtenerInstancia()->getRepositorioParaCliente("Carlos Lopez");

    // Mostrar estados globales
    repoJuan->estadoCuentaGlobal("2024-12-31");
    repoMaria->estadoCuentaGlobal("2024-12-31");
    repoCarlos->estadoCuentaGlobal("2024-12-31");

    // Prestamo personal a cuenta Empresarial de Juan (y Carlos)
    repoJuan->aplicarPrestamoPersonal("Empresarial", 5000.0, 0.9);

    // Transferir de Ahorro de Juan a Credito de Maria
    repoJuan->transferirHacia(repoMaria, "Ahorro", "Credito", 1000.0);

    // Compra con tarjeta de credito de Maria
    repoMaria->compraTarjetaCredito("Credito", 500.0);

    // Pagar deuda en cuenta Credito de Maria
    repoMaria->pagarDeuda("Credito", 300.0);
    repoMaria->pagarDeuda("Credito", 100.0);

    // Aplicar interes base a cuenta Ahorro de Juan
    repoJuan->aplicarInteresBase("Ahorro");

    // Mostrar estados de nuevo
    repoJuan->estadoCuentaGlobal("2025-01-10");
    repoMaria->estadoCuentaGlobal("2025-01-10");
    repoCarlos->estadoCuentaGlobal("2025-01-10");

    std::cout << "\n[Pruebas finalizadas]\n";
    return 0;
}

// ejecucion dsede to_lab/ejer01 : g++ -o CuentaMain src/*.cpp -I ./src
// ./CuentaMain

