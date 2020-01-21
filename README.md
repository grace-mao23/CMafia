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


At the beginning of the game, everyone gets assigned one of the four following roles: Civilian, Mafia, Detective, and Nurse. Civilians are basically useless. Mafia gets to kill one person during the night phase. Nurse gets to heal one person during the night phase. Detectives get to find out the role of one other person in the night phase.


Once the game begins, the day phase will begin. The first day, there will be nothing for you to do. Then, comes the night phase, where each of the following roles can do their actions:
- Mafia will get to choose to kill person. If there are more than one mafia, only one mafia per turn gets to pick who dies. First, the mafia will be given a list of players to kill from, which excludes himself and other mafia. If he types the wrong name, then he will be prompted to pick again.
```
Mafia! Here are all of your victims.
Victims: a, b, c
\Vote for your victim: aba

You have voted for an invalid victim.
Here are all of your possible victims.
Victims: a, b, c
\Vote for your victim: b
You have selected to kill: b

Waiting for Detective...

Waiting for Nurse...
```
- Detective will get to choose to investigate one person. The person must be a player that is playing right now, or else they will be prompted to choose again. Likewise with mafia, if there are more than one detective, then only one detective gets to pick per round.
```
Waiting for Mafia...

Detective! Here are all of your suspects.
Suspects: a, b, c, d
\Choose to investigate a suspect: aba

You have voted for an invalid suspect.
Here are all of your possible suspects.
Victims: a, b, c, d
\Choose to investigate a suspect: b
You have selected to investigate: b
b's identity is: Civilian

Waiting for Nurse...
```
- Nurse will get to choose to save one person to save. The person must be a player that is playing right now, or else they will be prompted to choose again. Likewise with mafia, if there are more than one nurse, then only one nurse gets to pick per round.
```
Waiting for Mafia...

Waiting for Detective...

Nurse! Here are all of your patients:
Patients: a, b, c, d
\Choose to save a patient: aba

You have voted for an invalid patient.
Here are all of your possible patients.
Patients: a, b, c, d
\Choose to save a patient: b
You have selected to save: b
```
