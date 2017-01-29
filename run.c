#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>

#include "bfi.h"
#include "err.h"


void run(const char *bfString)
{
	unsigned char cells[CELL_COUNT] = { 0 },
		*ptr = &cells[0];

	unsigned int i, j, nestedJumps;
	char c;
	pid_t pid;
	for ( i = 0; (c = *(bfString + i)) != '\0'; ++i )
	{
		switch (c)
		{ /* cells and values wrap-around within respective bounds */
        	case 0x2B /* '+' */:    ( *ptr < 0xFF ) ? ++*ptr : (*ptr = 0); break;
        	case 0x2D /* '-' */:    ( *ptr > 0 ) ? --*ptr : (*ptr = 0xFF); break;
        	case 0x3E /* '>' */:    ( ptr < cells + CELL_COUNT - 1 ) ? ++ptr : (ptr = cells); break;
        	case 0x3C /* '<' */:    ( ptr > cells ) ? --ptr : (ptr = cells + CELL_COUNT - 1); break;
        	case 0x2E /* '.' */:    putchar(*ptr); break;
        	case 0x2C /* ',' */:    *ptr = getchar(); break;
        	case 0x5B /* '[' */:
				if ( !*ptr ) /* zero value */
				{
					nestedJumps = 0;
					for ( j = i; (c = *(bfString + j)) != '\0'; ++j)
					{
						if ( c == 0x5B )
							++nestedJumps;
						else if ( c == 0x5D ) {
							if ( --nestedJumps == 0 ) {
								i = j;
								break;
							}
						}
					}
				}
				break;
        	case 0x5D /* ']' */:
				if ( *ptr ) /* non-zero value */
				{
					nestedJumps = 0;
					for ( j = i; j >= 0; --j)
					{
						if ( (c = *(bfString + j)) == 0x5D )
							++nestedJumps;
						else if ( c == 0x5B ) {
							if ( --nestedJumps == 0 ) {
								i = j;
								break;
							}
						}
					}
				}
            	break;
			case 0x59 /* 'Y' */:
            if ( (pid = fork()) < 0 )
            	    errExit(errno, "Fork failed");
        	    ( pid ) ? (*ptr = 0) /* parent */ : (*(++ptr) = 1);
    	        break;
			default: /* ignore other characters */
				/* ergo, do nothing */
    	}
	}

	printf("\n");
}
