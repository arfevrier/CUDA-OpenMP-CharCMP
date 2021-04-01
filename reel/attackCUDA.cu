#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define DICT_WORD_SIZE 25
#define DICT_LENGHT 22740
#define MAX_READ_SIZE 10000
#define NBCUDA 3

typedef char TITLES[MAX_READ_SIZE][DICT_WORD_SIZE];

//Define un HASH = 40 caractères = 5*DOUBLE de 8 octets
typedef struct {
	long long p1;
	long long p2;
	long long p3;
	long long p4;
	long long p5;
} HASH;

//Check if two HASH is the same
__device__ bool same_hash(HASH* one, HASH* two){
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
__global__ void gpu(HASH* sha_tab, HASH* hash_tab, char* result){
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int stride = blockDim.x * gridDim.x;
	for (int i = x; i < DICT_LENGHT; i += stride){
		for (int j = y; j < MAX_READ_SIZE; j += stride){
			if(same_hash(&sha_tab[i], &hash_tab[j])){
				result[j]=1;
			}
		}
	}
}


// The crack executable take the dict file and the hash in parameter
int main(int argc, char *argv[]) {
	char* dict_file = argv[1];
	char* sha_file = argv[2];

	HASH* sha_tab; cudaMallocManaged(&sha_tab, sizeof(HASH)*DICT_LENGHT);
	FILE* ds = openFile(sha_file);
	char* currline = readline(ds);
	int nbLine = 0;
	while (currline!=NULL)
	{
		memcpy(&sha_tab[nbLine], currline, sizeof(HASH));
		nbLine++;

		currline = readline(ds);
	}
	fclose(ds);
	

	ds = openFile(dict_file);	
	for(int i=0;i<52;i++){
		//Store each line in an array
		TITLES* title_tab = (TITLES*)malloc(sizeof(TITLES));
		HASH* hash_tab; cudaMallocManaged(&hash_tab, sizeof(HASH)*MAX_READ_SIZE);
		char* result; cudaMallocManaged(&result, sizeof(char)*MAX_READ_SIZE); memset(result, 0, sizeof(char)*MAX_READ_SIZE);
		for(int j=0;j<MAX_READ_SIZE;j++){
			currline = readline(ds);
			char *tmp = strtok(currline, "\t");
			strcpy((*title_tab)[j], tmp);
			tmp = strtok(NULL, "\t");
			memcpy(&hash_tab[j], tmp, sizeof(HASH));
		}

		//Start the GPU
		dim3 dimBlock(NBCUDA, NBCUDA);
		dim3 dimGrid(NBCUDA, NBCUDA);
		gpu<<<dimGrid, dimBlock>>>(sha_tab, hash_tab, result);
		cudaDeviceSynchronize();

		//Print the final result
		for(int i=0;i<MAX_READ_SIZE;i++){
			if(result[i]==1) printf("FINDED - %s\n", (*title_tab)[i]);
		}

		cudaFree(title_tab);
		cudaFree(hash_tab);
	}

	fclose(ds);	
    return 0;
}
