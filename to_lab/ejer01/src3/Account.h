#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Account {
private:
    std::vector<std::string> holders; // Titulares (mancomunada si más de uno)
    std::string accountType;          // Tipo de cuenta (Ahorro, Joven, Pluss, Empresarial, Credito)
    double balance;
    double debt; // Deuda asociada, si aplica (ej: tarjeta de crédito, préstamo)

public:
    Account(const std::vector<std::string>& holders, const std::string& accountType, double initialBalance);

    // Getters
    const std::vector<std::string>& getHolders() const;
    std::string getAccountType() const;
    double getBalance() const;
    double getDebt() const;

    // Operaciones sobre la cuenta
    void deposit(double amount);
    bool withdraw(double amount);
    void applyInterest(double percentage);
    void increaseDebt(double amount);
    bool reduceDebt(double amount); // Liquidar o adelantar deuda
    void setBalance(double amount);
};

#endif
