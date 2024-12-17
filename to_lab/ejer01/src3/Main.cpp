#include "AccountsManager.h"
#include <iostream>

int main() {
    AccountsManager* mgr = AccountsManager::getInstance();

    // Registrar clientes
    mgr->registerClient("Juan Perez");
    mgr->registerClient("Maria Garcia");
    mgr->registerClient("Carlos Lopez");

    // Crear cuentas (individuales y mancomunadas)
    mgr->createAccount({"Juan Perez"}, "Ahorro", 5000.0);
    mgr->createAccount({"Maria Garcia"}, "Credito", 2000.0);
    mgr->createAccount({"Juan Perez", "Carlos Lopez"}, "Empresarial", 10000.0);

    mgr->listAllAccounts();

    // Estado global de Juan Perez
    mgr->printAccountStatementGlobal("Juan Perez", "2024-12-31");

    // Estado por tipo (Credito) de Maria Garcia
    mgr->printAccountStatementByType("Maria Garcia", "Credito", "2024-12-31");

    // Aplicar préstamo personal a la cuenta empresarial de Juan Perez (y Carlos Lopez)
    mgr->applyPersonalLoan("Juan Perez", "Empresarial", 5000.0, 0.9); // rating alto -> 8%
    // Ahora la deuda en la cuenta empresarial aumenta.

    // Hacer una transferencia desde la cuenta de Ahorro de Juan Perez a la Credito de Maria Garcia
    mgr->transfer("Juan Perez", "Ahorro", "Maria Garcia", "Credito", 1000.0);

    // Compra con tarjeta de crédito (Maria Garcia)
    mgr->creditCardPurchase("Maria Garcia", "Credito", 500.0);

    // Liquidar parcialmente la deuda de la cuenta crédito de Maria Garcia
    mgr->payDebt("Maria Garcia", "Credito", 300.0);

    // Adelantar pago de deuda (otro pago)
    mgr->payDebt("Maria Garcia", "Credito", 100.0);

    // Aplicar interés base a la cuenta de ahorro de Juan Perez
    mgr->applyBaseInterestToAccount("Juan Perez", "Ahorro");

    // Volver a mostrar estados de cuenta
    mgr->printAccountStatementGlobal("Juan Perez", "2025-01-10");
    mgr->printAccountStatementByType("Maria Garcia", "Credito", "2025-01-10");
    mgr->printAccountStatementGlobal("Carlos Lopez", "2025-01-10");

    // Verificar que el Singleton sea el mismo objeto
    AccountsManager* mgr2 = AccountsManager::getInstance();
    if (mgr == mgr2) {
        std::cout << "\n[TEST] Singleton OK: mgr y mgr2 son la misma instancia.\n";
    } else {
        std::cout << "[TEST] Singleton FAIL: mgr y mgr2 son distintas instancias.\n";
    }

    std::cout << "\n[Pruebas finalizadas]\n";

    return 0;
}
