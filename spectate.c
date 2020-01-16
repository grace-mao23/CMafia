#include "networking.c"

int main() {
    int sd_conn, game_start = 0;
    char buffer[BUFFER_SIZE] = "q";

    sd_conn = client_setup(TEST_IP);

    if (sd_conn > 0) {
        write(sd_conn, buffer, sizeof(buffer));
    }

    return 0;
}
