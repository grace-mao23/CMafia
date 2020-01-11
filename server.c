#include "project.h"


// NOT WORKING I REPEAT BE CAUTIOUS
int main() {
    int inplay = 1;

    int sd, f, client, sub_num = 0;
    int taken[13]; //boolean of numbers saying which fd's are taken already;
    taken_setup(taken);
    int fd1[12][2]; // host reading from subserver
    int fd2[12][2]; // host writing to subserver
    sd = server_setup();

    while (inplay != 0){
        int client = server_connect(sd);
        sub_num = lowest_available(taken);
        taken[sub_num] = 1;
        char buffer[BUFFER_SIZE] = "Start";
        f = fork();
        if (f) { // parent
            close(client);
            sleep(1); // give time for creation of pipe
            printf("SERVER: %d\n", sub_num);
            if (sub_num >= 2) { //CHANGE TO 5 LATER
              int i = 0;
              for (; i < 13; i++) {
                write(fd2[i][1], buffer, BUFFER_SIZE);
              }
            }
        } else {
            printf("Current: %d\n", sub_num);
            printf("Waiting for players to join...\n");
            fd1[sub_num][0] = getppid();
            fd1[sub_num][1] = getpid();
            pipe(fd1[sub_num]);
            fd2[sub_num][0] = getpid();
            fd2[sub_num][1] = getppid();
            pipe(fd2[sub_num]);
            printf("Just added 1: %d\n", sub_num);
            //WILL WORK ON LATER
            int quitted = 0;
            while (read(fd2[0][0], buffer, sizeof(buffer))) {
              if (strcmp(buffer, "Start") == 0) {
                write(client, buffer, sizeof(buffer));
              }
            }
            while (read(client, buffer, sizeof(buffer)) && !quitted) { //this quitting is when you want to quit before the game starts
                if (strcmp(buffer, "quit") == 0) {
                    quitted = 1;
                }
            }
            printf("Just removed 1: %d\n", sub_num);
            close(client);
            exit(0);
            //WILL WORK ON LATER
        }
    }
    close(sd);
    return 0;
}



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
