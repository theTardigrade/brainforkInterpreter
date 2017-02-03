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

	struct
	{
		bool legacy;
	} globalOptions;

	#define CONDITIONAL_ASSIGN_TO_PTR(ptr, address) (( !(ptr) ) ? ((ptr) = (address)) : 0)

	char *loadFile(const char *filename);

	#define MEGABYTE_IN_BYTES (1 << 20)
	#define MAX_FILE_SIZE (MEGABYTE_IN_BYTES * 8 + 1) /* 8 MB, including final NUL */

	#define MIN(a, b) (( (a) < (b) ) ? (a) : (b))
	#define MIN_OF_THREE(a, b, c) MIN(MIN(a, b), c)

	void run(const char *bfString);

	#define CELL_COUNT 30000 /* BF memory table is standardly defined at 3e4 cells */

	#define JUMPS_ERR_MSG "Incorrect syntax: there should be an equal number of left and right jump statements."

#endif

