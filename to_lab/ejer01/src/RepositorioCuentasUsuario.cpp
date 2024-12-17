#include "RepositorioCuentasUsuario.h"
#include <iostream>

RepositorioCuentasUsuario::RepositorioCuentasUsuario(const std::string& nombreCliente, GlobalStore* store)
    : nombreCliente(nombreCliente), globalStore(store) 
{
    // Configurar intereses base segun tipo
    interesBaseCuenta["Ahorro"] = 1.0;
    interesBaseCuenta["Joven"] = 1.5;
    interesBaseCuenta["Pluss"] = 2.0;
    interesBaseCuenta["Empresarial"] = 3.0;
    interesBaseCuenta["Credito"] = 0.0;

    recargarCuentas();
}

void RepositorioCuentasUsuario::recargarCuentas() {
    cuentasUsuario = globalStore->obtenerCuentasDeCliente(nombreCliente);
}

void RepositorioCuentasUsuario::estadoCuentaGlobal(const std::string& fechaCorte) {
    std::cout << "\n[Estado de Cuenta Global] Cliente: " << nombreCliente << " | Fecha: " << fechaCorte << "\n";
    if (cuentasUsuario.empty()) {
        std::cout << "[Info] El cliente no tiene cuentas.\n";
        return;
    }
    for (auto& c : cuentasUsuario) {
        std::cout << "Tipo: " << c->obtenerTipoCuenta() << " | Saldo: " << c->obtenerSaldo() << " | Deuda: " << c->obtenerDeuda() << "\n";
    }
}

void RepositorioCuentasUsuario::estadoCuentaPorTipo(const std::string& tipoCuenta, const std::string& fechaCorte) {
    std::cout << "\n[Estado de Cuenta por Tipo] Cliente: " << nombreCliente << " | Tipo: " << tipoCuenta 
              << " | Fecha: " << fechaCorte << "\n";
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (acc) {
        std::cout << "Saldo: " << acc->obtenerSaldo() << " | Deuda: " << acc->obtenerDeuda() << "\n";
    } else {
        std::cerr << "[Error] No se encontro la cuenta.\n";
    }
}

void RepositorioCuentasUsuario::aplicarPrestamoPersonal(const std::string& tipoCuenta, double monto, double calificacion) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para prestamo.\n";
        return;
    }

    double interes = 10.0;
    if (calificacion > 0.8) interes = 8.0;
    else if (calificacion > 0.5) interes = 9.0;

    double deudaTotal = monto + (monto * interes / 100.0);
    acc->aumentarDeuda(deudaTotal);
    std::cout << "[Prestamo Personal] Monto: " << monto << " | Interes: " << interes << "%\n";
}

bool RepositorioCuentasUsuario::transferirHacia(RepositorioCuentasUsuario* destino, const std::string& tipoOrigen,
                                                const std::string& tipoDestino, double monto) {
    Cuenta* accOrigen = buscarCuenta(tipoOrigen);
    if (!accOrigen) {
        std::cerr << "[Error] Cuenta origen no encontrada.\n";
        return false;
    }
    Cuenta* accDestino = destino->buscarCuenta(tipoDestino);
    if (!accDestino) {
        std::cerr << "[Error] Cuenta destino no encontrada.\n";
        return false;
    }

    // Suponemos una comision fija de 2%
    double comision = 2.0;
    double fee = monto * comision / 100.0;
    double total = monto + fee;

    if (accOrigen->obtenerSaldo() >= total) {
        accOrigen->retirar(total);
        accDestino->depositar(monto);
        std::cout << "[Transferencia] De: " << nombreCliente << "(" << tipoOrigen << ") A: " 
                  << destino->nombreCliente << "(" << tipoDestino << ") Monto: " << monto << " | Comision: " << fee << "\n";
        return true;
    } else {
        std::cerr << "[Error] Saldo insuficiente en cuenta origen.\n";
        return false;
    }
}

void RepositorioCuentasUsuario::compraTarjetaCredito(const std::string& tipoCuenta, double montoCompra) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para compra.\n";
        return;
    }
    // Interes 20% fijo ejemplo
    double interes = 20.0;
    double total = montoCompra + (montoCompra * interes / 100.0);
    acc->aumentarDeuda(total);
    std::cout << "[Compra Credito] Monto: " << montoCompra << " | Interes: " << interes << "%\n";
}

bool RepositorioCuentasUsuario::pagarDeuda(const std::string& tipoCuenta, double monto) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para pagar deuda.\n";
        return false;
    }
    return acc->reducirDeuda(monto);
}

bool RepositorioCuentasUsuario::retirar(const std::string& tipoCuenta, double monto) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para retirar.\n";
        return false;
    }
    return acc->retirar(monto);
}

void RepositorioCuentasUsuario::depositar(const std::string& tipoCuenta, double monto) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para depositar.\n";
        return;
    }
    acc->depositar(monto);
}

void RepositorioCuentasUsuario::aplicarInteresBase(const std::string& tipoCuenta) {
    Cuenta* acc = buscarCuenta(tipoCuenta);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para aplicar interes base.\n";
        return;
    }
    double tasa = 0.0;
    if (interesBaseCuenta.find(acc->obtenerTipoCuenta()) != interesBaseCuenta.end()) {
        tasa = interesBaseCuenta[acc->obtenerTipoCuenta()];
    }
    if (tasa > 0) {
        acc->aplicarInteres(tasa);
    } else {
        std::cout << "[Info] Tipo de cuenta " << acc->obtenerTipoCuenta() << " sin interes base aplicable.\n";
    }
}

Cuenta* RepositorioCuentasUsuario::buscarCuenta(const std::string& tipoCuenta) {
    for (auto& c : cuentasUsuario) {
        if (c->obtenerTipoCuenta() == tipoCuenta) {
            return c;
        }
    }
    return nullptr;
}
