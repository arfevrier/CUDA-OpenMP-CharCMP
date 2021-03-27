#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <omp.h>

#define DICT_WORD_SIZE 25
#define DICT_LENGHT 22740
#define NUM_THREADS 3

typedef char TITLES[DICT_LENGHT][DICT_WORD_SIZE];

//Define un HASH = 40 caractères = 5*DOUBLE de 8 octets
typedef struct {
	double p1;
	double p2;
	double p3;
	double p4;
	double p5;
} HASH;

//Check if two HASH is the same
int same_hash(HASH* one, HASH* two){
	return one->p1==two->p1 &&
	       one->p2==two->p2 &&
	       one->p3==two->p3 &&
	       one->p4==two->p4 &&
	       one->p5==two->p5;
}

// Two fonctions which help manage files
// --------
FILE* openFile(char* url){
	FILE * ds = fopen(url, "r");
	if (ds == NULL){
		exit(EXIT_FAILURE);
	}
	return ds;
}

char * line = NULL;
size_t len = 0;
char * readline(FILE * f){
    ssize_t read;
    if ((read = getline(&line, &len, f)) != -1) {
	line[read-1] = '\0';
      return line;
    }
	return NULL;
}
// --------


// The crack executable take the dict file and the hash in parameter
// ./crack dict.txt shadow.txt
int main(int argc, char *argv[]) {
	char* dict_file = argv[1];
	char* sha_file = argv[2];
	char* result = malloc(sizeof(char)*DICT_LENGHT);
	int i = 0;

	// Convert the hash to HASH type
	//for (size_t count = 0; count < sizeof val/sizeof *val; count++) {
	//	sscanf(sha_hash, "%2hhx", &val[count]);
	//	SHAtoFind += 2;
	//}
	
	//Store each line in an array
	TITLES* title_tab = malloc(sizeof(TITLES));
	HASH* hash_tab = malloc(sizeof(HASH)*22740);
	
	FILE * ds = openFile(dict_file);
	char *currline = readline(ds);
	int nbLine = 0;
	while (currline!=NULL)
	{
		char *tmp = strtok(currline, "\t");
		strcpy((*title_tab)[nbLine], tmp);
		tmp = strtok(NULL, "\t");
		memcpy(&hash_tab[nbLine], tmp, sizeof(HASH));
		// printf("%s-%s\n", title_tab+(nbLine*DICT_WORD_SIZE), hash_tab+(nbLine*DICT_HASH_SIZE));
		nbLine++;

		currline = readline(ds);
	}
	fclose(ds);


	ds = openFile(sha_file);
	
	// ----- OpenMP -----
	omp_set_num_threads(NUM_THREADS);
	#pragma omp parallel for default(none) private(i, currline) shared(hash_tab, nbLine, ds, title_tab, result)
	for(i=0; i<DICT_LENGHT; i++){
		#pragma omp critical
		{
			currline = readline(ds);
		}

		if(currline!=NULL){
			// printf("Element %s traité par le thread %d \n",currline,omp_get_thread_num());
			// For each line, check if the hash of the dict is the same
			int tmpnbLine = nbLine;

			//#pragma omp critical // With this critical zone we get 22739 results
			//{
			for(tmpnbLine = nbLine; tmpnbLine>0; tmpnbLine--)
			{
				if(same_hash((HASH*)currline, &hash_tab[tmpnbLine])){
					result[i]=1;
				}
			}
			//}

		}
	}
	// ------------------
	//Print the final result
	for(int i=0;i<22740;i++){
		if(result[i]==1) printf("FINDED - %s\n", (*title_tab)[i]);
	}
	
	fclose(ds);
	printf("Number of Threads : %i", NUM_THREADS);
	
    return 0;
}
