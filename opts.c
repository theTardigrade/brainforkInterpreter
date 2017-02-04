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
}

CmdLineOpts recognizedCommandLineOptions = {
    { "legacy", 'l', false, handleLegacyOption },
    { "file", 'f', true, handleFileOption },
    { "string", 's', true, handleStringOption }
};

void loadCommandLineOptions(int argc, char **argv)
{
    int i;
    for ( i = 2; i <= argc; ++i )
    {
        char *flag = *(argv + i - 1);
        int flagLen = strlen(flag);

        if ( flag[0] != '-' || flagLen < 2 )
        {
            errWarn(NO_ERRNO, "Unrecognized argument [%s] found.", flag);
            continue;
        }

/*
        increment i below to skip parsing argument on the next iteration,
        but decrement again in the default case of the switch statement
        if no recognized flag is found (and therefore no options argument
        is expected)
*/

        char *argument = *(argv + i++);
        bool usedArgument = false;

        if ( flag[1] == '-' )
        {
            usedArgument = loadLongformCommandLineOption(flag + 2, argument);
        }
        else
        {
            int j;
            for ( j = 1; j < flagLen; ++j )
                usedArgument = loadShortformCommandLineOption(flag[j], argument, flagLen - j == 1);
        }

        if ( !usedArgument )
            --i;
    }
}

bool loadLongformCommandLineOption(char *option, char *argument)
{
    CmdLineOpt recognizedOption;
    bool foundRecognizedOption = false;
    int i, l = sizeof(recognizedCommandLineOptions) / sizeof(recognizedCommandLineOptions[0]);
    for ( i = 0; i < l; ++i )
        if ( recognizedCommandLineOptions[i].longform && !strcmp(recognizedCommandLineOptions[i].longform, option) )
        {
            recognizedOption = recognizedCommandLineOptions[i];
            foundRecognizedOption = true;
        }

    if ( !foundRecognizedOption )
    {
        errWarn(NO_ERRNO, "Unrecognized option [--%s] found.", option);
        return false;
    }

    if ( recognizedOption.function )
        recognizedOption.function(argument);

    return recognizedOption.expectsArgument;
}

bool loadShortformCommandLineOption(char option, char *argument, bool lastFlag)
{
    CmdLineOpt recognizedOption;
    bool foundRecognizedOption = false;
    int i, l = sizeof(recognizedCommandLineOptions) / sizeof(recognizedCommandLineOptions[0]);
    for ( i = 0; i < l; ++i )
        if ( recognizedCommandLineOptions[i].shortform && recognizedCommandLineOptions[i].shortform == option )
        {
            recognizedOption = recognizedCommandLineOptions[i];
            foundRecognizedOption = true;
        }

    if ( !foundRecognizedOption )
    {
        errWarn(NO_ERRNO, "Unrecognized flag [-%c] found.", option);
        return false;
    }

    if ( recognizedOption.function && (!recognizedOption.expectsArgument || lastFlag) )
        recognizedOption.function(argument);

    if ( recognizedOption.expectsArgument && !lastFlag )
        errWarn(NO_ERRNO, "Flag [-%c] should be immediately followed by an argument value.", option);

    return recognizedOption.expectsArgument;
}

void handleLegacyOption(char *argument)
{
    globalOptions.legacy = true;
}

void handleFileOption(char *argument)
{
    globalOptions.sourceFilePath = argument;
}

void handleStringOption(char *argument)
{
    globalOptions.sourceCode = argument;
}