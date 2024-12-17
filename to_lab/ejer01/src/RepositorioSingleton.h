#ifndef REPOSITORIO_SINGLETON_H
#define REPOSITORIO_SINGLETON_H

#include "RepositorioCuentasUsuario.h"
#include <string>
#include <map>
#include <memory>

class RepositorioSingleton {
private:
    static RepositorioSingleton* instancia;
    GlobalStore* globalStore;
    std::map<std::string, RepositorioCuentasUsuario*> repositorios;

    RepositorioSingleton(GlobalStore* store);
public:
    static void inicializar(GlobalStore* store);
    static RepositorioSingleton* obtenerInstancia();

    RepositorioCuentasUsuario* getRepositorioParaCliente(const std::string& nombreCliente);
};

#endif
