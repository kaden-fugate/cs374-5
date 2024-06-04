#include "client.h"

// this function will simply log an error if a file is invalid
void file_err(char file[]){ 
    fprintf(stderr, "CLIENT - ERROR: File '%s' invalid.\n", file); 
    exit(1); 
}

/*  this function will take a filename as an input
    and will return the text from that file an output. */
char *make_text(char file[]){

    // open file, log error if file not valid
    FILE *textfile = fopen(file, "r");
    if( !textfile ){ file_err(file); }

    // allocate 2^17 bytes for text (arbitrary number)
    char *text = malloc(131072);
    char ch = fgetc(textfile);
    int i;

    // read all characters from file
    for (i = 0; ch != EOF && ch != '\n'; i++) {

        // if current char is out of range and not ' ',
        // the current char is not valid
        if( ((int) ch < 65 || (int) ch > 90) && ch != 32 ){ file_err(file); }

        text[i] = ch;
        ch = fgetc(textfile);
    }
    text[i] = '\0';

    return text;
}

/*  function will handle everything involved with setting up a connection
    to the server, contacting the server, and recieving the servers response */
void contactServer(char *combined, int port){

    int server_fd;
    struct sockaddr_in server;

    // init servers data members 
    server.sin_family = AF_INET; 
    server.sin_port = htons(port);

    // set up the socket, log error if socket failed
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){ fprintf(stderr, "CLIENT - ERROR: Socket failed.\n"); } 

    // connect to server, log error if connection failed
    if (connect(server_fd, (struct sockaddr*) &server, sizeof(server)) < 0){ fprintf(stderr, "CLIENT - ERROR: Connection refused.\n"); }
    
    // write the text to the server, log error if we encounter one
    int numWritten = send(server_fd, combined, strlen(combined), 0);
    if (numWritten < 0){ fprintf(stderr, "CLIENT - ERROR: Send failed.\n"); }
    if (numWritten < strlen(combined)) fprintf(stderr, "CLIENT - WARNING: Not all data written to socket!\n");

    // recieve the text from the sever, print to STDOUT
    char *ciphered = malloc(131072);
    int charsRead = recv(server_fd, ciphered, 131072, 0);
    printf("%s\n", ciphered);

}