#include "project.h"


// NOT WORKING I REPEAT BE CAUTIOUS
int main() {
  #undef INPLAY
  #define INPLAY 1
  // when game ends, redefine inplay as 0

  int sd, f, client;
  sd = server_setup();

  while (INPLAY != 0){
    int client = server_connect(sd);
    f = fork();
    if (f == 0) {
      printf("Subserverrrr\n");
      close(client);
      exit(0);
    } else {
      close(client);
    }
    #undef INPLAY
    #define INPLAY 0
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
