#ifndef CUENTA_H
#define CUENTA_H

#include <string>
#include <vector>

class Cuenta {
private:
    std::vector<std::string> titulares; 
    std::string tipoCuenta;             
    double saldo;
    double deuda;

public:
    Cuenta(const std::vector<std::string>& titulares, const std::string& tipoCuenta, double saldoInicial);

    const std::vector<std::string>& obtenerTitulares() const;
    std::string obtenerTipoCuenta() const;
    double obtenerSaldo() const;
    double obtenerDeuda() const;

    void depositar(double monto);
    bool retirar(double monto);
    void aplicarInteres(double porcentaje);
    void aumentarDeuda(double monto);
    bool reducirDeuda(double monto);
    void establecerSaldo(double monto);
};

#endif
