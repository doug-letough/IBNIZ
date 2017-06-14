/*
    Basic TCP server for IBNIZ
    © 2017 - Doug Le Tough
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h>

int create_server_socket(int server_port){
    int socket_desc;
    struct sockaddr_in server;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket. Error");
        return 1;
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( server_port );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    printf("Binding done on port TCP/%d\n", server_port);
    return socket_desc;
}

int server_listen(int socket_desc, int ibniz_buffer, void *x_void_ptr){
    int client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    listen(socket_desc , 3);

    puts("Waiting for incoming connections");
    c = sizeof(struct sockaddr_in);

    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        if (strcmp(client_message, "plop\r\n") == 0)
        {
            printf("GO");
        } else {
            printf("fuck");
        }
        //Send the message back to client
        printf("Received from client: %s", client_message);
        write(client_sock , client_message , strlen(client_message));
    }
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}

int main(int argc , const char *argv[])
{
    int server_socket, server_port;
    pthread_t server_thread;
    
    printf("%s", argv[1]);
    if (argc != 3 || strcmp(argv[1], "-p")){
        printf("Usage: %s -p <int PORT>\n", argv[0]);
        return 1;
    }
    
    server_port = atoi(argv[2]);
    server_socket = create_server_socket(server_port);
    if (server_socket == 1)
    {
        perror("Fatal: Unable to create socket");
        return 1;
    }

    if(pthread_create(&server_thread, NULL, server_listen, server_socket, 0))
    {
        perror("Fatal: Unable creating server thread");
        return 1;
    }
    
    //~ server_listen(server_socket, 0);
}
