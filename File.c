#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

// CSV File Structure
struct csv {
	// File name and related data
	short unsigned int nameLen;
	char* csvFile;
	bool isPrepared;
	
	// Data about lines in CSV file
	unsigned int maxLineLen;
	unsigned int lineNum;
	unsigned int headerLine;
	bool readyToRead;
	
	// Content
	char seperator;
	char* headers;
	
};

int prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen);

int preReading(struct csv* file, const char sep, char* heads);

int main(){
	struct csv myCsv;
	
	printf("%i\n", prepareCsv(&myCsv, "myData.csv", 10));
	printf("%i, %s\n", myCsv.nameLen, myCsv.csvFile);
	preReading(&myCsv, ';', "  ");
}

int prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen){
	file->nameLen = nameLen;
	file->csvFile = calloc(nameLen, 1);
	
	short unsigned int *i = malloc(sizeof(nameLen));
	*i = 0;
	
	strcpy(file->csvFile, name);
	
	for(*i = nameLen; *i < sizeof(file); ++(*i)){
		*(file->csvFile + *i) = '\0';
	}
	
	free(i);
	i = NULL;
	
	if(strcmp(file->csvFile, name) && (file->nameLen == nameLen))return 1;

	file->isPrepared = true;
	return 0;
}

int preReading(struct csv* file, const char sep, char* heads){
	if(!file->isPrepared){
		printf("Prepare file first.\nCall prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen)\n");
		return 1;
	}
	FILE *data;
	data = fopen(file->csvFile, "r");
	if(data == NULL){
		printf("File cannot be reached.\n");
		return 1;
	}
	printf("File opened succesfully!\n");
}