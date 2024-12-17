#include "Account.h"
#include <iostream>

Account::Account(const std::vector<std::string>& holders, const std::string& accountType, double initialBalance)
    : holders(holders), accountType(accountType), balance(initialBalance), debt(0.0) {}

const std::vector<std::string>& Account::getHolders() const {
    return holders;
}

std::string Account::getAccountType() const {
    return accountType;
}

double Account::getBalance() const {
    return balance;
}

double Account::getDebt() const {
    return debt;
}

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "[Deposito] Tipo: " << accountType << " | Monto: " << amount << " | Nuevo saldo: " << balance << "\n";
    } else {
        std::cerr << "[Error] El monto a depositar debe ser mayor que 0.\n";
    }
}

bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        std::cout << "[Retiro] Tipo: " << accountType << " | Monto: " << amount << " | Nuevo saldo: " << balance << "\n";
        return true;
    }
    std::cerr << "[Error] Monto inválido o saldo insuficiente.\n";
    return false;
}

void Account::applyInterest(double percentage) {
    if (percentage > 0) {
        double interest = balance * (percentage / 100.0);
        balance += interest;
        std::cout << "[Interes] Tipo: " << accountType << " | %" << percentage << " aplicado | Interes: " 
                  << interest << " | Nuevo saldo: " << balance << "\n";
    }
}

void Account::increaseDebt(double amount) {
    if (amount > 0) {
        debt += amount;
        std::cout << "[Deuda] Tipo: " << accountType << " | Aumenta deuda en: " << amount << " | Deuda total: " << debt << "\n";
    }
}

bool Account::reduceDebt(double amount) {
    if (amount > 0 && debt > 0) {
        debt -= amount;
        if (debt < 0) debt = 0;
        std::cout << "[Deuda Pagada] Tipo: " << accountType << " | Pago: " << amount << " | Deuda restante: " << debt << "\n";
        return true;
    }
    std::cerr << "[Error] Monto inválido o no hay deuda.\n";
    return false;
}

void Account::setBalance(double amount) {
    if (amount >= 0) {
        balance = amount;
    }
}
