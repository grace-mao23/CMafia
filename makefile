all: server.o mafia.o client.o
	gcc -c server.c -o server
	gcc -c mafia.c -o mafia
	gcc -c client.c -o client

server:
	./server

mafia:
	./mafia

client:
	./client
