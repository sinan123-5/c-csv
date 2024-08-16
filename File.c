#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

// CSV File Structure
typedef struct csv {
	// File name and related data
	short unsigned int nameLen; // File name's length
	char* csvFile; // File's name
	bool isPrepared; // Shows whether prepareCsv() executed succesfully or not
	
	// Data about lines in CSV file
	unsigned int lineNum; // Number of lines in the file
	unsigned int headerLine; // Line which contains the headers
	unsigned long long size; // File size, the amount of characters it has
	char seperator; // Seperator char, used to seperate one header's data from others
	bool readyToRead; // Shows whether preReading() executed succesfully or not
	
	// Content
	short unsigned int headerNum; // Number of headers
	char* headers; // Header data in the format of "h1 sep h2 sep ... sep hn"
} csv;

// Function prototypes
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
char** getLines(csv*file, unsigned int startLine, unsigned int endLine); // Extracts data between lines (inclusive)

int main(){
	csv myCsv; // Sample Structure
	prepareCsv(&myCsv, "myData.csv", 10); // Sample csv file
	
	// Test log for preReading
	preReading(&myCsv, ';', 0);
	char str[200];
	
	unsigned int i;
	for(i = 160; i <= 164; ++i){
	getLine(&myCsv, i, str);
	printf("Line %i : %s\n", i, str);
	}
}

// Function to write the csv file's name into the pointed csv struct
int prepareCsv(csv* file, const char* name, const short unsigned int nameLen){
	file->nameLen = nameLen; // Register the file name's length to the struct, so as to alloc enough memory
	file->csvFile = calloc(nameLen, 1); // Allocate memory for the file name
	
	short unsigned int i = 0; // Create an i value to be used in the for loop
	
	strcpy(file->csvFile, name); // Copy the name (parameter) into the file name in the csv struct

	// Loop to null out any characters that may have occured at the end of the file name
	for(i = nameLen; i < sizeof(file); ++(i)){
		*(file->csvFile + i) = '\0';
	}

	// If the file name or name length registered in the csv struct is different, exit with an error code
	if(strcmp(file->csvFile, name) || !(file->nameLen == nameLen))return 1;

	file->isPrepared = true; // Register to the struct that the file is prepared
	return 0; // Return without error
}

// Function to calculate the file's size
unsigned long long calculateSize(csv*file){
	unsigned short int start = 0; // Points the start of the file
	unsigned long long end = 0; // Points the end of the file
	unsigned long long size = 0; // Stores the size of the file
	
	FILE*data;
	
	data = fopen(file->csvFile, "r");
	
	fseek(data, 0, SEEK_SET); // Set file position indicator to the start of the file
	start = ftell(data); // Save file's start pos
	
	fseek(data, 0, SEEK_END); // Set file position indicator to the end of the file
	end = ftell(data); // Save file's end pos
	
	size = end - start; // Substrack the value of start from the value of end to get the size
	
	size -= countLines(file); // New lines count as two bytes, 1 byte is substracted for each one to get the true size
	
	return size; // Return the file's size
}

// Function to count the number of lines in file
unsigned int countLines(csv*file){
	 FILE*data; // File pointer
	
	 data = fopen(file->csvFile, "r"); // Open registered file
	 char read; // File reader to count new line characters, allocated 1 byte
	 unsigned int lines = 0; // Variable to increment when a new line is read
	 
	 // Loop while end is not reached
	 while(!feof(data)){
		 read = getc(data); // Read a character from data
		 if(read == 10 || read == 13) ++lines; // Increment the value of lines if character is a line feed or carriage return
	 }
	 fclose(data); // Close the open file
	 return lines; // Return the amount of counted lines 
}

// Function to get csv file's length, seperator character and line of headers
int preReading(csv* file, const char sep, unsigned int headline){
	// If the csv struct is not prepared beforehand
	if(!file->isPrepared) return 1; // Exit with error code 1
	
	FILE *data;
	data = fopen(file->csvFile, "r"); // Open the file registered in the csv struct
	
	//Check if the file exists
	if(data == NULL) return 2; // Exit with error code 2 if the file doesn't 
	
	// Counting lines of the file
	file->lineNum = countLines(file) + 1;
	
	// Setting file->size
	file->size = 	calculateSize(file); // Register the file's size into the csv struct	
	
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
	if(lineNum > countLines(file) + 1 || !lineNum)return 1; // Exit with error code 1
	
	FILE*data;
	data = fopen(file->csvFile, "r"); // Open Registered File
	
	unsigned int currentLine = 1; // Variable to count lines
	char read; // Variable to store chars read from the file
	
	// Loop untill program gets to the start of the intended line (lineNum)
	while(currentLine < lineNum){
		read = getc(data); // 1 char read from the file
		
		if(read ==10 || read == 13){ 
			++currentLine; // If the char is a new line or a carriage return, increment the currentLine
		}
	}
	
	read = getc(data); // Read the first char of the reached line (lineNum-th line)
	unsigned int lineLen = 0; // Variable to store this line's length
	
	// Loop untill the end of line or file is reached
	while(read != 10 && read != 13 && !feof(data)){
		read = getc(data); // Continue reading from the file char by char
		++lineLen; // Increment lineLen for every character read
	}
	if(feof(data)){
		fseek(data, -1, SEEK_CUR);
		read = getc(data);
		++lineLen;
	}
	fseek(data, -1 * lineLen - 2, SEEK_CUR); // Set the file position indicator at the start of the line
	
	char outputString[lineLen + 1]; // Create a String with the lenght of lineLen
	unsigned int index; // Variable for the loop below
	for(index = 0; index < lineLen; index++){
		outputString[index] = getc(data); // Write chars read from the file into the string
	}
	outputString[lineLen] = '\0'; // Null character at the end to tell the program the string is finished
	
	memcpy(saveto, outputString, sizeof(outputString)); // Copy the outputString into intended destination
	
	return 0;
}