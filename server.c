#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MULTICAST_ADDR "224.3.29.71" // Multicast group address
#define PORT 10000                   // Port number

int main() {
    int sock;
    struct sockaddr_in multicast_addr;
    struct ip_mreq mreq;
    char buffer[1024];
    
    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    
    // Set up multicast address and port
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    multicast_addr.sin_port = htons(PORT);
    
    // Allow multiple applications to receive datagrams on the same port
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("setting SO_REUSEADDR error");
        exit(EXIT_FAILURE);
    }
    
    // Bind to the port
    if (bind(sock, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Join the multicast group
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        printf("Waiting to receive message...\n");
        memset(buffer, 0, sizeof(buffer));
        if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        printf("Received message: %s\n", buffer);
    }
    
    close(sock);
    return 0;
}
