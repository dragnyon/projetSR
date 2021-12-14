#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define NB_J 2


struct sockaddr_in serv;
int fd;
int conn;
char message[100] = "";
char test[100]="vous etes connecte\0";
int nbjoueur=0;
int joueur[NB_J];
int manche=1;

//int cartesjoueurs[nbjoueur][manche+1];
//int cartejouer[manche*nbjoueur];

void afficheconn();


//void initcarte();
void main(int argc, char** argv)
{
	printf("Démarrage du jeu...\n");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(strtol(argv[1], NULL, 10));
	serv.sin_addr.s_addr = INADDR_ANY;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(fd, (struct sockaddr *)&serv, sizeof(serv));
	listen(fd,5);





	
	while(nbjoueur<=2)
	{
		conn = accept(fd, (struct sockaddr *)NULL, NULL);
		joueur[nbjoueur]=conn;
		nbjoueur++;
		afficheconn();
		printf("%d joueurs - %d %d %d",nbjoueur,joueur[0],joueur[1]);
		if(nbjoueur==2)
		{
			if(fork()==0)
			{
				while(recv(conn, message, 100, 0))
				{
						printf("\nCartes jouer par le joueur %d: %s\n",conn, message);
    					for(int i=0;i<2;i++)
    					{
    						send(joueur[i], message, strlen(message), 0);
    					}
    					memset(message,0,sizeof(message));
				}
			}
		}

	
	

   

   }

 fprintf(stderr,"Joueur %d deconnecter ",conn);
}





void afficheconn()
{
	
	printf("Le joueur %d viens de se connecté !\n",conn);
	printf("Il y a %d joueurs\n",nbjoueur);
}



