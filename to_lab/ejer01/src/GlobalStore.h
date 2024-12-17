#ifndef GLOBAL_STORE_H
#define GLOBAL_STORE_H

#include "Cuenta.h"
#include <vector>
#include <string>

class GlobalStore {
private:
    std::vector<Cuenta> cuentas;

public:
    void crearCuenta(const std::vector<std::string>& titulares, const std::string& tipoCuenta, double saldoInicial);
    std::vector<Cuenta*> obtenerCuentasDeCliente(const std::string& nombreCliente);
    void listarTodasLasCuentas();
};

#endif
