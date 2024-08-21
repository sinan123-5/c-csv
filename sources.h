#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Boolean enum
typedef enum {false, true} bool;

// Sources for testing
typedef struct csv {
	char* path;
	bool isPrepared;

	// Data about lines in CSV file
	unsigned int lineNum;
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

int prepareFile(csv*file, char*path);