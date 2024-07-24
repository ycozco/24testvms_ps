#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *message = "Hola desde el cliente";
    
    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nError al crear el socket\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convertir direcciones IPv4 e IPv6 de texto a binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida / Dirección no soportada\n");
        return -1;
    }
    
    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nFallo en la conexión\n");
        return -1;
    }
    
    // Enviar mensaje al servidor
    send(sock, message, strlen(message), 0);
    printf("Mensaje enviado\n");
    
    // Leer respuesta del servidor
    read(sock, buffer, 1024);
    printf("Servidor: %s\n", buffer);
    
    close(sock);
    return 0;
}
