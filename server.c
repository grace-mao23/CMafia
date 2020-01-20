#include "networking.c"

int main() {
    int inplay = 1;
    int game_start = 0;
    int sd, f, client, sub_num = 0;
    int taken[13]; //boolean of numbers saying which fd's are taken already;
    taken_setup(taken);
    int fd1[13][2]; // host reading from subserver
    int fd2[13][2]; // host writing to subserver
    char ** players;
    players = calloc(12, sizeof(char*));
    for (size_t i = 0; i < 12; i++) {
        players[i] = malloc(sizeof(char) * 1000);
        strcpy(players[i], "\0");
    }
    //parent -> use fd2[i][1] write to subserver write function
    //forked ->fd2[sub_num][0] (subserver reads from server read function
    //parent fd1[i][0] server read from subserver read function
    //forked fd1[sub_num][1]  subserver writes to server write function


    //sever writes to all subserver using fd2[i][1] "N*num*"   server.c
    //subserver in while loop checks to see servers, check if buffer: buffer[0]=='N' server.c
    //send buffer to clients use write (client, buffer,buffer)  mafia.c
    //if buffer[0]=='N' set num_players=parsed number at the end  mafia.c




    sd = server_setup();
    int checkin = 2; // when to prompt host // chenge to 6
    char *buffer_p = 0; // buffer pointer


    // someone has run server.c !!!!!!!!
    while (inplay != 0) {
        int client = server_connect(sd); // server receives client !!!!
        sub_num = lowest_available(taken);
        taken[sub_num] = 1;
        char buffer[BUFFER_SIZE] = "not yet";
        pipe(fd1[sub_num]); // pipes created woohoo !!!
        pipe(fd2[sub_num]);
        f = fork();
        if (f) { // parent ==> SERVER
            close(client); // closing client connection
            sleep(1); // give time for creation of pipe
            if (sub_num >= checkin) { // if it's time to start
                printf("%d players in the game. Ready to start? (yes/no) ", sub_num);
                buffer_p = fgets(buffer, sizeof(buffer), stdin);
                buffer_p[strlen(buffer_p) - 1] = '\0';
                printf("\n");
                if (strcmp(buffer_p, "no") == 1) { // if no, give them more time
                    checkin += 3;
                } else if (strcmp(buffer_p, "yes") == 0) { // if yes, let's start
                    strcpy(buffer, "Start\n");
                    printf("Game begins!\n");
                    game_start = 1; // game has started
                    int i = 0;
                    for (; i <= sub_num; i++) {
                        write(fd2[i][1], buffer, sizeof(buffer));
                        // host writes Start to subserver
                    }
                    for (i = 1; i <= sub_num; i++) {
                        char numP = sub_num + 96;
                        strcpy(buffer, "Num");
                        strncat(buffer, &numP, 1);
                        write(fd2[i][1], buffer, sizeof(buffer));
                        // host writes number of players to subserver
                    }
                    int j = 0;
                    printf("george is a bad advice giver\n");
                    sleep(1);
                    printf("george has wierd passwords\n");
                    printf("Host: subnum %d\n", sub_num);
                    for (i = 1; i <= sub_num; i++) { // i is the subserver number
                        read(fd1[i][0], buffer, sizeof(buffer));
                        // host reads username from EACH subserver
                        printf("Host: received %s from %d\n", buffer, i);
                        if (buffer[0] == 'U') {
                            while (strcmp(players[j], "\0") == 0) {
                                j++;
                            } // j is the username slot number
                            int k = 0;
                            for (k = 1; k < strlen(buffer); k++) { // k is the character number
                                players[j][k - 1] = buffer[k];
                            }
                            printf("Host: username is %s\n", players[j]);
                            // copying usernames into players
                        }
                    }
                    strcpy(buffer, "\0");
                    strcpy(buffer, "U");
                    for (size_t a = 0; a <= sub_num; a++) {
                        strcat(players[sub_num], ",");
                        strcat(buffer, players[sub_num]);
                    }
                    for (i = 1; i < 12; i++) {
                        write(fd2[i][1], buffer, sizeof(buffer));
                        printf("\n!!!!!!!!!!!!!!!\n");
                        // host writes list of players to EACH subserver
                    }
                }
            }
        } else { // child ==> SUBSERVER
            int quitted = -3;
            printf("Waiting for players to join...\n");
            while (quitted == -3 && read(fd2[sub_num][0], buffer, sizeof(buffer))) {
                // subserver reads Start from server
                if (strcmp(buffer, "Start\n") == 0) {
                    write(client, buffer, sizeof(buffer));
                    // subserver writes Start to client
                    strcpy(buffer, "Game Started");
                } else if ((strlen(buffer) == 4) && ('N' == buffer[0])) {
                    write(client, buffer, sizeof(buffer));
                    // subserver writes number of players to client
                    quitted = -2;
                }
            }
            while (quitted == -2 && read(client, buffer, sizeof(buffer))) {
              if (buffer[0] == 'U') {
                // subserver reads username from client
                printf("Subserver: Read username\n");
                write(fd1[sub_num][1], buffer, sizeof(buffer));
                // subserver writes username to host
                printf("Subserver: Wrote username to host\n");
                quitted = -1;
              }
            }
            while (quitted == -1 && read(fd2[sub_num][0], buffer, sizeof(buffer))) {
              // subserver reads the list of usernames
              if (buffer[0] == 'U') {
                printf("Subserver: Got list of usernames %s\n", buffer);
                write(client, buffer, sizeof(buffer));
                // subserver writes the list of usernames to client
                printf("Subserver: Wrote list of usernames to client\n");
                quitted = 0;
              }
            }
            //WILL WORK ON LATER
            quitted = 0; // may not need
            while (!quitted && read(client, buffer, sizeof(buffer))) {
                printf("%c\n", buffer[0]);
                if (buffer[0] == 'n') { //when the nurse tells server who is being saved
                    int night = buffer[1] - '0'; //format of buffer: [n or m][night number][person]
                } else if (buffer[0] == 'q') { //this quitting is when you want to quit before the game starts
                    quitted = 1;
                    write(fd1[sub_num][0], "q", sizeof("q")); //isn't this for reading
                } else if (buffer[0] == 'U') {
                    write(fd1[sub_num][1], buffer, sizeof(buffer));
                    printf("subserver writes to server\n");
                }
            }
            close(client);
            exit(0);
        }
    }
    close(sd);
    return 0;
}
