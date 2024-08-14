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
	unsigned int size;
	char seperator;
	bool readyToRead;
	
	// Content
	char* headers;
	char* lines;
	
};

// Function prototypes
// Functions to prepare the csv struct
int prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen);
int preReading(struct csv* file, const char sep, unsigned int headline);
	// Functions used in/for preReading
	unsigned int calculateSize(FILE*data); // Calculates the file's size
	bool isValidSep(const char sep); // Checks if the intended seperator is valid to use
/*preReading Err Codes
* 1 : CSV file not prepared. prepareCsv was never called with a pointer pointing at this functions csv struct
* 2 : File was not found. It either doesn't exist or another problem has occured
* 3 : Invalid seperator. Seperator can't be a number or a letter.
*/

int main(){
	struct csv myCsv; // Sample Structure
	
	printf("%i\n", prepareCsv(&myCsv, "myData.csv", 10)); // Sample csv file
	printf("%i, %s\n", myCsv.nameLen, myCsv.csvFile); // Test log if changes are applied
	
	// Test log for preReading
	preReading(&myCsv, ';', 0);
	printf("This CSV file is %lu bytes/characters long!\n", myCsv.size);
}

// Function to write the csv file's name into the pointed csv struct
int prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen){
	file->nameLen = nameLen; // Register the file name's length to the struct, so as to alloc enough memory
	file->csvFile = calloc(nameLen, 1); // Allocate memory for the file name
	
	short unsigned int *i = malloc(2);
	*i = 0; // Create an i value to be used in the for loop
	
	strcpy(file->csvFile, name); // Copy the name (parameter) into the file name in the csv struct

	// Loop to null out any characters that may have occured at the end of the file name
	for(*i = nameLen; *i < sizeof(file); ++(*i)){
		*(file->csvFile + *i) = '\0';
	}
	
	free(i); // Dealloc i
	i = NULL; // Null out i to prevent accidental usage of it again
	
	// If the file name or name length registered in the csv struct is different, exit with an error code
	if(strcmp(file->csvFile, name) || !(file->nameLen == nameLen))return 1;

	file->isPrepared = true; // Register to the struct that the file is prepared
	return 0; // Return without error
}

// Function to calculate the file's size
unsigned int calculateSize(FILE*data){
	unsigned short int* start = malloc(2); // Points the start of the file
	unsigned int* end = malloc(4); // Points the end of the file
	unsigned int* size = malloc(4); // Stores the size of the file
	
	fseek(data, 0, SEEK_SET); // Set file position indicator to the start of the file
	*start = ftell(data); // Save file's start pos
	
	fseek(data, 0, SEEK_END); // Set file position indicator to the end of the file
	*end = ftell(data); // Save file's end pos
	
	*size = *(end) - *(start); // Substrack the value of start from the value of end to get the size
	
	free(start); // Deallocate start
	start = NULL; // Null out start to prevent accidental usage
	
	free(end); // Deallocate end
	end = NULL; // Null out end to prevent accidental usage
	
	return *size; // Return the file's size
}

// Function to check if the intended seperator is valid to use
bool isValidSep(const char sep){
	if(sep <= 32)return false; // Invalid if sep is an unpressable char
	if(sep >= 48 && sep <= 57) return false; // Invalid if sep is a number
	if(sep >= 65 && sep <= 90)return false; // Invalid if sep is a capital letter
	if(sep >= 97 && sep <= 122)return false; // Invalid if sep is a non-capital letter
	return true;
}

// Function to get csv file's length, seperator character and line of headers
int preReading(struct csv* file, const char sep, unsigned int headline){
	// If the csv struct is not prepared beforehand
	if(!file->isPrepared){
		// Print message;
		printf("Prepare file first.");
		printf("Call prepareCsv(struct csv* file, const char* name, const short unsigned int nameLen)\n");
		
		return 1; // Exit with error code 1
	}
	FILE *data;
	data = fopen(file->csvFile, "r"); // Open the file registered in the csv struct
	
	//Check if the file exists
	if(data == NULL) return 2; // Exit with error code 2 if the file doesn't exist
	
	// Setting file->size
	file->size = calculateSize(data); // Register the file's size into the csv struct	
	
	file->readyToRead = true; // Register into the csv struct that file is ready to read
	
	// Seperator validation
	if(!isValidSep(sep))return 3; // If seperator is invalid, exits with error code 3
	file->seperator = sep;
	
	return 0;
}
