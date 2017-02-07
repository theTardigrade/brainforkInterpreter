#include "bfi.h"
#include "err.h"


int main(int argc, char **argv)
{
	initGlobalOptions();
	loadCommandLineOptions(argc, argv);

	if ( globalOptions.sourceFilePath )
		globalOptions.sourceCode = loadFile(globalOptions.sourceFilePath);

	run(globalOptions.sourceCode);
	
	if ( globalOptions.sourceFilePath )
		free(globalOptions.sourceCode);

	return EXIT_SUCCESS;
}
