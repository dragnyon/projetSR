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


struct sockaddr_in serv;
int fd;
int conn;
char message[SIZE_BUFF] = "Attente d'autre joueurs\n";
char message1[SIZE_BUFF] = "La partie commence !\n";
int nbjoueur=0;
int joueur[NB_J];
int manchecourante=1;
int nbcoups=0;
char recoi [SIZE_BUFF]="";
char cartesjoueur1[NB_MANCHE];
char cartesjoueur2[NB_MANCHE];
char gagner[SIZE_BUFF]="g";
char perdu[SIZE_BUFF]="p";
char cartejoue[NB_J*NB_MANCHE];



void* ecoute1(void* jsp);
void* ecoute2(void* jsp);
void initcarte();
void affichecartes1();
void affichecartes2();
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
	
   
	while(nbjoueur<NB_J)
	{
		conn = accept(fd, (struct sockaddr *)NULL, NULL);
		joueur[nbjoueur]=conn;
		nbjoueur++;
		if(nbjoueur<NB_J)
		{
			send(conn, message, strlen(message), 0);
		}
		printf("Le joueur %d viens de se connecter !\n",conn);
		printf("Il y a %d joueurs\n",nbjoueur);
		printf("%d joueurs - %d %d \n",nbjoueur,joueur[0],joueur[1]);
   }
   initcarte();
   affichecartes1();
   affichecartes2();
 	printf("Manche : %d",manchecourante);
  
   pthread_t thread1;
	pthread_t thread2;
	
   pthread_create(&thread1, NULL, ecoute1, NULL);
 	pthread_create(&thread2, NULL, ecoute2, NULL);
 
	while(1)
	{
		if(nbcoups==(2*manchecourante))
		{
			if(verif()==0)
			{
				manchecourante++;
				for(int p=0;p<2;p++)
				{		
					send(joueur[p], gagner, strlen(gagner), 0);
				}
			}
			else 
			{
				for(int p=0;p<2;p++)
				{		
					send(joueur[p], perdu, strlen(perdu), 0);
				}
			}
			nbcoups=0;
			initcarte();
			affichecartes1();
   	   affichecartes2();
			memset(cartejoue,0,(NB_J*NB_MANCHE));	
		}
	}
}

void initcarte()
{
	srand(time(NULL));
	for(int j=0;j<NB_MANCHE;j++)
	{	
		cartesjoueur1[j]=rand()%100;	
		cartesjoueur2[j]=rand()%100;			
	}	
	cartesjoueur1[NB_MANCHE]='\0';	
	cartesjoueur2[NB_MANCHE]='\0';
	//trie();
}



void trie()
{
	int temp;
	for(int r=0;r<100;r++)
	{
		for(int i=0;i<NB_J;i++)
		{
			
				if(cartesjoueur1[i]>cartesjoueur1[i+1])
				{
					temp=cartesjoueur1[i];
					cartesjoueur1[i]=cartesjoueur1[i+1];
					cartesjoueur1[i+1]=temp;
				}
					
				if(cartesjoueur2[i]>cartesjoueur2[i+1])
				{
					temp=cartesjoueur2[i];
					cartesjoueur2[i]=cartesjoueur2[i+1];
					cartesjoueur2[i+1]=temp;
				}	
		}
	}


}





void affichecartes1()
{
		printf("\nCartes du joueur %d:",1);
		for(int j=0;j<NB_MANCHE;j++)
		{		
			printf("%d-",cartesjoueur1[j]);	
		}
}

void affichecartes2()
{
		printf("\nCartes du joueur %d:",2);
		for(int i=0;i<NB_MANCHE;i++)
		{	
			printf("%d-",cartesjoueur2[i]);	
		}
}


void joue(int j, int coup)
{
	int i =0;
	int t=0;
	if(j==0)
	{
		do 
		{
			if(cartesjoueur1[t]!=0)
			{
				printf("ca joue\n");
				cartejoue[coup]=cartesjoueur1[t];
				cartesjoueur1[t]=0;
				i=1;
			}
			else 
			{
			t++;
			}
		}
		while(i==0);
	}
	else
	{
		do 
		{
			if(cartesjoueur2[t]!=0)
			{
				printf("ca joue\n");
				cartejoue[coup]=cartesjoueur2[t];
				cartesjoueur2[t]=0;
				i=1;
			}
			else 
			{
			t++;
			}
		}
		while(i==0);
	}	
		printf("cartes jouées");	
		for(int q=0;q<10;q++)
		{
			if(cartejoue[q]!=0)
			{
				printf("%d-",cartejoue[q]);
			}
		}
}


int verif()
{
	int j=0;
	for(int i=0;i<(2*manchecourante)-1;i++)
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
	send(joueur[0], cartesjoueur1, manchecourante, 0);
	while(recv(joueur[0], recoi, SIZE_BUFF, 0)) 
	{
		printf("\nLe joueur %d a jouer \n",joueur[0]);
		joue(0, nbcoups);
		nbcoups++;
		affichecartes1();
		affichecartes2();
		send(joueur[0], cartesjoueur1, manchecourante, 0);
		send(joueur[1], cartesjoueur2, manchecourante, 0);
		bzero(recoi,SIZE_BUFF);
	}	
}


void* ecoute2(void* jsp)
{
	send(joueur[1], message1, strlen(message1), 0);			
	send(joueur[1], cartesjoueur2, manchecourante, 0);
	while(recv(joueur[1],recoi, SIZE_BUFF, 0)) 
	{
		printf("\nLe joueur %d a jouer \n",joueur[1]);
		joue(1, nbcoups);
		affichecartes1();
		affichecartes2();
		nbcoups++;
		send(joueur[0], cartesjoueur1, manchecourante, 0);
		send(joueur[1], cartesjoueur2, manchecourante, 0);
		bzero(recoi,SIZE_BUFF);
	}	
}










