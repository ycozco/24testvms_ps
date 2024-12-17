#ifndef CUENTA_MANAGER_H
#define CUENTA_MANAGER_H

#include <iostream>
#include <map>
#include <string>
#include <memory> // Para std::unique_ptr

class CuentaManager {
private:
    static std::unique_ptr<CuentaManager> instance;
    std::map<std::string, double> cuentas; // Simula el repositorio de cuentas

    // Constructor privado
    CuentaManager();  

public:
    // El Singleton retorna un puntero crudo, 
    // pero internamente mantiene un unique_ptr
    static CuentaManager* getInstance();  

    // Operaciones sobre cuentas
    void registrarCuenta(const std::string& tipoCuenta, double saldoInicial);
    void mostrarCuentas() const;
    double calcularInteres(const std::string& tipoCuenta, double monto) const;
};

#endif
