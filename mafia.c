//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include "networking.c"
//Amanda's Code
char **players;
int *roles; //0 is regular person, 1 is mafia, 2 is detective, 3 is nurse
int *votes;
int maf, nur, det, mdone, ddone, ndone, num_players;
char *username;
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
    printf("Total Mafia Member(s): %d\n", mafia);
    sleep(1);
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
    sleep(1);
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
    sleep(1);
    return detective;
}

void printint(int *list) {
    printf("starting to print\n\n");
    for (size_t i = 0; i < num_players; i++) {
        printf("%d, ", list[i]);
    }
    printf("\n");
}
//need to initialize turns to 0
void startSpecial() {
    m_turn.member = calloc(maf, sizeof(char*));
    d_turn.member = calloc(det, sizeof(char*));
    n_turn.member = calloc(nur, sizeof(char*));
    m_turn.index = 0;
    d_turn.index = 0;
    n_turn.index = 0;
    for (size_t i = 0; i < maf; i++) {
        m_turn.member[i] = malloc(sizeof(char) * 1000);
        strcpy(m_turn.member[i], "\0");
    }
    for (size_t i = 0; i < det; i++) {
        d_turn.member[i] = malloc(sizeof(char) * 1000);
        strcpy(d_turn.member[i], "\0");
    }
    for (size_t i = 0; i < nur; i++) {
        n_turn.member[i] = malloc(sizeof(char) * 1000);
        strcpy(n_turn.member[i], "\0");
    }
    for (size_t i = 0; i < num_players; i++) {
        if (roles[i] == 1){
            strcpy(m_turn.member[m_turn.index], players[i]);
            m_turn.index++;
        } else if (roles[i] == 2) {
            strcpy(d_turn.member[d_turn.index], players[i]);
            d_turn.index++;
        } else if (roles[i] == 3) {
            strcpy(n_turn.member[n_turn.index], players[i]);
            n_turn.index++;
        }
    }
    m_turn.index = 0;
    d_turn.index = 0;
    n_turn.index = 0;
}
int getRole(char *check) {
    for (size_t i = 0; i < num_players; i++) {
        if (strcmp(players[i], check) == 0) {
            return i;
        }
    }
    return -1;
}

int len_single(char *ary) {
    int count = 0;
    for (count = 0; &ary[count] != NULL; count++) {
        //omg isn't this so cool!!! this actually works
    }
    return count;
}


void print_players() {
    printf("\\Mafia$ In Game: ");
    for (size_t i = 0; i < num_players; i++) {
        if (players[i] >= 0) { //negative means they died
            printf("%s", players[i]);
            if (i != num_players - 1) {
                printf(", ");
            }
        }
    }
    printf("\n");
}

