#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <MQTT_FRAME.h>

#define PORT 5050

int main(int argc, char const *argv[]) {
    int sock = 0, valread, i;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char *hello = "Hello World";
    char Client_ID[20];
    sConnect connect_return;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // FRAME CONNECT

    connect_return = vfnConnect()


    // Send hello message to server
    send(sock, hello, strlen(hello), 0);

    while (1) {
        // Get user input
        printf("Enter message: ");
        fgets(buffer, 1024, stdin);

        // Send user input to server
        send(sock, buffer, strlen(buffer), 0);

        // Receive response from server
        valread = read(sock, buffer, 1024);

        // Show server response
        printf("Server response: %s\n", buffer);

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close socket
    close(sock);

    return 0;
}