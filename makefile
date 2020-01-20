all: server.c mafia.c
	gcc server.c -o server
	gcc mafia.c -o mafia

clean:
	rm server
	rm mafia