void print_players_mafia() { //this is for mafia when printing who's in the game that they can kill
    printf("\\Mafia$ Victims: ");
    for (size_t i = 0; i < num_players; i++) {
        if (roles[i] >= 0 && roles[i] != 1) { //negative means they died; 1 means they're mafia
            printf("%s", players[i]);
            if (i != num_players - 1) {
                printf(", ");
            }
        }
    }
    printf("\n");
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

void parse_int(char *line) {
    for (size_t i = 0; i < num_players; i++) {
        roles[i] = line[2 * i] - 48;
    }
}

int valid(char *name) {
    int i = 0;
    for (; i < num_players; i++) {
        if (strcmp(players[i], name) == 0) {
            return (roles[i] >= 0);
        }
    }
    return 0;
}

int valid_mafia(char *name) {
    int i = 0;
    for (; i < num_players; i++) {
        if (strcmp(players[i], name) == 0) {
            return (roles[i] >= 0 && roles[i] != 1);
        }
    }
    return 0;
}
//change turn struct
void removeMember(char *name) {
    int index = getRole(name);
    int replace = -1;
    num_players--;
    if (roles[index] == 1) {
        for (size_t i = 0; i < maf; i++) {
            if (strcmp(m_turn.member[i], name) == 0) {
                replace = i;
            }
            if (replace == maf - 1) {
                if (maf - 1 != 0) {
                    m_turn.index = (m_turn.index + 1) % (maf - 1);
                } else {
                    m_turn.index = -1;
                }
                i = maf;
            } else if (replace != -1) {
                if (i != maf - 1) {
                    strcpy(m_turn.member[i], m_turn.member[i + 1]);
                }
            }
        }
        maf--;
    } else if (roles[index] == 2) {
        for (size_t i = 0; i < det; i++) {
            if (strcmp(d_turn.member[i], name) == 0) {
                replace = i;
            }
            if (replace == det - 1) {
                if (det - 1 != 0) {
                    d_turn.index = (d_turn.index + 1) % (det - 1);
                } else {
                    d_turn.index = -1;
                }
                i = det;
            } else if (replace != -1) {
                if (i != det - 1) {
                    strcpy(d_turn.member[i], d_turn.member[i + 1]);
                }
            }
        }
        det--;
    } else if (roles[index] == 3) {
        for (size_t i = 0; i < nur; i++) {
            if (strcmp(n_turn.member[i], name) == 0) {
                replace = i;
            }
            if (replace == nur - 1) {
                if (nur - 1 != 0) {
                    n_turn.index = (n_turn.index + 1) % (nur - 1);
                } else {
                    n_turn.index = -1;
                }
                i = nur;
            } else if (replace != -1) {
                if (i != nur - 1) {
                    strcpy(n_turn.member[i], n_turn.member[i + 1]);
                }
            }
        }
        nur--;
    }
    for (size_t i = index; i < num_players; i++) {
        strcpy(players[i],players[i + 1]);
        //roles[i] = roles[i + 1];
    }
}

void readVotes(char *line){
    for (size_t i = 0; i < num_players; i++) {
        votes[line[i] - 97]++;
    }
    int max = 0;
    int dup = 0;
    for (size_t i = 0; i < num_players; i++) {
        if (votes[i] > votes[max]) {
            max = i;
            dup = 0;
        } else if (votes[i] == votes[max]) {
            dup = 1;
        }
    }
    if (dup) {
        printf("It was a tie! Nobody Gets Voted Out!\n");
    } else {
        printf("%s was voted to die! \n", players[max]);
        //insert gameover code for players max
        removeMember(players[max]);
    }
}
// notes:
// num_players attainable through length of users later
// OR maybe game start when num_players equals length of char ** with usernames
// going with assumption that at least 5 people needed, will change later

// LATER SHOULD BE MOVED TO MAFIA.C

int main() {
    int sd_conn, game_start = -1;
    char buffer[BUFFER_SIZE] = "not yet";
    num_players = 0;
    num_night = 0;
    num_day = 0;
    username = malloc(sizeof(char) * 1000);
    victim = malloc(sizeof(char) * 1000);
    strcpy(username, "\0");
    sd_conn = client_setup(TEST_IP);
    players = calloc(12, sizeof(char*));
    roles = calloc(12, sizeof(int));
    votes = calloc(12, sizeof(int));
    for (size_t i = 0; i < 12; i++) {
        players[i] = malloc(sizeof(char) * 1000);
        strcpy(players[i], "\0");
        roles[i] = -1;
        votes = 0;
    }
    if (sd_conn >= 0) {
        printf("Waiting for players to join...\n");
        while (game_start == -1 && read(sd_conn, buffer, sizeof(buffer))) {
            // client reads from subserver
            if (strcmp(buffer, "Start\n") == 0) {
                // client reads Start from subserver
                strcpy(buffer, "Game Started");
            } else if ((strlen(buffer) == 4) && (buffer[0] == 'N')) {
                // client reads number of players from subserver
                num_players = buffer[3] - 96;
                game_start = 0;
                printf("\n\n\nLET'S BEGIN!\n\n\n");
            }
        }
        srand(time(NULL));
        if (strcmp(username, "\0") == 0) {
            printf("\\Mafia$ Enter Username: ");
            fgets(buffer, 1000, stdin); // reads username from stdin
            buffer[strlen(buffer) - 1] = '\0';
            while (strlen(buffer) <= 0 || strlen(buffer) > 15) {
                printf("\\Mafia$ That is a username of invalid length. Try again\n");
                printf("\\Mafia$ Enter Username: ");
                fgets(buffer, 1000, stdin);
                buffer[strlen(buffer) - 1] = '\0';
            }
            printf("\\Mafia$ Your Username is: %s\n", buffer);
            strcpy(username, buffer); // username copied
            printf("\\Mafia$ Waiting for other players...\n");
            strcpy(buffer, "\0");
            strcpy(buffer, "U");
            strncat(buffer, username, strlen(username));
            write(sd_conn, buffer, sizeof(buffer));
            // client writes username to subserver
        }
        while (game_start == 0 && read(sd_conn, buffer, sizeof(buffer))) {
            // client reads list of usernames from subserver
            if (buffer[0] == 'U') {
                memmove(buffer, buffer + 1, strlen(buffer));
                for (size_t i = 0; i < 12; i++) {
                    free(players[i]);
                }
                free(players);
                players = parse_args(buffer, ", ");
                game_start = 1;
            }
            if (buffer[0] == 'R') {
                printf("\\Mafia$ Generating Roles...\n\n");
                memmove(buffer, buffer + 1, strlen(buffer));
                parse_int(buffer);
                //printint(roles);
            }
        }
        print_players();

        mafiaNum(num_players);
        detectiveNum(num_players);
        nurseNum(num_players);
        startSpecial(); //has to be after these three functions above
        printf("\n");
        if (roles[getRole(username)] == 0) {
            printf("\\Mafia$ Your Role: Civilian\n");
        } else if (roles[getRole(username)] == 1) {
            printf("\\Mafia$ Your Role: Mafia\n");
        } else if (roles[getRole(username)] == 2) {
            printf("\\Mafia$ Your Role: Detective\n");
        } else {
            printf("\\Mafia$ Your Role: Nurse\n");
        }
        printf("\n");
        game_start = 1;
        night = 0;
        num_day = 1;
        int type_day = 0;
        num_night = 1;
        type_night = 0;
        votes = calloc(num_players, sizeof(int));
        for (int i = 0; i < num_players; i++) {
            votes[i] = 0;
        }
        mdone = 0;
        ndone = 0;
        ddone = 0;
        // EVERYTHING UNDER THIS POINT SHOULD USE GAME_BUFFER, NOT BUFFER
        char game_buffer[BUFFER_SIZE] = "";
        //printf("all mafs\n");
        //for (int i = 0; i < maf; i++){
          //  printf("%s\n", m_turn.member[i]);
      //  }
        while (!game_over) {
            if (!night) { //daytime
                printf("\\Mafia$ It's Daytime!\n");
                sleep(1);
                if (num_day == 1) {
                    printf("\\Mafia$ Welcome to Mafia!\n");
                    sleep(2);
                    printf("\\Mafia$ The night will begin shortly...\n");
                    sleep(1);
                    night = 1;
                    num_day++;
                } else {
                    if (type_day == 0) {
                        if (strcmp(victim, "\0") == 0) {
                            printf("\\Mafia$ Nobody Died!\n");
                            sleep(2);
                            type_day++;
                        } else {
                            removeMember(victim);
                            printf("\\Mafia$ %s has died!\n", victim);
                            type_day++;
                        }
                    }
                    if (type_day == 1) { // statements
                        char actual[1000] = "\\";
                        if (getRole(username) != -1) {
                            printf("\\Mafia$ You will now have the chance to enter your statements\n");
                            printf("\\Mafia$ Your statement: ");
                            fgets(game_buffer, 1000, stdin);
                            game_buffer[strlen(game_buffer) - 1] = '\0';
                            printf("\\Mafia$ You entered: \"%s\"\n", game_buffer);
                            strcat(actual, username);
                            strcat(actual, "$ ");
                            strcat(actual, game_buffer);
                        } else {
                            strcpy(actual, "dead");
                        }
                        write(sd_conn, actual, sizeof(actual)); // write statement to subserver
                        int waiting_thing = 1;
                        while (waiting_thing && read(sd_conn, game_buffer, sizeof(game_buffer))) {
                            if (game_buffer[0] == '\\') {
                                waiting_thing = 0;
                            }
                        }
                        printf("%s\n", game_buffer);
                        type_day++;
                    }
                    if (type_day == 2) {
                        for (size_t i = 0; i < num_players; i++) {
                            votes[i] = 0;
                        }
                        if (getRole(username) != -1) {
                            printf("\\Mafia$ Please Vote for Who You Think is the Mafia!\n");
                            printf("\\Mafia$ Here are all your candidates:\n");
                            print_players();
                            printf("\\Mafia$ Vote: ");
                            fgets(game_buffer, 1000,stdin);
                            game_buffer[strlen(game_buffer) - 1] = '\0';
                            while (!valid(game_buffer)) { //function to see if its valid victim
                                printf("\n\\Mafia$ You have chosen an invalid candidate.\n\\Mafia$ Here are all of your possible suspects.\n");
                                print_players();
                                sleep(1);
                                printf("\\Mafia$ Vote: ");
                                fgets(game_buffer, 1000, stdin);
                                game_buffer[strlen(game_buffer) - 1] = '\0';
                            }
                            printf("\\Mafia$ You have selected to vote for %s\n", game_buffer);
                            char vote = 97 + getRole(game_buffer);
                            strcpy(game_buffer,"\0");
                            strcat(game_buffer, &vote);
                        } else {
                            strcpy(game_buffer, "dead");
                        }
                        write(sd_conn, game_buffer, sizeof(game_buffer));
                        int waiting_thing = 1;
                        while (waiting_thing && read(sd_conn, game_buffer, sizeof(game_buffer))) {
                            if (game_buffer[0] == 'V') {
                                waiting_thing = 0;
                            }
                        }
                        char *actual = "V";
                        actual = strstr(game_buffer, actual) + 1;
                        readVotes(actual);
                        //game_over = 1;
                        night = 1;
                        num_day++;
                    }
                    if (2 * maf >= num_players || maf <= 0) { //checks to see if game is over
                        game_over = 1;
                        printf("There are %d mafia and %d players left in the game\n", maf, num_players);
                        sleep(1);
                        if (maf <= 0) {
                            printf("The TOWN wins!\n");
                        } else {
                            printf("The MAFIA wins!\n");
                        }
                        strcpy(game_buffer, "over");
                        write(sd_conn, game_buffer, sizeof(game_buffer));
                        game_over = 1;
                    } else {
                        strcpy(game_buffer, "notover");
                        write(sd_conn, game_buffer, sizeof(game_buffer));
                        printf("\\Mafia$ There are %d mafia, %d detectives, %d nurses left in the game, with a total of %d players\n", maf, det, nur, num_players);
                        sleep(1);
                        printf("\\Mafia$ The game will end shortly...\n");
                        sleep(2);
                    }
                }
            } else { //nighttime
                printf("\nNIGHT BEGINNING!\n");
                if (type_night == 0) {
                    if (strcmp(username, m_turn.member[m_turn.index]) == 0) {
                        printf("\n\\Mafia$ Mafia! Here are all of your victims.\n");
                        print_players_mafia();
                        sleep(1);
                        printf("\\Mafia$ Vote for your victim: ");
                        fgets(victim, 1000, stdin);
                        victim[strlen(victim) - 1] = '\0';
                        while (!valid_mafia(victim)) { //function to see if its valid victim
                            printf("\nYou have voted for an invalid victim.\nHere are all of your possible victims.\n");
                            print_players_mafia();
                            sleep(1);
                            printf("\\Vote for your victim: ");
                            fgets(victim, 1000, stdin);
                            victim[strlen(victim) - 1] = '\0';
                        }
                        printf("\n\\Mafia$ You have selected to kill: %s\n", victim);
                        write(sd_conn, victim, sizeof(victim));
                        m_turn.index = (m_turn.index + 1) % maf;
                        sleep(1);
                    } else {
                        printf("\n\\Mafia$ Waiting for Mafia...\n");
                        strcpy(game_buffer, "done");
                        write(sd_conn, game_buffer, sizeof(game_buffer));
                    }
                    read(sd_conn, game_buffer, sizeof(game_buffer)); //block until server sends signal
                    type_night++;
                }
                if (type_night == 1) {
                    if (det > 0) {
                        if (strcmp(username, d_turn.member[d_turn.index]) == 0) {
                            printf("\n\\Mafia$ Detective! Here are all of your suspects.\n");
                            print_players();
                            sleep(1);
                            printf("\\Mafia$ Choose to investigate a suspect: ");
                            fgets(game_buffer, 1000, stdin);
                            game_buffer[strlen(game_buffer) - 1] = '\0';
                            while (!valid(game_buffer)) { //function to see if its valid victim
                                printf("\n\\Mafia$ You have chosen an invalid suspect.\nHere are all of your possible suspects.\n");
                                print_players();
                                sleep(1);
                                printf("\\Mafia$ Choose to investigate a suspect: ");
                                fgets(game_buffer, 1000, stdin);
                                game_buffer[strlen(game_buffer) - 1] = '\0';
                            }
                            printf("\n\\Mafia$ You have chosen to investigate: %s\n", game_buffer);
                            sleep(1);
                            if (roles[getRole(game_buffer)] == 0) {
                                printf("\\Mafia$ %s's identity is: Civilian\n", game_buffer);
                            } else if (roles[getRole(game_buffer)] == 1) {
                                printf("\\Mafia$ %s's identity is: Mafia\n", game_buffer);
                            } else if (roles[getRole(game_buffer)] == 2) {
                                printf("\\Mafia$ %s's identity is: Detective\n", game_buffer);
                            } else {
                                printf("\\Mafia$ %s's identity is: Nurse\n", game_buffer);
                            }
                            strcpy(game_buffer, "done");
                            write(sd_conn, game_buffer, sizeof(game_buffer));
                            sleep(1);
                        } else {
                            printf("\n\\Mafia$ Waiting for Detective...\n");
                            strcpy(game_buffer, "done");
                            write(sd_conn, game_buffer, sizeof(game_buffer));
                        }
                        read(sd_conn, game_buffer, sizeof(game_buffer)); //block until server sends signal
                        d_turn.index = (d_turn.index + 1) % det;
                    }
                    type_night++;
                }
                if (type_night == 2) {
                    if (nur > 0) {
                        if (strcmp(username, n_turn.member[n_turn.index]) == 0) {
                            printf("\n\\Mafia$ Nurse! Here are all of your patients.\n");
                            print_players();
                            sleep(1);
                            printf("\\Mafia$ Choose to save a patients: ");
                            fgets(game_buffer, 1000, stdin);
                            game_buffer[strlen(game_buffer) - 1] = '\0';
                            while (!valid(game_buffer)) { //function to see if its valid victim
                                printf("\n\\Mafia$ You have voted for an invalid patient.\nHere are all of your possible patients.\n");
                                print_players();
                                sleep(1);
                                printf("\\Mafia$ Choose to save a patient: ");
                                fgets(game_buffer, 1000, stdin);
                                game_buffer[strlen(game_buffer) - 1] = '\0';
                            }
                            printf("\n\\Mafia$ You have chosen to save: %s\n", game_buffer);
                            write(sd_conn, game_buffer, sizeof(game_buffer));
                        } else {
                            printf("\n\\Mafia$ Waiting for Nurse...\n");
                            strcpy(game_buffer, "done");
                            write(sd_conn, game_buffer, sizeof(game_buffer));
                        }

                        read(sd_conn, game_buffer, sizeof(game_buffer)); //block until server sends signal
                        n_turn.index = (n_turn.index + 1) % nur;
                    }
                    type_night = 0;
                }
                num_night++;
                night = 0;
                read(sd_conn, victim, sizeof(victim));
                sleep(1);
                if (strcmp(victim, username) == 0) { //checking to see if he dead
                    printf("\\Mafia$ Unfortunately, you have DIED\n");
                    sleep(1);
                    printf("\n\\Mafia$ Spectating the game now...\n");
                } else {
                    printf("\\Mafia$ Congradulations, you have SURVIVED the night\n");
                }
            }
        }
    }
    free(username);
    free(victim);
    free(players);
    free(votes);
    free(roles);
    for (size_t i = 0; i < maf; i++) {
        free(m_turn.member[i]);
    }
    for (size_t i = 0; i < det; i++) {
        free(d_turn.member[i]);
    }
    for (size_t i = 0; i < nur; i++) {
        free(n_turn.member[i]);
    }
    free(m_turn.member);
    free(d_turn.member);
    free(n_turn.member);
    return 0;
}
