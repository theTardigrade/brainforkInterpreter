#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "bfi.h"
#include "err.h"

extern int opterr;
extern char *optarg;


int main(int argc, char **argv)
{
	char *brainforkCode = NULL;

	int i;
	for ( i = 2; i < argc && *(argv + i - 1)[0] == '-' && strlen(*(argv + i - 1)) == 2; ++i )
	{
		char *argument = *(argv + i);
		bool brainforkCodeFound = true;

		switch ( argv[i - 1][1] )
		{
			case 'f':
				brainforkCode = loadFile(argument);
				break;
			case 's':
				brainforkCode = argument;
				break;
			default:
				brainforkCodeFound = false;
		}

		if ( brainforkCodeFound )
			break;
	}

	if ( brainforkCode )
		run(brainforkCode);
	else
		errExit(NO_ERRNO, "Usage:\n\n\t%s -f <filename>|-s <string>", *argv);
/*
	malloc is used if code is loaded from file,
	but system will automatically reclaim memory
*/
	return EXIT_SUCCESS;
}
