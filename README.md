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


Next, you can run the game on other terminals or machines. Make sure to specify the correct IP Address, if you are going to use a custom IP address! Otherwise, it will use localhost. In addition, there must be a minimum of 3 players. The maximum number of players is 12
```
$ ./mafia
```
Now you can play! To initiate game play, the main server terminal must indicate in agreement that the game will begin, like so:
```
3 players in the game. Ready to start? (yes/no)
```
You must respond correctly with "yes" or "no" and then press the Return key on your keyboard. Responding with any other response will cause the program to fail. In addition, if there are 4 or more users connected, but you are still on the prompt concerning with 3 people, that is okay! If you say yes, it will start the game without the latest players, and only play with the first 3 people in the game. If you say no, then it will give you the prompt regarding 4 players.


At the beginning of the game, everyone gets assigned one of the four following roles: Civilian, Mafia, Detective, and Nurse. Civilians are basically useless. Mafia gets to kill one person during the night phase
