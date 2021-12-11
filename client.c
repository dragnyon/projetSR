#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>


struct sockaddr_in serv; 
int fd; 
int conn; 
int manche=3;
char envoi[100] = ""; 
char recu[100]="";



void* recevoir(void* infojeu);
void* env(void* cartej);


void error(char *msg)
{
    perror(msg);
    exit(0);
}


void main(){

fd = socket(AF_INET, SOCK_STREAM, 0);
serv.sin_family = AF_INET;
serv.sin_port = htons(1234);
inet_pton(AF_INET, "localhost", &serv.sin_addr);
if(connect(fd, (struct sockaddr *)&serv, sizeof(serv))<0)
{
	error("Serveur fermer");
}
 	pthread_t threadRecv;
	pthread_t threadSend;
   pthread_create(&threadRecv, NULL, recevoir, (void*)&recu);
 	pthread_create(&threadSend, NULL, env, (void*)&envoi);
	while(1) 
	{
		
	};


}









void* recevoir(void* infojeu)
{
	while(1)
	{	
		if(recv(fd,recu,100,0)>0)
		{
			printf("%s",recu);
			bzero(recu,100);
			printf("Quelle carte jouez-vous ?\n");
		}
		else
		{
			printf("vous etes deconnecte du serveur\n");
			exit(1);
		}	
	}
	
}

void* env(void* cartej)
{
	while(1) 
	{
		printf("Quelle carte jouez-vous ?\n");
    	fgets(envoi, 100, stdin); 	
    	send(fd, envoi, strlen(envoi), 0);
    	bzero(envoi,100);
	};
}



