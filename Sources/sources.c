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

int infoForReading(csv*file, unsigned int headerLine, char seperator){
	if(!file->isPrepared)return 1;
	if(!ispunct(seperator)) return 2;
	if(headerLine <= 0 || headerLine > countLines(file->path)) return 3;
	if(file == NULL)return 4;
	
	file->headerLine = headerLine;
	file->seperator = seperator;
	return 0;
}

int getLine(csv*file, unsigned int lineNumber, char*saveto, bool keepNewLine){
	if(file == NULL) return 1;
	if(lineNumber <= 0 || lineNumber > countLines(file->path)) return 2;
	
	FILE* csv = fopen(file->path, "r");
	if(csv == NULL) return 3;
	
	rewind(csv);
	char currentChar;
	unsigned int currentLine = 1;
	
	while(currentLine != lineNumber){
		currentChar = getc(csv);
		if(currentChar == 10) ++currentLine;
	}
	
	unsigned int lineLength = 1;
	currentChar = getc(csv);
	while(currentChar != 10){		
		++lineLength;
		currentChar = getc(csv);
		if(feof(csv))break;
	}
	bool isLastLine = feof(csv);
	
	if(!isLastLine) fseek(csv, -(lineLength + 1), SEEK_CUR);
	else fseek(csv, -(lineLength - 1), SEEK_CUR);
	
	if(!keepNewLine) --lineLength;	
	char line[lineLength + 1];
	line[lineLength] = '\0';
	
	unsigned int index;
	for(index = 0; index < lineLength; ++index){
		currentChar = getc(csv);
		if(currentChar == 10 && !keepNewLine) continue;
		line[index] = currentChar;
	}
	strcpy(saveto, line);
	return 0;
}