#include "GlobalStore.h"
#include <iostream>

void GlobalStore::crearCuenta(const std::vector<std::string>& titulares, const std::string& tipoCuenta, double saldoInicial) {
    cuentas.emplace_back(titulares, tipoCuenta, saldoInicial);
    std::cout << "[GlobalStore] Cuenta creada: Tipo: " << tipoCuenta << " | Saldo inicial: " << saldoInicial << " | Titulares: ";
    for (auto& t : titulares) std::cout << t << " ";
    std::cout << "\n";
}

std::vector<Cuenta*> GlobalStore::obtenerCuentasDeCliente(const std::string& nombreCliente) {
    std::vector<Cuenta*> resultado;
    for (auto& c : cuentas) {
        for (auto& h : c.obtenerTitulares()) {
            if (h == nombreCliente) {
                resultado.push_back(&c);
                break;
            }
        }
    }
    return resultado;
}

void GlobalStore::listarTodasLasCuentas() {
    std::cout << "\n[Lista global de cuentas]\n";
    for (auto& c : cuentas) {
        std::cout << "Tipo: " << c.obtenerTipoCuenta() << " | Saldo: " << c.obtenerSaldo() 
                  << " | Deuda: " << c.obtenerDeuda() << " | Titulares: ";
        for (auto& h : c.obtenerTitulares()) std::cout << h << " ";
        std::cout << "\n";
    }
}
