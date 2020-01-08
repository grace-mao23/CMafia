#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#define PORT "9001"
#define TEST_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define INPLAY 0

// networking.c
int server_setup();
int server_connect(int sd);
int client_setup(char *server);
void error_check( int i, char *s );void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}
