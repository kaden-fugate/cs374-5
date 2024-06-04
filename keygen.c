#include "keygen.h"

int main(int argc, char *argv[]){

    // if user doesnt include a length, exit program and log error
    if(argc < 2){ fprintf(stderr, "ERROR! Please use the following format:\n'./%s num'\n", argv[0]); exit(1); }

    // init vars:
    //      - rand seed (no variable)
    //      - make num from specified key size (keySize)
    //      - init mem for key (key)
    srand(time(0));
    const int keySize = atoi(argv[1]);
    char *key = malloc(keySize + 1);
    
    // generate N rand chars (N == key length)
    for(int i = 0; i < keySize; i++){
        
        // generate random number between 65 and 91.
        // if num is equal to 91, set the num equal to 32.
        // 32 is the ASCII value for ' '.
        int randNum = (rand() % 27) + 65; 
        if(randNum == 91){ randNum = 32; }

        // typecast randNum to char form and add to key
        key[i] = (char) randNum;

        if( ((int) key[i] < 65 || (int) key[i] > 90) && (int) key[i] != 32 && key[i] != '\n' && key[i] != EOF){ printf("\n\nERROR! - \n\n"); break; }


    }

    // write key to stdout so it can be redirected
    printf("%s\n", key);

    return 0;
}