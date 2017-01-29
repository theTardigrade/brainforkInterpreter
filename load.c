#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "bfi.h"
#include "err.h"

char *loadFile(const char *filename)
{
	struct stat fileStat;
	char *contents;
	FILE *fp;

	if ( stat(filename, &fileStat) < 0 )
		errExit(errno, "Unsuccessful stat");

	if ( !S_ISREG(fileStat.st_mode) )
		errExit(NO_ERRNO, "Given path does not point to a regular file");

	if( (fp = fopen(filename, "r")) == NULL )
		errExit(errno, "Error while opening the file");

	readFile(fp, &contents);

	fclose(fp);
	return contents;
}

static void readFile(FILE *fp, char **contents)
{
	size_t fileSize = getFileSize(fp) + 1,
		contentsSize = 0,
		contentsMemSize = (1 << 8); /* initial memory allocation */

	if ( (*contents = malloc(contentsMemSize)) == NULL ) /* char is always one byte, so one per index */
		errExit(errno, "Memory allocation error");

	while ( contentsSize < MAX_FILE_SIZE && (*(*contents + contentsSize++) = fgetc(fp)) != EOF ) 
		if ( contentsSize >= contentsMemSize ) {
			contentsMemSize = MIN_OF_THREE(contentsMemSize * 2, fileSize, MAX_FILE_SIZE); /* limited exponential allocation */
			if ( (*contents = realloc(*contents, contentsMemSize)) == NULL )
				errExit(errno, "Memory reallocation error");
		}

	if ( *(*contents + contentsSize - 1) != EOF ) {
		char choice;
		errWarn(NO_ERRNO, "Maximum file size reached");
		printf("\tWould you like to run the truncated file? [Y/n]\n");
		if ( (choice = getchar()) != 'Y' && choice != 'y' && choice != '\n' )
			exit(EXIT_FAILURE);
	}

	(*contents)[contentsSize - 1] = '\0';
}

static size_t getFileSize(FILE *fp)
{
	size_t fileSize;

	fseek(fp, 0L, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);

	return fileSize;
}
