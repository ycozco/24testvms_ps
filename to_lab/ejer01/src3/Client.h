#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client {
private:
    std::string name;

public:
    Client(const std::string& name);
    std::string getName() const;
};

#endif
