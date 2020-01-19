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
    players=calloc(12,sizeof(char*));
    for (size_t i = 0; i < 12; i++) {
      players[i]=malloc(sizeof(char)*1000);
      strcpy(players[i],"\0");
    }
    //parent -> use fd2[i][1] write to subserver write function
    //forked ->fd2[sub_num][0] (subserver reads from server) read function
    //parent fd1[i][0] server read from subserver read function
    //forked fd1[sub_num][1]  subserver writes to server write function


    //sever writes to all subserver using fd2[i][1] "N*num*"   server.c
    //subserver in while loop checks to see servers, check if buffer: buffer[0]=='N' server.c
    //send buffer to clients use write (client, buffer,buffer)  mafia.c
    //if buffer[0]=='N' set num_players=parsed number at the end  mafia.c
    sd = server_setup();
    int checkin = 2; // when to prompt host // chenge to 6
    char *buffer_p = 0; // buffer pointer

    while (inplay != 0) {
        int client = server_connect(sd);
        sub_num = lowest_available(taken);
        taken[sub_num] = 1;
        char buffer[BUFFER_SIZE] = "not yet";
        pipe(fd1[sub_num]);
        pipe(fd2[sub_num]);
        f = fork();
        if (f) { // parent
            close(client);
            sleep(1); // give time for creation of pipe

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
                    for (; i <= sub_num; i++) {
                        char numP=sub_num+96;
                        strcpy(buffer,"Num");
                        strncat(buffer,&numP,1);
                        write(fd2[i][1],buffer,sizeof(buffer));
                    }
                    int j=0;
                    for(i=0;i<=sub_num; i++){
                      read(fd1[i][0],buffer,sizeof(buffer));
                      if(buffer[0]=='U'){
                        while(strcmp(players[0]!="\0")){
                          j++;
                        }
                        for (i = 1; i < strlen(buffer); i++) {
                          players[j][i-1]=buffer[i];
                        }
                      }
                    }
                    strcpy(buffer,"\0");
                    strcpy(buffer,"U");
                    for (size_t a = 0; a <= sub_num; a++) {
                      buffer+=players[sub_num]+",";
                    }
                    for (i = 0; i < 12; i++) {
                        write(fd2[i][1],buffer,sizeof(buffer));
                    }
                  }
                }
        } else {
            printf("Waiting for players to join...\n");
            while (read(fd2[sub_num][0], buffer, sizeof(buffer))) {
                if (strcmp(buffer, "Start\n") == 0) {
                    write(client, buffer, sizeof(buffer));
                    strcpy(buffer, "Game Started");
                }else if(strlen(buffer)==4&&'N'==buffer[0]){
                  write(client,buffer,sizeof(buffer));
                }else if(buffer[0]=='U'){
                  printf("Read from server part 2\n" );
                  write(client,buffer,sizeof(buffer));
                  printf("wrote to client part 2\n");
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
                  write(fd1[sub_num][0], "q", sizeof("q")); //isn't this for reading
                }
                if(buffer[0]=='U'){
                  write(fd1[sub_num][1],buffer,sizeof(buffer));
                  printf("subserver writes to server\n")
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
