#ifndef CUENTA_CLIENTE_H
#define CUENTA_CLIENTE_H

#include "CuentaManager.h"
#include <string>

class CuentaCliente {
private:
    std::string nombre;
    CuentaManager* manager;

public:
    CuentaCliente(const std::string& nombre);
    void registrarCuentaCliente(const std::string& tipoCuenta, double saldoInicial);
    void mostrarCuentasCliente() const;
    void calcularInteresCliente(const std::string& tipoCuenta, double monto) const;
};

#endif
