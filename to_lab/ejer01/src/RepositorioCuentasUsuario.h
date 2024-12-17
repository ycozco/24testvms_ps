#ifndef REPOSITORIO_CUENTAS_USUARIO_H
#define REPOSITORIO_CUENTAS_USUARIO_H

#include "GlobalStore.h"
#include <string>
#include <vector>
#include <map>

class RepositorioCuentasUsuario {
private:
    std::string nombreCliente;
    GlobalStore* globalStore;
    std::vector<Cuenta*> cuentasUsuario;

    // Intereses base por tipo de cuenta (ejemplo)
    std::map<std::string,double> interesBaseCuenta;

public:
    RepositorioCuentasUsuario(const std::string& nombreCliente, GlobalStore* store);

    void recargarCuentas();

    // Estados de cuenta
    void estadoCuentaGlobal(const std::string& fechaCorte);
    void estadoCuentaPorTipo(const std::string& tipoCuenta, const std::string& fechaCorte);

    // Operaciones
    // Prestamo personal: 8% a 10%, sujeto a calificacion
    void aplicarPrestamoPersonal(const std::string& tipoCuenta, double monto, double calificacion);

    // Transferencias nacionales: 0.5% a 4% (usamos 2% ejemplo)
    bool transferirHacia(RepositorioCuentasUsuario* destino, const std::string& tipoOrigen, 
                         const std::string& tipoDestino, double monto);

    // Compras con tarjeta de credito: 15% a 25% (usamos 20% ejemplo)
    void compraTarjetaCredito(const std::string& tipoCuenta, double montoCompra);

    // Liquidar y/o adelantar deudas
    bool pagarDeuda(const std::string& tipoCuenta, double monto);

    // Depositar, retirar
    bool retirar(const std::string& tipoCuenta, double monto);
    void depositar(const std::string& tipoCuenta, double monto);

    // Aplicar interes base segun tipo de cuenta
    void aplicarInteresBase(const std::string& tipoCuenta);

    // Buscar cuenta
    Cuenta* buscarCuenta(const std::string& tipoCuenta);
};

#endif
