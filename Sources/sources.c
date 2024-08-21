#include"sources.h"

int isReadable(char*path){
	FILE*file;
	file = fopen(path, "r");
	int readable = !(file == NULL);
	fclose(file);
	return readable;
}

unsigned int countLines(char*path){
	if(!isReadable(path))return 0;
	
	FILE*file;
	file = fopen(path, "r");
	
	unsigned int lines = 1;
	char currentChar;
	
	while(!feof(file)){
		currentChar = getc(file);
		if(currentChar == 10)++lines;
	}
	return lines;
}

int prepareFile(csv*file, char*path){
	if(!isReadable(path)) return 1;
	
	file->path = calloc(strlen(path) + 1, 1);
	*(file->path + strlen(path)) = '\0';
	strcpy(file->path, path);
	file->isPrepared = true;
	return 0;
}