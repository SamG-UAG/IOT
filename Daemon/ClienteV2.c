#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_MSG_SIZE 1024

void *receive_messages(void *args) {
    int server_socket = *(int *)args;
    char message[MAX_MSG_SIZE];
    int read_size;
    while ((read_size = recv(server_socket, message, MAX_MSG_SIZE, 0)) > 0) {
        message[read_size] = '\0';
        printf("Mensaje recibido del servidor: %s", message);
    }
    if (read_size == 0) {
        printf("Servidor desconectado\n");
    } else if (read_size == -1) {
        printf("Error al recibir mensaje del servidor\n");
    }
    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_address;
    char message[MAX_MSG_SIZE];
    
    // Crear socket del cliente
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error al crear socket del cliente\n");
        return 1;
    }
    
    // Configurar dirección del servidor
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8081);
    
    // Conectar con el servidor
    if (connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Error al conectar con el servidor\n");
        return 1;
    }
    printf("Conectado al servidor\n");
    
    // Crear thread para recibir mensajes del servidor
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, receive_messages, (void *)&server_socket) < 0) {
        printf("Error al crear thread para recibir mensajes del servidor\n");
        return 1;
    }
    
    // Enviar mensajes al servidor
    while (1) {
        printf("Escribe un mensaje: ");
        fgets(message, MAX_MSG_SIZE, stdin);
        if (send(server_socket, message, strlen(message), 0) < 0) {
            printf("Error al enviar mensaje\n");
            return 1;
        }
    }
    
    return 0;
}