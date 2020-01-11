#include "project.h"

// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
  int sd_conn, game_start = 0;
  char buffer[BUFFER_SIZE];

  sd_conn = client_setup(TEST_IP);

  if (sd_conn >= 0) {
    printf("Waiting for players to join...\n");

    while(read(sd_conn, buffer, sizeof(buffer))) {
      if (strcmp(buffer, "Start") == 0) {
        game_start = 1;
        strcpy(buffer, "Game Started");
        printf("\n\n\nLET'S BEGIN!\n\n\n");
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
