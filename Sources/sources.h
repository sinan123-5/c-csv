#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Boolean enum
typedef enum {false, true} bool;

// Sources for testing
typedef struct csv {
	char* path;
	bool isPrepared;

	// Data about lines in CSV file
	unsigned int lineNumber;
	unsigned int headerLine;
	unsigned long long size;
	char seperator;
	bool readyToRead;

	// Headers
	short unsigned int headerNum;
	char* headers;
} csv;

int isReadable(char*path);
unsigned int countLines(char*path);
unsigned int getLineLength(char*path, unsigned int lineNumber, bool includeCarriageReturns);

int prepareFile(csv*file, char*path);
int infoForReading(csv*file, unsigned int headerLine, char seperator);

int getLine(csv*file, unsigned int lineNumber, char*saveto, bool keepNewLine);
int addLine(csv*file, unsigned int lineNumber, char*saveto, bool keepNewLine);

int storeLinesInString(csv*file, unsigned int startLine, unsigned int endLine, char*saveto, bool keepNewLines);