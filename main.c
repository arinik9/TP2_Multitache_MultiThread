#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>


static pthread_mutex_t mutexScanf;

void* print_prime_factors(void* fd)
{
	
	int combien_de_nombres_lus ;

	uint64_t premier;
	uint64_t nombre = 0;
	FILE* fichier = (FILE*) fd;
	
	while(1)
	{
		pthread_mutex_lock(&mutexScanf);
		combien_de_nombres_lus = fscanf(fichier, "%lu", &nombre);
		pthread_mutex_unlock(&mutexScanf);
		
		if( combien_de_nombres_lus != 1)
		{
			break;
		}
		

		printf("%lu :", nombre);
		
		premier=2;
		while(premier <= sqrt(nombre))
		{
			if(nombre%premier == 0)
			{
				printf(" %lu", premier);
				nombre /= premier;
			}
			else
			{
				premier++;
			}
		}
		
		printf(" %lu", nombre);
		printf("\n");
	}
	
	//dummy return
	return NULL;
}

int main(int argc, char* argv[])
{
	
	FILE* fichier = NULL;
	pthread_t tid1, tid2;
	
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
		pthread_create(&tid1, NULL, print_prime_factors, fichier);
		pthread_create(&tid2, NULL, print_prime_factors, fichier);
		
		pthread_join(tid2, NULL);
		pthread_join(tid1, NULL);	
	}
	else
	{
		printf("Impossible d'ouvrir le fichier nombresPremiers.txt");
	}
	
	return 0;
}
