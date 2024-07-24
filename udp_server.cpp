#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error en el enlace (bind)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(cliaddr);
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, reinterpret_cast<socklen_t *>(&len));
    buffer[n] = '\0';
    std::cout << "Cliente: " << buffer << std::endl;

    const char *response = "Mensaje recibido";
    sendto(sockfd, response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    std::cout << "Respuesta enviada." << std::endl;

    close(sockfd);
    return 0;
}
