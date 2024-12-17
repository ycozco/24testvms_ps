#include "AccountsManager.h"
#include <iostream>

std::unique_ptr<AccountsManager> AccountsManager::instance = nullptr;

AccountsManager::AccountsManager() {
    std::cout << "Inicializando AccountsManager (Singleton)...\n";
    // Definimos intereses bases para los distintos tipos de cuenta
    baseInterest["Ahorro"] = 1.0;       // Ejemplo: 1% interés base
    baseInterest["Joven"] = 1.5;
    baseInterest["Pluss"] = 2.0;
    baseInterest["Empresarial"] = 3.0;
    baseInterest["Credito"] = 0.0;      // La cuenta de crédito podría no tener interés sobre saldo positivo, sino deuda
}

AccountsManager* AccountsManager::getInstance() {
    if (instance == nullptr) {
        instance.reset(new AccountsManager());
    }
    return instance.get();
}

Client* AccountsManager::findClient(const std::string& clientName) {
    for (auto& c : clients) {
        if (c.getName() == clientName) return &c;
    }
    return nullptr;
}

void AccountsManager::registerClient(const std::string& name) {
    if (!findClient(name)) {
        clients.emplace_back(name);
        std::cout << "[Registro Cliente] " << name << " registrado.\n";
    } else {
        std::cerr << "[Error] Cliente " << name << " ya existe.\n";
    }
}

void AccountsManager::createAccount(const std::vector<std::string>& holders, const std::string& type, double initialBalance) {
    // Verificamos que todos los titulares estén registrados
    for (auto& h : holders) {
        if (!findClient(h)) {
            std::cerr << "[Error] Cliente titular " << h << " no está registrado.\n";
            return;
        }
    }
    accounts.emplace_back(holders, type, initialBalance);
    std::cout << "[Creación de Cuenta] Tipo: " << type << " | Titulares: ";
    for (auto& h : holders) std::cout << h << " ";
    std::cout << "| Saldo inicial: " << initialBalance << "\n";
}

Account* AccountsManager::findAccount(const std::string& clientName, const std::string& accountType) {
    for (auto& acc : accounts) {
        const auto& hs = acc.getHolders();
        for (auto& h : hs) {
            if (h == clientName && acc.getAccountType() == accountType) {
                return &acc;
            }
        }
    }
    return nullptr;
}

void AccountsManager::printAccountStatementGlobal(const std::string& clientName, const std::string& cutOffDate) {
    std::cout << "\n[Estado de Cuenta Global] Cliente: " << clientName << " | Fecha de corte: " << cutOffDate << "\n";
    bool found = false;
    for (auto& acc : accounts) {
        for (auto& h : acc.getHolders()) {
            if (h == clientName) {
                found = true;
                std::cout << "Tipo: " << acc.getAccountType() 
                          << " | Saldo: " << acc.getBalance() 
                          << " | Deuda: " << acc.getDebt() << "\n";
            }
        }
    }
    if (!found) {
        std::cout << "[Info] El cliente no tiene cuentas.\n";
    }
}

void AccountsManager::printAccountStatementByType(const std::string& clientName, const std::string& accountType, const std::string& cutOffDate) {
    std::cout << "\n[Estado de Cuenta por Tipo] Cliente: " << clientName 
              << " | Tipo: " << accountType << " | Fecha: " << cutOffDate << "\n";
    Account* acc = findAccount(clientName, accountType);
    if (acc) {
        std::cout << "Saldo: " << acc->getBalance() << " | Deuda: " << acc->getDebt() << "\n";
    } else {
        std::cerr << "[Error] No se encontró la cuenta del cliente.\n";
    }
}

