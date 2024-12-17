#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

class Cliente {
private:
    std::string nombre;

public:
    Cliente(const std::string& nombre);
    std::string obtenerNombre() const;
};

#endif
