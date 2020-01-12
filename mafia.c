//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//Amanda's Code
char **players;
int *roles; //0 is regular person, 1 is mafia, 2 is detective, 3 is nurse
int *votes;
int maf, nur, det, village, num_day;

typedef struct turns{
    char **member;
    int index;
}
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
int nurseNum(int users){
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
        dectecive = 1;
    }
    det = detective;
    printf("Total Detective(s): %d\n", detective);
    return detective;
}
//gonna needa set village = length(players)
//need to initialize turns to 0
int genRoles() {
    total = village;
    m = mafia
    d = det
    n = nur
    roles = malloc(total * sizeof(int));
    //need to be freed
    unsigned int r;
    for (size_t i = 0; i < village; i++) {
        r = srand(time(0)) % total;
        if(r < m) {
            roles[i] = 1;
            m_turn->member[m_turn->index] = players[i];
            m_turn->index++;
            m--;
        } else if (r < m + d) {
            roles[i] = 2;
            d_turn->member[m_turn->index] = players[i];
            d_turn->index++;
            d--;
        } else {
            roles[i]=0;
            n_turn->member[m_turn->index] = players[i];
            n_turn->index++;
            n--;
        }
        total--;
    }
}

void assignTurns() {
    struct turns m_turn
}
//=================================================================================================================
//George's Code
int len_single(char *ary) {
    int count = 0;
    for (count = 0; ary[count] != NULL; count++) {
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
    char *line = malloc(size * sizeof(char));
    line[0] = NULL;
    for (i = 0; i < len_double(ary); i++) {
        strcat(line, ary[i]);
        if (i < len_double(ary) - 1) {
            strcat(line, ", ");
        }
    }
    return line;
}



//=================================================================================================================
//Grace's Code

// notes:
// num_players attainable through length of users later
// OR maybe game start when num_players equals length of char ** with usernames
// going with assumption that at least 5 people needed, will change later
int game_over = 0;
int game_start = 0;
int num_players;
int night;

int main() {
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
            usernames(); // SOME CODE TO UPDATE PLAYER USERNAMES
            printf("Your Username is: %s\n", buffer);
            printf("\\Mafia$ Waiting for other players...");
            while (num_players != len(players)) {
                sleep(1);
            }
            printf("\nIn game: %s\n", to_string(players)); // DEVELOP A TO STRING FOR CHAR **
            printf("\\Mafia$ Generating Role...\n");
            genRoles();
            printf("Your Role: %s", role(user)); // DEVELOP FUNCTION TO DISPLAY ROLE BASED ON USER
            //George's part
            game_start = 1;
            night = 0;
            num_day = 1;
            votes = malloc(num_players * (sizeof(int) + 1));
            for (int i = 0; i < num_players; i++) {
                votes[i] = 0;
            }
            votes[num_players] = NULL;
            mafiaNum(num_players);
            detectiveNum(num_players);
            nurseNum(num_players);
        }
    }

    while (!game_over) {
        if (!night) { //daytime
            if (num_day == 1) {

            } else {

            }
        } else { //nighttime

        }
    }
}
