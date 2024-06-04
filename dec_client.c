#include "client.h"

int main(int argc, char *argv[]){

    // exit if not all arguments were specified in the command line
    if(argc < 2){ fprintf(stderr, "CLIENT - ERROR: Please use the following format:\n'./%s ciphertext key port'\n", argv[0]); exit(1); }

    // set up text to be written to the server
    char *ciphertext = make_text(argv[1]), *key = make_text(argv[2]);
    char *combined = malloc( strlen(ciphertext) + strlen(key) + 3 );
    sprintf(combined, "d%s+%s", ciphertext, key); // combine text to send less data segments

    // if the key is shorter than the ciphertext, we cannot encrypt
    if(strlen(key) < strlen(ciphertext)){ fprintf(stderr, "CLIENT - ERROR: Key too short.\n"); exit(1); }

    // contact the server
    contactServer(combined, atoi(argv[3]));

    return 0;

}