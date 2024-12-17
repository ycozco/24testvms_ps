#include "RepositorioSingleton.h"
#include <iostream>

RepositorioSingleton* RepositorioSingleton::instancia = nullptr;

RepositorioSingleton::RepositorioSingleton(GlobalStore* store) : globalStore(store) {}

void RepositorioSingleton::inicializar(GlobalStore* store) {
    if (!instancia) {
        instancia = new RepositorioSingleton(store);
    }
}

RepositorioSingleton* RepositorioSingleton::obtenerInstancia() {
    return instancia;
}

RepositorioCuentasUsuario* RepositorioSingleton::getRepositorioParaCliente(const std::string& nombreCliente) {
    if (repositorios.find(nombreCliente) == repositorios.end()) {
        repositorios[nombreCliente] = new RepositorioCuentasUsuario(nombreCliente, globalStore);
    }
    return repositorios[nombreCliente];
}
