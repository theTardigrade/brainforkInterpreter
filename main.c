#include "bfi.h"
#include "err.h"


int main(int argc, char **argv)
{
	initGlobalOptions();
	loadCommandLineOptions(argc, argv);

	if ( globalOptions.sourceFilePath )
		globalOptions.sourceCode = loadFile(globalOptions.sourceFilePath);

	if ( globalOptions.sourceCode )
	{
		run(globalOptions.sourceCode);
		if ( globalOptions.sourceFilePath )
			free(globalOptions.sourceCode);
	}
	else
		errExit(NO_ERRNO, "Usage:\n\n\t%s -f <filename>|-s <string>", *argv);

	return EXIT_SUCCESS;
}
