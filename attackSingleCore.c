#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <openssl/sha.h>
#include <string.h>

#define DICT_WORD_SIZE 25
#define DICT_HASH_SIZE 42

//Define un HASH = 40 caractères HEXA = 20 octets = 5*DOUBLE
typedef struct {
	double p1;
	double p2;
	double p3;
	double p4;
	double p5;
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
// ./crack dict.txt shadow.txt
int main(int argc, char *argv[]) {
	char* dict_file = argv[1];
	char* sha_file = argv[2];

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
		char *tmp = strtok(currline, "\t");
		strcpy(title_tab+(nbLine*DICT_WORD_SIZE), tmp);
		tmp = strtok(NULL, "\t");
		strcpy(hash_tab+(nbLine*DICT_HASH_SIZE), tmp);
		//printf("%s-%s\n", title_tab+(nbLine*DICT_WORD_SIZE), hash_tab+(nbLine*DICT_HASH_SIZE));
		nbLine++;

		currline = readline(ds);
	}
	fclose(ds);

	ds = openFile(sha_file);
	currline = readline(ds);
	while (currline!=NULL)
	{

		// For each line, check if the hash of the dict is the same
		int tmpnbLine = nbLine;
		while (tmpnbLine>0)
		{
			tmpnbLine--;
			
			if(strcmp(currline, hash_tab+(tmpnbLine*DICT_HASH_SIZE))==0){
				printf("FINDED - %s\n", title_tab+(tmpnbLine*DICT_WORD_SIZE));
			}
		}

		currline = readline(ds);
	}
	fclose(ds);

	
    return 0;
}
