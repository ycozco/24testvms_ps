#include "Client.h"

Client::Client(const std::string& name): name(name) {}

std::string Client::getName() const {
    return name;
}
