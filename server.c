#include "networking.c"

int main() {
    int inplay = 1;
    int game_start = 0;

    int sd, f, client, sub_num = 0;
    int taken[13]; //boolean of numbers saying which fd's are taken already;
    taken_setup(taken);
    int fd1[12][2]; // host reading from subserver
    int fd2[12][2]; // host writing to subserver
    sd = server_setup();
    int checkin = 2; // when to prompt host // chenge to 6
    char *buffer_p = 0; // buffer pointer

    while (inplay != 0){
        int client = server_connect(sd);
        sub_num = lowest_available(taken);
        taken[sub_num] = 1;
        char buffer[BUFFER_SIZE] = "not yet";
        f = fork();
        if (f) { // parent
            close(client);
            sleep(1); // give time for creation of pipe

            if (sub_num >= checkin) {
                printf("%d players in the game. Ready to start? (yes/no) ", sub_num);
                buffer_p = fgets(buffer, sizeof(buffer), stdin);
                buffer_p[strlen(buffer_p)-1]='\0';
                printf("\n");
                if (strcmp(buffer_p, "no") == 1) {
                    checkin += 3;
                } else if (strcmp(buffer_p, "yes") == 0) {
                    strcpy(buffer, "Start");
                    printf("Game begins!\n");
                    game_start = 1;
                    int i = 0;
                    for (; i < 13; i++) {
                        write(fd2[i][1], buffer, sizeof(buffer));
                    }
                }
            }
            // WHILE GAME IN PLAY, READ AND WRITE CONSISTENTLY
        } else {
            printf("Waiting for players to join...\n");
            fd1[sub_num][0] = getppid();
            fd1[sub_num][1] = getpid();
            pipe(fd1[sub_num]);
            fd2[sub_num][0] = getpid();
            fd2[sub_num][1] = getppid();
            pipe(fd2[sub_num]);

            while (read(fd2[0][0], buffer, sizeof(buffer))) {
                if (strcmp(buffer, "Start") == 0) {
                    write(client, buffer, sizeof(buffer));
                    strcpy(buffer, "Game Started");
                }
            }

            //WILL WORK ON LATER
            int quitted = 0;

            while (read(client, buffer, sizeof(buffer)) && !quitted) { //this quitting is when you want to quit before the game starts
                if (strcmp(buffer, "quit") == 0) {
                    quitted = 1;
                }
            }
            close(client);
            exit(0);
            //WILL WORK ON LATER
        }
    }
    close(sd);
    return 0;
}
