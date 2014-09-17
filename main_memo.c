#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>

#define MAX_FACTORS 64
#define TABLEAU_PREMIERS 2000

static pthread_mutex_t mutexScanf;
static nombresFactorises = 0;

typedef struct nombreFactorise
{
	uint64_t nombre;
	uint64_t nombresPremiers[MAX_FACTORS];
} nombreFactorise;

typedef struct fichierAvecTabFactorises
{
	FILE* monFichier;
	nombreFactorise tabNombresFactorises[TABLEAU_PREMIERS];
} fichierAvecTabFactorises;



int get_prime_factors(uint64_t nombre, fichierAvecTabFactorises* structThread)
{
	uint64_t premier = 2;
	printf("%lu :", nombre);
	int taille = 0;
	int appartient;
	int indiceTableau;
	int i;

		
	while(premier <= sqrt(nombre))
	{
		appartient = -1;
	
		for(i=0; i<nombresFactorises ; i++)
		{
			if((structThread->tabNombresFactorises[i]).nombre == nombre)
			{
				appartient = 1;
				indiceTableau = i;
				break;
			}
		}
	
		if(appartient == 1)
		{
			for(i=0; i<64 && taille<64; i++)
			{
				(structThread->tabNombresFactorises[taille]).nombresPremiers[taille] = (structThread->tabNombresFactorises[indiceTableau]).nombresPremiers[indiceTableau];
				taille++;
			}
			
			return taille;
		}
		else
		{
			if(nombre%premier == 0)
			{
				printf(" %lu", premier);
				nombre /= premier;
				(structThread->tabNombresFactorises[taille]).nombresPremiers[taille] = premier;
				taille++;
			}
			else
			{
				premier++;
			}
		}
	}
	
	(structThread->tabNombresFactorises[taille]).nombresPremiers[taille] = nombre;
	
	return taille;
}

void print_prime_factors(uint64_t n, fichierAvecTabFactorises* structThread)
{
	uint64_t factors[MAX_FACTORS];
	int j,k;

	k = get_prime_factors(n, structThread);
	printf("%llu: ",n);
	
	for(j=0; j<k; j++)
	{
		printf("%llu ", (structThread->tabNombresFactorises[nombresFactorises]).nombresPremiers[j]);
	}

	printf("\n");
}

void* thread_prime_factors(void* fd)
{
	
	int combien_de_nombres_lus ;

	uint64_t nombre = 0;
	fichierAvecTabFactorises* fichierFacteurs = (fichierAvecTabFactorises*) fd;
	FILE* fichier = fichierFacteurs->monFichier;
	
	while(1)
	{
		pthread_mutex_lock(&mutexScanf);
		combien_de_nombres_lus = fscanf(fichier, "%lu", &nombre);
		pthread_mutex_unlock(&mutexScanf);
		
		if( combien_de_nombres_lus != 1)
		{
			break;
		}
		
		print_prime_factors(nombre, fichierFacteurs);
	}
	
	//dummy return
	return NULL;
}

int main(int argc, char* argv[])
{
	FILE* fichier = NULL;
	pthread_t tid1, tid2;
	//nombreFactorise tabNombresFactorises = (uint64_t *)calloc(1, sizeof(uint64_t ));
	
	fichierAvecTabFactorises* structThread;
	structThread = (fichierAvecTabFactorises*) calloc(TABLEAU_PREMIERS, sizeof(struct fichierAvecTabFactorises));
	
	char* filename;
	if(argc > 1)
	{
		filename=argv[1];
	}
	else
	{
		filename="nombresGeneres.txt";
	}
	
    fichier = fopen(filename, "r+");
	
	if(fichier != NULL)
	{
		structThread->monFichier = fichier;
		
		pthread_create(&tid1, NULL, thread_prime_factors, (void*)structThread);
		pthread_create(&tid2, NULL, thread_prime_factors, (void*)structThread);
		
		pthread_join(tid2, NULL);
		pthread_join(tid1, NULL);	
	}
	else
	{
		printf("Impossible d'ouvrir le fichier nombresPremiers.txt");
	}
	
	return 0;
}
