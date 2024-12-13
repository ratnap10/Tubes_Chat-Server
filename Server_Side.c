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

int clients[MAX_CLIENTS];
int client_count = 0;

// Broadcast pesan ke semua klien
void broadcast_message(char *message, int sender_fd) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
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
            FD_SET(clients[i], &readfds);
            if (clients[i] > max_fd) max_fd = clients[i];
        }

        // Tunggu aktivitas
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select failed");
            continue;
        }

        // Jika ada klien baru yang terhubung
        if (FD_ISSET(server_fd, &readfds)) {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
                perror("Accept failed");
                continue;
            }
            clients[client_count++] = client_fd;
            printf("New client connected: %d\n", client_fd);
        }

        // Periksa setiap klien untuk pesan
        for (int i = 0; i < client_count; i++) {
            if (FD_ISSET(clients[i], &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(clients[i], buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    printf("Client disconnected: %d\n", clients[i]);
                    close(clients[i]);
                    clients[i] = clients[client_count - 1];
                    client_count--;
                } else {
                    printf("Client %d: %s", clients[i], buffer);
                    broadcast_message(buffer, clients[i]);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
