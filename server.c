#include "project.h"

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
            if (sub_num >= 2) { //CHANGE TO 6 LATER --> sub_num = number of subservers + 1
              int i = 0;
              for (; i < 13; i++) {
                write(fd2[i][1], buffer, BUFFER_SIZE);
              }
            }
        } else {
        //    printf("Current: %d\n", sub_num);
            printf("Waiting for players to join...\n");
            fd1[sub_num][0] = getppid();
            fd1[sub_num][1] = getpid();
            pipe(fd1[sub_num]);
            fd2[sub_num][0] = getpid();
            fd2[sub_num][1] = getppid();
            pipe(fd2[sub_num]);
      //      printf("Just added 1: %d\n", sub_num);
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
          //  printf("Just removed 1: %d\n", sub_num);
            close(client);
            exit(0);
            //WILL WORK ON LATER
        }
    }
    close(sd);
    return 0;
}
