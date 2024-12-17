#include "CuentaCliente.h"

CuentaCliente::CuentaCliente(const std::string& nombre) : nombre(nombre) {
    manager = CuentaManager::getInstance();
}

void CuentaCliente::registrarCuentaCliente(const std::string& tipoCuenta, double saldoInicial) {
    manager->registrarCuenta(tipoCuenta, saldoInicial);
}

void CuentaCliente::mostrarCuentasCliente() const {
    std::cout << "Cuentas del cliente " << nombre << ":\n";
    manager->mostrarCuentas();
}

void CuentaCliente::calcularInteresCliente(const std::string& tipoCuenta, double monto) const {
    double interes = manager->calcularInteres(tipoCuenta, monto);
    std::cout << "Interés calculado para '" << tipoCuenta << "': " << interes << "\n";
}
