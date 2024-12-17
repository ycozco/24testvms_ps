#include "CuentaManager.h"

std::unique_ptr<CuentaManager> CuentaManager::instance = nullptr;

CuentaManager::CuentaManager() {
    std::cout << "Inicializando CuentaManager (Singleton)...\n";
}

CuentaManager* CuentaManager::getInstance() {
    if (instance == nullptr) {
        // Se crea una única instancia
        instance = std::unique_ptr<CuentaManager>(new CuentaManager());
    }
    return instance.get();
}

void CuentaManager::registrarCuenta(const std::string& tipoCuenta, double saldoInicial) {
    cuentas[tipoCuenta] = saldoInicial;
    std::cout << "Cuenta '" << tipoCuenta << "' registrada con saldo: " << saldoInicial << "\n";
}

void CuentaManager::mostrarCuentas() const {
    for (const auto& cuenta : cuentas) {
        std::cout << "Tipo: " << cuenta.first << " | Saldo: " << cuenta.second << "\n";
    }
}

double CuentaManager::calcularInteres(const std::string& tipoCuenta, double monto) const {
    double interes = 0.0;
    if (tipoCuenta == "Ahorro") interes = 0.05;
    else if (tipoCuenta == "Joven") interes = 0.08;
    else if (tipoCuenta == "Pluss") interes = 0.1;
    else if (tipoCuenta == "Empresarial") interes = 0.15;
    else if (tipoCuenta == "Credito") interes = 0.25;
    else {
        std::cerr << "[Advertencia] Tipo de cuenta desconocido: " << tipoCuenta 
                  << ". No se aplica interés.\n";
        return 0.0;
    }

    return monto * interes;
}
