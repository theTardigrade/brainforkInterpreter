#include "bfi.h"
#include "err.h"


Opts globalOptions;

void initGlobalOptions()
{
	Opts *o = &globalOptions;

	o->legacy = false;
	o->maxFileSize = DEFAULT_MAX_FILE_SIZE;
	o->sourceCode = NULL;
	o->sourceFilePath = NULL;
	o->pointerRollover = o->byteRollover = false;
}

typedef void (*CmdLineOptHandler)(char *argument);
typedef struct {
	char *longform;
	char shortform;
	bool expectsArgument;
	CmdLineOptHandler function;
} CmdLineOpt;
typedef CmdLineOpt CmdLineOpts[6];

CmdLineOpts recognizedCommandLineOptions = {
	{ "file", 'f', true, handleFileOption },
	{ "string", 's', true, handleStringOption },
	{ "legacy", 'l', false, handleLegacyOption },
	{ "pointer-rollover", 'p', false, handlePointerRolloverOption },
	{ "byte-rollover", 'b', false, handleByteRolloverOption },
	{ "rollover", 'r', false, handleRolloverOption }
};

int compareCommandLineOptionsByLongform(const void *p1, const void *p2)
{
	char *s1 = ((const CmdLineOpt *) p1)->longform;
	char *s2 = ((const CmdLineOpt *) p2)->longform;

	return strcmp(s1, s2);
}

void sortRecognizedCommandLineOptions(int (*comparator)(const void *, const void*))
{
	qsort(
		recognizedCommandLineOptions,
		sizeof(recognizedCommandLineOptions) / sizeof(*recognizedCommandLineOptions),
		sizeof(*recognizedCommandLineOptions),
		comparator
	);
}

CmdLineOpt *findRecognizedCommandLineOption(const void *key, int (*comparator)(const void *, const void *))
{
	return (CmdLineOpt *) bsearch(
		key,
		recognizedCommandLineOptions,
		sizeof(recognizedCommandLineOptions) / sizeof(*recognizedCommandLineOptions),
		sizeof(*recognizedCommandLineOptions),
		comparator
    );
}

void loadCommandLineOptions(int argc, char **argv)
{
	int i;

/*
	sort the recognized command-line options below so that binary search
	can later be used to find long-form options
*/

	sortRecognizedCommandLineOptions(compareCommandLineOptionsByLongform);

	for ( i = 2; i <= argc; ++i )
	{
		char *opts = *(argv + i - 1);
		int optsLen = strlen(opts);

		if ( opts[0] != '-' || optsLen < 2 || (opts[1] == '-' && optsLen == 2) )
		{
			errWarn(NO_ERRNO, "Unrecognized argument [%s] found.", opts);
			continue;
		}

/*
		increment i below to skip parsing argument on the next iteration,
		but decrement again in the default case of the switch statement
		if no recognized flag is found (and therefore no options argument
		is expected)
*/

		{
			char *optArg = *(argv + i++);
			bool usedOptArg = false;

			if ( opts[1] == '-' )
			{
				usedOptArg = loadLongformCommandLineOption(opts + 2, optArg);
			}
			else
			{
				int j;
				for ( j = 1; j < optsLen; ++j )
					usedOptArg = loadShortformCommandLineOption(opts[j], optArg, !(optsLen - j - 1));
			}

			if ( !usedOptArg )
				--i;
		}
	}

	validateGlobalOptions(argv);
}

bool loadLongformCommandLineOption(char *option, char *argument)
{
	CmdLineOpt *recognizedOption = findRecognizedCommandLineOption(&option, compareCommandLineOptionsByLongform);

	if ( recognizedOption == NULL )
	{
		errWarn(NO_ERRNO, "Unrecognized option [--%s] found.", option);
		return false;
	}

	if ( recognizedOption->function )
		recognizedOption->function(argument);

	return recognizedOption->expectsArgument;
}

bool loadShortformCommandLineOption(char option, char *argument, bool lastFlag)
{
	CmdLineOpt *recognizedOption = NULL;
	int i, l = sizeof(recognizedCommandLineOptions) / sizeof(recognizedCommandLineOptions[0]);
	for ( i = 0; i < l; ++i )
		if ( recognizedCommandLineOptions[i].shortform && recognizedCommandLineOptions[i].shortform == option )
		{
			recognizedOption = &recognizedCommandLineOptions[i];
			break;
		}

	if ( recognizedOption == NULL )
	{
		errWarn(NO_ERRNO, "Unrecognized option [-%c] found.", option);
		return false;
	}

	if ( recognizedOption->function && (!recognizedOption->expectsArgument || lastFlag) )
		recognizedOption->function(argument);

	if ( recognizedOption->expectsArgument && !lastFlag )
		errWarn(NO_ERRNO, "Option [-%c] should be immediately followed by an argument value.", option);

	return recognizedOption->expectsArgument;
}

void validateGlobalOptions(char **argv)
{
	static char *exampleInputs = "(e.g. string, file)";
	Opts *o = &globalOptions;
	int inputs = 0;

	if (o->sourceCode)
		++inputs;

	if (o->sourceFilePath)
		++inputs;

	if (inputs > 1)
		errExit(NO_ERRNO, "Only one input source %s allowed but multiple detected.", exampleInputs);
	else if (inputs < 1)
		errExit(NO_ERRNO, "No input source %s detected.", exampleInputs, *argv);
}

void handleFileOption(char *argument)
{
	globalOptions.sourceFilePath = argument;
}

void handleStringOption(char *argument)
{
	globalOptions.sourceCode = argument;
}

void handleLegacyOption(__attribute__((unused)) char *argument)
{
	globalOptions.legacy = true;
}

void handlePointerRolloverOption(__attribute__((unused)) char *argument)
{
	globalOptions.pointerRollover = true;
}

void handleByteRolloverOption(__attribute__((unused)) char *argument)
{
	globalOptions.byteRollover = true;
}

void handleRolloverOption(__attribute__((unused)) char *argument)
{
	globalOptions.pointerRollover = globalOptions.byteRollover = true;
}
