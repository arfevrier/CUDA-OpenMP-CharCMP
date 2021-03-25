// my_sha_utils.c
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

// into the file, there will be 1/ratio*n fake shadows
int ratio = 5;
// chances to actually read that line
int CHANCE = 400;
int main(int argc, char *argv[]) {

    char * dict_file = argv[1];
    char * shasum_file = argv[2]; 
    char *p;
    int wanted_shadows = strtol(argv[3], &p, 10);
	//for fake password, using a little salt.
	char * salt="THIS IS A LITTLE SALT";
	

	// Creating output file
    FILE * fptr = fopen(shasum_file, "wb");
    if (fptr == NULL)
		exit(EXIT_FAILURE);
	
	
	int number_of_shadows = 0;
while (number_of_shadows < wanted_shadows)
{
	// opening file
	FILE * ds = fopen(dict_file, "r");
      	if (ds == NULL)
        exit(EXIT_FAILURE);
	char * currline = readline(ds);

	while (currline!=NULL && (number_of_shadows < wanted_shadows))
	{
	    if (rand() % CHANCE ==0)
{
		if (!(number_of_shadows % ratio))
		{ // just a little salt to add 
			char * concat = (char *) malloc(strlen(salt) + strlen(currline)+1);
			strcpy(concat, salt);
			strcat(concat,currline);
			currline = concat;
		}
		size_t length = strlen(currline);
		unsigned char hash[100];
		SHA1(currline, length, hash);
		
		//fprintf(fptr,"%s ",currline);
		int i;
		for(i = 0; i<SHA_DIGEST_LENGTH; i++){

 		   fprintf(fptr, "%x", hash[i]);
		}
		fprintf(fptr,"\n");
		number_of_shadows++;
}

		currline = readline(ds);
	}		

	fclose(ds);
}
	fclose(fptr);
    return 0;
}
