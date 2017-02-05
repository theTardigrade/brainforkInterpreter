#include "bfi.h"
#include "err.h"


void run(const char *bfString)
{
	uint8_t cells[CELL_COUNT] = { 0 },
		*ptr = &cells[0];

	int32_t i, nestedJumps;
	char c;
	pid_t pid;
	for ( i = nestedJumps = 0; (c = *(bfString + i)) != '\0'; ++i )
	{
		switch ( c )
		{
			case 0x2B /* '+' */:
				if ( *ptr < UINT8_MAX )
					++*ptr;
				else if ( globalOptions.byteRollover )
					*ptr = 0;
				break;
			case 0x2D /* '-' */:
				if ( *ptr > 0 )
					--*ptr;
				else if ( globalOptions.byteRollover )
					*ptr = UINT8_MAX;
				break;
			case 0x3E /* '>' */:
				if ( ptr < cells + CELL_COUNT - 1 )
					++ptr;
				else if ( globalOptions.pointerRollover )
					ptr = cells;
				break;
			case 0x3C /* '<' */:
				if ( ptr > cells )
					--ptr;
				else if ( globalOptions.pointerRollover )
					ptr = cells + CELL_COUNT - 1;
				break;
			case 0x2E /* '.' */:    putchar(*ptr); break;
			case 0x2C /* ',' */:    *ptr = getchar(); break;
			case 0x5B /* '[' */:
				if ( !*ptr ) /* zero value */
				{
					for ( ; (c = *(bfString + i)) != '\0'; ++i)
					{
						if ( c == 0x5B )
							++nestedJumps;
						else if ( c == 0x5D && !--nestedJumps )
							break;
					}
					goto jumpErrCheck;
				}
				break;
			case 0x5D /* ']' */:
				if ( *ptr ) /* non-zero value */
				{
					for ( ; i >= 0; --i )
					{
						if ( (c = *(bfString + i)) == 0x5D )
							++nestedJumps;
						else if ( c == 0x5B && !--nestedJumps )
							break;
					}
					goto jumpErrCheck;
				}
				break;
			case 0x59 /* 'Y' */:
				if ( !globalOptions.legacy )
				{
					if ( (pid = fork() ) < 0 )
						errExit(errno, "Fork failed");
					( pid ) ? (*ptr = 0) /* parent */ : (*(++ptr) = 1);
				} 
			   break;
			default: /* ignore other characters */
				break;
		}

		continue;

		jumpErrCheck:
			if ( nestedJumps )
				errExit(NO_ERRNO, JUMPS_ERR_MSG);
	}

	putchar('\n');
}
