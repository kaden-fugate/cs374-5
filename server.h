#ifndef SERVER_H
#define SERVER_H

#define true 1
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

char *decrypt(char*,char*);
char *encrypt(char*,char*);
void set_up(struct sockaddr_in*,int*,int);
void cipherLoop(int,int,struct sockaddr_in*,socklen_t*,char*(char*,char*),char);

#endif