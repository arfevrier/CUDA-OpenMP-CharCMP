#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream> 
#include <set>
#include <map>

#define DICT_WORD_SIZE 25
#define DICT_LENGHT 22740

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
	
	//Store each line in an array
	std::map<std::string, std::string> allValue;
	
	FILE * ds = openFile(dict_file);
	char *currline = readline(ds);
	while (currline!=NULL)
	{
		char *tmp = strtok(currline, "\t");
		std::string title(tmp);
		tmp = strtok(NULL, "\t");
		std::string hash(tmp);
		allValue.insert({hash, title});
		//printf("%s-%s\n", element.title, tmp);
		//std::cout << title << "\n";

		currline = readline(ds);
	}
	fclose(ds);

	ds = openFile(sha_file);
	currline = readline(ds);
	while (currline!=NULL)
	{
		std::map<std::string, std::string>::iterator it = allValue.find(std::string(currline));
		if (it != allValue.end()){
			std::string title = it->second;
			std::cout << "Finded: " << title << "\n";  
		}			

		currline = readline(ds);
	}
	fclose(ds);
	
    return 0;
}
