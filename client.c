#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

struct sockaddr_in serv;
int fd;


char envoi[100] = "";
char recu[100]="";
char message[100] = "Attente d'autre joueurs\n";
char message1[100] = "La partie commence !\n";
char gagner[50]="g";
char perdu[50]="p";

void* recevoir(void* infojeu);
void* env(void* cartej);


void error(char *msg)
{
    perror(msg);
    exit(0);
}


void main(int argc, char** argv){

fd = socket(AF_INET, SOCK_STREAM, 0);
serv.sin_family = AF_INET;
serv.sin_port = htons(strtol(argv[1], NULL, 10));
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
		if(recv(fd,recu,100,0))
		{
			if(strcmp(recu,perdu)==0)
			{
				printf("Manche perdu");
			}
			else
			{
				if(strcmp(recu,gagner)==0)
				{
					printf("Manche gagne");
				}
				else
				{
					if(strcmp(recu,message)==0)
					{
						printf("Attente d'autre joueurs\n");
					}
					else
					{
						if(strcmp(recu,message1)==0)
						{
						printf("La partie commence !\n");
						}
						else
						{
							printf("Vos cartes : ");
							for(int i=0;i<10;i++)
							{
								if(recu[i]!=0)				
								{
									printf("%d-",recu[i]);
								}
							}
							printf("\nAppuyez sur Entrée pour jouer: \n(vous jouerez votre plus petite carte)\n");
						}	
					}
				}
			}
			bzero(recu,100);
		
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
		//printf("\nAppuyez sur Entrée pour jouer: \n(vous jouerez votre plus petite carte)\n");
      fgets(envoi, 100, stdin);
		send(fd, envoi, strlen(envoi), 0);
    	bzero(envoi,100);
	};
}
