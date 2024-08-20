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

// Boolean enum
typedef enum {false, true} bool;
