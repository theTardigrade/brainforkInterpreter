#ifndef BFI_H
#define BFI_H

	#define CELL_COUNT 30000 /* BF memory table is standardly defined at 3e4 cells */

	#define MEGABYTE_IN_BYTES (1 << 20)
	#define MAX_FILE_SIZE (MEGABYTE_IN_BYTES * 8 + 1) /* 8 MB, including final NUL */

	#define MIN(a, b) ((a < b) ? a : b)
	#define MIN_OF_THREE(a, b, c) MIN(MIN(a, b), c)

	char *loadFile(const char *filename);
	static void readFile(FILE *, char **);
	static size_t getFileSize(FILE *);

	void run(const char *bfString);

#endif

