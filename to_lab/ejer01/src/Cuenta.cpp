#include "Cuenta.h"
#include <iostream>

Cuenta::Cuenta(const std::vector<std::string>& titulares, const std::string& tipoCuenta, double saldoInicial)
    : titulares(titulares), tipoCuenta(tipoCuenta), saldo(saldoInicial), deuda(0.0) {}

const std::vector<std::string>& Cuenta::obtenerTitulares() const {
    return titulares;
}

std::string Cuenta::obtenerTipoCuenta() const {
    return tipoCuenta;
}

double Cuenta::obtenerSaldo() const {
    return saldo;
}

double Cuenta::obtenerDeuda() const {
    return deuda;
}

void Cuenta::depositar(double monto) {
    if (monto > 0) {
        saldo += monto;
        std::cout << "[Deposito] Tipo: " << tipoCuenta << " | Monto: " << monto << " | Nuevo saldo: " << saldo << "\n";
    } else {
        std::cerr << "[Error] Monto a depositar invalido.\n";
    }
}

bool Cuenta::retirar(double monto) {
    if (monto > 0 && monto <= saldo) {
        saldo -= monto;
        std::cout << "[Retiro] Tipo: " << tipoCuenta << " | Monto: " << monto << " | Nuevo saldo: " << saldo << "\n";
        return true;
    }
    std::cerr << "[Error] Monto invalido o saldo insuficiente.\n";
    return false;
}

void Cuenta::aplicarInteres(double porcentaje) {
    if (porcentaje > 0) {
        double interes = saldo * (porcentaje / 100.0);
        saldo += interes;
        std::cout << "[Interes] Tipo: " << tipoCuenta << " | %" << porcentaje << " aplicado | Interes: "
                  << interes << " | Nuevo saldo: " << saldo << "\n";
    }
}

void Cuenta::aumentarDeuda(double monto) {
    if (monto > 0) {
        deuda += monto;
        std::cout << "[Deuda] Tipo: " << tipoCuenta << " | Aumenta deuda: " << monto << " | Deuda total: " << deuda << "\n";
    }
}

bool Cuenta::reducirDeuda(double monto) {
    if (monto > 0 && deuda > 0) {
        deuda -= monto;
        if (deuda < 0) deuda = 0;
        std::cout << "[Pago Deuda] Tipo: " << tipoCuenta << " | Pago: " << monto << " | Deuda restante: " << deuda << "\n";
        return true;
    }
    std::cerr << "[Error] Monto invalido o no hay deuda.\n";
    return false;
}

void Cuenta::establecerSaldo(double monto) {
    if (monto >= 0) {
        saldo = monto;
    }
}
