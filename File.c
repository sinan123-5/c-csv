#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Boolean enum
typedef enum {false, true} bool;

// CSV File Structure
typedef struct csv {
	// File name and related data
	short unsigned int nameLen;
	char* csvFile;
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

// Function prototypes
// Utility functions
unsigned long long getLineLen(csv*file, unsigned int lineNumber);

// Functions to prepare the csv struct
int prepareCsv(csv* file, const char* name, const short unsigned int nameLen);
int preReading(csv* file, const char sep, unsigned int headline);
	// Functions used in/for preReading
	unsigned long long calculateSize(csv*file); // Calculates the file's size
	unsigned int countLines(csv* file); // Counts the number of lines in pointed file
/*preReading Err Codes
* 1 : CSV file not prepared. prepareCsv was never called with a pointer pointing at this functions csv struct
* 2 : File was not found. It either doesn't exist or another problem has occured
* 3 : Invalid seperator. Seperator can only be a punctuation character
*/

// Functions to read data from the CSV file, which is registered in the csv struct
int getLine(csv*file, unsigned int lineNum, void*saveto); // Extracts a singular line from the CSV file
/*getLine Error Codes
* 1 : Invalid line number. Line number is either 0 or greater than the number of lines in the CSV file
*/
int getHeaders(csv*file); // Extracts header data
int getLines(csv*file, unsigned int startLine, unsigned int endLine, void*saveto); // Extracts data between lines (inclusive)

int main(){
	csv myCsv;
	prepareCsv(&myCsv, "myData.csv", 10); // Sample csv file

	// Test log for preReading
	preReading(&myCsv, ';', 0);
	char str[200];
	
	getHeaders(&myCsv);
	getLine(&myCsv, 1, str);
	printf("%s\n", str);
	
	int i;
	for(i = 0; i < 200; ++i){
		str[i] = '\0';  
	}
	getLines(&myCsv, 2, 3 , str);
	printf("%s\n", str);
	printf("%s\n%lu\n", myCsv.headers, getLineLen(&myCsv, 2));
}

// Function to get a line's length
unsigned long long getLineLen(csv*file, unsigned int lineNum){
	if(lineNum > file->lineNum || !lineNum)return 1;

	FILE*data;
	data = fopen(file->csvFile, "r");

	unsigned int currentLine = 1;
	char read;

	// Loop until program gets to the start of the intended line (lineNum)
	while(currentLine < lineNum){
		read = getc(data);

		if(read == '\r' || read == '\n'){
			++currentLine;
		}
	}

	read = getc(data); // Read the first char of the reached line (lineNum-th line)
	unsigned long lineStart = ftell(data);
	 
	
	while(read != 10 && read != 13 && read != 0 && !feof(data)){
		read = getc(data);
	}
	unsigned long lineEnd = ftell(data);
	
	fclose(data);
	return (lineEnd - lineStart);
}

// Function to write the csv file's name into the pointed csv struct
int prepareCsv(csv* file, const char* name, const short unsigned int nameLen){
	file->nameLen = nameLen
	file->csvFile = calloc(nameLen, 1);

	short unsigned int i = 0;

	strcpy(file->csvFile, name);

	// Loop to null out any characters that may have occured at the end of the file name
	for(i = nameLen; i < sizeof(file); ++(i)){
		*(file->csvFile + i) = '\0';
	}

	// If the file name or name length registered in the csv struct is different, exit with an error code
	if(strcmp(file->csvFile, name) || !(file->nameLen == nameLen))return 1;

	file->isPrepared = true;
	return 0;
}

// Function to calculate the file's size
unsigned long long calculateSize(csv*file){
	unsigned short int start = 0;
	unsigned long long end = 0;
	unsigned long long size = 0;

	FILE*data;

	data = fopen(file->csvFile, "r");

	fseek(data, 0, SEEK_SET);
	start = ftell(data);

	fseek(data, 0, SEEK_END);
	end = ftell(data);

	size = end - start;

	size -= countLines(file);
	
	fclose(data);
	return size;
}

// Function to count the number of lines in file
unsigned int countLines(csv*file){
	 FILE*data;

	 data = fopen(file->csvFile, "r");
	 char read;
	 unsigned int lines = 0;

	 // Loop while end is not reached
	 while(!feof(data)){
		 read = getc(data);
		 if(read == 10 || read == 13) ++lines;
	 }
	 fclose(data);
	 return lines;
}

// Function to get csv file's length, seperator character and line of headers
int preReading(csv* file, const char sep, unsigned int headline){
	// If the csv struct is not prepared beforehand
	if(!file->isPrepared) return 1; // Exit with error code 1

	FILE *data;
	data = fopen(file->csvFile, "r");

	//Check if the file exists
	if(data == NULL) return 2; // Exit with error code 2 if the file doesn't

	// Counting lines of the file
	file->lineNum = countLines(file) + 1;

	// Setting file->size
	file->size = 	calculateSize(file);

	// Seperator validation
	if(!ispunct(sep))return 3; // If seperator is invalid, exits with error code 3
	file->seperator = sep;

	if(headline > file->lineNum	|| headline < 0)return 4; // If header line data is invalid, exit with error code 4
	file->headerLine = headline;

	file->readyToRead = true; // Register into the csv struct that file is ready to read
	return 0; // Function executed without an error
}

// Function to extract a line from the CSV file
int getLine(csv*file, unsigned int lineNum, void*saveto){
	if(lineNum > file->lineNum || !lineNum)return 1; // Exit with error code 1

	FILE*data;
	data = fopen(file->csvFile, "r");

	unsigned int currentLine = 1;
	char read;

	// Loop until program gets to the start of the intended line (lineNum)
	while(currentLine < lineNum){
		read = getc(data);

		if(read ==10 || read == 13){
			++currentLine;
		}
	}

	read = getc(data);
	unsigned int lineLen = getLineLen(file, lineNum);

	char outputString[lineLen + 1];
	unsigned int index = 0;
	unsigned int counter;
	for(counter = 0; counter < lineLen; counter++){
		read = getc(data);
		if(read == 10 ||read == 13){
			--counter;
			continue;
		}
		outputString[index] = read;
		++index;
	}
	fclose(data);
	outputString[lineLen] = '\0';

	memcpy(saveto, outputString, sizeof(outputString));
	return 0;
}

// Function to extract header data
int getHeaders(csv*file){	
	unsigned int headerLen = getLineLen(file, file->headerLine);
	char headerString[headerLen + 1];
	headerString[headerLen] = '\0';
	return getLine(file, file->headerLine, headerString);
}

// Function to extract data between lines (inclusive)
int getLines(csv*file, unsigned int startLine, unsigned int endLine, void*saveto){
	unsigned long long totalSize = 0;
	unsigned int index = 0;
	unsigned long maxLineLen = 0;
	unsigned long currentLen;
	
	// Loop for calculating the max line length and the total size
	for(index = startLine; index <= endLine; ++index){
		currentLen = getLineLen(file, index);
		totalSize += currentLen;
		if(maxLineLen < currentLen) maxLineLen = currentLen;
	}
	
	char outputString[totalSize + 1];
	outputString[totalSize] = '\0';
	char buffer[maxLineLen];
	unsigned int bufferCleaner;
	
	// Loop until the last line is read
	for(index = startLine; index <= endLine; ++index){
		getLine(file, index, buffer);
		strcat(outputString, buffer);
		strcat(outputString, "\r\n");
		
		// Clean the buffer
		for(bufferCleaner = 0; buffer[bufferCleaner] != 0; ++bufferCleaner)buffer[bufferCleaner] = '\0';
	}
	
	printf("%s\n", outputString); // (Debugging) Print the outputString
	memcpy(saveto, outputString, sizeof(outputString));

	return 0;
}