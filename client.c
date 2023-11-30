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
    char *message = "Hello, multicast!";
    
    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    
    // Set up multicast address and port
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    multicast_addr.sin_port = htons(PORT);

    // Set the local interface to use for sending multicast datagrams
    struct in_addr localInterface;
    localInterface.s_addr = inet_addr("192.168.1.5"); // Replace with your interface's IP
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
        perror("setting local interface error");
        exit(EXIT_FAILURE);
    }
    
    // Send message to multicast group
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    
    printf("Message sent to multicast group.\n");
    close(sock);
    return 0;
}
