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
#define NB_MANCHE 10
#define SIZE_BUFF 50

int comIntern[NB_J][2];
struct sockaddr_in serv;
int fd;
int conn;
char message[SIZE_BUFF] = "\nAttente d'autre joueurs\n";
char message1[SIZE_BUFF] = "\nLa partie commence !\n";
int nbjoueur=0;
int joueur[NB_J];
int manchecourante=1;
int nbcoups=0;
char cartesjoueur[NB_J][NB_MANCHE];
char cartejoue [NB_J*NB_MANCHE];
char gagner[SIZE_BUFF]="Manche gagné";
char perdu[SIZE_BUFF]="Manche perdu";

void* ecoute1(void* jsp);
void* ecoute2(void* jsp);
void initcarte();
void affichecartes();
char* cartedejoueur(char* infoj,int joueur,int manchec);
void joue(int j, int coup);//joue la premiere carte du tableau du joueur qui joue
int verif();//verifie si la manche est perdu ou gagné
void trie();//trie les carte de chaque joueur dans l'ordre croissant

void main(int argc, char* argv[])
{

	printf("Démarrage du jeu...\n");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(strtol(argv[1], NULL, 10));
	serv.sin_addr.s_addr = INADDR_ANY;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(fd, (struct sockaddr *)&serv, sizeof(serv));
	listen(fd,5);
	initcarte();
   
	while(nbjoueur<NB_J)
	{
		conn = accept(fd, (struct sockaddr *)NULL, NULL);
		joueur[nbjoueur]=conn;
		cartesjoueur[nbjoueur][0]=conn;
		nbjoueur++;
		if(nbjoueur<NB_J)
		{
			send(conn, message, strlen(message), 0);
		}
		printf("Le joueur %d viens de se connecter !\n",conn);
		printf("Il y a %d joueurs\n",nbjoueur);
		printf("%d joueurs - %d %d %d",nbjoueur,joueur[0],joueur[1]);
   }
   
   affichecartes();
   pthread_t thread1;
	pthread_t thread2;
	
   pthread_create(&thread1, NULL, ecoute1, (void*)&joueur[0]);
 	pthread_create(&thread2, NULL, ecoute2, (void*)&joueur[1]);
	while(1)
	{
		if(nbcoups==(NB_J*manchecourante))
		{
			if(verif()==0)
			{
				manchecourante++;
				for(int p=0;p<2;p++)
				{	
					//send(joueur[p], cartejoue, SIZE_BUFF, 0);
					send(joueur[p], gagner, strlen(gagner), 0);
				}
			}
			else 
			{
				for(int p=0;p<2;p++)
				{	
					//send(joueur[p], cartejoue, SIZE_BUFF, 0);
					send(joueur[p], perdu, strlen(perdu), 0);
				}
			}
			nbcoups=0;
			initcarte();
			affichecartes();
			bzero(cartejoue,(NB_J*NB_MANCHE));
		}
	};
   
	
	
		
	
}

void initcarte()
{
	srand(time(NULL));
	for(int i=0;i<NB_J;i++)
	{
		for(int j=1;j<=NB_MANCHE;j++)
		{	
			cartesjoueur[i][j]=rand()%100;	
		}	
	}
	//trie();
}

void trie()
{
	int temp;
	for(int r=0;r<100;r++)
	{
		for(int i=0;i<NB_J;i++)
		{
			for(int j=1;j<NB_MANCHE;j++)
			{
				if(cartesjoueur[i][j]>cartesjoueur[i][j+1])
				{
					temp=cartesjoueur[i][j];
					cartesjoueur[i][j]=cartesjoueur[i][j+1];
					cartesjoueur[i][j+1]=temp;
				}
			}		
		}
	}


}





void affichecartes()
{
	for(int i=0;i<NB_J;i++)
	{
		printf("\nCartes du joueur %d:",cartesjoueur[i][0]);
		for(int j=1;j<NB_MANCHE;j++)
		{
			
			printf("%d  ",cartesjoueur[i][j]);	
		}
		printf("\n");	
	}
} 


//renvoi dans un tableau les cartes du joueur d'id passé en paramètre 
char* cartedejoueur(char* infoj,int joueur, int manchec)
{
	
	for(int i=1;i<=manchec+1;i++)
		snprintf(infoj,SIZE_BUFF,"%s-%d",infoj,cartesjoueur[joueur][i]);
	return infoj;
}

void joue(int j, int coup)
{
	int i =0;
	int t=1;
	do 
	{
		
		if(cartesjoueur[j][t]!=0)
		{
			printf("ca joue\n");
			cartejoue[coup]=cartesjoueur[j][t];
			cartesjoueur[j][t]=0;
			i=1;
		}
		else 
		{
			t++;
		}
	}
	while(i==0);
	printf("cartes jouées");	
	for(int q=0;q<10;q++)
	{
		if(cartejoue!=0)
		{
			printf("%d-",cartejoue[q]);
		}
	}
	
}


int verif()
{
	int j=0;
	for(int i=0;i<(NB_J*manchecourante);i++)
	{
		if(cartejoue[i+1]<cartejoue[i])
		{
			j++;
		}
	}
	return j;

}

void* ecoute1(void* jsp)
{

		
			send(joueur[0], message1, strlen(message1), 0);			
			char infoj[SIZE_BUFF]="";
			send(joueur[0], cartedejoueur(infoj,joueur[0],manchecourante), SIZE_BUFF, 0);
			while(recv(joueur[0], message, SIZE_BUFF, 0)) 
			{
				printf("\nLe joueur %d a jouer \n",joueur[0]);
				joue(0, nbcoups);
				nbcoups++;
				affichecartes();
				for(int i=0;i<2;i++)
				{	
					send(joueur[i], cartedejoueur(infoj,i,manchecourante), SIZE_BUFF, 0);
					//send(joueur[i], cartejoue, SIZE_BUFF, 0);
				}
		
			}	
}


void* ecoute2(void* jsp)
{

		
			send(joueur[1], message1, strlen(message1), 0);			
			char infoj[SIZE_BUFF]="";
			send(joueur[1], cartedejoueur(infoj,joueur[1],manchecourante), SIZE_BUFF, 0);
			while(recv(joueur[1],message, SIZE_BUFF, 0)) 
			{
				printf("\nLe joueur %d a jouer \n",joueur[1]);
				joue(1, nbcoups);
				affichecartes();
				nbcoups++;
				for(int i=0;i<2;i++)
				{	
					send(joueur[i], cartedejoueur(infoj,i,manchecourante), SIZE_BUFF, 0);
					//send(joueur[i], cartejoue, SIZE_BUFF, 0);
				}
						
			}	
}










