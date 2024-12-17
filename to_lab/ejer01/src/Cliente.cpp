#include "Cliente.h"

Cliente::Cliente(const std::string& nombre): nombre(nombre) {}

std::string Cliente::obtenerNombre() const {
    return nombre;
}
