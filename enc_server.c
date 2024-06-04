#include "server.h"

int main(int argc, char *argv[]){

    // exit if port number was not specified when running executable
    if(argc < 2){ fprintf(stderr, "SERVER - ERROR: Please use the following format:\n'%s port'\n", argv[0]); exit(1); }

    // init vars
    int server_fd, client_fd, port = atoi(argv[1]);
    struct sockaddr_in server, client; 
    socklen_t client_len;
    
    /* this function will set up everything for the server
       i.e. server details, socket, binding, listening, etc) */
    set_up(&server, &server_fd, port);
    client_len = sizeof(client);

    cipherLoop(server_fd, client_fd, &client, &client_len, encrypt, 'e');

    return 0;

}