#include "networking.c"

int maf=0;
int nur=0;
int det=0;
int mafiaNum(int users) {
    int mafia = 0;
    if (users > 8) {
        mafia = 3;
    } else if (users > 6) {
        mafia = 2;
    } else {
        mafia = 1;
    }
    maf = mafia;
    printf("Total Mafia Members: %d\n", mafia);
    return mafia;
}

//number of nurse based off of users (number of players)
int nurseNum(int users) {
    int nurse = 0;
    if (users > 7) {
        nurse = 2;
    } else if (users > 2) {
        nurse = 1;
    } else {
      nurse = 0;
    }
    nur = nurse;
    printf("Total Number of Nurse(s): %d\n", nurse);
    return nurse;
}

//number of dectecive based off of users (number of players)
int detectiveNum(int users) {
    int detective;
    if (users > 10) {
        detective = 2;
    } else {
        detective = 1;
    }
    det = detective;
    printf("Total Detective(s): %d\n", detective);
    return detective;
}
//need to initialize turns to 0
char *genRoles(int total) {
    int players = total;
    char *string = malloc(sizeof(char) * 1000);
    char *assign = malloc(total * sizeof(char));
    for (size_t i = 0; i < maf; i++) {
        assign[i] = '1';
    }
    for (size_t i = 0; i < det; i++) {
        assign[maf + i] = '2';
    }
    for (size_t i = 0; i < nur; i++) {
        assign[maf + det + i] = '3';
    }
    for (size_t i = maf + nur + det; i < total; i++) {
        assign[i] = '0';
    }
    for (size_t i = 0; i < players; i++) {
        int index = rand() % total;
        strncat(string, &assign[index], 1);
        strcat(string, ",");
        total--;
        char temp = assign[total];
        assign[total] = assign[index];
        assign[index] = temp;
    }
    return string;
}
int main() {
    int inplay = 1;
    int game_start = 0;
    int sd, f, client, sub_num = 0;
    int taken[13]; //boolean of numbers saying which fd's are taken already;
    taken_setup(taken);
    int fd1[13][2]; // host reading from subserver
    int fd2[13][2]; // host writing to subserver
    char *victim;
    char *saved;
    char **players;
    srand(time(NULL));

    victim = malloc(sizeof(char) * 1000);
    saved = malloc(sizeof(char) * 1000);
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
    int checkin = 3; // when to prompt host // chenge to 6
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
                    mafiaNum(sub_num);
                    nurseNum(sub_num);
                    detectiveNum(sub_num);
                    strcpy(buffer, "\0");
                    strcpy(buffer, "R");
                    strcat(buffer, genRoles(sub_num));
                    for (i = 1; i <= sub_num; i++) {
                        write(fd2[i][1], buffer, sizeof(buffer));
                        // host writes number of players to subserver
                    }
                    strcpy(buffer,"\0");
                    int j = 0;
                    sleep(1);
                    for (i = 1; i <= sub_num; i++) { // i is the subserver number
                        read(fd1[i][0], buffer, sizeof(buffer));
                        // host reads username from EACH subserver
                        if (buffer[0] == 'U') {
                            int k = 0;
                            for (k = 1; k < strlen(buffer); k++) { // k is the character number
                                players[i - 1][k - 1] = buffer[k];
                            }
                        }
                    }
                    strcpy(buffer, "\0");
                    strcpy(buffer, "U");
                    int a = 0;// which username slot we are on
                    for (a = 0; a <= sub_num; a++) {
                        strcat(players[a], ",");
                        strcat(buffer, players[a]);
                    }
                    for (i = 1; i <= sub_num; i++) {
                        write(fd2[i][1], buffer, sizeof(buffer));
                        // host writes list of players to EACH subserver
                    }

                    int continue1 = 1;
                    while (continue1) {
                        for (i = 1; i <= sub_num; i++) { //host reads what the victim is
                            read(fd1[i][0], buffer, sizeof(buffer));
                            if (strcmp(buffer, "done") != 0) {
                                strcpy(victim, buffer); //sent by mafia
                                printf("%s\n", victim);
                            }
                        }
                        for (i = 1; i <= sub_num; i++) { //host writes the signal to each subserver
                            strcpy(buffer, "mafia done");
                            write(fd2[i][1], buffer, sizeof(buffer));
                        }

                        for (i = 1; i <= sub_num; i++) {
                            read(fd1[i][0], buffer, sizeof(buffer));
                        }

                        for (i = 1; i <= sub_num; i++) {
                            strcpy(buffer, "detective done");
                            write(fd2[i][1], buffer, sizeof(buffer));
                        }

                        for (i = 1; i <= sub_num; i++) {
                            read(fd1[i][0], buffer, sizeof(buffer));
                            if (strcmp(buffer, "done") != 0) {
                                strcpy(saved, buffer);
                            }
                        }
                        if (strcmp(victim, saved) == 0) { //if victim and saved are the same, then no one dies
                            strcpy(buffer, "");
                            for (i = 1; i <= sub_num; i++) {
                                write(fd2[i][1], buffer, sizeof(buffer));
                            }
                        } else {
                            for (i = 1; i <= sub_num; i++) {
                                write(fd2[i][1], victim, sizeof(victim));
                            }
                        }

                        char *statements = malloc(sizeof(char) * 12000);
                        strcpy(statements, "Here are the statements: \n");

                        for (i = 1; i <= sub_num; i++) {
                            read(fd1[i][0], buffer, sizeof(buffer));
                            printf("HOST read %s\n", buffer);
                            strcat(statements, buffer);
                            strcat(statements, "\n");
                            printf("Statements is now %s\n", statements);
                        }

                        for (i = 1; i <= sub_num; i++) {
                            write(fd2[i][1], statements, 12000);
                            printf("HOST wrote statements to %d\n", i);
                        }
                    }
                }
            }
        } else { // child ==> SUBSERVER
            int mode = -3;
            printf("Waiting for players to join...\n");
            while (mode == -3 && read(fd2[sub_num][0], buffer, sizeof(buffer))) {
                // subserver reads Start from server
                if (strcmp(buffer, "Start\n") == 0) {
                    write(client, buffer, sizeof(buffer));
                    // subserver writes Start to client
                    strcpy(buffer, "Game Started");
                } else if ((strlen(buffer) == 4) && ('N' == buffer[0])) {
                    write(client, buffer, sizeof(buffer));
                    // subserver writes number of players to client
                    mode = -2;
                } else if (strcmp(buffer, "done")) {
                    write(client, buffer, sizeof(buffer));
                }
            }
            while (mode == -2 && read(client, buffer, sizeof(buffer))) {
                if (buffer[0] == 'U') {
                    // subserver reads username from client
                    write(fd1[sub_num][1], buffer, sizeof(buffer));
                    // subserver writes username to host
                    mode = -1;
                }
            }
            while (mode == -1 && read(fd2[sub_num][0], buffer, sizeof(buffer))) {
                // subserver reads the list of usernames
                if (buffer[0] == 'U') {
                    write(client, buffer, sizeof(buffer));
                    // subserver writes the list of usernames to client
                    mode = 0;
                } else if (buffer[0] == 'R') {
                    write(client, buffer, sizeof(buffer));
                }
            }
            int continue1 = 1;
            while (continue1) {
                read(client, buffer, sizeof(buffer)); //subserver sending victim/saved/done
                write(fd1[sub_num][1], buffer, sizeof(buffer));
                read(fd2[sub_num][0], buffer, sizeof(buffer)); // subserver receives signal
                write(client, buffer, sizeof(buffer)); // subserver sends signal to client

                read(client, buffer, sizeof(buffer)); //subserver does detective signalling
                write(fd1[sub_num][1], buffer, sizeof(buffer));
                read(fd2[sub_num][0], buffer, sizeof(buffer));
                write(client, buffer, sizeof(buffer));

                read(client, buffer, sizeof(buffer)); //nurse signalling
                write(fd1[sub_num][1], buffer, sizeof(buffer));

                read(fd2[sub_num][0], buffer, sizeof(buffer)); //server sending who the dead person is
                write(client, buffer, sizeof(buffer));
                //printf("Transmission\n");

                mode = 4;
                while (mode == 4) {
                    read(client, buffer, sizeof(buffer));
                    printf("Subserver read %s from client\n", buffer);
                    write(fd1[sub_num][1], buffer, sizeof(buffer));
                    char spec_buffer[12000] = "";
                    read(fd2[sub_num][0], spec_buffer, sizeof(spec_buffer));
                    printf("Subserver read %s statements\n", spec_buffer);
                    write(client, spec_buffer, sizeof(spec_buffer));
                    mode = 0;
                }
            }

            close(client);
            exit(0);
        }
    }
    close(sd);
    return 0;
}
