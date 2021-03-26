// my_sha_utils.c
// - Generate shasum from dictionnary
// - Generate random password to search for, including a fixed ratio of non-existing ones ...

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <openssl/sha.h>
#include <string.h>

char * readline(FILE * f)
{char * line = NULL;
 
    size_t len = 0;
	ssize_t read;
    if ((read = getline(&line, &len, f)) != -1) {
	line[read-2] = '\0';
	
      return line;
    }
	return NULL;
}

int main(int argc, char *argv[]) {
    char * dict_file = argv[1];
	char * shasum_file = argv[2];
	
	// opening file
	FILE * ds = fopen(dict_file, "r");
    if (ds == NULL)
        exit(EXIT_FAILURE);
	
	// Creating output file
	FILE * fptr = fopen(shasum_file, "wb");
    if (fptr == NULL)
		exit(EXIT_FAILURE);
	//inserting lines 
	char * currline = readline(ds);
	while (currline!=NULL)
	{
		size_t length = strlen(currline);
		unsigned char hash[100];
		SHA1(currline, length, hash);
		
		fprintf(fptr,"%s\t",currline);
		int i;
		for(i = 0; i<SHA_DIGEST_LENGTH; i++){

 		   fprintf(fptr, "%02hhx", hash[i]);
		}
		fprintf(fptr,"\n");
		currline = readline(ds);
	}

	fclose(fptr);
	fclose(ds);
    return 0;
}
