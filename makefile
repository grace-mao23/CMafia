ifeq ($(type), server)
	type = server
endif

ifeq ($(type), client)
	type = client
endif

all: $(type).c
	gcc $(type).c -o $(type)  

run:
	./$(type)

clean:
	rm *.o
