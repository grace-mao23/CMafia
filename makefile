ifeq ($(type), server)
	type = server
endif

ifeq ($(type), client)
	type = client
endif

all: server.c client.c
	gcc server.c -o server
	gcc client.c -o client

clean:
	rm server
	rm client
	rm *.o
