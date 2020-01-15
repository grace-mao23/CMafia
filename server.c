#include "networking.c"

int main() {
    int sd = server_setup(); // sd is the server listening socket descriptor

    int taken[13]; // which fd slots are taken already
    taken_setup(taken); // the first one is taken by the server
    int sub_num, inplay, game_start = 0;
    int checkin = 2;
    char buffer[BUFFER_SIZE] = "";

    int fd1[13][2]; // host - reading        subserver - writing
    int fd2[13][2]; // subserver - reading   host -writing
    fd1[0][0] = 0;
    fd1[0][1] = 0;
    fd2[0][0] = 0;
    fd2[0][1] = 0;

    while (inplay != 0) {
      int client = server_connect(sd); // client is the client socket descriptor
      printf("%d: client connected\n", client);
      sub_num = lowest_available(taken);
      taken[sub_num] = 1;
  //    printf("The lowest available is %d\n", sub_num);
      int f = fork();
      if (f) { // parent
        close(client);
        sleep(1); // time for subserver to pipe
      //  printf("I'm the parent!\n");

      //  close(fd1[sub_num][1]);

        fd2[sub_num][0] = fd1[sub_num][1];
        fd2[sub_num][1] = fd1[sub_num][0];
        int p2 = pipe(fd2[sub_num]);
      /*  int flag3 = fcntl(fd2[sub_num][0], F_GETFL, 0);
        int flag4 = fcntl(fd2[sub_num][1], F_GETFL, 0);
        flag3 &= ~O_NONBLOCK;
        flag4 &= ~O_NONBLOCK;
        fcntl(fd2[sub_num][0], F_SETFL, flag3);
        fcntl(fd2[sub_num][1], F_SETFL, flag4); */
      //  printf("Parent pipe: %d\n", p2);

        printf("Parent speaking: sub_num is %d\n", sub_num);
        if (sub_num >= checkin) {
          printf("%d players in the game. Ready to start? (yes/no) ", sub_num);
          fgets(buffer, sizeof(buffer), stdin);
          printf("You answered: %s!\n", buffer);
          if (strcmp(buffer, "yes\n") == 0) {
            strcpy(buffer, "Start\n");
            printf("Game begins!\n");
            game_start = 1;
            int i = 1;
            for (; i < 13; i++) {
              //printf("wassup\n");
              write(fd2[i][1], buffer, sizeof(buffer));
              //printf("hi\n");
            }
          } else if (strcmp(buffer, "no\n") == 0) {
            checkin += 3;
          }

        }

      } else { // subserver
    //    printf("I'm the child!\n");

        fd1[sub_num][0] = getppid();
        fd1[sub_num][1] = getpid();
        int p1 = pipe(fd1[sub_num]);
      /*  int flag1 = fcntl(fd1[sub_num][0], F_GETFL, 0);
        int flag2 = fcntl(fd1[sub_num][1], F_GETFL, 0);
        flag1 &= ~O_NONBLOCK;
        flag2 &= ~O_NONBLOCK;
        fcntl(fd1[sub_num][0], F_SETFL, flag1);
        fcntl(fd1[sub_num][1], F_SETFL, flag2); */
    //    printf("Child pipe: %d\n", p1);
      //  close(fd2[sub_num][1]);
    //    int flag = fcntl(fd2[sub_num][0], F_GETFL, 0);
      //  printf("Non blocking? %d!\n", flag & O_NONBLOCK);

        while (!game_start) {
          int r = read(fd2[sub_num][0], buffer, sizeof(buffer));
        //  printf("Just read\n");
          while(r > 0) {
            printf("Reading %s!!\n", buffer);
          }
        }


      }

    }
/*    int inplay = 1;
    int game_start = 0;

    int sd, f, client, sub_num = 0;
    int taken[13]; //boolean of numbers saying which fd's are taken already;
    taken_setup(taken);
    int fd1[12][2]; // host reading from subserver
    int fd2[12][2]; // host writing to subserver
    sd = server_setup();
    int checkin = 2; // when to prompt host // chenge to 6
    char *buffer_p = 0; // buffer pointer

    while (inplay != 0) {
        int client = server_connect(sd);
        sub_num = lowest_available(taken);
        taken[sub_num] = 1;
        char buffer[BUFFER_SIZE] = "not yet";
        f = fork();
        if (f) { // parent
            close(client);
            sleep(1); // give time for creation of pipe

            fd2[sub_num][0] = fd1[sub_num][1][]
            fd2[sub_num][1] = getpid();
            pipe(fd2[sub_num]);

            if (sub_num >= checkin) {
                printf("%d players in the game. Ready to start? (yes/no) ", sub_num);
                buffer_p = fgets(buffer, sizeof(buffer), stdin);
                buffer_p[strlen(buffer_p) - 1] = '\0';
                printf("\n");
                if (strcmp(buffer_p, "no") == 1) {
                    checkin += 3;
                } else if (strcmp(buffer_p, "yes") == 0) {
                    strcpy(buffer, "Start\n");
                    printf("Game begins!\n");
                    game_start = 1;
                    int i = 0;
                    for (; i < 13; i++) {
                      printf("wassup\n");
                      write(fd2[i][1], buffer, sizeof(buffer));
                      printf("hi\n");
                    }
                }
            }
            // WHILE GAME IN PLAY, READ AND WRITE CONSISTENTLY
        } else {
            printf("Waiting for players to join...\n");
            fd1[sub_num][0] = getppid();
            fd1[sub_num][1] = getpid();
            pipe(fd1[sub_num]);
            //fd2[sub_num][0] = getpid();
            //fd2[sub_num][1] = getppid();
            //pipe(fd2[sub_num]);

            while (read(fd2[sub_num][1], buffer, sizeof(buffer))) {
                printf("[%s]\n",buffer );
                if (strcmp(buffer, "Start\n") == 0) {
                    write(client, buffer, sizeof(buffer));
                    strcpy(buffer, "Game Started");
                }
            }

            //WILL WORK ON LATER
            int quitted = 0;

            while (read(client, buffer, sizeof(buffer)) && !quitted) {
                if (buffer[0] == 'n') { //when the nurse tells server who is being saved
                    int night = buffer[1] - '0'; //format of buffer: [n or m][night number][person]
                }
                if (buffer[0] == 'q') { //this quitting is when you want to quit before the game starts
                    quitted = 1;
                    write(fd1[sub_num][0], "q", sizeof("q"));
                }
            }
            close(client);
            exit(0);
            //WILL WORK ON LATER
        }
    }
    close(sd);*/
    return 0;
}
