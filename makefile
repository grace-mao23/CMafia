all: server.c mafia.c spectate.c
	gcc server.c -o server
	gcc mafia.c -o mafia
	gcc spectate.c -o spectate

clean:
	rm server
	rm mafia
	rm spectate
	rm *.o
