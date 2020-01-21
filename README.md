# CMafia
## Team Members
* Grace Mao, pd 10
* Amanda Zheng, pd 10
* George Zhou, pd 10

## Description
Our final project is a simplified version of the classic Mafia game in the terminal. Using networking, users will be able to run the game, with one host server, and play a full game, complete with night and day rounds, as well as a suitable number of mafia, detectives, and nurses. Up to 12 players can play, but no less than 3 players. During the night phase, the mafia will choose who to kill, the detectives will choose who to investigate, and the nurses will choose who to heal. During the day phase, each user will be able to input a statement defending themselves, after which users will vote for who they think is the mafia. The game will continue until the mafia are all dead or the number of mafia is equal to the number of other players.

## Instructions
First make sure to compile everything!
``` 
$ make
```


Second, run the main server on a separate terminal. This will handle game control, like initiating the game when all the players have joined.
```
$ ./server
```


Next, you can run the game on other terminals or machines. Make sure to specify the IP Address!
```
$ ./mafia
```


Now you can play!
