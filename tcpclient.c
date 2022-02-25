//two way communication 

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"
#include <signal.h>
#include<string.h>
#define PORT 8000

struct socketR{
	int r;
};

struct socketS{
	int s;
};

char recvBuffer[1000],sendBuffer[1000];

void * receiver(void * socketDescriptor)
{
	while(1)
	{	
		usleep(1000);
		struct socketS *info2=socketDescriptor;
		bzero(&sendBuffer,sizeof(sendBuffer));
		printf("\nType a message here ...  ");
		/*Read the message from client*/
		fgets(sendBuffer,10000,stdin);
		send(info2->s,sendBuffer,strlen(sendBuffer)+1,0);
		printf("\nMessage sent !\n");
	}
}

void * sender(void* socketDescriptor)
{
	while(1)
	{
		usleep(2000);
		struct socketR *info1=socketDescriptor;
		bzero(&recvBuffer,sizeof(recvBuffer));
		recv(info1->r,recvBuffer,sizeof(recvBuffer),0);
		printf("\nCLIENT : %s\n",recvBuffer);
	}
}

int main(int argc,char *argv[])
{
	int err1,err2;
	int clientSocketDescriptor,socketDescriptor,a;
	struct sockaddr_in serverAddress,clientAddress;
	struct socketR *info1= malloc(sizeof(struct socketR));
	struct socketS *info2= malloc(sizeof(struct socketS));
	socklen_t clientLength;
	bzero(&serverAddress,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr= inet_addr("192.168.1.117");
	serverAddress.sin_port = PORT;

	socketDescriptor=socket(AF_INET,SOCK_STREAM,0);

	connect(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
	info1->r=socketDescriptor;info2->s=socketDescriptor;

 	pthread_t send,receive;
	err1=pthread_create(&send,NULL,sender,info2);
	if(err1){
		printf("Error is:%d",err1);
		return 1;
	}
	err2=pthread_create(&receive,NULL,receiver,info1);
	if(err2){
		printf("Error is:%d",err2);
		return 1;	
	}
	pthread_join(send,NULL);
	pthread_join(receive,NULL);	
 
  close(socketDescriptor);
  close(clientSocketDescriptor);
  return 0;
}
