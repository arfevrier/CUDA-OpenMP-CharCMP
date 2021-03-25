#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <openssl/sha.h>
#include <string.h>

#define DICT_WORD_SIZE 25
#define DICT_HASH_SIZE 42

//Define a DWORD = INT = 4 octets 
typedef unsigned int dword;

//Define un HASH = 40 caractères HEXA = 20 octets = 5*DWORD
typedef struct {
   dword part[5];
} HASH;

//Check if two HASH is the same
int same_hash(HASH* one, HASH* two){
	return memcmp(one, two, sizeof(HASH))==0;
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
// ./crack dict.txt 3b1fe340dba76bf37270abad774f327f50b5e1d8
int main(int argc, char *argv[]) {
	char* dict_file = argv[1];
	char* sha_hash = argv[2];

	// Convert the hash to HASH type
	//for (size_t count = 0; count < sizeof val/sizeof *val; count++) {
	//	sscanf(sha_hash, "%2hhx", &val[count]);
	//	SHAtoFind += 2;
	//}
	
	//Store each line in an array
	char* title_tab = malloc(sizeof(char)*DICT_WORD_SIZE*22740);
	char* hash_tab = malloc(sizeof(char)*DICT_HASH_SIZE*22740);
	
	FILE * ds = openFile(dict_file);
	char *currline = readline(ds);
	int nbLine = 0;
	while (currline!=NULL)
	{
		char *tmp = strtok(currline, " ");
		strcpy(title_tab+(nbLine*DICT_WORD_SIZE), tmp);
		tmp = strtok(NULL, " ");
		strcpy(hash_tab+(nbLine*DICT_HASH_SIZE), tmp);
		//printf("%s-%s\n", title_tab+(nbLine*DICT_WORD_SIZE), hash_tab+(nbLine*DICT_HASH_SIZE));
		nbLine++;

		currline = readline(ds);
	}
	fclose(ds);

	// For each line, check if the hash of the dict is the same
	while (nbLine>0)
	{
		nbLine--;

		if(strcmp(sha_hash, hash_tab+(nbLine*DICT_HASH_SIZE))==0){
			printf("FINDED - %s\n", title_tab+(nbLine*DICT_WORD_SIZE));
		}
	}
    return 0;
}
