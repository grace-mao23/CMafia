//Team: Grace Mao Amanda Zheng George Zhou (Team Gag)
#include<time.h>
#include<stdlib.h>
//Amanda's Code
char ** players;
int * roles;
int maf;
int nur;
int det;
int village;
//method for length of char **
int len(char ** ary){
  int count=0;
  for (count = 0; ary[count] != NULL; count++) {
  //omg isn't this so cool!!! this actually works
  }
}
//havent figured out exact numbers yet
//number of people based off of users (number of players)
int mafiaNum(int users){
  int mafia=0;
  if(users>17){
    mafia=7;
  }else if(users>13){
    mafia=6;
  }else if(users>9){
    mafia=5;
  }else if(users>7){
    mafia=4;
  }else if(users>5){
    mafia=3;
  }else{
    mafia=2;
  }
  maf=mafia;
  printf("Total Mafia Members: %d\n",mafia);
  return mafia;
}

//number of nurse based off of users (number of players)
int nurseNum(int users){
  int nurse=0;
  if(users>17){
    nurse=4;
  }else if(users>13){
    nurse=3;
  }else if(users>7){
    nurse=2;
  }else if(users>5){
    nurse=1;
  }else{
    nurse=0;
  }
  nur=nurse;
  printf("Total Number of Nurse(s): %d\n",nurse);
  return nurse;
}

//number of dectecive based off of users (number of players)
int detectiveNum(int users){
  int detective;
  if(users>13){
    detective=4;
  }else if(users>9){
    detective=3;
  }else if(users>7){
    detective=2;
  }else{
    dectecive=1;
  }
  det=detective;
  printf("Total Detective(s): %d\n",detective);
  return detective;
}
//gonna needa set village = length(players)
int genRoles(){
  srand(time(NULL));
  unsigned int r;
  for (size_t i = 0; i < village; i++) {
    r=rand()%village;
    roles[i]=r;
  }
}

//=================================================================================================================
//Grace's Code
