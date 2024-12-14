// Server-side code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 50

typedef struct {
    int fd;
    char name[MAX_NAME_LENGTH];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

// Broadcast pesan ke semua klien
void broadcast_message(char *message, int sender_fd) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].fd != sender_fd) {
            send(clients[i].fd, message, strlen(message), 0);
        }
    }
}

// Cek apakah nama sudah digunakan
int is_name_taken(char *name) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Membuat socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Konfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket ke alamat
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Mendengarkan koneksi
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", PORT);

    while (1) {
        fd_set readfds; // Set file descriptor untuk monitoring
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds); // Tambahkan server socket
        int max_fd = server_fd;

        for (int i = 0; i < client_count; i++) {
            FD_SET(clients[i].fd, &readfds);
            if (clients[i].fd > max_fd) max_fd = clients[i].fd;
        }

        // Tunggu aktivitas
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select failed");
            continue;
        }

        // Jika ada klien baru yang terhubung
        // If there is a new client connection
        if (FD_ISSET(server_fd, &readfds)) {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
                perror("Accept failed");
                continue;
            }

            // Request name from new client
            send(client_fd, "Enter your name: ", 17, 0);
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                buffer[strcspn(buffer, "\n")] = 0; // Remove newline

                while (is_name_taken(buffer)) {
                    send(client_fd, "Name already taken. Enter a different name: ", 41, 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
                    if (bytes_received > 0) {
                        buffer[bytes_received] = '\0';
                        buffer[strcspn(buffer, "\n")] = '\0';
                    }
                }

                clients[client_count].fd = client_fd;
                strncpy(clients[client_count].name, buffer, MAX_NAME_LENGTH - 1);
                clients[client_count].name[MAX_NAME_LENGTH - 1] = '\0';
                client_count++;

                printf("New client connected: %s (fd: %d)\n", buffer, client_fd);
                char welcome_message[BUFFER_SIZE];
                snprintf(welcome_message, BUFFER_SIZE, "Welcome %.49s!\n", buffer);
                send(client_fd, welcome_message, strlen(welcome_message), 0);
            } else {
                close(client_fd);
            }
        }

        // Periksa setiap klien untuk pesan
        for (int i = 0; i < client_count; i++) {
            if (FD_ISSET(clients[i].fd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(clients[i].fd, buffer, BUFFER_SIZE - 1, 0);
                if (bytes_received <= 0) {
                    printf("Client disconnected: %s (fd: %d)\n", clients[i].name, clients[i].fd);
                    close(clients[i].fd);
                    clients[i] = clients[client_count - 1];
                    client_count--;
                } else {
                    buffer[bytes_received] = '\0';
                    printf("%s: %s", clients[i].name, buffer);
                    char message_with_name[BUFFER_SIZE];
                    int avaible_space = BUFFER_SIZE - strlen(clients[i].name) - 3;
                    snprintf(message_with_name, BUFFER_SIZE, "%s: %.*s", clients[i].name, avaible_space, buffer);
                    broadcast_message(message_with_name, clients[i].fd);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
