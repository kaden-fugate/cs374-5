#include "server.h"

/*  this function will be used to decrypt the ciphertext using the key. 
    i totally understand if i get graded down for the long lines lol. */
char *decrypt(char *ciphertext, char *key){

    char *decrypted = malloc(131072);
    
    for(int i = 0; i < strlen(ciphertext); i++){

        /* 🏌️ <--- lets go code golfing! were gonna go code golfing :) trust me- it works... 
            basically what the line below is doing is decrypting the character. it does this
            by typecasting both the ciphertext and key characters to ints and subtracting the
            key int from the ciphertext int. from here, we will need to use mod 27 on our value.
            i tried to do this with the '%' operator for negative values but it just wasnt working
            for some reason? so because of that, i add 27 to the key int subtracted from the
            ciphertext int when this value is negative. when its >= 0, i simply dont add 27.
            finally, i typecast this value to a char if it is not equal to 26. otherwise, i
            set it to a ' ' character. sorry that this line is 2x longer than the encrypt one... */

        decrypted[i] = ( (((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) < ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65)) ? 27 + (((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) - ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65))) : (((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) - ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65)))) ) == 26 ? ( (char) 32 ) :  (char) ( ((((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) < ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65)) ? 27 + (((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) - ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65))) : (((int) ciphertext[i] == 32 ? 26 : (((int) ciphertext[i]) - 65)) - ((int) key[i] == 32 ? 26 : (((int) key[i]) - 65))))) + 65);
    }
    
    return decrypted;
}

/*  this function will be used to encrypt the plaintext using the key. 
    i totally understand if i get graded down for the long lines lol. */
char *encrypt(char *plaintext, char *key){

    char *encrypted = malloc(131072);

    // loop through all letters in the plaintext that needs to be encrypted
    for(int i = 0; i < strlen(plaintext); i++){

        /* 🏌️ <--- lets go code golfing! were gonna go code golfing :) trust me- it works...
           basically what the line below is doing is encrypting the character. this does this 
           by taking the character from the plaintext string and subtracting 65 to put it on
           a 0 - 26 scale. HOWEVER, if it is a space character, it will have the ASCII val
           of 32 so we will simply set it to 26. from here, we take this value and use mod
           27 on it to make sure it stays between 0-26. then we will simply typecast this
           integer back to a character and insert it into the encrypted string. */

        encrypted[i] = ( (((int) key[i] == 32 ? 26 : (((int) key[i]) - 65)) + ((int) plaintext[i] == 32 ? 26 : (((int) plaintext[i]) - 65))) % 27 ) == 26 ? ( (char) 32 ) : ( (char) (( (((int) key[i] == 32 ? 26 : (((int) key[i]) - 65)) + ((int) plaintext[i] == 32 ? 26 : (((int) plaintext[i]) - 65))) % 27 ) + 65) );
    }

    return encrypted;
}

/*  this will initialize all of the socket things and server stuff like bind
    and listen and such. */
void set_up(struct sockaddr_in *server, int *server_fd, int port){

    // socket will listen to localhost on the specified port
    server->sin_family = AF_INET;
    server->sin_port = htons(port);
    server->sin_addr.s_addr = INADDR_ANY;

    // create socket, report error if there is one
    if( (*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ fprintf(stderr, "SERVER - ERROR: Socket failed.\n"); exit(1); }

    /* enable the socket and start listening with it with up to 5
       concurrent connections at any given time */
    if( bind(*server_fd, (struct sockaddr *) server, sizeof(*server)) < 0 ){ fprintf(stderr, "SERVER - ERROR: Binding failed.\n"); exit(1); }

    // start listening
    listen(*server_fd, 5);

}

void updateChildren(int childPIDS[5], int *children){

    int status;
    for(int i = 0; i < 5; i++){

        // -255 indicates no more childPIDS in array
        if(childPIDS[i] == -255){ break; }

        // check if child was killed
        pid_t killed = waitpid(childPIDS[i], &status, WNOHANG);

        // if killed, shift values in childPIDS to left
        if(killed){

            // shift to left, set NULL child pid, decrement children count
            for(int j = i + 1; j < 5; j++){ childPIDS[j - 1] = childPIDS[j];  }
            childPIDS[4] = -255;
            (*children)--;

        }

    }

}

/*  this loop will run after the server has been initialized. this function can be used for both server files.
    it will create a loop that will accept connections indefinitely. different server files will pass in
    different function pointers for how to cipher their text. different servers will also expect different
    characters at the beginning of their data to indicate which client is sending data. */
void cipherLoop(int server_fd, int client_fd, struct sockaddr_in *client, socklen_t *client_len, char *(cipherFunc)(char*,char*), char expected_client){

    // array to hold child PIDS
    int childPIDS[5] = {-255,-255,-255,-255,-255};
    int children = 0;

    /*  now that the server has been set up, we will listen indefinitely for 
        connections. when we find one, we can fork off a child and handle the 
        encryption there while the parent waits for more connections */
    while(true){

        // block until connection detected
        client_fd = accept(server_fd, (struct sockaddr *) client, client_len);

        // update array of children pids
        updateChildren(childPIDS, &children);

        // if 5 connections already on going, dont allow connection
        if(children == 5){ fprintf(stderr, "SERVER - ERROR: max connections (5) reached\n"); continue; }

        /* lets fork off a child to handle the encryption and accept a new
           connection in the parent process */
        int parent = fork();

        // child process
        if(!parent){

            // allocate mem for combined, text, and key 
            char *combined = malloc(262147);
            char *text = malloc(131072), *key = malloc(131072);

            /* this line will recieve a message in the format "text+key" with
            this, we will split the line into by the '+' delimiter. i did this
            because i was having trouble accepting two messages separately. */
            usleep(50000);
            int charsRead = recv(client_fd, combined, 262147, 0);
            
            // if client attempts to incorrect server
            if(combined[0] != expected_client){ 
                fprintf(stderr, "SERVER - ERROR: Invalid client attempting to connect to server.\n"); 
                send(client_fd, "\0", 1, 0); // send empty data so client wont block indefinitely
                exit(0); 
            }

            // we will get the text first then the key
            text = strtok(combined + 1, "+");
            key = strtok(NULL, "+");

            // encrypt the message and send it back to the client
            char *ciphered = cipherFunc(text, key);
            int numWritten = send(client_fd, ciphered, strlen(ciphered), 0);

            exit(0);
        }

        // add child pid to array of PIDS
        childPIDS[children] = parent;
        children++;

    }

}