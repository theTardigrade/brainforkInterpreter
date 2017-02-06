#ifndef BFI_H
#define BFI_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdint.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <errno.h>
	#include <unistd.h>

	typedef struct {
		bool legacy;
		bool pointerRollover;
		bool byteRollover;
		int maxFileSize;
		char *sourceCode;
		char *sourceFilePath;
	} Opts;
	extern Opts globalOptions;

	void initGlobalOptions();
	void loadCommandLineOptions(int argc, char **argv);
	bool loadLongformCommandLineOption(char *option, char *argument);
	bool loadShortformCommandLineOption(char option, char *argument, bool lastFlag);
	void handleFileOption(char *argument);
	void handleStringOption(char *argument);
	void handleLegacyOption(__attribute__((unused)) char *argument);
	void handlePointerRolloverOption(__attribute__((unused)) char *argument);
	void handleByteRolloverOption(__attribute__((unused)) char *argument);
	void handleRolloverOption(__attribute__((unused)) char *argument);

	char *loadFile(const char *filename);

	void run(const char *bfString);

	#define CONDITIONAL_ASSIGN_TO_PTR(ptr, address) (( !(ptr) ) ? ((ptr) = (address)) : 0)

	#define MEGABYTE_IN_BYTES (1 << 20)
	#define DEFAULT_MAX_FILE_SIZE (MEGABYTE_IN_BYTES * 8 + 1) /* 8 MB, including final NUL */

	#define MIN(a, b) (( (a) < (b) ) ? (a) : (b))
	#define MIN_OF_THREE(a, b, c) MIN(MIN(a, b), c)

	#define CELL_COUNT 30000 /* BF memory table is standardly defined at 3e4 cells */

	#define JUMPS_ERR_MSG "Incorrect syntax: there should be an equal number of left and right jump statements."

#endif

