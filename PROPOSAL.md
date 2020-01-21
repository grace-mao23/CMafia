# CMafia
## Team Members
* Grace Mao, pd 10
* Amanda Zheng, pd 10
* George Zhou, pd 10

## PROPOSAL
### Description
For our final project, we will be creating a multiplayer Mafia game in the terminal. Up to 12 people can play, but once a game has started, no new players may join. At the start, roles (mafia, nurse, or detective) will be distributed randomly, and all other players will remain citizens. The number of mafias will be determined by how many players there are. At the beginning of each round called the “night” phase, the mafia will decide who to kill, the nurse will decide who to heal, and the detective can find out one player’s role (might be more roles)... After that, dead players will only be able to view the game, not participate. After that phase of the game, each player can write a statement explaining why they are not the mafia. Then, each player votes for a player that they think are the mafia for a minute. If any person gets the majority of the votes, the voting goes further: There will be a minute for players to vote if they think the person should be executed. If a player gets a majority of yeses, then he shall be executed. Ties will result in no one being executed. The voted person's role will not be displayed. The game ends when either all mafia die or mafia outnumber the citizens.

### User Experience
A user will either join a server or open a server. After the minimum amount of people are on the server, the host can choose to start game or wait for more players. Then everyone enters a username and gets a role displayed only to themselves. Next, the night round commences and one person of each special role vote for a person of their choice (to kill, to heal, or check identity depending on the role of the person). Then, in the morning, the results are announced. Every user can type up their defense. Then everyone will vote for someone to be executed (ties means no one dies). Another night round continues and the round keeps going until all mafia members are dead or mafia outnumbers the innocent people. The people who died before the game ends can choose to spectate the game or leave the game.

**Example**
```
\Mafia$ Waiting for players to Join…
\Mafia$ 5 players in game, start? (yes/no)
\Mafia$ Waiting for other players to Join
\Mafia$ Enter Username : dan_
Your Username is: dandymandy
\Mafia$ Waiting for other players...
In game: horhay63, maobao123, dandymandy (you), mabelline, dwdwdwdw, apples111
\Mafia$ Generating Role…
      Your Role: Mafia
Night Round Begins!
Mafia Please Vote for Your Victim
------------------------------------------------//shown only to this special role
Other Mafias: horhay63
\Mafia$ maybell_
You voted for maybelline!
Next Mafia to vote: horhay63
\Mafia$ Waiting for Doctor to select someone to save…
\Mafia$ Waiting for Police to select someone to save…
\Mafia$ Night Ended!
\Mafia$ maybelline was killed!
Please Present your statements!
Horhay63: I'm not the mafia. I'm just a civilian
maobao123: I am the detective
dandymandy: I am a detectivie and I checked that horhay was a civilian
dwdwdwdw: I am the doctor
apples111: I am innocent
Vote for who you think is the mafia!
\vote$ maob_
\Mafia$ maobao123 died :( The next night is beginning…
```

### Technical Design
Topics Implemented:
1. Allocating Memory
2. Networking
3. Pipes
4. Forking

ALLOCATING MEMORY

For many of our variables (such as char * and char * * pointers) we need to calloc and malloc. For example, to hold variables such as username of the client, list of player names, voting list ... etc. 

NETWORKING

We will need networking because this is a multiplayer game (not pass and play version) so everyone will get their computer and join into a game like a server. This requires networking to coordinate all the computers together with the game.

PIPES

We will need pipes so that the main server can communicate with all the subservers and transmit and read information. The server has to facilitate all the usernames being written and write it to all the other clients so they can have an updated players list and also for coordinating votes and individual choices of individual clients must be shared with subserver to share with server to give to all other subservers to write to clients.

FORK

We will need forking so that we can fork a server-subserver off of the main client so that we have a more organized code. A forked child will get all the data from the main parent and if it messes up, we can make the child die and fork again and revert to the original data.

### Development Stages
1. Modular Setup and Data Structures
   * Create any necessary files, including makefiles, header files etc
   * Code in #includes and other necessary definitions
   * Decide on function headers and required functionality methods
   * Initialize data structures needed, can be added onto later
2. Game Shell
   * Open shell every time game is run, ask to start after 5 players have joined
   * Semaphore will only allow up to 12 players to join, nobody will be able to join after the game begins
   * Writing in-game shell before prompted should have no effect
3. Initial game setup
   * Implement shell functionality of organizing the game and simulating the different rounds
   * Some text-only visible for certain people
   * Voting system should work with a tie system etc
4. Networking
   * Multiple people should be able to access the server and play together
5. Extra Features (with more time)
   * Mafia will be able to communicate with each other during the Night Round in order to decide who they will kill
   * With a tie, players will be able to revote between the ones voted on in the tie

### Data Structures and Algorithms
**Structures**
* int[] roles
   * The list of roles at the beginning of the game.
* *char[] players
   * The list of nicknames. The indices correspond to the roles in the arrays
* int[] votes
   * Gives a tally of the number of votes each person has (you can’t change your vote by the way)
* int num_players
   * Keeps track of how many players are on the server.
int maf
   * Keeps track of how many mafia members left
int nur
   * Keeps track of how many nurse members left
int det
   * Keeps track of how many detective members left
char * username
   * Keeps track of name of each client
char *victim
   * Keeps track of who mafia chose to kill
int game_over
   * Keeps track of if game is over or not
int game_start
   * Keeps track when to start game
int num_day
   * Keeps track of how day phases occured
int num_night
   * Keeps track of how night phases occured
int type_night
   * Keeps track of what phase of night it is
int type_day
   * Keeps track of what phase of day it is

struct turns {
    char * *member; //shows members of this role
    int index;      //which member is voting next
};
struct turns m_turn;
struct turns n_turn;
struct turns d_turn;

**Algorithms**
* Random
   * We randomize the roles by first creating a list of all the possible roles (if a game has 2 mafia 1 nurse 1 detective and 2 civilians then the list would be [1,1,3,2,0,0] where the numbers 0-3 represent a specific role) and then choose an index of it to be put into roles one by one. One it is chosen, the number is swapped with the last element and the modder is decreased
* Chat Piping
   * We will need to interpret the information given by pipes. This allows secret conversations between people, whether they are in the mafia or not. It will create a pipe starting from the person who wants to initiate a *secret* and then the end at the person who it wants to be sent to. Then, it must be formatted correctly to be displayed in the recipient’s terminal.
* Sending int info
  * We will send information of integers by converting them into chars so we don't have to deal with how many digits there are and parsing integers

### Timeline
Date | Objective
--- | ---
JANUARY 2 | Proposal Due
JANUARY 4 | Modular Setup with Data Structures
JANUARY 6 | Set up Mafia shell (complete with working semaphore)
JANUARY 8 | Simulate Night Round of the game
JANUARY 10 | Distribution of roles finished with correct number of Mafia by number of players
JANUARY 13 | Create chatting functionality after night round using pipes
JANUARY 14 | Voting system finished if not finished before
JANUARY 15 | Finish networking and start final debugging
JANUARY 17 | Final Project Due

### Work Division
Symbol | Level
--- | ---
H | Hard
M | Medium
E | Easy

**George**
* Networking (H)
* Nurse Role (M)
* Username Controls (E)
* End Game (E)

**Amanda**
* Mafia Role (H)
* Creating Server (M)
* Vote (M)
* Quit Game (E)

**Grace**
* Statements (H)
* Detective Role (M)
* Allowing people to join server (M)
* Terminal Updates (E)
