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
	fclose(file);
	return lines;
}

unsigned int getLineLength(char*path, unsigned int lineNumber, bool includeCarriageReturns){
	if(lineNumber <= 0 || lineNumber > countLines(path)) return 0;
	
	FILE*file = fopen(path, "r");
	if(NULL == file) return 0;
	
	unsigned int currentLine = 1;
	char currentChar;
	
	while(currentLine < lineNumber){
		currentChar = getc(file);
		if(currentChar == 10) ++currentLine;		
	}
	
	unsigned int lineLength = 1;
	currentChar = getc(file);
	
	while(currentChar != 10 && !feof(file)){
		currentChar = getc(file);
		++lineLength;
	}
	if(!includeCarriageReturns)--lineLength;
	fclose(file);
	return lineLength;
}

int prepareFile(csv*file, char*path){
	if(!isReadable(path)) return 1;
	
	file->path = calloc(strlen(path) + 1, 1);
	*(file->path + strlen(path)) = '\0';
	strcpy(file->path, path);
	file->lineNumber = countLines(file->path);
	file->isPrepared = true;
	return 0;
}

int infoForReading(csv*file, unsigned int headerLine, char seperator){
	if(!file->isPrepared)return 1;
	if(!ispunct(seperator)) return 2;
	if(headerLine <= 0 || headerLine > file->lineNumber) return 3;
	if(file == NULL)return 4;
	
	file->headerLine = headerLine;
	file->seperator = seperator;
	file->readyToRead = true;
	return 0;
}

int getLine(csv*file, unsigned int lineNumber, char*saveto, bool keepNewLine){
	if(file == NULL) return 1;
	if(lineNumber <= 0 || lineNumber > file->lineNumber) return 2;
	
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
	fclose(csv);
	strcpy(saveto, line);
	return 0;
}

int addLine(csv*file, unsigned int lineNumber, char*saveto, bool keepNewLine){
	if(file == NULL) return 1;
	if(lineNumber <= 0 || lineNumber > file->lineNumber) return 2;
	
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
	fclose(csv);
	strcat(saveto, line);
	return 0;
}

int storeLinesInString(csv*file, unsigned int startLine, unsigned int endLine, char*saveto, bool keepNewLines){
	unsigned long totalLength = 0;
	unsigned int currentLine;
	for(currentLine = startLine; currentLine <= endLine; ++currentLine){
		totalLength += getLineLength(file->path, currentLine, true);
	}
	
	char output[totalLength + 1];
	memset(output, 0, sizeof(output));
	
	for(currentLine = startLine; currentLine <= endLine; ++currentLine){
		addLine(file, currentLine, output, keepNewLines);
	}
	realloc(output, 1);
	output[totalLength + 1] = '\0';
	printf("%s", output);
}