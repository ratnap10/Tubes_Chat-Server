// Client-side code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void receive_messages(int socket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            printf("%s", buffer);
        }
    }
}

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Membuat socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Konfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Menghubungkan ke server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Receive prompt for name
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        printf("%s", buffer);
    }

    // Send name to server
    memset(buffer, 0, BUFFER_SIZE);
    fgets(buffer, BUFFER_SIZE, stdin);
    send(client_fd, buffer, strlen(buffer), 0);

    // Membuat thread untuk menerima pesan
    if (fork() == 0) {
        receive_messages(client_fd);
        exit(0);
    }

    // Mengirim pesan ke server
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    return 0;
}
