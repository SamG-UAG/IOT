#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_MSG_SIZE 1024
int max_clients = 10;
int current_clients = 0;
int client_sockets[10];
int num_clients = 0;


void *receive_messages(void *args) 
{
    int client_socket = *(int *)args;
    int read_size;
    char message[MAX_MSG_SIZE];
    printf("Recieve thread");
    if ((read_size = recv(client_socket, message, MAX_MSG_SIZE, 0)) > 0) {
        message[read_size] = '\0';
        printf("Mensaje recibido del cliente: %s", message);
    }
    if (read_size == 0) {
        printf("Cliente desconectado\n");
        close(client_socket);
        pthread_exit(NULL);
    } else if (read_size == -1) {
        printf("Error al recibir mensaje del cliente\n");
        pthread_exit(NULL);
    }
}

void *send_messages(void *args) {
    int client_socket = *(int *)args;
    char message[MAX_MSG_SIZE];
    int read_size;
    printf("Send message");
    while ((read_size = recv(client_socket, message, MAX_MSG_SIZE, 0)) > 0) {
        message[read_size] = '\0';
        printf("Mensaje recibido del cliente: %s", message);
        for (int i = 0; i < num_clients; i++) {
            if (client_sockets[i] != client_socket) {
                if (send(client_sockets[i], message, strlen(message), 0) < 0) {
                    printf("Error al enviar mensaje a cliente %d\n", i);
                    break;
                }
            }
        }
    }
    if (read_size == 0) {
        printf("Cliente desconectado\n");
        close(client_socket);
        pthread_exit(NULL);
    } else if (read_size == -1) {
        printf("Error al recibir mensaje del cliente\n");
        pthread_exit(NULL);
    }
}

int main() {
    int server_socket, client_address_size;
    uint8_t client_socket;
    struct sockaddr_in server_address, client_address;
    pthread_t receive_thread_id, send_thread_id;

    // Crear socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error al crear socket del servidor\n");
        return 1;
    }

    // Configurar dirección del servidor
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8081);

    // Asociar socket del servidor a la dirección y puerto
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Error al asociar socket del servidor a la dirección y puerto\n");
        return 1;
    }

    // Escuchar conexiones entrantes
    listen(server_socket, 1);

    printf("Servidor esperando conexiones entrantes...\n");
    pthread_t receive_threads[10], send_threads[10];
    //for (int i = 0; i < num_clients; i++) 
    {

        // Crear thread para recibir mensajes del cliente
        if (pthread_create(&receive_threads, NULL, receive_messages, &client_sockets) < 0) 
        {
            printf("Error al crear thread para recibir mensajes del cliente ");
            return 1;
        }
        
        // Crear thread para enviar mensajes al cliente
        if (pthread_create(&send_threads, NULL, send_messages, &client_sockets) < 0) {
            printf("Error al crear thread para enviar mensajes al cliente ");
            return 1; 
        }

    }
    
    // Aceptar conexión entrante
    while (1) 
    {
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_size);
        if (client_socket < 0) 
        {
            printf("Error al aceptar conexión entrante\n");
            break;
        }
        printf("Cliente conectado\n");
        client_sockets[current_clients++] = client_socket;
        if (current_clients == max_clients) 
        {
            printf("Número máximo de clientes alcanzado (%d)\n", max_clients);
            break;
        }
    }
    printf("Test");

    for (int i = 0; i < num_clients; i++) {
        pthread_join(receive_threads[i], NULL);
        pthread_join(send_threads[i], NULL);
    }

    

    return 0;
}