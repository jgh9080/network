#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
#define PASSWORD "ghost30"
#define CORRECT "Correct\n"
#define ERROR "Error\n"
void error_handling(char *mesage);

int main( int argc, char *argv[])
{
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;

    struct sockaddr_in serv_adr,clnt_adr;
    if(argc!=2)
    {
        printf("usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock= socket(PF_INET,SOCK_DGRAM,0);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))== -1)
        error_handling("bind() error");
    
    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len= recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        
        if( !strcmp(message,PASSWORD))
            sendto(serv_sock, CORRECT, 8,0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
        else
            sendto(serv_sock, ERROR, 6,0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);

    }

}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
