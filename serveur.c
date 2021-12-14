#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in serv;
int fd;
int conn;
char message[100] = "";
char test[100]="vous etes connecte\0";
int nbjoueur=0;
int joueur[5];
int manche=1;

//int cartesjoueurs[nbjoueur][manche+1];
//int cartejouer[manche*nbjoueur];

void afficheconn();

//void topdepart();
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





	
	while(conn = accept(fd, (struct sockaddr *)NULL, NULL))
	{
		joueur[nbjoueur]=conn;
		nbjoueur++;
		afficheconn();
		printf("%d joueurs - %d %d %d %d %d",nbjoueur,joueur[0],joueur[1],joueur[2],joueur[3],joueur[4]);
		if(fork()==0)
		{
				while(recv(conn, message, 100, 0))
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

 fprintf(stderr,"Joueur %d deconnecter ",conn);
}





void afficheconn()
{
	//nbjoueur++;
	printf("Le joueur %d viens de se connecté !\n",conn);
	printf("Il y a %d joueurs\n",nbjoueur);
}



/*
void topdepart()
{
	int temps=0;
	time_t begin=time(NULL);
	printf("debut compteur");
	while(temps<10) {


	sleep(1);

	temps=(int)difftime(end,begin);
	if(temps<10) {
		printf("%d",temps);

	}
	else {

		printf("temps ecoule");
		exit(1);
	}
}

}
*/
