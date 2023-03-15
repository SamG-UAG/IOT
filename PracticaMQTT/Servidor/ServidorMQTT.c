#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>

#define PORT 5050

int new_socket1, new_socket2;

void *handle_client1(void *arg) {
    char buffer[1024] = {0};
    char *hello = "Hello from server to client 1";

    while (1) {
        // Reading from client 1
        if (read(new_socket1, buffer, 1024) <= 0) {
            perror("read");
            break;
        }
        printf("Client 1 says: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        // Writing to client 1
        send(new_socket1, hello, strlen(hello), 0);
    }
    close(new_socket1);
    printf("Client 1 disconnected\n");
    pthread_exit(NULL);
}

void *handle_client2(void *arg) {
    char buffer[1024] = {0};
    char *hello = "Hello from server to client 2";

    while (1) {
        // Reading from client 2
        if (read(new_socket2, buffer, 1024) <= 0) {
            perror("read");
            break;
        }
        printf("Client 2 says: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        // Writing to client 2
        send(new_socket2, hello, strlen(hello), 0);
    }
    close(new_socket2);
    printf("Client 2 disconnected\n");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    int server_fd, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t client1_thread, client2_thread;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Client 1 connected\n");
    if ((new_socket2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
    exit(EXIT_FAILURE);
    }
    printf("Client 2 connected\n");

    // Creating threads to handle client 1 and client 2
    if (pthread_create(&client1_thread, NULL, handle_client1, NULL)) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&client2_thread, NULL, handle_client2, NULL)) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    if (pthread_join(client1_thread, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(client2_thread, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    // Close server socket
    close(server_fd);

    return 0;
}
