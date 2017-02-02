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
		char *flag = *(argv + i - 1),
			*longformFlag = flag + 2;
		int flagLen = strlen(flag);

		if ( flag[0] != '-' || flagLen < 2 || (flag[1] != '-' && flagLen != 2) )
		{
			errWarn(NO_ERRNO, "Unrecognized argument value [%s] found.", flag);
			continue;
		}

/*
		increment i below to skip parsing argument on the next iteration,
		but decrement again in the default case of the switch statement
		if no recognized flag is found (and therefore no options argument
		is expected)
*/

		char *argument = *(argv + i++); 

		switch ( flag[1] )
		{
			case 'f':
				CONDITIONAL_ASSIGN_TO_PTR(brainforkCode, loadFile(argument));
				break;
			case 's':
				CONDITIONAL_ASSIGN_TO_PTR(brainforkCode, argument);
				break;
			case '-':
/*
				utilize short-form flag cases above to perform the logic
				for long-form flags by converting the form of the flag
				and then decrementing the counter to reparse the edited flag
*/
				if ( !strcmp(longformFlag, "file") || !strcmp(longformFlag, "string") )
				{
					flag[1] = longformFlag[0];
					flag[2] = '\0';
					i -= 2;
				}
				else
				{
					errWarn(NO_ERRNO, "Unrecognized option [--%s] found.", longformFlag);
					--i;
				}
				break;
			default:
				errWarn(NO_ERRNO, "Unrecognized flag [-%c] found.", flag[1]);
				--i;
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
