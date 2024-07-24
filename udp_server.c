#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    
    // Crear socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    // Configurar dirección del servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    // Enlazar el socket con la dirección del servidor
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error en el enlace (bind)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    int len, n;
    len = sizeof(cliaddr);
    
    // Recibir mensaje
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Cliente: %s\n", buffer);
    
    // Enviar respuesta
    const char *response = "Mensaje recibido";
    sendto(sockfd, response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    printf("Respuesta enviada.\n");
    
    close(sockfd);
    return 0;
}
