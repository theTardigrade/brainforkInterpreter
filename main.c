#include <stdio.h>
#include <stdlib.h>

#include "bfi.h"
#include "err.h"


int main(int argc, char **argv)
{
	char *fileContents;

	if ( argc != 2 )
		errExit(NO_ERRNO, "\n\tUsage: %s filename", *argv);

	fileContents = loadFile(*(argv + 1));

	run(fileContents);
	free(fileContents);

	return EXIT_SUCCESS;
}
