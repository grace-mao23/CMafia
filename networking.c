#include "project.h"

// BASIC NETWORKING FUNCTIONS
// Source: Mr. DW's brilliant code

// server side socket set up --> set to listening state
// RETURNS SERVER SOCKET DESCRIPTOR
int server_setup() {
    int sd, i;

    sd = socket(AF_INET, SOCK_STREAM, 0); // create
    // insert error check

    struct addrinfo *hints, *results;
    hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; // TPC
    hints->ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, PORT, hints, &results);
    i = bind(sd, results->ai_addr, results->ai_addrlen); // bind socket
    error_check(i, "binding server socket");

    i = listen(sd, 12); // set ot listening -> max length of queue is 12 players
    error_check(i, "listening state");

    free(hints);
    freeaddrinfo(results);

    //printf("Server Socket: %d\n", sd);
    return sd;
}

// have the server accept the client in the queue
// RETURNS CLIENT-SERVER CONNECTING SOCKET DESCRIPTOR
int server_connect(int sd) {
    int client_socket, f;
    socklen_t sock_size;
    struct sockaddr_storage client_address;

    sock_size = sizeof(client_address);
    client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
    // insert error check

    //printf("Client Socket: %d\n", client_socket);
    return client_socket;
}

// client side socket set up --> sends message to connect to server
// RETURNS CLIENT SOCKET DESCRIPTOR
int client_setup(char *server) {
    int sd, i;

    // creation same as server socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    // insert error check

    struct addrinfo *hints, *results;
    hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    getaddrinfo(server, PORT, hints, &results);

    i = connect(sd, results->ai_addr, results->ai_addrlen); // connection to server
    error_check(i, "connecting");

    free(hints);
    freeaddrinfo(results);

    //printf("Client Setup: %d\n", sd);
    return sd;
}

// tracking number of subservers
void taken_setup(int *taken) {
    taken[0] = 1; //false means not taken, true means taken
    int i = 1;
    for (; i < sizeof(taken); i++) {
        taken[i] = 0;
    }
}

int lowest_available(int *taken) { //finds the closest available fd
    int i = 1;
    for (; i < sizeof(taken); i++) {
        printf("%d\n", taken[i]);
        if (!taken[i]) {
            return i;
        }
    }
    return -1;
}
