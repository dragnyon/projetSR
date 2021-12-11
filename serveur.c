#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in serv; 
int fd; 
int conn; 
char message[100] = ""; 
char test[100]="vous etes connecte\0";
int nbjoueur=0;
int joueur[5];
pid_t pid;



void afficheconn();
void* envinfo(void* info);
void main()
{
	printf("Démarrage du jeu...\n");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1234); //Define the port at which the server will listen for connections.
	serv.sin_addr.s_addr = INADDR_ANY;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(fd, (struct sockaddr *)&serv, sizeof(serv));
	listen(fd,3);

	while(1)
	{
		
		conn = accept(fd, (struct sockaddr *)NULL, NULL);
		
		nbjoueur++; 
		joueur[nbjoueur]=conn;
		
		afficheconn();
		pid=fork();
		pthread_t threadenv;
		pthread_create(&threadenv,NULL,envinfo,(void*)&message);
		
    
    //fprintf(stderr,"Joueur %d deconnecter ",conn);
	
   }
   
    
}





void afficheconn()
{
	//nbjoueur++;
	printf("Le joueur %d viens de se connecté !\n",conn);
	printf("Il y a %d joueurs\n",nbjoueur);
}

void* envinfo(void* info)
{
	while(1)
	{
		
		if(pid==0)
		{
			
				if(recv(conn, message, 100, 0)>1)
				{
						printf("\nCartes jouer par le joueur %d: %s\n",conn, message);  
    					for(int i=0;i<5;i++)
    					{         
    						send(joueur[i], message, strlen(message), 0);   
    					}	        
    					memset(message,0,sizeof(message));
				}
			
		}
	}

}






