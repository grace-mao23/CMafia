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
        fgets(buffer, sizeof(buffer), stdin);
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
