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
#include<string.h>
#include<signal.h>
#define PORT 8000

struct socketR{
	int r;
};

struct socketS{
	int s;
};


char recvBuffer[1000],sendBuffer[1000];

void * receiver(void*clientSocketDescriptor)
{
	while(1)
	{
		usleep(2000);
		//printf("checking rcvr in server"); 
		struct socketR *info1=clientSocketDescriptor;
		bzero(&recvBuffer,sizeof(recvBuffer));
		recv(info1->r,recvBuffer,sizeof(recvBuffer),0);
		printf("\nclient: %s\n",recvBuffer);
	}
}

void * sender(void * clientSocketDescriptor)
{
	while(1)
	{
		//printf("sender server\n");
		usleep(1000);
		struct socketS *info2=clientSocketDescriptor;
		bzero(&sendBuffer,sizeof(sendBuffer));
		printf("\nType a message here ...  ");
		/*Read the message from client*/
		fgets(sendBuffer,10000,stdin);
		send(info2->s,sendBuffer,strlen(sendBuffer)+1,0);
		printf("\nMessage sent !\n");
	}
}

void main()
{
	int err1,err2,a;
	int clientSocketDescriptor,socketDescriptor;
	struct sockaddr_in serverAddress,clientAddress;
	struct socketR *info1= malloc(sizeof(struct socketR));
	struct socketS *info2= malloc(sizeof(struct socketS));
	socklen_t clientLength;
	bzero(&serverAddress,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port=PORT;
	socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
	bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

	listen(socketDescriptor,5);
	clientSocketDescriptor=accept(socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);

	printf("%lx",serverAddress.sin_addr.s_addr);

	info1->r=clientSocketDescriptor;info2->s=clientSocketDescriptor;

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
/*	return 0;*/
	
}
 

