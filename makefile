CC=gcc --std=c99 -g

all: keygen enc_server enc_client dec_server dec_client

keygen: keygen.c
	$(CC) keygen.c -o keygen

enc_server: enc_server.c server.o
	$(CC) enc_server.c server.o -o enc_server

enc_client: enc_client.c client.o
	$(CC) enc_client.c client.o -o enc_client

dec_server: dec_server.c server.o
	$(CC) dec_server.c server.o -o dec_server

dec_client: dec_client.c client.o
	$(CC) dec_client.c client.o -o dec_client

server.o: server.c
	$(CC) server.c -c

client.o: client.c
	$(CC) client.c -c
	
clean: 
	rm -f *.o keygen enc_server enc_client dec_server dec_client