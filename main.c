#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "bfi.h"
#include "err.h"

extern int opterr;
extern char *optarg;


int main(int argc, char **argv)
{
	char *brainforkCode = NULL;

	opterr = 0;
	char opt;
	if ( (opt = getopt(argc, argv, "f:s:")) != -1 )
	{
		if ( opt == 'f' )
			brainforkCode = loadFile(optarg);
		else if (opt == 's')
			brainforkCode = optarg;
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
