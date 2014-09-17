#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>

void* print_prime_factors(void* fd)
{
	uint64_t premier = 2;
	uint64_t *nombreRecu = (uint64_t*) fd;
	uint64_t nombre = *nombreRecu;
		
	printf("%lu :", nombre);
			
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

int main(int argc, char* argv[])
{
	
	FILE* fichier = NULL;
	pthread_t tid1, tid2;
	int combien_de_nombres_lus ;
	uint64_t nombre = 0;
	
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
	
	if( fichier == NULL)
	{
		printf("Impossible d'ouvrir le fichier %s",filename);
		exit(-1);
	}	
	
	while(1)
	{
		combien_de_nombres_lus = fscanf(fichier, "%lu", &nombre);
				
		if( combien_de_nombres_lus != 1)
		{
			break;
		}

		pthread_create(&tid1, NULL, print_prime_factors, &nombre);
				
		combien_de_nombres_lus = fscanf(fichier, "%lu", &nombre);
				
		if( combien_de_nombres_lus != 1)
		{
			break;
		}

		pthread_create(&tid2, NULL, print_prime_factors, &nombre);
		pthread_join(tid2, NULL);
		pthread_join(tid1, NULL);	

	}
	return 0;
}

