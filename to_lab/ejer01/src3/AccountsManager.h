#ifndef ACCOUNTS_MANAGER_H
#define ACCOUNTS_MANAGER_H

#include "Account.h"
#include "Client.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class AccountsManager {
private:
    static std::unique_ptr<AccountsManager> instance;
    std::vector<Client> clients;
    std::vector<Account> accounts;

    // Tablas de interés/porcentaje según tipo de operación
    // Por simplicidad se ponen rangos fijos, en la práctica podrían ser configurables.
    // Ejemplos basados en el enunciado:
    // Prestamos personales: 8% a 10%
    // Transferencias nacionales: 0.5% a 4%
    // Compras tarjeta crédito: 15% a 25%
    // Cuentas: Ahorro, Joven, Pluss, Empresarial, Credito (con intereses o tarifas base)
    std::map<std::string, double> baseInterest;  // Interes base según tipo de cuenta
    // Para simplificar, usamos un map por operación también:
    double personalLoanMin = 8.0;
    double personalLoanMax = 10.0;
    double transferMin = 0.5;
    double transferMax = 4.0;
    double creditPurchaseMin = 15.0;
    double creditPurchaseMax = 25.0;

    AccountsManager();
    Client* findClient(const std::string& clientName);

public:
    static AccountsManager* getInstance();

    // Gestión de clientes
    void registerClient(const std::string& name);

    // Gestión de cuentas
    // Una cuenta puede tener uno o varios titulares (mancomunada)
    void createAccount(const std::vector<std::string>& holders, const std::string& type, double initialBalance);

    // Estados de cuenta
    void printAccountStatementGlobal(const std::string& clientName, const std::string& cutOffDate);
    void printAccountStatementByType(const std::string& clientName, const std::string& accountType, const std::string& cutOffDate);

    // Operaciones:
    // Aplicar préstamo personal: se incrementa la deuda de la cuenta seleccionada (según calificación)
    void applyPersonalLoan(const std::string& clientName, const std::string& accountType, double amount, double rating);

    // Transferencia entre cuentas:
    // Aplica una comisión entre 0.5% a 4%. Aquí se asume el tipo "nacional".
    bool transfer(const std::string& fromClient, const std::string& fromType,
                  const std::string& toClient, const std::string& toType,
                  double amount);

    // Compras con tarjeta de crédito (aumenta la deuda entre 15% a 25% extra)
    void creditCardPurchase(const std::string& clientName, const std::string& accountType, double purchaseAmount);

    // Liquidar o adelantar deuda
    bool payDebt(const std::string& clientName, const std::string& accountType, double amount);

    // Aplicar interés base a una cuenta (ejemplo: para cuentas de ahorro)
    void applyBaseInterestToAccount(const std::string& clientName, const std::string& accountType);

    // Funciones auxiliares
    Account* findAccount(const std::string& clientName, const std::string& accountType);
    void listAllAccounts();
};

#endif
