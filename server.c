#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "server.h"

int process_client(int client_sock, net_params *net) {
  int read_size;
  char client_message[2000];
  while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ) {
    if (strcmp(client_message, "/quit\r\n") == 0) {
      printf("Connection reset by peer.\n");
      close(client_sock);
      return 0;
    }
    ed_setprogbuf(client_message);
    (*net).codechanged = 1;
  }
  if (read_size == -1) {
    perror("recv failed");
    close(client_sock);
    return 1;
  }
  return 0;
}

int server_listen(net_params *net){
  int client_sock, c, pid;
  int listening = 1;
  struct sockaddr_in server, client;
  
  listen((*net).server_socket , 3);
  printf("Waiting for incoming connections...\n");
  c = sizeof(struct sockaddr_in);
  while(listening) {
    client_sock = accept((*net).server_socket, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
      perror("Error while accepting connection.");
      return 1;
    }
    printf("Client connected\n");
    listening = 0;
    process_client(client_sock, net);
    printf("Client disconnected\n");
    fflush(stdout);
    server_listen(net);
  }
}


int create_server_socket(int server_port){
  int server_sock;
  struct sockaddr_in server;
  server_sock = socket(AF_INET , SOCK_STREAM , 0);
  if (server_sock == -1) {
    perror("Could not create socket. Error");
    return 1;
  }
  printf("Socket created on port %d\n", server_port);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( server_port );
  if( bind(server_sock,(struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("bind failed. Error");
    return 1;
  }
  printf("Binding done on port TCP/%d\n", server_port);
  return server_sock;
}
