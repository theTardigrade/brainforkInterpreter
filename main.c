#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bfi.h"
#include "err.h"


int main(int argc, char **argv)
{
	char *brainforkCode = NULL;

/*
	parse and handle command-line flags and arguments
*/

	int i;
	for ( i = 2; i <= argc; ++i )
	{
		char *flag = *(argv + i - 1);

		if ( flag[0] != '-' || strlen(flag) != 2)
			continue;

		char *argument = *(argv + i);

		switch ( flag[1] )
		{
			case 'f':
				CONDITIONAL_ASSIGN_TO_PTR(brainforkCode, loadFile(argument));
				break;
			case 's':
				CONDITIONAL_ASSIGN_TO_PTR(brainforkCode, argument);
				break;
			default:
				errWarn(NO_ERRNO, "Unrecognized flag [-%c] used.", flag[1]);
		}
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
