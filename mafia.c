//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include "networking.c"
//Amanda's Code
char **players;
int *roles; //0 is regular person, 1 is mafia, 2 is detective, 3 is nurse
int *votes;
int maf, nur, det, mdone, ddone, ndone, num_players;
char * username;
char *victim;
int game_over = 0;
int game_start = 0;
int num_players, num_day, type_night, num_night; //type night is for the roles, while num night is how many nights have passed
int night;

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
    } else {
        nurse = 1;
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
void genRoles() {
    int total = num_players;
    int m = maf;
    int d = det;
    int n = nur;
    m_turn.member=calloc(m,sizeof(char*));
    n_turn.member=calloc(d,sizeof(char*));
    d_turn.member=calloc(n,sizeof(char*));
    for (size_t i = 0; i < m; i++) {
      m_turn.member=malloc(sizeof(char)*1000);
      strcpy(m_turn.member[i],"\0");
    }
    for (size_t i = 0; i < n; i++) {
      n_turn.member=malloc(sizeof(char)*1000);
      strcpy(n_turn.member[i],"\0");
    }
    for (size_t i = 0; i < d; i++) {
      d_turn.member=malloc(sizeof(char)*1000);
      strcpy(d_turn.member[i],"\0");
    }
    unsigned int r;
    for (size_t i = 0; i < num_players; i++) {
        r = rand() % total;
        if (r < m) {
            roles[i] = 1;
            m_turn.member[m_turn.index] = players[i];
            m_turn.index++;
            m--;
        } else if (r < m + d) {
            roles[i] = 2;
            d_turn.member[d_turn.index] = players[i];
            d_turn.index++;
            d--;
        } else {
            roles[i] = 0;
            n_turn.member[n_turn.index] = players[i];
            n_turn.index++;
            n--;
        }
        total--;
    }
}

void usernames(char *new) {
    int i = 0;
    strcpy(username, new);
    printf("Players in Game:");
    while (strcmp(players[i],"\0")!= 0) {
        printf("[[%s]], ", players[i]);
        i++;
    }
    strcpy(players[i], new);
    printf("[[%s]]\n",players[i]);
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

char **parse_args(char *line, char *del) {
    char *token;
    char **tokens;
    tokens = malloc(sizeof(char*) * 10);
    int i = 0;
    while (line != NULL) {
        token = strsep(&line, del);
        tokens[i] = token;
        i++;
    }
    tokens[i] = 0;
    return tokens;
}
//=================================================================================================================
//Grace's Code

// notes:
// num_players attainable through length of users later
// OR maybe game start when num_players equals length of char ** with usernames
// going with assumption that at least 5 people needed, will change later

// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
    int sd_conn, game_start = 0;
    char buffer[BUFFER_SIZE] = "not yet";
    num_players=0;
    num_night=0;
    num_day=0;
    username=malloc(sizeof(char)* 1000);
    strcpy(username,"\0");
    sd_conn = client_setup(TEST_IP);
    players=calloc(12,sizeof(char*));
    roles=calloc(12,sizeof(int));
    for (size_t i = 0; i < 12; i++) {
      players[i]=malloc(sizeof(char)*1000);
      strcpy(players[i],"\0");
      roles[i]=-1;
    }
    if (sd_conn >= 0) {
        printf("Waiting for players to join...\n");
        while(game_start == 0 && read(sd_conn, buffer, sizeof(buffer))) {
            if (strcmp(buffer, "Start\n") == 0) {
                game_start = 1;
                strcpy(buffer, "Game Started");
                printf("\n\n\nLET'S BEGIN!\n\n\n");
            }else if(strlen(buffer)==4&&buffer[0]=='N'){
              num_players=buffer[3]-96;
            }
        }

        srand(time(NULL));
        while(len_double(players)!=num_players){
          while(read(sd_conn,buffer,sizeof(buffer))){
            printf("Reading other playeres info\n");
            if(buffer[0]=='U'){
              buffer[0]=',';
              players=parse_args(buffer,",");
            }
          }
          if(strcmp(username,"\0")!=0){
            printf("\\Mafia$ Enter Username: ");
            fgets(buffer, 1000, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            printf("Your Username is: %s\n", buffer);
            usernames(buffer);
            printf("\\Mafia$ Waiting for other players...");
            strcpy(buffer,"U");
            strncat(buffer,username,strlen(username));
            printf("Copied over %s\n", buffer);
            write(sd_conn,buffer,sizeof(buffer));
          }

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
        m_turn.index = 0;
        d_turn.index = 0;
        n_turn.index=0;
        mdone = 0;
        ndone = 0;
        ddone = 0;
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
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.%s\n Here are all of your victims\n", to_string(players));
                            printf("\\Vote for your victim: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }*/
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
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have chosen an invalid suspect.\n Here are all of your suspects: %s\n", to_string(players));
                            printf("\\Choose to investigate a suspects: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer) - 1] = '\0';
                        }*/
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
                        /*while (!valid(&buffer)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.\n Here are all of your patients: %s\n", to_string(players));
                            printf("\\Choose to save a patient: ");
                            fgets(buffer, 1000, stdin);
                            buffer[strlen(buffer)-1] = '\0';
                        }*/
                        printf("\nYou have chosent to save: %s\n", buffer);
                        if (strcmp(victim, buffer) == 0) {
                            victim = NULL;
                        }
                        n_turn.index++;
                    }
                }
                m_turn.index++;
                num_night=2;
            }
        }
    }
    return 0;
}
