#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** players;
int * roles;
int main(int argc, char const *argv[]) {
  roles=calloc(12,sizeof(int));
  for (size_t i = 0; i < 12; i++) {
    roles[i]=-1;
    printf("%d\n", roles[i]);
  }


  players=calloc(12,sizeof(char*));
  for (size_t i = 0; i < 12; i++) {
    players[i]=malloc(sizeof(char)*1000);
    strcpy(players[i],"\0");
  //  printf("%s\n",players[i]);
  }
  if(players==NULL){
    strcpy(players[0],"happy");
    printf("%s\n",players[0] );
    return 0;
  }
}
