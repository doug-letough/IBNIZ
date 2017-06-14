#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

 /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char s[])
 {
     int i, sign;
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }


int message_to_int(char command[])
{
    char buffer[42];
    char strfinal[42];
    for (int i=0; i<strlen(command); i++ )
    {
        int ch;
        ch = command[i];
        itoa(ch, buffer);
        strcat(strfinal, buffer);
    }
    return atoi(strfinal);
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
