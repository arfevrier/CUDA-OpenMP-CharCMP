#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define DICT_WORD_SIZE 25
#define DICT_LENGHT 22740

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
__device__ int same_hash(HASH* one, HASH* two){
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

//Lets define the CUDA fonction
__global__ void gpu(HASH* shadow_tab, HASH* hash_tab, char* result){
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int stride = blockDim.x * gridDim.x;
	for (int i = x; i < DICT_LENGHT; i += stride){
		for (int j = y; j < DICT_LENGHT; j += stride){
			if(same_hash(&shadow_tab[i], &hash_tab[j])){
				result[i]=1;
			}
		}
	}
}


// The crack executable take the dict file and the hash in parameter
int main(int argc, char *argv[]) {
	char* dict_file = argv[1];
	char* sha_file = argv[2];
	
	//Store each line in an array
	TITLES* title_tab = (TITLES*)malloc(sizeof(TITLES));
	HASH* hash_tab; cudaMallocManaged(&hash_tab, sizeof(HASH)*DICT_LENGHT);
	char* result; cudaMallocManaged(&result, sizeof(char)*DICT_LENGHT); memset(result, 0, sizeof(char)*DICT_LENGHT);
	
	FILE * ds = openFile(dict_file);
	char *currline = readline(ds);
	int nbLine = 0;
	while (currline!=NULL)
	{
		char *tmp = strtok(currline, "\t");
		strcpy((*title_tab)[nbLine], tmp);
		tmp = strtok(NULL, "\t");
		memcpy(&hash_tab[nbLine], tmp, sizeof(HASH));
		//printf("%s-%s\n", title_tab+(nbLine*DICT_WORD_SIZE), hash_tab+(nbLine*DICT_HASH_SIZE));
		nbLine++;

		currline = readline(ds);
	}
	fclose(ds);
	
	//Store each hash to find in an array
	HASH* shadow_tab; cudaMallocManaged(&shadow_tab, sizeof(HASH)*DICT_LENGHT);
	ds = openFile(sha_file);
	currline = readline(ds);
	nbLine = 0;
	while (currline!=NULL)
	{
		memcpy(&shadow_tab[nbLine], currline, sizeof(HASH));	
		nbLine++;		

		currline = readline(ds);
	}

	//Start the GPU
	dim3 dimBlock(32, 32);
	dim3 dimGrid(32, 32);
	gpu<<<dimGrid, dimBlock>>>(shadow_tab, hash_tab, result);
	cudaDeviceSynchronize();

	//Print the final result
	for(int i=0;i<22740;i++){
		if(result[i]==1) printf("FINDED - %s\n", (*title_tab)[i]);
	}

	fclose(ds);	
    return 0;
}