void AccountsManager::applyPersonalLoan(const std::string& clientName, const std::string& accountType, double amount, double rating) {
    Account* acc = findAccount(clientName, accountType);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para aplicar préstamo.\n";
        return;
    }
    // Interés del préstamo según calificación (rating)
    // Si rating > 0.8 -> 8%, rating < 0.8 y > 0.5 -> 9%, else 10%
    double interestRate = 10.0;
    if (rating > 0.8) interestRate = 8.0;
    else if (rating > 0.5) interestRate = 9.0;

    double totalDebt = amount + (amount * interestRate / 100.0);
    acc->increaseDebt(totalDebt);
    std::cout << "[Préstamo Personal] Monto: " << amount 
              << " | Interés: " << interestRate << "% | Deuda resultante: " << acc->getDebt() << "\n";
}

bool AccountsManager::transfer(const std::string& fromClient, const std::string& fromType,
                               const std::string& toClient, const std::string& toType,
                               double amount) {
    Account* accFrom = findAccount(fromClient, fromType);
    Account* accTo = findAccount(toClient, toType);
    if (!accFrom || !accTo) {
        std::cerr << "[Error] No se pudo realizar la transferencia. Cuentas no encontradas.\n";
        return false;
    }

    // Comisión aleatoria entre 0.5% a 4% (aquí, por simplicidad, elegimos 2% fijo)
    double feePercentage = 2.0;
    double fee = amount * feePercentage / 100.0;
    double totalDeduct = amount + fee;

    if (accFrom->getBalance() >= totalDeduct) {
        accFrom->withdraw(totalDeduct);
        accTo->deposit(amount);
        std::cout << "[Transferencia] De: " << fromClient << " (" << fromType << ") A: " << toClient << " (" << toType << ") "
                  << "Monto: " << amount << " | Fee: " << fee << "\n";
        return true;
    } else {
        std::cerr << "[Error] Saldo insuficiente para cubrir la transferencia y la comisión.\n";
        return false;
    }
}

void AccountsManager::creditCardPurchase(const std::string& clientName, const std::string& accountType, double purchaseAmount) {
    Account* acc = findAccount(clientName, accountType);
    if (!acc) {
        std::cerr << "[Error] Cuenta no encontrada para compra con tarjeta de crédito.\n";
        return;
    }
    // Interés entre 15% y 25%, escogemos 20% fijo por simplicidad
    double interest = 20.0;
    double total = purchaseAmount + (purchaseAmount * interest / 100.0);
    acc->increaseDebt(total);
    std::cout << "[Compra Tarjeta Crédito] Monto: " << purchaseAmount 
              << " | Interés: " << interest << "% | Deuda resultante: " << acc->getDebt() << "\n";
}

bool AccountsManager::payDebt(const std::string& clientName, const std::string& accountType, double amount) {
    Account* acc = findAccount(clientName, accountType);
    if (!acc) {
        std::cerr << "[Error] No se encontró la cuenta para pagar deuda.\n";
        return false;
    }
    return acc->reduceDebt(amount);
}

void AccountsManager::applyBaseInterestToAccount(const std::string& clientName, const std::string& accountType) {
    Account* acc = findAccount(clientName, accountType);
    if (!acc) {
        std::cerr << "[Error] No se encontró la cuenta para aplicar interés.\n";
        return;
    }
    double rate = 0.0;
    if (baseInterest.find(acc->getAccountType()) != baseInterest.end()) {
        rate = baseInterest[acc->getAccountType()];
    }
    if (rate > 0) {
        acc->applyInterest(rate);
    } else {
        std::cout << "[Info] La cuenta tipo " << acc->getAccountType() << " no tiene interés base aplicable.\n";
    }
}

void AccountsManager::listAllAccounts() {
    std::cout << "\n[Lista de todas las cuentas]\n";
    for (auto& acc : accounts) {
        std::cout << "Tipo: " << acc.getAccountType() << " | Titulares: ";
        for (auto& h : acc.getHolders()) std::cout << h << " ";
        std::cout << "| Saldo: " << acc.getBalance() << " | Deuda: " << acc.getDebt() << "\n";
    }
}
