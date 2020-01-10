#include "project.h"

// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
  int sd_conn;
  char buffer[BUFFER_SIZE];

  sd_conn = client_setup(TEST_IP);

  if (sd_conn >= 0) {
    printf("Waiting for players to join...\n");

    while(read(sd_conn, buffer, sizeof(buffer))) {
      if (strcmp(buffer, "Start") == 0) {
        printf("2 Players in the game. Ready to start?");
      }
    }
    //printf("enter data: ");
    //fgets(buffer, sizeof(buffer), stdin);
    //*strchr(buffer, '\n') = 0;
    //write(sd_conn, buffer, sizeof(buffer));
    //read(sd_conn, buffer, sizeof(buffer));
    //printf("here: %s", buffer);
  }

  return 0;
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
