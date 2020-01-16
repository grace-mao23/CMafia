//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include "networking.c"
//Amanda's Code
char **players;
int *roles; //0 is regular person, 1 is mafia, 2 is detective, 3 is nurse
int *votes;
int maf, nur, det, village, num_day,num_night,mdone,ddone,ndone;
char * username;
char *victim;
struct turns {
    char **member;
    int index;
};
struct turns m_turn;
struct turns n_turn;
struct turns d_turn;
//method for length of char **
int len_double(char **ary) {
    int count = 0;
    for (count = 0; ary[count] != NULL; count++) {
        //omg isn't this so cool!!! this actually works
    }
    return count;
}
//havent figured out exact numbers yet
//number of people based off of users (number of players)
int mafiaNum(int users) {
    int mafia = 0;
    if (users > 17) {
        mafia = 7;
    } else if (users > 13) {
        mafia = 6;
    } else if (users > 9) {
        mafia = 5;
    } else if (users > 7) {
        mafia = 4;
    } else if (users > 5) {
        mafia = 3;
    } else{
        mafia = 2;
    }
    maf = mafia;
    printf("Total Mafia Members: %d\n", mafia);
    return mafia;
}

//number of nurse based off of users (number of players)
int nurseNum(int users) {
    int nurse = 0;
    if (users > 17) {
        nurse = 4;
    } else if (users > 13) {
        nurse = 3;
    } else if (users > 7) {
        nurse = 2;
    } else if (users > 5) {
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
    if (users > 13) {
        detective = 4;
    } else if (users > 9) {
        detective = 3;
    } else if (users > 7) {
        detective = 2;
    } else {
        detective = 1;
    }
    det = detective;
    printf("Total Detective(s): %d\n", detective);
    return detective;
}
//gonna needa set village = length(players)
//need to initialize turns to 0
int genRoles() {
    int total = village;
    int m = maf;
    int d = det;
    int n = nur;
    roles = malloc(total *sizeof(int));
    //need to be freed
    unsigned int r;
    for (size_t i = 0; i < village; i++) {
        r = srand(time(0)) % total;
        if (r < m) {
            roles[i] = 1;
            m_turn.member[m_turn.index] = players[i];
            m_turn.index++;
            m--;
        } else if (r < m + d) {
            roles[i] = 2;
            d_turn.member[m_turn.index] = players[i];
            d_turn.index++;
            d--;
        } else {
            roles[i]=0;
            n_turn.member[m_turn.index] = players[i];
            n_turn.index++;
            n--;
        }
        total--;
    }
}

int usernames(char * new){
    int i = 0;
    strcpy(username, new);
    printf("Players in Game:");
    for (i = 0; players[i] != NULL; i++) {
        printf("%s, ", players[i]);
    }
    strcpy(players[i], new);
    printf("%s\n", new);
}

int getRole(char *check) {
    for (size_t i = 0; players[i] != NULL; i++) {
        if (strcmp(players[i], check) == 0) {
            return i;
        }
    }
    return -1;
}

void startSpecial() {
    m_turn.index = 0;
    d_turn.index = 0;
    n_turn.index = 0;
    for (size_t i = 0; players[i] != NULL; i++) {
        if (roles[i] == 1){
            m_turn.member[m_turn.index]=players[i];
            m_turn.index++;
        } else if (roles[i] == 2) {
            d_turn.member[m_turn.index]=players[i];
            d_turn.index++;
        } else if (roles[i] == 3) {
            n_turn.member[m_turn.index]=players[i];
            n_turn.index++;
        }
    }
}
//=================================================================================================================
//George's Code
int len_single(char *ary) {
    int count = 0;
    for (count = 0; &ary[count] != NULL; count++) {
        //omg isn't this so cool!!! this actually works
    }
    return count;
}

char *to_string(char **ary) {
    int size = 0;
    int i = 0;
    for (; i < len_double(ary); i++) {
        size += len_single(ary[i]);
        size += 2;
    }
    size -= 1;
    char *line = malloc(size *sizeof(char));
    line[0] = 0;
    for (i = 0; i < len_double(ary); i++) {
        if (strcmp(ary[i], " ") != 0) { //" " means the player has died
            strcat(line, ary[i]);
        }
        if (i < len_double(ary) - 1) {
            strcat(line, ", ");
        }
    }
    return line;
}

int remove_name(char **ary, char *name) {
    int i = 0;
    for (; i < len_double(ary); i++) {
        if (strcmp(ary[i], name) == 0) {
            strcpy(ary[i], " ");
            return 1;
        }
    }
    return 0;
}

//=================================================================================================================
//Grace's Code

// notes:
// num_players attainable through length of users later
// OR maybe game start when num_players equals length of char ** with usernames
// going with assumption that at least 5 people needed, will change later
int game_over = 0;
int game_start = 0;
int num_players, num_day, type_night, num_night; //type night is for the roles, while num night is how many nights have passed
int night;


// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
    int sd_conn, game_start = 0;
    char buffer[BUFFER_SIZE] = "not yet";

    sd_conn = client_setup(TEST_IP);

    if (sd_conn >= 0) {
        printf("Waiting for players to join...\n");

        while(read(sd_conn, buffer, sizeof(buffer))&&game_start==0) {
            if (strcmp(buffer, "Start\n") == 0) {
                game_start = 1;
                strcpy(buffer, "Game Started");
                printf("\n\n\nLET'S BEGIN!\n\n\n");
            }
        }
        srand(time(NULL));
        while (!game_start) {
            printf("\\Mafia$ Waiting for players to join...");
            while (num_players < 5) {
                update_players(); // PLAYERS JOIN
                sleep(1);
            }
            printf("\n\\Mafia$ %d players in game, start? (y/n)", num_players);
            char buffer[1000];
            fgets(buffer, 1000, stdin);
            if (strcmp(buffer, "y") == 0) {
                printf("\\Mafia$ Enter Username: ");
                fgets(buffer, 1000, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                printf("Your Username is: %s\n", buffer);
                usernames(buffer);
                printf("\\Mafia$ Waiting for other players...");
                while (num_players != len_double(players)) {
                    sleep(1);
                }
                printf("\nIn game: %s\n", to_string(players)); // DEVELOP A TO STRING FOR CHAR **
                printf("\\Mafia$ Generating Role...\n");
                genRoles();
                if (getRole(username) == 0) {
                    printf("Your Role: Civilian\n");
                } else if (getRole(username) == 1) {
                    printf("Your Role: Mafia\n");
                } else if (getRole(username) == 2) {
                    printf("Your Role: Detective\n");
                } else {
                    printf("Your Role: Nurse\n");
                }
                game_start = 1;
                night = 0;
                num_day = 1;
                num_night = 1;
                type_night = 0;
                votes = malloc(num_players * (sizeof(int) + 1));
                for (int i = 0; i < num_players; i++) {
                    votes[i] = 0;
                }
                votes[num_players] = 0;
                mafiaNum(num_players);
                detectiveNum(num_players);
                nurseNum(num_players);
                startSpecial();
                m_turn.index=0;
                d_turn.index=0;
                n_turn.index=0;
                mdone=0;
                ndone=0;
                ddone=0;
            }
        }
        while (!game_over) {
            if (!night) { //daytime
                printf("It's Daytime!\n");
                if (num_day == 1) {
                    printf("Welcome to Mafia!\n");
                    sleep(2);
                    printf("The night will begin shortly...\n");
                } else if (num_day == 1) {
                    if (victim != NULL) {
                        printf("%s has died!\n", victim);
                        if (strcmp(username,victim) == 0) {
                            game_over = 1;
                        } else {
                            printf("You have 5 minutes to discuss.\n"); //George's Timer
                        }
                    } else {
                        printf("Nobody Died!\n");
                    }
                    num_day = 2;
                } else if (num_day == 2) {
                  //chatbox
                } else {

                  //voting
                }
            } else { //nighttime
                if (type_night == 0) {
                    printf("Waiting for Mafia\n");
                    if (strcmp(username, m_turn.member[m_turn.index]) == 0) {
                        printf("Here are all of your victims: %s\n", to_string(players));
                        printf("\\Vote for your victim: ");
                        fgets(buffer, 1000, stdin);
                        buffer[strlen(buffer)-1] = '\0';
                        while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.%s\n Here are all of your victims\n", to_string(players));
                            printf("\\Vote for your victim: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }
                        printf("\nYou have selected to kill: %s\n", buffer);
                        strcpy(victim, buffer);
                        m_turn.index++;
                    }
                } else if (type_night == 1) {
                    printf("Waiting for Detective\n");
                    if (strcmp(username, d_turn.member[d_turn.index]) == 0) {
                        printf("Here are all of your suspects: %s\n", to_string(players));
                        printf("\\Choose to investigate a suspects: ");
                        fgets(buffer, 1000, stdin);
                        buffer[strlen(buffer) - 1] = '\0';
                        while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have chosen an invalid suspect.\n Here are all of your suspects: %s\n", to_string(players));
                            printf("\\Choose to investigate a suspects: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }
                        printf("\nYou have chosen to investigate: %s\n", buffer);
                        sleep(2);
                        if (getRole(buffer) == 0) {
                            printf("%s's identity is: Civilian\n", buffer);
                        } else if (getRole(buffer) == 1) {
                            printf("%s's identity is: Mafia\n", buffer);
                        } else if (getRole(buffer) == 2) {
                            printf("%s's identity is: Detective\n", buffer);
                        } else {
                            printf("%s's identity is: Nurse\n", buffer);
                        }
                    }
                    d_turn.index++;
                } else {
                    printf("Waiting for Nurse\n");
                    if (strcmp(username, n_turn.member[n_turn.index]) == 0) {
                        printf("Here are all of your patients: %s\n", to_string(players));
                        printf("\\Choose to save a patients: ");
                        fgets(buffer, 1000, stdin);
                        buffer[strlen(buffer) - 1] = '\0';
                        while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.\n Here are all of your patients: %s\n", to_string(players));
                            printf("\\Choose to save a patient: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer)-1] = '\0';
                        }
                        printf("\nYou have chosent to save: %s\n", buffer);
                        if (strcmp(victim, buffer) == 0) {
                            victim = NULL;
                        }
                        n_turn.index++;
                    }
                }
                m_turn.index++;
                num_night=2;
            } /*else if (num_night == 1) {
                printf("Waiting for Detective\n");
                if(strcmp(username,d_turn.member[d_turn.index])==0){
                  printf("Here are all of your suspects: %s\n", to_string(players));
                  printf("\\Choose to investigate a suspects: ");
                  fgets(buffer, 1000, stdin);
                  buffer[strlen(buffer)-1]='\0';
                  while(!valid(&buffer)){ //function to see if its valid victim
                    printf("\nYou have chosen an invalid suspect.\n Here are all of your suspects: %s\n", to_string(players));
                    printf("\\Choose to investigate a suspects: ");
                    fgets(buffer, 1000, stdin);
                    buffer[strlen(buffer)-1]='\0';
                  }
                  printf("\nYou have chosen to investiage: %s\n", buffer);
                  if(getRole(buffer)==0){
                    printf("%s's identity is: Civilian\n",buffer);
                  }else if(getRole(buffer)==1){
                    printf("%s's identity is: Mafia\n",buffer);
                  }else if(getRole(buffer)==2){
                    printf("%s's identity is: Detective\n",buffer);
                  }else{
                    printf("%s's identity is: Nurse\n",buffer);
                  }
                }
                while (!ddone) {
                  sleep(1);
                }
                d_turn.index++;
                num_night=3;
              }
              else{
                printf("Waiting for Nurse\n");
                if(strcmp(username,n_turn.member[n_turn.index])==0){
                  printf("Here are all of your patients: %s\n", to_string(players));
                  printf("\\Choose to save a patients: ");
                  fgets(buffer, 1000, stdin);
                  buffer[strlen(buffer)-1]='\0';
                  while(!valid(&buffer)){ //function to see if its valid victim
                    printf("\nYou have voted for an invalid victim.\n Here are all of your patients: %s\n", to_string(players));
                    printf("\\Choose to save a patient: ");
                    fgets(buffer, 1000, stdin);
                    buffer[strlen(buffer)-1]='\0';
                  }
                  printf("\nYou have chosent to save: %s\n", buffer);
                  if(strcmp(victim,buffer)==0){
                    victim=NULL;
                  }
                }else{
                  while (!ddone) {
                    sleep(1);
                  }
                }
                n_turn.index++;
                num_night=1;
              }
          }*/
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
